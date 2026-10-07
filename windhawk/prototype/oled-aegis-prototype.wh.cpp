// ==WindhawkMod==
// @id              oled-aegis-prototype
// @name            OLED Aegis feasibility prototype
// @description     Manual stock-saver preview experiment; not OLED protection
// @version         0.1
// @author          OLED Aegis contributors
// @include         windhawk.exe
// @architecture    x86-64
// @compilerOptions -lshell32 -luser32 -lgdi32 -lshcore -lole32 -luuid -lwtsapi32
// ==/WindhawkMod==

// ==WindhawkModSettings==
/*
- monitor: 0
  $name: Monitor index (see prototype catalog)
- saver: 1
  $name: Saver (0 Bubbles, 1 Mystify, 2 Ribbons, 3 3D Text, 4 Photos, 5 Blank)
- smallPreview: true
  $name: Use a small preview instead of covering the selected display
- inputDismissal: true
  $name: Dismiss on mouse on that display or keyboard on cursor/foreground displays
- lifetimeSeconds: 60
  $name: Safety expiration (5–120 seconds; always enabled)
*/
// ==/WindhawkModSettings==

// Original disposable experiment. No inherited OLED Aegis code/assets copied.
// Platform mechanism references and dependency terms: ../docs/PROVENANCE.md.
#include <windows.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <wtsapi32.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <endpointvolume.h>
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <mutex>

namespace prototype {
constexpr wchar_t kClass[] = L"OLED Aegis feasibility host";
constexpr wchar_t kStopName[] = L"Local\\OLED-Aegis-Feasibility-Stop";
constexpr UINT kTray = WM_APP + 1, kSettings = WM_APP + 2;
constexpr UINT kExit = WM_APP + 3, kStart = WM_APP + 4;
constexpr wchar_t const* kSavers[] = {L"Bubbles.scr", L"Mystify.scr", L"Ribbons.scr",
    L"ssText3d.scr", L"PhotoScreensaver.scr", L"scrnsave.scr"};
struct Config { int monitor=0, saver=1, lifetime=60; bool small=true, input=true; };
Config config;
std::mutex configLock;
struct Monitor { HMONITOR handle; RECT rect; wchar_t name[32]; UINT dpi; };
struct Session {
    HWND window{}; HANDLE process{}, job{}; DWORD pid{};
    ULONGLONG start{}, closeAt{}; bool stopping{}, fallback{};
};
std::vector<Monitor> monitors;
Session sessions[16];
std::mutex sessionLock;
HANDLE uiThread{}, safetyThread{}, exitEvent{}, readyEvent{}, safetyReady{}, singleton{};
std::atomic<HWND> uiWindow{};
std::atomic<bool> initialized{}, safetyOK{};
std::atomic<bool> unloading{};
UINT taskbarMessage{};
bool paused=false;
ULONGLONG quitAt=0;
#ifdef AEGIS_HARNESS
bool probe=false;
std::wstring fixtureArgs;
#endif

void Note(const wchar_t* what, DWORD value=0) {
#ifdef AEGIS_HARNESS
    wprintf(L"%llu %ls %lu\n", GetTickCount64(), what, value); fflush(stdout);
#else
    Wh_Log(L"%s: %lu", what, value);
#endif
}
Config GetConfig() { std::lock_guard guard(configLock); return config; }
void ApplyConfig(Config next) {
    next.monitor=std::clamp(next.monitor,0,15); next.saver=std::clamp(next.saver,0,5);
    next.lifetime=std::clamp(next.lifetime,5,120);
    std::lock_guard guard(configLock); config=next;
}
BOOL CALLBACK Catalog(HMONITOR h, HDC, LPRECT r, LPARAM) {
    if (monitors.size() == 16) return FALSE;
    MONITORINFOEXW info{}; info.cbSize=sizeof(info);
    if (!GetMonitorInfoW(h,&info)) return TRUE;
    UINT x=96,y=96; GetDpiForMonitor(h,MDT_EFFECTIVE_DPI,&x,&y);
    Monitor m{h,*r,{},x}; wcscpy_s(m.name,info.szDevice); monitors.push_back(m);
    return TRUE;
}
void Enumerate() { monitors.clear(); EnumDisplayMonitors(nullptr,nullptr,Catalog,0); }
std::wstring SaverPath(int saver, bool x86=false) {
    wchar_t directory[MAX_PATH]; UINT n=x86?GetSystemWow64DirectoryW(directory,MAX_PATH):GetSystemDirectoryW(directory,MAX_PATH);
    if (!n || n>=MAX_PATH) return {};
    return std::wstring(directory)+L"\\"+kSavers[std::clamp(saver,0,5)];
}
// Process handles, not filenames, establish ownership. No handle inheritance.
bool Launch(Session& s, const std::wstring& file, const std::wstring& args) {
    s.job=CreateJobObjectW(nullptr,nullptr);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit{};
    limit.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!s.job || !SetInformationJobObject(s.job,JobObjectExtendedLimitInformation,&limit,sizeof(limit))) {
        Note(L"job creation failed",GetLastError()); if(s.job) CloseHandle(s.job); s.job=nullptr; return false;
    }
    std::wstring command=L"\""+file+L"\" "+args;
    STARTUPINFOW si{}; si.cb=sizeof(si); si.dwFlags=STARTF_USESHOWWINDOW; si.wShowWindow=SW_SHOWNOACTIVATE;
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(file.c_str(),command.data(),nullptr,nullptr,FALSE,CREATE_SUSPENDED,nullptr,nullptr,&si,&pi)) {
        Note(L"CreateProcess failed",GetLastError()); CloseHandle(s.job); s.job=nullptr; return false;
    }
    if (!AssignProcessToJobObject(s.job,pi.hProcess)) {
        Note(L"job assignment failed",GetLastError()); TerminateProcess(pi.hProcess,1);
        WaitForSingleObject(pi.hProcess,2000); CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
        CloseHandle(s.job); s.job=nullptr; return false;
    }
    s.process=pi.hProcess; s.pid=pi.dwProcessId;
    bool resumed=ResumeThread(pi.hThread)!=DWORD(-1); CloseHandle(pi.hThread);
    if (!resumed) { CloseHandle(s.job); s.job=nullptr; CloseHandle(s.process); s.process=nullptr; return false; }
    Note(L"contained child started",s.pid); return true;
}
BOOL CALLBACK RequestClose(HWND w, LPARAM param) {
    auto s=reinterpret_cast<Session*>(param); DWORD pid=0; GetWindowThreadProcessId(w,&pid);
    HANDLE p=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid);
    BOOL owned=FALSE;
    if (p) { IsProcessInJob(p,s->job,&owned); CloseHandle(p); }
    if (owned) PostMessageW(w,WM_CLOSE,0,0);
    return TRUE;
}
void BeginStop(Session& s) {
    if (!s.window || s.stopping) return;
    // Hide the presentation promptly, then allow a bounded grace period.
    ShowWindow(s.window,SW_HIDE);
    if(s.job) { EnumChildWindows(s.window,RequestClose,reinterpret_cast<LPARAM>(&s)); EnumWindows(RequestClose,reinterpret_cast<LPARAM>(&s)); }
    s.stopping=true; s.closeAt=GetTickCount64()+750;
}
void FinishStop(Session& s) {
    if(s.job) { CloseHandle(s.job); s.job=nullptr; }
    if(s.process) { CloseHandle(s.process); s.process=nullptr; }
    if(s.window) DestroyWindow(s.window);
    s={};
}
void StopAll() { std::lock_guard guard(sessionLock); for(auto& s:sessions) BeginStop(s); }
void Start(int index, int saver, bool x86=false, bool missing=false) {
    if(paused || index<0 || index>=static_cast<int>(monitors.size())) return;
    std::lock_guard guard(sessionLock); auto& s=sessions[index];
    if(s.window) { Note(L"already running or stopping",index); return; }
    auto cfg=GetConfig(); auto r=monitors[index].rect;
    int width=r.right-r.left, height=r.bottom-r.top;
    if(cfg.small) { width=std::min(width,640); height=std::min(height,360); }
    s.window=CreateWindowExW(WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE|WS_EX_TOPMOST,kClass,
        L"OLED Aegis preview — Ctrl+Alt+Shift+F12 exits",WS_POPUP|WS_CLIPCHILDREN,
        r.left,r.top,width,height,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    if(!s.window) { Note(L"host creation failed",GetLastError()); return; }
    ShowWindow(s.window,SW_SHOWNOACTIVATE); UpdateWindow(s.window); s.start=GetTickCount64();
    auto path=missing?SaverPath(saver)+L".missing":SaverPath(saver,x86);
    auto args=L"/p "+std::to_wstring(reinterpret_cast<uintptr_t>(s.window));
#ifdef AEGIS_HARNESS
    if(!fixtureArgs.empty()) { wchar_t exe[32768]; GetModuleFileNameW(nullptr,exe,32768); path=exe; args=fixtureArgs; }
#endif
    s.fallback=!Launch(s,path,args); Note(s.fallback?L"black fallback":L"preview requested",index);
}
bool Tray(bool remove=false) {
    NOTIFYICONDATAW n{}; n.cbSize=sizeof(n); n.hWnd=uiWindow; n.uID=1;
    n.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP; n.uCallbackMessage=kTray;
    n.hIcon=LoadIconW(nullptr,IDI_APPLICATION);
    wcscpy_s(n.szTip,L"OLED Aegis EXPERIMENT — Ctrl+Alt+Shift+F12 exits");
    bool ok=!!Shell_NotifyIconW(remove?NIM_DELETE:NIM_ADD,&n);
    if(!ok && !remove) Note(L"tray add failed",GetLastError());
    return ok;
}
void Menu(HWND window) {
    HMENU menu=CreatePopupMenu(); if(!menu) return;
    for(size_t i=0;i<monitors.size();++i) {
        HMENU sub=CreatePopupMenu(); if(!sub) continue;
        for(int j=0;j<6;++j) AppendMenuW(sub,MF_STRING,100+i*6+j,kSavers[j]);
        auto label=std::to_wstring(i)+L": "+monitors[i].name;
        AppendMenuW(menu,MF_POPUP,reinterpret_cast<UINT_PTR>(sub),label.c_str());
    }
    AppendMenuW(menu,MF_STRING,1,L"Start Windhawk selection");
    AppendMenuW(menu,MF_STRING,2,L"Stop all");
    AppendMenuW(menu,MF_STRING|(paused?MF_CHECKED:0),3,L"Session pause");
    AppendMenuW(menu,MF_STRING,4,L"Show configuration / settings help");
    AppendMenuW(menu,MF_STRING,5,L"Exit experiment");
    POINT p{}; GetCursorPos(&p); SetForegroundWindow(window);
    UINT selected=TrackPopupMenu(menu,TPM_RETURNCMD|TPM_NONOTIFY,p.x,p.y,0,window,nullptr);
    DestroyMenu(menu); PostMessageW(window,WM_NULL,0,0);
    if(selected>=100) { int n=selected-100; auto cfg=GetConfig(); cfg.monitor=n/6; cfg.saver=n%6; ApplyConfig(cfg); Start(cfg.monitor,cfg.saver); }
    else if(selected==1) { auto cfg=GetConfig(); Start(cfg.monitor,cfg.saver); }
    else if(selected==2) StopAll();
    else if(selected==3) { paused=!paused; if(paused) StopAll(); }
    else if(selected==4) {
        auto cfg=GetConfig(); auto text=L"Manual-only prototype. Persistent defaults: Windhawk > this mod > Settings.\nTray choices override only this session; the next settings update replaces them.\nMonitor "+std::to_wstring(cfg.monitor)+L", saver "+kSavers[cfg.saver]+L".\nNo automatic startup activation or Windows setting changes.\nSafety exit: Ctrl+Alt+Shift+F12 (or harness --stop).";
        NOTIFYICONDATAW n{}; n.cbSize=sizeof(n); n.hWnd=window; n.uID=1; n.uFlags=NIF_INFO;
        wcscpy_s(n.szInfoTitle,L"Prototype configuration");
        wcsncpy_s(n.szInfo,text.c_str(),_TRUNCATE); Shell_NotifyIconW(NIM_MODIFY,&n);
    } else if(selected==5) PostMessageW(window,WM_CLOSE,0,0);
}
void Activity(HRAWINPUT handle) {
    if(!GetConfig().input) return;
    UINT bytes=sizeof(RAWINPUT); RAWINPUT input{};
    if(GetRawInputData(handle,RID_INPUT,&input,&bytes,sizeof(RAWINPUTHEADER))==UINT(-1)) return;
    bool keyboard=input.header.dwType==RIM_TYPEKEYBOARD;
    if(!keyboard && input.header.dwType!=RIM_TYPEMOUSE) return;
    POINT cursor{}; if(!GetCursorPos(&cursor)) { StopAll(); return; }
    HMONITOR cm=MonitorFromPoint(cursor,MONITOR_DEFAULTTONULL);
    HMONITOR fm=keyboard?MonitorFromWindow(GetForegroundWindow(),MONITOR_DEFAULTTONULL):nullptr;
    std::lock_guard guard(sessionLock);
    for(size_t i=0;i<monitors.size();++i) if(monitors[i].handle==cm || monitors[i].handle==fm) BeginStop(sessions[i]);
}
LRESULT CALLBACK WindowProc(HWND w,UINT msg,WPARAM wp,LPARAM lp) {
    if(taskbarMessage && msg==taskbarMessage && w==uiWindow) { Tray(); return 0; }
    switch(msg) {
    case WM_MOUSEACTIVATE: return MA_NOACTIVATE;
    case WM_PAINT: { PAINTSTRUCT p{}; HDC dc=BeginPaint(w,&p); FillRect(dc,&p.rcPaint,static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH))); EndPaint(w,&p); return 0; }
    case WM_INPUT: Activity(reinterpret_cast<HRAWINPUT>(lp)); break;
    case kTray: if(lp==WM_RBUTTONUP || lp==WM_LBUTTONUP) Menu(w); return 0;
    case kSettings: StopAll(); Note(L"settings applied"); return 0;
    case kStart: Start(static_cast<int>(wp),static_cast<int>(lp)&255,(lp&256)!=0,(lp&512)!=0); return 0;
    case WM_DISPLAYCHANGE: StopAll(); Note(L"topology changed; re-enable to refresh catalog"); return 0;
    case WM_POWERBROADCAST: if(wp==PBT_APMSUSPEND) StopAll(); return TRUE;
    case WM_WTSSESSION_CHANGE: if(wp==WTS_SESSION_LOCK) StopAll(); return 0;
    case WM_TIMER: {
        std::lock_guard guard(sessionLock); auto now=GetTickCount64(); auto cfg=GetConfig();
        for(auto& s:sessions) {
            if(!s.window) continue;
            if(!s.stopping && now-s.start>=static_cast<ULONGLONG>(cfg.lifetime)*1000) BeginStop(s);
            if(!s.stopping && s.process && WaitForSingleObject(s.process,0)==WAIT_OBJECT_0) {
                Note(L"early child exit; black fallback",s.pid); CloseHandle(s.job); s.job=nullptr;
                CloseHandle(s.process); s.process=nullptr; s.fallback=true;
            }
            if(s.stopping && now>=s.closeAt) FinishStop(s);
        } if(quitAt && now>=quitAt) SetEvent(exitEvent); return 0;
    }
    case kExit: EndMenu(); DestroyWindow(w); return 0;
    case WM_CLOSE: StopAll(); quitAt=GetTickCount64()+800; return 0;
    case WM_DESTROY: if(w==uiWindow) PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(w,msg,wp,lp);
}
DWORD WINAPI SafetyMain(void*) {
    MSG message{}; PeekMessageW(&message,nullptr,0,0,PM_NOREMOVE);
    safetyOK=!!RegisterHotKey(nullptr,1,MOD_CONTROL|MOD_ALT|MOD_SHIFT|MOD_NOREPEAT,VK_F12);
    SetEvent(safetyReady);
    HANDLE events[]={exitEvent};
    while(safetyOK) {
        DWORD status=MsgWaitForMultipleObjects(1,events,FALSE,INFINITE,QS_ALLINPUT);
        if(status==WAIT_OBJECT_0) break;
        if(status==WAIT_FAILED) { SetEvent(exitEvent); break; }
        while(PeekMessageW(&message,nullptr,0,0,PM_REMOVE)) if(message.message==WM_HOTKEY) SetEvent(exitEvent);
    }
    // Independent of UI timer/settings. Only this session's job handles are closed.
    { std::lock_guard guard(sessionLock); for(auto& s:sessions) {
        if(s.window) ShowWindowAsync(s.window,SW_HIDE);
        if(s.job) { CloseHandle(s.job); s.job=nullptr; }
    } }
    PostMessageW(uiWindow,kExit,0,0); UnregisterHotKey(nullptr,1); return 0;
}
DWORD WINAPI UiMain(void*) {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    HRESULT com=CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    WNDCLASSW wc{}; wc.lpfnWndProc=WindowProc; wc.hInstance=GetModuleHandleW(nullptr);
    wc.lpszClassName=kClass; wc.hCursor=LoadCursorW(nullptr,IDC_ARROW);
    wc.hbrBackground=static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    if(!RegisterClassW(&wc)) { SetEvent(readyEvent); if(SUCCEEDED(com)) CoUninitialize(); return 1; }
    uiWindow=CreateWindowExW(WS_EX_TOOLWINDOW,kClass,L"OLED Aegis experiment controller",WS_POPUP,0,0,0,0,nullptr,nullptr,wc.hInstance,nullptr);
    Enumerate(); taskbarMessage=RegisterWindowMessageW(L"TaskbarCreated");
    RAWINPUTDEVICE raw[]={{1,2,RIDEV_INPUTSINK,uiWindow},{1,6,RIDEV_INPUTSINK,uiWindow}};
    bool rawOK=uiWindow && RegisterRawInputDevices(raw,2,sizeof(raw[0]));
    bool timerOK=uiWindow && SetTimer(uiWindow,1,50,nullptr);
    initialized=rawOK && timerOK;
    if(initialized) {
        bool trayOK=Tray();
#ifndef AEGIS_HARNESS
        initialized=trayOK; // A real tool mod must never silently lose its visible controls.
#else
        (void)trayOK; // Shell-less harness results explicitly report this limitation.
#endif
        WTSRegisterSessionNotification(uiWindow,NOTIFY_FOR_THIS_SESSION);
    }
    SetEvent(readyEvent);
    MSG msg{};
    if(initialized) while(true) { int r=GetMessageW(&msg,nullptr,0,0); if(r<=0) break; TranslateMessage(&msg); DispatchMessageW(&msg); }
    SetEvent(exitEvent);
    { std::lock_guard guard(sessionLock); for(auto& s:sessions) FinishStop(s); }
    RAWINPUTDEVICE remove[]={{1,2,RIDEV_REMOVE,nullptr},{1,6,RIDEV_REMOVE,nullptr}};
    RegisterRawInputDevices(remove,2,sizeof(remove[0]));
    if(uiWindow) { KillTimer(uiWindow,1); WTSUnRegisterSessionNotification(uiWindow); Tray(true); if(IsWindow(uiWindow)) DestroyWindow(uiWindow); }
    uiWindow=nullptr; UnregisterClassW(kClass,wc.hInstance); if(SUCCEEDED(com)) CoUninitialize();
#ifndef AEGIS_HARNESS
    if(initialized && !unloading) {
        if(safetyThread) WaitForSingleObject(safetyThread,INFINITE);
        ExitProcess(0); // Session exit only after owned windows/jobs and watcher are done.
    }
#endif
    return 0;
}
void Shutdown() {
    unloading=true;
    if(uiThread && uiWindow) { PostMessageW(uiWindow,WM_CLOSE,0,0); WaitForSingleObject(uiThread,1500); }
    if(exitEvent) SetEvent(exitEvent);
    if(safetyThread) { WaitForSingleObject(safetyThread,INFINITE); CloseHandle(safetyThread); safetyThread=nullptr; }
    if(uiThread) { if(uiWindow) PostMessageW(uiWindow,kExit,0,0); WaitForSingleObject(uiThread,INFINITE); CloseHandle(uiThread); uiThread=nullptr; }
    for(HANDLE* h:{&exitEvent,&readyEvent,&safetyReady,&singleton}) if(*h) { CloseHandle(*h); *h=nullptr; }
    initialized=false;
}
bool Initialize() {
    unloading=false;
    singleton=CreateMutexW(nullptr,FALSE,L"Local\\OLED-Aegis-Feasibility-Singleton");
    if(!singleton || GetLastError()==ERROR_ALREADY_EXISTS) { if(singleton) CloseHandle(singleton); singleton=nullptr; return false; }
    exitEvent=CreateEventW(nullptr,TRUE,FALSE,kStopName); readyEvent=CreateEventW(nullptr,TRUE,FALSE,nullptr); safetyReady=CreateEventW(nullptr,TRUE,FALSE,nullptr);
    if(!exitEvent || !readyEvent || !safetyReady) { Shutdown(); return false; }
    uiThread=CreateThread(nullptr,0,UiMain,nullptr,0,nullptr);
    if(!uiThread || WaitForSingleObject(readyEvent,10000)!=WAIT_OBJECT_0 || !initialized) { Shutdown(); return false; }
    safetyThread=CreateThread(nullptr,0,SafetyMain,nullptr,0,nullptr);
    if(!safetyThread || WaitForSingleObject(safetyReady,10000)!=WAIT_OBJECT_0 || !safetyOK) { Shutdown(); return false; }
    Note(L"ready; emergency hotkey and external stop event registered"); return true;
}
} // namespace prototype

#ifndef AEGIS_HARNESS
// Independently written stable-host adapter following Windhawk's documented
// dedicated-process mechanism; no wiki/example source pasted here.
bool toolHost=false, launcher=false;
void LoadSettings() {
    prototype::Config c; c.monitor=Wh_GetIntSetting(L"monitor"); c.saver=Wh_GetIntSetting(L"saver");
    c.small=Wh_GetIntSetting(L"smallPreview")!=0; c.input=Wh_GetIntSetting(L"inputDismissal")!=0;
    c.lifetime=Wh_GetIntSetting(L"lifetimeSeconds"); prototype::ApplyConfig(c);
}
DWORD WINAPI DedicatedEntry() { ExitThread(0); return 0; }
BOOL Wh_ModInit() {
    DWORD session=0; if(!ProcessIdToSessionId(GetCurrentProcessId(),&session) || session==0) return FALSE;
    int count=0; wchar_t** args=CommandLineToArgvW(GetCommandLineW(),&count); if(!args) return FALSE;
    bool otherTool=false, excluded=false;
    for(int i=1;i<count;++i) {
        if(wcscmp(args[i],L"-service")==0 || wcscmp(args[i],L"-service-start")==0 || wcscmp(args[i],L"-service-stop")==0) excluded=true;
        if(wcscmp(args[i],L"-tool-mod")==0 && i+1<count) { toolHost=wcscmp(args[i+1],WH_MOD_ID)==0; otherTool=!toolHost; }
    }
    LocalFree(args); if(excluded || otherTool) return FALSE;
    if(!toolHost) { launcher=true; return TRUE; }
    LoadSettings();
    auto base=reinterpret_cast<BYTE*>(GetModuleHandleW(nullptr));
    auto dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    auto nt=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
    if(!Wh_SetFunctionHook(base+nt->OptionalHeader.AddressOfEntryPoint,reinterpret_cast<void*>(DedicatedEntry),nullptr)) return FALSE;
    if(!prototype::Initialize()) { prototype::Shutdown(); ExitProcess(1); }
    return TRUE;
}
void Wh_ModAfterInit() {
    if(!launcher) return;
    wchar_t path[32768]; DWORD n=GetModuleFileNameW(nullptr,path,32768); if(!n || n>=32768) return;
    std::wstring command=L"\""+std::wstring(path)+L"\" -tool-mod \""+WH_MOD_ID+L"\"";
    using CreateInternal=BOOL(WINAPI*)(HANDLE,LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCWSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION,PHANDLE);
    auto module=GetModuleHandleW(L"kernelbase.dll");
    auto create=reinterpret_cast<CreateInternal>(GetProcAddress(module,"CreateProcessInternalW")); if(!create) return;
    STARTUPINFOW si{}; si.cb=sizeof(si); PROCESS_INFORMATION pi{};
    if(create(nullptr,path,command.data(),nullptr,nullptr,FALSE,0,nullptr,nullptr,&si,&pi,nullptr)) { CloseHandle(pi.hThread); CloseHandle(pi.hProcess); }
}
void Wh_ModSettingsChanged() { if(toolHost) { LoadSettings(); PostMessageW(prototype::uiWindow,prototype::kSettings,0,0); } }
void Wh_ModUninit() { if(toolHost) { prototype::Shutdown(); ExitProcess(0); } }
#endif
