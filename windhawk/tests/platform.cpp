#define DAC_HARNESS
#include "../mods/dac-windhawk.wh.cpp"
#include <cstdlib>
#include <psapi.h>
using namespace dac;
void Check(bool ok,const char* what) { if(!ok) { fprintf(stderr,"FAIL %s error=%lu\n",what,GetLastError()); fflush(stdout);fflush(stderr);std::_Exit(1); } }
std::wstring Executable() { wchar_t exe[32768]; DWORD n=GetModuleFileNameW(nullptr,exe,32768); Check(n&&n<32768,"exe path"); return std::wstring(exe,n); }
void Pump() { MSG msg;while(PeekMessageW(&msg,nullptr,0,0,PM_REMOVE)) {TranslateMessage(&msg);DispatchMessageW(&msg);} }
struct SessionSnapshot {size_t runs{},windows{},jobs{},powers{};int powerPhase=0;State a{},b{},c{};Time aGeneration{},bGeneration{};bool hidden=true;};
SessionSnapshot snapshot;
LRESULT SetupSessions() {
    injectedObservations=true;controller=Controller{};controller.config.perInput=true;controller.config.media=false;
    controller.config.monitors={{"A",{true,1}},{"B",{true,2}},{"C",{true,-1}}};
    displays={{"A","",L"Injected A",nullptr,{-640,0,0,360},true},{"B","",L"Injected B",nullptr,{0,0,640,360},true},{"C","",L"Injected C",nullptr,{640,0,1280,360},true}};
    fixtureModes={{"A",L"--preview-responsive"},{"B",L"--preview-responsive"}};
    controller.Topology({"A","B","C"},GetTickCount64());InvalidateMedia();return 1;
}
LRESULT StartSessions() {ManualAll();Reconcile();return 1;}
LRESULT SnapshotSessions() {
    Reconcile();snapshot={};snapshot.runs=runs.size();
    snapshot.powers=powerTasks.size();if(!powerTasks.empty())snapshot.powerPhase=powerTasks.front()->status;
    {std::lock_guard lock(registryLock);snapshot.windows=ownedWindows.size();snapshot.jobs=ownedJobs.size();}
    snapshot.a=controller.nodes["A"].state;snapshot.b=controller.nodes["B"].state;snapshot.c=controller.nodes["C"].state;
    snapshot.aGeneration=controller.nodes["A"].generation;snapshot.bGeneration=controller.nodes["B"].generation;
    for(auto& r:runs) if(r->window&&IsWindowVisible(r->window)) snapshot.hidden=false;
    return 1;
}
LRESULT DismissA() {controller.Input(GetTickCount64(),{"A"},true);Reconcile();return 1;}
LRESULT StopSessions() {Reset();Reconcile();return 1;}
LRESULT HungSession() {fixtureModes["B"]=L"--preview-hung";controller.Manual("B",GetTickCount64());Reconcile();return 1;}
LRESULT PauseSessions() {controller.paused=true;controller.Tick(GetTickCount64(),{});Reconcile();return 1;}
LRESULT ResumeSessions() {controller.paused=false;Reset();return 1;}
LRESULT BlockSessions() {locked=true;SetBlocked();Reconcile();return 1;}
LRESULT UnblockSessions() {locked=false;SetBlocked();return 1;}
LRESULT RemoveA() {controller.Topology({"B","C"},GetTickCount64());displays.erase(displays.begin());Reconcile();return 1;}
LRESULT ChangeSettings() {Config c=controller.config;for(auto& [id,p]:c.monitors){(void)id;p.enabled=false;}return Save(c);}
LRESULT ConfigureSession() {fixtureModes["A"]=L"--hang";return AddRun(displays[0],0,1,true)&&!AddRun(displays[0],0,1,true);}
void OnUi(LRESULT(*action)()) {DWORD_PTR result=0;Check(SendMessageTimeoutW(ui,WM_APP+101,0,reinterpret_cast<LPARAM>(action),SMTO_ABORTIFHUNG,2000,&result)&&result,"UI-owner production integration command");}
template<class Predicate> void Until(Predicate predicate,DWORD timeout,const char* what) {
    Time begin=GetTickCount64();do {OnUi(SnapshotSessions);if(predicate())return;Sleep(20);}while(GetTickCount64()-begin<timeout);Check(false,what);
}
void Sessions(bool soak) {
    CloseHandle(stopEvent);stopEvent=nullptr;Check(Initialize(),"integration initialize");OnUi(SetupSessions);OnUi(StartSessions);
    Until([]{return snapshot.runs==3&&snapshot.a==State::Saver&&snapshot.b==State::Saver&&snapshot.c==State::Black;},4000,"two saver sessions plus black through controller/reconciler");
    Check(snapshot.hidden&&snapshot.jobs==2&&snapshot.windows==3,"injected presentations hidden and independently owned");
    OnUi(DismissA);Until([]{return snapshot.runs==2;},3000,"independent A cleanup");
    Check(snapshot.b==State::Saver&&snapshot.c==State::Black,"B and C survive input on A");OnUi(StopSessions);Until([]{return snapshot.runs==0;},3000,"all session cleanup");
    if(!soak) {
        OnUi(HungSession);Until([]{return snapshot.b==State::Saver;},3000,"fixture becomes responsive before hanging");
        Until([]{return snapshot.b==State::Fallback&&snapshot.jobs==0;},6500,"hung-after-start fallback and owned cleanup");
        auto generation=snapshot.bGeneration;Sleep(200);OnUi(SnapshotSessions);Check(snapshot.bGeneration==generation&&snapshot.runs==1,"fallback keeps black without retry");
        OnUi(StopSessions);Until([]{return snapshot.runs==0;},3000,"fallback stop");
        OnUi(SetupSessions);OnUi(StartSessions);OnUi(PauseSessions);Until([]{return snapshot.runs==0;},3000,"pause cleanup");
        OnUi(ResumeSessions);OnUi(StartSessions);OnUi(BlockSessions);Until([]{return snapshot.runs==0;},3000,"lock/power gate cleanup");
        OnUi(UnblockSessions);OnUi(StartSessions);OnUi(ChangeSettings);Until([]{return snapshot.runs==0;},3000,"saved settings invalidate active sessions");
        OnUi(SetupSessions);OnUi(ConfigureSession);OnUi(StopSessions);Until([]{return snapshot.runs==0;},3000,"configuration job duplicate rejection/stop");
        OnUi(SetupSessions);OnUi(StartSessions);OnUi(RemoveA);Until([]{return snapshot.runs==0;},3000,"topology invalidates all old generations");
        puts("HARNESS sessions PASS: controller->reconciler->windows/workers; concurrent independent hidden fixtures; hung health; settings/pause/session/topology cleanup");
    } else {
        DWORD handles0=0,handles1=0;PROCESS_MEMORY_COUNTERS_EX before{},after{};
        GetProcessHandleCount(GetCurrentProcess(),&handles0);GetProcessMemoryInfo(GetCurrentProcess(),reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&before),sizeof(before));
        Time began=GetTickCount64();
        for(int i=0;i<100;++i) {
            OnUi(StartSessions);Until([]{return snapshot.a==State::Saver&&snapshot.b==State::Saver;},3000,"soak startup");
            OnUi(StopSessions);Until([]{return snapshot.runs==0;},3000,"soak cleanup");
            if(i%20==19){printf("HARNESS presentation cycle %d/100\n",i+1);fflush(stdout);}
        }
        GetProcessHandleCount(GetCurrentProcess(),&handles1);GetProcessMemoryInfo(GetCurrentProcess(),reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&after),sizeof(after));
        printf("HARNESS presentation soak: 100 cycles, %llu ms; handles %lu -> %lu; private bytes %zu -> %zu (hidden fixture processes, not GPU/display soak)\n",GetTickCount64()-began,handles0,handles1,static_cast<size_t>(before.PrivateUsage),static_cast<size_t>(after.PrivateUsage));
        Check(handles1<=handles0+4&&snapshot.jobs==0&&snapshot.windows==0,"no retained jobs/windows/handle growth after presentation soak");
    }
    Shutdown();injectedObservations=false;Check(runs.empty()&&ownedJobs.empty()&&ownedWindows.empty(),"integration final empty ownership");
}
void WorkerFixture(const wchar_t* args,bool healthy=false) {
    Run run; run.path=Executable();run.args=args;
    std::thread worker(RunWorker,&run);
    Time began=GetTickCount64();
    while(!run.done&&GetTickCount64()-began<8000) Sleep(20);
    if(!run.done) {run.cancel=true;SetEvent(stopEvent);}
    worker.join();Check(run.done&&run.status==(healthy?1:2),"production worker failure fallback");
    Check(ownedJobs.empty(),"production worker unregisters job");
    printf("PLATFORM worker %ls elapsed=%llu ms\n",args,GetTickCount64()-began);
}
void CaptureHidden(HWND window,const wchar_t* suffix) {
    RECT rect{};GetClientRect(window,&rect);HDC source=GetDC(window),memory=CreateCompatibleDC(source);HBITMAP bitmap=CreateCompatibleBitmap(source,rect.right,rect.bottom);auto old=SelectObject(memory,bitmap);
    // Compose the actual native paint paths offscreen. PrintWindow alone can
    // return success with black pixels on a noninteractive/hidden desktop.
    SendMessageW(window,WM_PRINTCLIENT,reinterpret_cast<WPARAM>(memory),PRF_CLIENT);
    for(HWND child=GetWindow(window,GW_CHILD);child;child=GetWindow(child,GW_HWNDNEXT)){
        RECT r{};GetWindowRect(child,&r);MapWindowPoints(nullptr,window,reinterpret_cast<POINT*>(&r),2);
        int saved=SaveDC(memory);IntersectClipRect(memory,r.left,r.top,r.right,r.bottom);SetViewportOrgEx(memory,r.left,r.top,nullptr);
        wchar_t cls[32]{};GetClassNameW(child,cls,32);bool custom=(GetPropW(child,L"DacOriginalProc")&&!theme.highContrast)||wcscmp(cls,L"Edit")==0;
        SendMessageW(child,custom?WM_PRINTCLIENT:WM_PRINT,reinterpret_cast<WPARAM>(memory),PRF_CLIENT|PRF_NONCLIENT|PRF_ERASEBKGND);
        RestoreDC(memory,saved);
    }
    Check(GetPixel(memory,0,0)==theme.colors.base,"native offscreen theme background pixel");
    UINT count=0,bytes=0;Gdiplus::GetImageEncodersSize(&count,&bytes);std::vector<BYTE> buffer(bytes);auto codecs=reinterpret_cast<Gdiplus::ImageCodecInfo*>(buffer.data());Gdiplus::GetImageEncoders(count,bytes,codecs);
    CLSID encoder{};bool found=false;for(UINT i=0;i<count;++i)if(wcscmp(codecs[i].MimeType,L"image/png")==0){encoder=codecs[i].Clsid;found=true;}Check(found,"PNG encoder");
    {Gdiplus::Bitmap image(bitmap,nullptr);Check(image.Save((Executable()+suffix).c_str(),&encoder,nullptr)==Gdiplus::Ok,"settings preview saved");}
    if(wcscmp(suffix,L".fujin-dark.png")==0){
        HICON preview=DacIcon(128,true);Check(preview!=nullptr,"scaled icon renders");
        {Gdiplus::Bitmap image(preview);Check(image.Save((Executable()+L".icon.png").c_str(),&encoder,nullptr)==Gdiplus::Ok,"icon preview saved");}DestroyIcon(preview);
    }
    SelectObject(memory,old);DeleteObject(bitmap);DeleteDC(memory);ReleaseDC(window,source);
}
void ExerciseSettingsDpi(HWND window,int editId,SettingsLayout& layout,int& scroll) {
    auto field=GetDlgItem(window,editId);SetDlgItemTextW(window,editId,L"777");SendMessageW(field,EM_SETSEL,1,2);
    SetWindowPos(window,nullptr,0,0,700,400,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
    SendMessageW(window,WM_VSCROLL,SB_PAGEDOWN,0);Check(scroll>0,"settings fixture scrolled before monitor transition");
    for(int cycle=0;cycle<3;++cycle)for(int dpi:{144,192,120,96}) {
        // Keep requested size within Windows' maximum tracking dimensions on
        // small CI desktops. Negative placement and every DPI are still tested.
        int width=std::min(MulDiv(700,dpi,96),std::max(300,GetSystemMetrics(SM_CXMAXTRACK)-32));
        // Quick setup explicitly requires 620 logical pixels; Windows honors
        // that minimum even when a synthetic DPI exceeds this desktop's scale.
        if(window==setupWindow)width=std::max(width,MulDiv(620,dpi,96));
        RECT suggested{-1500,70,-1500+width,470};
        auto previousFont=reinterpret_cast<HFONT>(SendMessageW(field,WM_GETFONT,0,0));
        SendMessageW(window,WM_DPICHANGED,MAKEWPARAM(dpi,dpi),reinterpret_cast<LPARAM>(&suggested));
        auto currentFont=reinterpret_cast<HFONT>(SendMessageW(field,WM_GETFONT,0,0));
        Check(currentFont&&currentFont!=previousFont&&GetObjectType(currentFont)==OBJ_FONT&&GetObjectType(previousFont)!=OBJ_FONT,"DPI transition releases replaced font and installs live replacement");
        Check(IsWindow(window)&&GetDlgItem(window,editId)==field,"DPI transition preserves window and control handles");
        Check(ControlText(window,editId)==L"777"&&layout.dpi==dpi,"DPI transition preserves unsaved text and applies new scale");
        DWORD start=0,end=0;SendMessageW(field,EM_GETSEL,reinterpret_cast<WPARAM>(&start),reinterpret_cast<LPARAM>(&end));Check(start==1&&end==2,"DPI transition preserves edit selection");
        RECT actual{};GetWindowRect(window,&actual);
        if(actual.left!=suggested.left||actual.top!=suggested.top||actual.right!=suggested.right)fprintf(stderr,"DPI %d requested %ld,%ld,%ld,%ld actual %ld,%ld,%ld,%ld\n",dpi,suggested.left,suggested.top,suggested.right,suggested.bottom,actual.left,actual.top,actual.right,actual.bottom);
        Check(actual.left==suggested.left&&actual.top==suggested.top&&actual.right==suggested.right,"DPI suggested position honored including negative coordinates");
        SCROLLINFO info{};info.cbSize=sizeof(info);info.fMask=SIF_ALL;GetScrollInfo(window,SB_VERT,&info);Check(scroll==info.nPos&&scroll>0,"DPI transition retains valid scroll offset");
        for(auto& c:layout.controls){RECT rect{};GetWindowRect(c.window,&rect);MapWindowPoints(nullptr,window,reinterpret_cast<POINT*>(&rect),2);
            Check(rect.left==MulDiv(c.x,dpi,96)&&rect.top==MulDiv(c.y,layout.dpi,96)-scroll&&rect.right-rect.left==MulDiv(c.width,dpi,96),"controls reflow from original logical coordinates without drift");}
    }
    // Process-wide GDI counts can change as other UI/COM workers initialize.
    // Assert each owned font's lifetime above; lifecycle covers aggregate leaks.
}
LRESULT AdvancedUi() {
    ShowSettings();Check(editor&&GetDlgItem(editor,149)&&GetDlgItem(editor,144),"advanced monitor controls created");
    auto edit=GetDlgItem(editor,101),checkbox=GetDlgItem(editor,146),combo=GetDlgItem(editor,122);
    SetWindowTextW(edit,L"777");CheckDlgButton(editor,146,BST_CHECKED);auto choice=SendMessageW(combo,CB_GETCURSEL,0,0);
    DWORD resources=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);
    for(int pass=0;pass<4;++pass)for(int mode:{0,1,2}){
        testTheme=mode;RefreshSettingsTheme();
        Check(edit==GetDlgItem(editor,101)&&ControlText(editor,101)==L"777"&&IsDlgButtonChecked(editor,146)==BST_CHECKED&&SendMessageW(combo,CB_GETCURSEL,0,0)==choice,"theme transitions retain controls and drafts");
        Check(theme.highContrast==(mode==2),"high contrast overrides Fujin");
        HDC dc=GetDC(edit);auto brush=SendMessageW(editor,WM_CTLCOLOREDIT,reinterpret_cast<WPARAM>(dc),reinterpret_cast<LPARAM>(edit));
        if(mode!=2)Check(brush==reinterpret_cast<LRESULT>(theme.surface)&&GetTextColor(dc)==theme.colors.text&&GetBkColor(dc)==theme.colors.surface,"native edit receives Fujin text and surface colors");ReleaseDC(edit,dc);
        LOGFONTW font{};GetObjectW(editorFont,sizeof(font),&font);if(mode!=2)Check(wcscmp(font.lfFaceName,fujin::fontFamily)==0,"Fujin native font");
        if(pass==0)CaptureHidden(editor,mode==0?L".fujin-light.png":mode==1?L".fujin-dark.png":L".high-contrast.png");
        if(pass==0&&mode==2)resources=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS); // warm native theme/font/codec caches
    }
    Check(GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS)<=resources,"theme refresh has no GDI growth");
    testTheme=1;RefreshSettingsTheme();
    SendMessageW(checkbox,BM_CLICK,0,0);Check(IsDlgButtonChecked(editor,146)==BST_UNCHECKED,"themed checkbox keeps native toggle behavior");
    Check(SendMessageW(combo,CB_GETCOUNT,0,0)==11,"themed combo retains all monitor saver options");
    ICONINFO icon{};Check(theme.small&&theme.large&&theme.active&&GetIconInfo(theme.small,&icon),"custom tray and window icons exist");DeleteObject(icon.hbmMask);DeleteObject(icon.hbmColor);
    CaptureHidden(editor,L".settings.png");
    CheckDlgButton(editor,146,BST_CHECKED);auto selected=selection;ExerciseSettingsDpi(editor,101,editorLayout,editorScroll);
    Check(editor&&selection==selected&&IsDlgButtonChecked(editor,146)==BST_CHECKED,"main monitor draft survives move");
    ShowSaverOptions();Check(options&&GetDlgItem(options,201)&&GetDlgItem(options,220),"saver options created");
    ExerciseSettingsDpi(options,205,optionsLayout,optionsScroll);Check(editor&&!IsWindowEnabled(editor),"moving options retains modal ownership");
    CaptureHidden(options,L".options.png");
    Check(SendDlgItemMessageW(options,206,CB_GETCOUNT,0,0)==6,"six slideshow placements");
    SetDlgItemInt(options,205,11,FALSE);SendMessageW(options,WM_COMMAND,220,0);Check(!options&&draft.monitors[displays[selection].id].slideSeconds==11,"options apply to draft");
    DestroyWindow(editor);testTheme=-1;RefreshSettingsTheme();Check(!optionsFont&&!editorFont,"advanced UI font cleanup");return 1;
}
LRESULT SpanSession(){return Span(-1);}
LRESULT VerifySpan(){return runs.size()==1&&runs.front()->id=="@span"&&spanningDisplay.rect.left==-640&&spanningDisplay.rect.right==1280;}
LRESULT RejectSpan(){controller.config.monitors["C"].enabled=false;bool rejected=!Span(-1);controller.config.monitors["C"].enabled=true;return rejected;}
LRESULT ConfigurationFailure(){fixtureModes["A"]=L"--early";return AddRun(displays[0],0,1,true);}
LRESULT VerifyConfigurationFailure(){Reconcile();return configurationFailures==1;}
void Advanced() {
    CloseHandle(stopEvent);stopEvent=nullptr;trayFailures=1;Check(Initialize(),"tray unavailable is recoverable startup");
    OnUi(+[]()->LRESULT{
        Check(!trayPresent&&controller.blocked,"injected tray failure blocks controller");
        printf("ADVANCED initial session flags: locked=%d suspended=%d displayOff=%d; injecting awake session for tray recovery fixture\n",locked,suspended,displayOff);
        // Only the harness fixture is awake; the real desktop/session is untouched.
        locked=suspended=displayOff=false;trayFailures=0;Tray();SetBlocked();return trayPresent&&!controller.blocked;
    });OnUi(SetupSessions);OnUi(AdvancedUi);
    OnUi(SpanSession);Until([]{return snapshot.runs==1;},2000,"single desktop-spanning host");OnUi(VerifySpan);
    OnUi(DismissA);Until([]{return snapshot.runs==0;},2000,"span dismisses on activity anywhere");OnUi(RejectSpan);
    OnUi(ConfigurationFailure);Until([]{return snapshot.runs==0;},3000,"configuration failure cleaned");OnUi(VerifyConfigurationFailure);
    OnUi(+[]()->LRESULT{controller.config.hotkeys=true;RegisterControls();SendMessageW(ui,WM_HOTKEY,11,0);return controller.Any()&&controller.nodes["A"].sticky;});
    OnUi(+[]()->LRESULT{SendMessageW(ui,WM_HOTKEY,11,0);return !controller.Any();});Until([]{return snapshot.runs==0;},3000,"toggle-all cleanup");
    Shutdown();injectedObservations=false;puts("PLATFORM advanced PASS: Fujin light/dark/high-contrast painting and draft retention, native controls, icon creation, DPI/resource stability, recoverable tray startup, options/draft, span bounds/disabled exclusion, configuration error, sticky hotkey dispatch; no real hotkey registration or tray visibility claim");
}
void PowerTests() {
    configPath=Executable()+L".power-tests";const std::string id="injected-power-target";const auto path=PowerMarker(id);DeleteFileW(Extended(path).c_str());
    std::string token(32,'a');Check(PowerTicket(id+"\n"+token+"\nprobe",id,token,false)&&!PowerTicket(id+"\n"+token+"\noff",id,token,false),"off ticket single-use phase");
    Check(!PowerTicket(id+"\n"+token+"\nchanging",id,std::string(32,'b'),true)&&PowerTicket(id+"\n"+token+"\noff",id,token,true),"wake ticket operation binding");
    auto run=[&](int result,bool earlyCancel,bool wakeFail){
        fakePowerResult=result;fakePowerWakeResult=wakeFail?12:0;fakePowerDelay=earlyCancel?100:0;fakePowerOff=0;fakePowerOn=0;
        PowerTask task;task.id=id;std::thread worker(PowerWorker,&task);Time began=GetTickCount64();
        if(earlyCancel){Sleep(20);task.cancel=true;}else{while(!task.done&&task.status==0&&GetTickCount64()-began<2000)Sleep(1);task.cancel=true;}
        worker.join();Check(task.done,"power worker completed");return int(task.status.load());
    };
    Check(run(0,false,false)==4&&fakePowerOff==1&&fakePowerOn==1&&!PowerPending(id),"accepted off is woken once on cancellation");
    Check(run(0,true,false)==2&&fakePowerOff==1&&fakePowerOn==0&&!PowerPending(id),"cancellation during query prevents off and unnecessary wake");
    Check(run(11,false,false)==2&&fakePowerOn==0&&!PowerPending(id),"unsupported read never triggers wake");
    Check(run(12,false,false)==4&&fakePowerOn==1&&!PowerPending(id),"ambiguous failed write receives recovery wake");
    Check(run(0,false,true)==3&&fakePowerOn==1&&PowerPending(id),"failed wake retains quarantine");
    Check(run(0,false,false)==2&&fakePowerOff==0&&PowerPending(id),"quarantine blocks later off without DDC probe");
    DeleteFileW(Extended(path).c_str());controller=Controller{};Display display{id,"",L"Power fixture",nullptr,{0,0,100,100},true};
    fakePowerProcess=1;Check(run(0,false,false)==4&&!PowerPending(id),"real child transport observes off, signals cancellation and verifies awake acknowledgement");
    fakePowerProcess=2;Check(run(0,false,false)==3&&PowerPending(id),"zero process exit without acknowledgement is never success");DeleteFileW(Extended(path).c_str());fakePowerProcess=0;
    Check(StartPower(display,1)==0,"hardware disabled by default");controller.config.monitors[id].hardware=true;fakePowerDelay=100;
    Check(StartPower(display,1)==1&&StartPower(display,2)==-1,"one per-target worker; busy distinguished from rejection");StopPower();for(auto& task:powerTasks)task->worker.join();powerTasks.clear();DeleteFileW(Extended(path).c_str());
    fakePowerDelay=0;fakePowerWakeResult=0;puts("PLATFORM power PASS: simulated DDC only; operation tickets, off/wake/cancel, unsupported and ambiguous responses, wake quarantine, default opt-out and serialized generations");
}
std::wstring PhotoFixture() {
    auto folder=Executable()+L".photos";Check(CreateDirectoryW(folder.c_str(),nullptr)||GetLastError()==ERROR_ALREADY_EXISTS,"photo fixture directory");
    UINT count=0,bytes=0;Gdiplus::GetImageEncodersSize(&count,&bytes);std::vector<BYTE> memory(bytes);auto codecs=reinterpret_cast<Gdiplus::ImageCodecInfo*>(memory.data());Check(Gdiplus::GetImageEncoders(count,bytes,codecs)==Gdiplus::Ok,"image encoders");
    CLSID encoder{};bool found=false;for(UINT i=0;i<count;++i)if(wcscmp(codecs[i].MimeType,L"image/bmp")==0){encoder=codecs[i].Clsid;found=true;}Check(found,"BMP encoder");
    Gdiplus::Bitmap image(20,10,PixelFormat32bppARGB);Gdiplus::Graphics graphics(&image);graphics.Clear(Gdiplus::Color(255,255,0,0));
    Check(image.Save((folder+L"\\one.bmp").c_str(),&encoder,nullptr)==Gdiplus::Ok,"photo fixture save");return folder;
}
void Slides() {
    CloseHandle(stopEvent);stopEvent=nullptr;Check(Initialize(),"slideshow initialize");OnUi(SetupSessions);
    auto folder=PhotoFixture();Preference p;p.background=0x0000ff;
    for(int placement=0;placement<6;++placement){p.placement=placement;auto frame=SlideFrame(folder+L"\\one.bmp",100,100,p);Check(frame&&frame->GetWidth()==100&&frame->GetHeight()==100,"all placement frames rendered");
        Gdiplus::Color pixel;frame->GetPixel(50,50,&pixel);Check(pixel.GetR()>200,"photo remains centered or tiled");if(placement==0){frame->GetPixel(0,0,&pixel);Check(pixel.GetB()>200,"fit letterbox uses selected background");}}
    std::wstring corrupt=folder+L"\\bad.png";DWORD error=0;Check(WriteFileText(corrupt,"not an image",error),"corrupt fixture");Check(!SlideFrame(corrupt,100,100,p),"malformed image rejected");
    OnUi(+[]()->LRESULT{fixtureModes.clear();auto folder=Executable()+L".photos";for(auto id:{"A","B"}){auto& p=controller.config.monitors[id];p.saver=7;p.folder=Utf8(folder);p.slideSeconds=5;}controller.effectiveReady=false;controller.SelectPolicy(GetTickCount64(),0);ManualAll();return 1;});
    Until([]{return snapshot.a==State::Saver&&snapshot.b==State::Saver&&snapshot.runs==3;},4000,"two independent native slideshows plus black");Check(snapshot.jobs==0,"slideshow owns no saver process");
    OnUi(DismissA);Until([]{return snapshot.runs==2;},2000,"photo A independent dismissal");Check(snapshot.b==State::Saver,"photo B continues");OnUi(StopSessions);Until([]{return snapshot.runs==0;},2000,"photo frames and windows released");
    OnUi(+[]()->LRESULT{controller.config.monitors["A"].folder="C:\\dac-nonexistent-photo-fixture";controller.effectiveReady=false;controller.SelectPolicy(GetTickCount64(),0);controller.Manual("A",GetTickCount64());Reconcile();return 1;});
    Until([]{return snapshot.a==State::Fallback;},2500,"empty/missing folder uses black fallback");Shutdown();injectedObservations=false;
    puts("PLATFORM slideshow PASS: rendered six placements/background, malformed image, concurrent independent native frames, dismissal, missing-folder black fallback; hidden windows only");
}

std::array<XINPUT_STATE,4> injectedPads{};std::array<bool,4> injectedConnected{};unsigned padCalls=0;
DWORD WINAPI InjectedXInput(DWORD slot,XINPUT_STATE* state){++padCalls;if(!injectedConnected[slot])return ERROR_DEVICE_NOT_CONNECTED;*state=injectedPads[slot];return ERROR_SUCCESS;}
void ControllerAdapterTests(){XInputAdapter adapter;adapter.get=InjectedXInput;AdapterSnapshot sample;injectedConnected[0]=true;injectedPads[0].Gamepad.wButtons=XINPUT_GAMEPAD_A;
    adapter.Sample(0,true,sample);Check(sample.active&&sample.connected,"XInput initial held control");sample.active=false;adapter.Sample(250,true,sample);Check(sample.active&&sample.inputAt==250,"unchanged packet held control continues activity");injectedPads[0].Gamepad.wButtons=0;adapter.Sample(500,true,sample);Check(sample.active,"XInput release counts once");adapter.Sample(750,true,sample);Check(!sample.active,"neutral does not sustain activity");
    injectedPads[0].dwPacketNumber++;injectedPads[0].Gamepad.sThumbLX=100;adapter.Sample(1000,true,sample);Check(!sample.active,"noisy changed packet inside deadzone ignored");injectedConnected[0]=false;adapter.Sample(1250,true,sample);Check(!sample.connected&&!sample.active,"unplug clears held state");auto count=padCalls;adapter.Sample(1500,true,sample);Check(padCalls==count,"disconnected polling bounded");injectedConnected[0]=true;injectedPads[0].Gamepad={};adapter.Sample(3250,true,sample);Check(sample.connected&&!sample.active,"neutral reconnect is not activity");adapter.get=nullptr;adapter.Sample(3500,true,sample);Check(!sample.controllerAvailable&&!sample.active,"missing optional API defined inactive");}
LRESULT NextMigration(){auto savedPath=configPath;configPath=Executable()+L".nextbeta-settings";auto backup=configPath+L".v2-backup";DeleteFileW(Extended(backup).c_str());DWORD error=0;
    std::string original="\xef\xbb\xbf# reordered schema two fixture\ntimeout=17\nversion=2\nautomatic=0\nmonitor.41=0,2\npolicy.41=0,1,-1,0,0,0,0\nassets.41=||30|0|0|0|0\n",read;
    auto marker=PowerMarker("fixture-private-identity");std::string recovery="fixture-private-identity\nPRIVATE-RECOVERY-SECRET\nfault";Check(WriteFileText(marker,recovery,error)&&WriteFileText(configPath,original,error),"isolated schema2/recovery fixtures");Config next;next.monitors["A"].enabled=false;
    Check(WriteFileText(backup,"junk",error)&&!Save(next),"invalid preexisting rollback backup blocks migration");Check(ReadFileText(configPath,read,error,true)&&read==original,"rejected backup preserves exact source including BOM");DeleteFileW(Extended(backup).c_str());
    Check(Save(next),"first schema2 to schema3 transaction");Check(ReadFileText(backup,read,error,true)&&read==original,"migration backup byte exact");Config parsed;std::string parseError;Check(ReadFileText(configPath,read,error)&&Parse(read,parsed,parseError)&&parsed.sourceSchema==3&&!parsed.monitors["A"].enabled,"forward migration and disabled monitor survive");
    auto live=Serialize(controller.config);{Handle held(CreateFileW(Extended(configPath).c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,0,nullptr));Check(held&&!Save(Config{})&&Serialize(controller.config)==live,"failed replacement keeps active preferences");}
    Check(WriteFileText(configPath,"version=999\n",error)&&!Save(next),"future schema ordinary save fails closed");Check(ReadFileText(configPath,read,error)&&read=="version=999\n","future source preserved");Check(Save(Config{},true,true),"explicit reset can preserve and replace unsupported bytes");
    bool resetBackup=false;WIN32_FIND_DATAW item{};HANDLE find=FindFirstFileW(Extended(configPath+L".reset-backup-*").c_str(),&item);if(find!=INVALID_HANDLE_VALUE){auto dir=configPath.substr(0,configPath.find_last_of(L'\\')+1);do{std::string bytes;if(ReadFileText(dir+item.cFileName,bytes,error,true)&&bytes=="version=999\n")resetBackup=true;}while(FindNextFileW(find,&item));FindClose(find);}Check(resetBackup,"reset backup retains exact invalid original");
    Check(CopyFileW(Extended(backup).c_str(),Extended(configPath).c_str(),FALSE)&&ReadFileText(configPath,read,error,true)&&read==original,"documented rollback copies exact schema2 backup");Check(ReadFileText(configPath,read,error)&&Parse(read,parsed,parseError)&&parsed.sourceSchema==2&&parsed.monitors["A"].input==1&&!parsed.monitors["A"].enabled,"rollback schema2 parser fixture");Check(ReadFileText(marker,read,error)&&read==recovery,"migration reset rollback leave recovery bytes untouched");
    Policy imported;Config portable=Portable(parsed);Check(!portable.automatic&&!portable.monitors["A"].hardware,"portable excludes automatic and hardware grants");Check(!MapImported(portable,{},imported)&&MapImported(portable,{{"A","mapped-disconnected"}},imported)&&!imported.monitors["mapped-disconnected"].enabled&&!imported.monitors["mapped-disconnected"].hardware,"explicit remap required, disabled/disconnected retained");
    configPath=savedPath;return 1;
}
LRESULT NextUi(){ShowSettings();Check(editor&&GetDlgItem(editor,150)&&GetDlgItem(editor,160),"new stage/preview controls");SetDlgItemInt(editor,150,42,FALSE);SetDlgItemInt(editor,152,7,FALSE);SendDlgItemMessageW(editor,122,CB_SETCURSEL,9,0);StoreMonitorDraft();auto selected=editorDisplays[selection].id;
    ShowWorkspace();Check(workspace&&GetDlgItem(workspace,340)&&ControlText(workspace,303)==L"09:00","plain-language rules and HH:MM schedule controls");SetDlgItemTextW(workspace,343,L"C:\\trusted\\game.exe");SendMessageW(workspace,WM_COMMAND,323,0);Check(workspaceRules.size()==1&&workspaceRules[0].executable=="C:\\trusted\\game.exe","rule form drives validated production rule");SetDlgItemTextW(workspace,301,L"Fixture Work");SendMessageW(workspace,WM_COMMAND,310,0);Check(draft.profiles.size()==1&&draft.profiles[0].policy.monitors[selected].dim==42,"profile captures unsaved monitor policy");
    SetDlgItemTextW(workspace,343,L"unsaved.exe");ExerciseSettingsDpi(workspace,343,workspaceLayout,workspaceScroll);Check(workspaceRules.size()==1&&draft.profiles.size()==1,"workspace DPI preserves rule/profile draft");testTheme=2;RefreshSettingsTheme();Check(ControlText(workspace,343)==L"777"&&GetObjectType(workspaceFont)==OBJ_FONT,"workspace high contrast preserves draft and font");testTheme=-1;RefreshSettingsTheme();SendMessageW(workspace,WM_COMMAND,318,0);Check(!workspace&&draft.monitors[selected].dim==42&&GetDlgItemInt(editor,150,nullptr,FALSE)==42,"selected profile loads into reviewable main draft");
    ShowSaverOptions();SetDlgItemTextW(options,201,L"C:\\unsaved\\fixture.scr");auto editorHandle=editor,optionsHandle=options;topologyPending=true;SendMessageW(ui,WM_TIMER,1,0);Check(editor==editorHandle&&options==optionsHandle&&ControlText(options,201)==L"C:\\unsaved\\fixture.scr"&&draft.monitors[selected].dim==42,"topology refresh retains main/options draft by original identity");DestroyWindow(options);DestroyWindow(editor);
    ShowSetup();Check(setupWindow&&GetDlgItem(setupWindow,407),"reopenable quick setup");RECT suggested{-1200,50,-300,500};SendMessageW(setupWindow,WM_DPICHANGED,MAKEWPARAM(144,144),reinterpret_cast<LPARAM>(&suggested));Check(IsWindow(setupWindow)&&setupLayout.dpi==144,"setup DPI ownership");DestroyWindow(setupWindow);IdentifyDisplays();auto labels=identifyWindows.size();Check(labels==displays.size(),"one identity label per current output");IdentifyDisplays();Check(identifyWindows.size()==labels,"identify repeated action does not accumulate overlays");for(auto& item:identifyWindows)item.second=0;SendMessageW(ui,WM_TIMER,1,0);Check(identifyWindows.empty()&&identifyLabels.empty(),"identify timeout closes every overlay without power");
    Check(ConflictText().find(L"Windows screensaver")!=std::wstring::npos,"read-only OS conflict query reports result/errors");return 1;
}
LRESULT QuickSetupUi(){
    SetupSessions();auto savedPath=configPath;configPath=Executable()+L".quick-setup-settings";DeleteFileW(Extended(configPath).c_str());
    controller.config.monitors["disconnected"].folder="C:\\retained";
    controller.config.monitors["A"].hardware=true;controller.config.monitors["A"].powerAfter=0;
    controller.config.perInput=false;
    auto original=Serialize(controller.config);ShowSetup();Check(setupWindow&&!editor,"quick setup opens without full editor");
    Check(IsDlgButtonChecked(setupWindow,408)==BST_UNCHECKED,"existing global-input preference is visible in quick setup");
    SendDlgItemMessageW(setupWindow,408,BM_CLICK,0,0);
    Check(IsDlgButtonChecked(setupWindow,408)==BST_CHECKED&&setupEdited&&!controller.config.perInput,"independent input checkbox edits the draft only");
    for(int mode:{0,1,2}){testTheme=mode;RefreshSettingsTheme();CaptureHidden(setupWindow,mode==0?L".quick-setup-light.png":mode==1?L".quick-setup-dark.png":L".quick-setup-contrast.png");}testTheme=-1;RefreshSettingsTheme();
    LOGFONTW font{};GetObjectW(setupFont,sizeof(font),&font);Check(abs(font.lfHeight)>=MulDiv(18,setupLayout.dpi,96),"quick setup larger default type");
    SetDlgItemTextW(setupWindow,404,L"4");SendMessageW(setupWindow,WM_COMMAND,406,0);Check(setupWindow&&Serialize(controller.config)==original,"invalid idle time leaves setup and live settings intact");
    ExerciseSettingsDpi(setupWindow,404,setupLayout,setupScroll);
    testTheme=2;RefreshSettingsTheme();GetObjectW(setupFont,sizeof(font),&font);Check(abs(font.lfHeight)>=MulDiv(18,setupLayout.dpi,96)&&ControlText(setupWindow,404)==L"777","high contrast retains larger font and draft");testTheme=-1;RefreshSettingsTheme();
    CheckDlgButton(setupWindow,402,BST_UNCHECKED);SendDlgItemMessageW(setupWindow,403,CB_SETCURSEL,1,0);
    auto topology=displays;std::reverse(displays.begin(),displays.end());
    SendDlgItemMessageW(setupWindow,401,CB_SETCURSEL,1,0);SendMessageW(setupWindow,WM_COMMAND,MAKEWPARAM(401,CBN_SELCHANGE),0);
    Check(setupDraft.monitors["A"].timeout==777&&!setupDraft.monitors["A"].enabled&&setupDraft.monitors["A"].saver==8,"setup selection follows original stable display identity after reorder");
    SendMessageW(setupWindow,WM_COMMAND,4,0);Check(!setupWindow&&editor&&draft.perInput&&draft.monitors["A"].timeout==777&&Serialize(controller.config)==original,"advanced continues unsaved quick setup draft");
    ShowSetup();Check(setupWindow&&!IsWindowEnabled(editor),"quick setup owns editing while full editor draft remains open");
    SendMessageW(setupWindow,WM_COMMAND,3,0);Check(!setupWindow&&IsWindowEnabled(editor)&&draft.monitors["A"].timeout==777,"cancel restores editor without losing its draft");
    ShowSetup();SendMessageW(setupWindow,WM_COMMAND,406,0);Check(setupWindow&&editor&&controller.config.perInput&&controller.config.monitors["A"].timeout==777&&!setupEdited,"quick setup saves through canonical transaction and stays open");SendMessageW(setupWindow,WM_COMMAND,3,0);Check(!setupWindow&&IsWindowEnabled(editor),"close after save is independent and restores editor");
    Check(controller.config.monitors["A"].hardware&&controller.config.monitors["disconnected"].folder=="C:\\retained","setup preserves advanced and disconnected preferences");
    DestroyWindow(editor);ShowSetup();
    Check(IsDlgButtonChecked(setupWindow,408)==BST_CHECKED,"saved independent input persists when quick setup reopens");
    SendDlgItemMessageW(setupWindow,408,BM_CLICK,0,0);setupCloseResponse=IDYES;SendMessageW(setupWindow,WM_COMMAND,3,0);
    Check(!setupWindow&&controller.config.perInput,"discarding independent input edits preserves the saved choice");
    ShowSetup();controller.config.timeout=123;SendMessageW(setupWindow,WM_COMMAND,406,0);Check(setupWindow&&controller.config.timeout==123,"concurrent saved preference change rejects stale setup save");DestroyWindow(setupWindow);
    ShowSetup();auto current=Serialize(controller.config);{Handle held(CreateFileW(Extended(configPath).c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,0,nullptr));Check(!!held,"hold setup save target");SendMessageW(setupWindow,WM_COMMAND,406,0);Check(setupWindow&&Serialize(controller.config)==current,"failed setup save retains draft and active settings");}DestroyWindow(setupWindow);
    displays.clear();ShowSetup();Check(!IsWindowEnabled(GetDlgItem(setupWindow,402)),"no-display setup disables monitor controls");Check(StoreSetupDraft(),"no-display setup draft remains valid");DestroyWindow(setupWindow);
    displays=topology;ShowSetup();legacyStartupOverride=1;
    CheckDlgButton(setupWindow,405,BST_CHECKED);SendMessageW(setupWindow,WM_COMMAND,MAKEWPARAM(405,BN_CLICKED),0);
    SendMessageW(setupWindow,WM_COMMAND,406,0);
    Check(setupWindow&&ControlText(setupWindow,410).find(L"old OLED Aegis startup")!=std::wstring::npos&&setupEdited,"legacy blocker appears inline without discarding draft");
    CheckDlgButton(setupWindow,405,BST_UNCHECKED);SendMessageW(setupWindow,WM_COMMAND,406,0);
    Check(setupWindow&&!setupEdited&&!controller.config.automatic&&saveFailure.empty(),"manual save succeeds with legacy startup and clears old error");legacyStartupOverride=-1;
    SetDlgItemTextW(setupWindow,404,L"43");setupCloseResponse=IDNO;SendMessageW(setupWindow,WM_CLOSE,0,0);
    Check(setupWindow&&ControlText(setupWindow,404)==L"43","declining discard keeps unsaved controls");
    setupCloseResponse=IDYES;SendMessageW(setupWindow,WM_COMMAND,IDCANCEL,0);Check(!setupWindow,"Escape confirms discard separately from Save");setupCloseResponse=IDNO;
    // More than two displays: each save retains the selection and earlier saves.
    controller=Controller{};displays.clear();for(int i=0;i<6;++i){auto id="DisplayFixture"+std::to_string(i);displays.push_back({id,"",L"Panel "+std::to_wstring(i+1),nullptr,{i*100,0,(i+1)*100,100},true});}
    ShowSetup();auto window=setupWindow;
    for(int i=0;i<6;++i){
        SendDlgItemMessageW(window,401,CB_SETCURSEL,i,0);SendMessageW(window,WM_COMMAND,MAKEWPARAM(401,CBN_SELCHANGE),0);
        SetDlgItemInt(window,404,30+i,FALSE);SendDlgItemMessageW(window,403,CB_SETCURSEL,i%3,0);
        SendMessageW(window,WM_COMMAND,406,0);
        Check(setupWindow==window&&setupSelection==i&&!setupEdited&&ControlText(window,410).find(L"Setup saved")!=std::wstring::npos,"repeated Save retains window selection and success feedback");
        Config disk;std::string bytes,error;DWORD code=0;Check(ReadFileText(configPath,bytes,code)&&Parse(bytes,disk,error),"six-display save persists valid configuration");
        for(int j=0;j<=i;++j)Check(disk.monitors[displays[j].id].timeout==30+j,"each save preserves earlier display settings");
    }
    SetWindowPos(window,nullptr,0,0,620,700,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
    SetupStatus(L"Turn off Automatic to save now, or disable the old OLED Aegis startup entry and exit that app before enabling Automatic.");
    RECT status{},save{},close{},advanced{};GetWindowRect(GetDlgItem(window,410),&status);GetWindowRect(GetDlgItem(window,406),&save);GetWindowRect(GetDlgItem(window,3),&close);GetWindowRect(GetDlgItem(window,4),&advanced);
    Check(status.bottom<=save.top&&advanced.right<save.left&&save.right<close.left,"wrapped status and footer controls do not overlap at minimum width");
    for(int id:{4,406,3,407}){
        auto button=GetDlgItem(window,id);RECT bounds{};GetClientRect(button,&bounds);HDC dc=GetDC(button);auto font=reinterpret_cast<HFONT>(SendMessageW(button,WM_GETFONT,0,0));auto old=SelectObject(dc,font);
        RECT text{};auto label=ControlText(window,id);DrawTextW(dc,label.c_str(),-1,&text,DT_CALCRECT|DT_SINGLELINE);SelectObject(dc,old);ReleaseDC(button,dc);
        Check(text.right+MulDiv(fujin::spacing,setupLayout.dpi,96)<=bounds.right,"setup action labels fit without ellipsis");
    }
    SendMessageW(window,WM_COMMAND,3,0);Check(!setupWindow&&!setupHeadingFont,"clean close releases heading font");
    displays=topology;configPath=savedPath;SetupSessions();return 1;
}
LRESULT NextDiagnostics(){controller.config.monitors["A"].custom="C:\\Users\\PRIVATE-USER\\SECRET.scr";for(int i=0;i<300;++i)Record("PRIVATE-ID",std::string(500,'x')+"PRIVATE-SECRET",123);Record("Display 1",ReasonText(Reason::Fallback));auto report=DiagnosticText();{std::lock_guard lock(diagnosticLock);Check(diagnosticEvents.size()==128&&diagnosticEvents.front().event.size()<=96,"bounded reason/event memory");}Check(report.size()<24000&&report.find("PRIVATE")==std::string::npos&&report.find("SECRET")==std::string::npos&&report.find("C:\\")==std::string::npos&&report.find("event redacted")!=std::string::npos&&report.find("Presentation failed")!=std::string::npos,"diagnostics default redacts identities paths secrets, retains failure category");DWORD error=0;Check(WriteFileText(Executable()+L".redacted-diagnostics.txt",report,error),"diagnostics export works after prior failed operation");return 1;}
LRESULT NextMapping(){SetupSessions();ShowSettings();draft.monitors["disconnected"].enabled=false;draft.monitors["disconnected"].folder="C:\\retained";Profile small;small.name="Imported";small.policy.monitors["A"].dim=37;draft.profiles={small};ShowWorkspace();importPending=true;importSources={"source"};importTargets=displays;pendingMapping.clear();SendDlgItemMessageW(workspace,331,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(L"source"));SendDlgItemMessageW(workspace,331,CB_SETCURSEL,0,0);for(auto label:{L"Keep disconnected",L"A",L"B",L"C"})SendDlgItemMessageW(workspace,332,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(label));SendDlgItemMessageW(workspace,332,CB_SETCURSEL,1,0);auto saved=displays;displays.erase(displays.begin());SendMessageW(workspace,WM_COMMAND,319,0);Check(pendingMapping.empty(),"stale visible import target cannot silently map to another output");SendDlgItemMessageW(workspace,332,CB_SETCURSEL,2,0);SendMessageW(workspace,WM_COMMAND,319,0);Check(pendingMapping["source"]=="B","surviving import target bound by stable ID after reorder");displays=saved;SendDlgItemMessageW(workspace,300,CB_SETCURSEL,0,0);SendMessageW(workspace,WM_COMMAND,318,0);Config checked;Check(Commit(checked,draft,[](const std::string&){return true;})&&checked.monitors["A"].dim==37&&!checked.monitors["disconnected"].enabled&&checked.monitors["disconnected"].folder=="C:\\retained","profile draft load/save preserves omitted disconnected preferences");DestroyWindow(editor);return 1;}
LRESULT NextAdapterConsumer(){controller=Controller{};controller.config.controllerInput=true;controller.config.automatic=true;controller.config.media=false;controller.config.timeout=5;controller.config.monitors["A"].media=0;controller.config.monitors["B"].media=1;controller.Topology({"A","B"},4000);controller.foreground.monitors={"A"};controller.foreground.valid=true;consumedControllerInput=0;AdapterSnapshot held;held.at=held.inputAt=10000;held.active=held.connected=held.controllerAvailable=true;ConsumeAdapters(held,10000);controller.Tick(10000,{});Check(controller.nodes["A"].state==State::Desktop,"production adapter consumer credits held input");ConsumeAdapters(held,20000);controller.Tick(20000,{});Check(controller.nodes["A"].state==State::Suppressed&&controller.Explain("A",{},20000).primary==Reason::Stale&&controller.Explain("A",{},20000).kind==DeadlineKind::None,"stalled shared media observer cannot blank controller-enabled media-opt-out output");AdapterSnapshot missing;missing.at=22000;ConsumeAdapters(missing,22000);controller.Tick(22000,{});Check(!controller.activityStale&&!controller.Any(),"fresh missing API clears stale gate with fresh idle interval");controller.Tick(25000,{});Check(controller.nodes["A"].state==State::Black,"fresh inactive adapter allows ordinary idle activation");controller.config.automatic=false;controller.Reset(GetTickCount64());return 1;}
LRESULT StartDim(){controller=Controller{};controller.config.media=false;controller.config.automatic=true;controller.config.timeout=5;auto& p=controller.config.monitors["A"];p.dim=50;p.fadeMs=1000;p.dimSeconds=3;p.saver=8;p.blackAfter=5;displays={{"A","",L"Dim A",nullptr,{-640,-360,0,0},true}};auto now=GetTickCount64();controller.Topology({"A"},now-5000);controller.Tick(now,{});Reconcile();Check(runs.size()==1&&runs[0]->presentation==-3,"native dim created from production policy");auto style=GetWindowLongPtrW(runs[0]->window,GWL_EXSTYLE);Check((style&(WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_NOACTIVATE))==(WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_NOACTIVATE),"dim is layered click-through nonactivating");RECT bounds{};GetWindowRect(runs[0]->window,&bounds);Check(bounds.left==-640&&bounds.top==-360,"dim exact negative monitor bounds");BYTE alpha=0;DWORD flags=0;Check(GetLayeredWindowAttributes(runs[0]->window,nullptr,&alpha,&flags)&&alpha<=128,"dim alpha initialized before display");controller.Activity(now+1,false,"A",{},true);Reconcile();Check(runs.empty()&&powerTasks.empty(),"input immediately reverses dim with no hardware");return 1;}
LRESULT StartScenes(){controller=Controller{};controller.config.media=false;controller.config.monitors={{"A",{true,8}},{"B",{true,9}}};displays={{"A","",L"Clock",nullptr,{-640,0,0,360},true},{"B","",L"Sparse",nullptr,{0,0,640,360},true}};controller.Topology({"A","B"},GetTickCount64());ManualAll();return 1;}
LRESULT RenderScenes(bool measureResources){
    Check(runs.size()==2,"two concurrent native scenes");auto savedPadding=runs[0]->padding;
    controller.config.padding=40;controller.effectiveReady=false;controller.SelectPolicy(GetTickCount64(),0);
    Check(runs[0]->padding==savedPadding,"manual presentation retains padding across policy change");
    BITMAPINFO info{};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);info.bmiHeader.biWidth=640;info.bmiHeader.biHeight=-360;info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;info.bmiHeader.biCompression=BI_RGB;
    void* pixels=nullptr;HDC dc=CreateCompatibleDC(nullptr);HBITMAP bitmap=CreateDIBSection(dc,&info,DIB_RGB_COLORS,&pixels,nullptr,0);
    Check(dc&&bitmap&&pixels,"scene DIB target");auto old=SelectObject(dc,bitmap);RECT rect{0,0,640,360};size_t nonblack=0;
    // Measure steady-state growth after lazy scene fonts and GDI text resources
    // exist. Hidden windows may or may not have received WM_PAINT before this test.
    for(int frame=0;frame<4;++frame)for(auto& run:runs)PaintScene(dc,rect,*run,run->began+frame*100);
    GdiFlush();Check(runs[0]->sceneFont&&GetObjectType(runs[0]->sceneFont)==OBJ_FONT,"clock owns its reusable scene font");
    DWORD handles0=0,handles1=0;GetProcessHandleCount(GetCurrentProcess(),&handles0);auto gdi=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);
    FILETIME created,exited,kernel0,user0,kernel1,user1;GetProcessTimes(GetCurrentProcess(),&created,&exited,&kernel0,&user0);
    LARGE_INTEGER frequency,wall0,wall1;QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&wall0);
    for(int frame=0;frame<200;++frame)for(auto& run:runs){FillRect(dc,&rect,static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));PaintScene(dc,rect,*run,run->began+frame*100);GdiFlush();auto data=static_cast<DWORD*>(pixels);for(int i=0;i<640*360;++i)if(data[i]&0xffffff)++nonblack;}
    Check(nonblack>0&&nonblack<640*360*200,"native scenes draw sparse dim pixels");
    GetProcessTimes(GetCurrentProcess(),&created,&exited,&kernel1,&user1);auto value=[](FILETIME t){return (uint64_t(t.dwHighDateTime)<<32)|t.dwLowDateTime;};
    GetProcessHandleCount(GetCurrentProcess(),&handles1);QueryPerformanceCounter(&wall1);auto gdiAfter=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);
    printf("NEXTBETA %s scene render sample: 400 hidden frames after warmup; wall=%.3f ms; CPU=%.3f ms; handles=%lu->%lu; GDI=%lu->%lu; no GPU/display endurance claim\n",measureResources?"isolated":"integrated",1000.0*(wall1.QuadPart-wall0.QuadPart)/frequency.QuadPart,(value(kernel1)+value(user1)-value(kernel0)-value(user0))/10000.0,handles0,handles1,gdi,gdiAfter);
    if(measureResources)Check(handles1<=handles0+2&&gdiAfter<=gdi+2,"bounded isolated native renderer resources");
    SelectObject(dc,old);Check(DeleteObject(bitmap)&&DeleteDC(dc),"scene test target cleanup");return 1;
}
// Process-wide GDI counts must be sampled before starting UI/media workers.
// The integration pass below still exercises real Run windows and pixel output.
void IsolatedRenderScenes(){
    Check(runs.empty()&&!ui&&!uiThread,"renderer resource fixture precedes host startup");
    for(int scene:{8,9}){auto run=std::make_unique<Run>();run->presentation=scene;run->began=GetTickCount64();runs.push_back(std::move(run));}
    RenderScenes(true);GdiFlush();auto gdiBefore=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);runs.clear();GdiFlush();
    auto gdiAfter=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);
    Check(gdiBefore==gdiAfter+1,"isolated scene font released with owner");controller=Controller{};
}
LRESULT RenderScenesUi(){return RenderScenes(false);}
LRESULT StartPreviewFixture(){SetupSessions();controller.config.monitors["A"].enabled=false;controller.config.monitors["B"].enabled=false;controller.Manual("C",GetTickCount64(),true);Reconcile();ShowSettings();Preference p;p.saver=6;p.hardware=true;p.powerAfter=5;fixtureModes["@preview"]=L"--preview-responsive";launchDelay=600;Check(StartDraftPreview(p),"contained draft preview starts independently");for(auto& r:runs)if(r->contained){Check(!r->preference.hardware&&!r->preference.powerAfter&&!controller.nodes.contains("@preview"),"preview has no controller or hardware authority");SendMessageW(r->window,WM_SIZE,0,MAKELPARAM(800,500));Check(GetParent(r->preview)==r->window,"resizable child remains contained");}DestroyWindow(editor);Check(std::any_of(runs.begin(),runs.end(),[](auto& r){return r->contained&&r->cancel;}),"closing settings cancels delayed preview launch");return 1;}
LRESULT IntegrationUi(){
    SetupSessions();integrationFixture.clear();ReadIntegrationSettings();
    Check(!OpenSetupOnStart(false)&&OpenSetupOnStart(true)&&integration.trayAction==0&&integration.appearance==0,"missing native settings preserve defaults and first-run setup");
    integrationFixture={{L"StartupPresentation",L"setup"},{L"TrayClickAction",L"setup"},{L"Appearance",L"light"}};
    SendMessageW(ui,kIntegration,0,0);Check(OpenSetupOnStart(false),"startup preference loaded");
    SendMessageW(ui,kTray,0,WM_LBUTTONUP);Check(setupWindow&&!editor,"tray click opens quick setup");
    SetDlgItemTextW(setupWindow,404,L"456");auto window=setupWindow;auto baseline=Serialize(controller.config);
    integrationFixture[L"Appearance"]=L"dark";testTheme=0;SendMessageW(ui,kIntegration,0,0);
    Check(theme.dark&&setupWindow==window&&ControlText(window,404)==L"456"&&Serialize(controller.config)==baseline,"live dark override preserves unsaved setup and saved policy");
    integrationFixture[L"Appearance"]=L"light";testTheme=1;SendMessageW(ui,kIntegration,0,0);Check(!theme.dark,"light override wins over Windows dark");
    testTheme=2;SendMessageW(ui,kIntegration,0,0);Check(theme.highContrast&&theme.colors.base==GetSysColor(COLOR_WINDOW),"high contrast wins over appearance override");
    DestroyWindow(setupWindow);integrationFixture[L"TrayClickAction"]=L"settings";SendMessageW(ui,kIntegration,0,0);
    SendMessageW(ui,kTray,0,WM_LBUTTONUP);Check(editor&&!setupWindow,"tray click opens advanced editor");
    SetDlgItemTextW(editor,150,L"37");auto editorBefore=editor;integrationFixture[L"Appearance"]=L"system";testTheme=0;SendMessageW(ui,kIntegration,0,0);
    Check(!theme.dark&&editor==editorBefore&&ControlText(editor,150)==L"37","follow Windows resumes and preserves advanced edits");DestroyWindow(editor);
    integrationFixture={{L"StartupPresentation",L"invalid"},{L"TrayClickAction",L"invalid"},{L"Appearance",L"invalid"}};SendMessageW(ui,kIntegration,0,0);
    Check(!OpenSetupOnStart(false)&&integration.trayAction==0&&integration.appearance==0,"invalid native settings fall back safely");
    controller.Manual("A",GetTickCount64(),true,-1);Check(controller.Any(),"active protection fixture");SendMessageW(ui,kTray,0,WM_LBUTTONUP);Check(!controller.Any(),"default tray click stops protection");
    integrationFixture.clear();testTheme=-1;SendMessageW(ui,kIntegration,0,0);SetupSessions();return 1;
}
LRESULT StartHungPreview(){launchDelay=0;fixtureModes["@preview"]=L"--preview-hung";Preference p;p.saver=6;return StartDraftPreview(p);}
LRESULT StaleEditor(){
    SetupSessions();ShowSettings();Check(editor!=nullptr,"stale editor fixture");
    auto changed=controller.config;changed.timeout=123;Check(Save(changed,false),"external settings change");
    auto saved=Serialize(controller.config);SendMessageW(editor,WM_COMMAND,130,0);
    Check(Serialize(controller.config)==saved&&saveFailure.find(L"changed elsewhere")!=std::wstring::npos,"stale Advanced draft cannot overwrite live settings");
    ShowWorkspace();Check(workspace!=nullptr,"stale workspace fixture");SendMessageW(workspace,WM_COMMAND,312,0);
    Check(Serialize(controller.config)==saved,"stale workspace cannot overwrite live settings");
    DestroyWindow(workspace);DestroyWindow(editor);return 1;
}
LRESULT ClosePreview(){for(auto& r:runs)if(r->contained)SendMessageW(r->window,WM_CLOSE,0,0);Reconcile();return 1;}
LRESULT VerifyPreviewFallback(){for(auto& r:runs)if(r->contained){Check(r->presentation==-1&&r->done,"contained fallback consumed");ValidateRect(r->window,nullptr);for(int i=0;i<10;++i)Reconcile();Check(!GetUpdateRect(r->window,nullptr,FALSE),"contained black fallback stops repaint requests");return 1;}return 0;}
LRESULT PreviewPhotoAndExpiry(){Preference p;p.saver=7;p.folder="C:\\dac-missing-fixture-folder";Check(StartDraftPreview(p),"invalid photos preview starts safe black fallback");for(auto& r:runs)if(r->contained)r->began=GetTickCount64()-120001;Reconcile();return 1;}
LRESULT StartBatterySaver(){SetupSessions();controller.config.monitors["A"].batteryBlack=true;controller.Manual("A",GetTickCount64(),true);Reconcile();return 1;}
LRESULT BatteryBlack(){auto& node=controller.nodes["A"];auto began=node.began,oldGeneration=node.generation;auto window=runs[0]->window;controller.powerSource=PowerSource::DC;controller.Tick(GetTickCount64(),{});Reconcile();Check(node.state==State::Black&&node.began==began&&runs[0]->window==window&&runs[0]->presentation==-1&&!IsWindowVisible(runs[0]->preview),"battery replaces expensive content behind same opaque cover without resetting origin");Check(!controller.Result("A",oldGeneration,true)&&powerTasks.empty(),"late result cannot revive retired content or request power");ValidateRect(window,nullptr);for(int i=0;i<10;++i)Reconcile();Check(!GetUpdateRect(window,nullptr,FALSE),"retired battery black stops repaint requests");controller.powerSource=PowerSource::AC;controller.Tick(GetTickCount64(),{});Check(node.state==State::Black&&controller.config.monitors["A"].saver==1,"AC return preserves current black and saved assignment");return 1;}
LRESULT StartRevocation(){controller.Reset(GetTickCount64());controller.config.monitors["A"].hardware=true;controller.effectiveReady=false;controller.Manual("A",GetTickCount64(),true,-1);fakePowerResult=0;fakePowerWakeResult=0;fakePowerDelay=0;fakePowerOff=fakePowerOn=0;Check(StartPower(displays[0],controller.nodes["A"].generation)==1,"fake off starts under explicit grant");return 1;}
LRESULT HardwareDiagnostics(){Reconcile();auto text=DiagnosticText();Check(text.find("hardware off accepted")!=std::string::npos&&text.find("hardware wake accepted")!=std::string::npos,"actual fake off/revocation wake outcomes exported");for(int status:{2,3}){auto task=std::make_unique<PowerTask>();task->id="PRIVATE-TICKET-TARGET";task->status=status;task->done=true;powerTasks.push_back(std::move(task));Reconcile();}text=DiagnosticText();Check(text.find("hardware unavailable")!=std::string::npos&&text.find("hardware wake failed")!=std::string::npos&&text.find("PRIVATE-TICKET-TARGET")==std::string::npos,"injected hardware failure outcomes exported without recovery identity");return 1;}
void NextBeta(){IsolatedRenderScenes();ControllerAdapterTests();CloseHandle(stopEvent);stopEvent=nullptr;Check(Initialize(),"next beta initializes");OnUi(SetupSessions);OnUi(NextMigration);OnUi(NextAdapterConsumer);OnUi(SetupSessions);OnUi(NextUi);OnUi(QuickSetupUi);OnUi(IntegrationUi);OnUi(StaleEditor);OnUi(NextMapping);OnUi(NextDiagnostics);OnUi(StopSessions);Until([]{return snapshot.runs==0;},3000,"next UI cleanup");OnUi(StartDim);OnUi(StartScenes);Until([]{return snapshot.runs==2&&snapshot.a==State::Saver&&snapshot.b==State::Saver;},2000,"two independent native scenes");OnUi(RenderScenesUi);OnUi(StopSessions);Until([]{return snapshot.runs==0;},2000,"scene timers/windows/fonts stop");
    OnUi(StartPreviewFixture);Until([]{return snapshot.runs==1&&snapshot.jobs==0;},3000,"preview closed during launch cleans only preview");Check(snapshot.c==State::Black,"unrelated production black survives preview close");OnUi(StartHungPreview);Until([]{return snapshot.runs==2&&snapshot.jobs==1;},2000,"hung preview initially launches contained job");Until([]{return snapshot.runs==2&&snapshot.jobs==0;},6500,"hung preview job health cleanup");OnUi(VerifyPreviewFallback);OnUi(ClosePreview);Until([]{return snapshot.runs==1;},2000,"hung preview close");OnUi(PreviewPhotoAndExpiry);Until([]{return snapshot.runs==1;},2500,"invalid photo/preview maximum duration cleanup");OnUi(StopSessions);Until([]{return snapshot.runs==0;},2000,"pre-battery cleanup");
    OnUi(StartBatterySaver);Until([]{return snapshot.a==State::Saver;},2500,"battery fixture saver ready");OnUi(BatteryBlack);Until([]{return snapshot.jobs==0;},3000,"battery retires child behind black");OnUi(StopSessions);Until([]{return snapshot.runs==0;},2000,"battery stop");OnUi(StartRevocation);Until([]{return snapshot.powerPhase==1;},2000,"fake off accepted and reported on UI owner");Check(fakePowerOff==1,"fake off observed");OnUi(+[]()->LRESULT{SendMessageW(ui,WM_TIMER,1,0);Check(!controller.hardwareFaults.contains("A"),"healthy owned power ticket is not fault quarantine");controller.config.monitors["A"].hardware=false;Reconcile();return 1;});Until([]{return snapshot.powers==0;},3000,"terminal power outcome observed before task drain");Check(fakePowerOn==1,"grant revocation requests fake recovery wake");OnUi(HardwareDiagnostics);
    OnUi(StopSessions);Until([]{return snapshot.runs==0;},3000,"next beta final sessions stop");Shutdown();injectedObservations=false;Check(runs.empty()&&powerTasks.empty()&&ownedWindows.empty()&&ownedJobs.empty()&&!workspace&&!setupWindow,"next beta all owners released");puts("NEXTBETA PASS: production policy/native UI, schema2 migration and byte-exact rollback, recovery retention, rules/profiles/mapping, hidden DPI/topology, dim/scenes/preview, XInput injection, battery and fake permission-revocation wake, bounded redacted diagnostics. No real off/lock/OS-setting mutation.");}

int wmain(int argc,wchar_t** argv) {
    if(argc==5&&wcscmp(argv[1],L"--power-helper")==0) {
        if(wcscmp(argv[4],L"2")==0)return 0; // models missing mod injection, not a successful hardware cycle
        configPath=Executable()+L".power-tests";std::string id,token=Utf8(argv[3]);Check(Unhex(Utf8(argv[2]),id),"helper identity");
        Handle cancelled(OpenEventW(SYNCHRONIZE,FALSE,PowerCancelName(token).c_str()));Check(cancelled,"helper cancellation event");
        return PowerCycle(id,token,[&]{return WaitForSingleObject(cancelled,0)!=WAIT_TIMEOUT;},[&](bool wake){DWORD error=0;return wake?0:(WriteFileText(PowerMarker(id),id+"\n"+token+"\noff",error)?0:11);});
    }
    if(argc>2&&(wcscmp(argv[1],L"--preview-responsive")==0||wcscmp(argv[1],L"--preview-hung")==0)) {
        WNDCLASSW wc{};wc.lpfnWndProc=DefWindowProcW;wc.hInstance=GetModuleHandleW(nullptr);wc.lpszClassName=L"Dac-Beta-Owned-Fixture";
        Check(RegisterClassW(&wc),"fixture class");HWND parent=reinterpret_cast<HWND>(static_cast<uintptr_t>(_wcstoui64(argv[2],nullptr,10)));
        HWND child=CreateWindowExW(0,wc.lpszClassName,L"",WS_CHILD,0,0,100,100,parent,nullptr,wc.hInstance,nullptr);Check(child,"owned fixture child");
        Time start=GetTickCount64();bool hang=wcscmp(argv[1],L"--preview-hung")==0;
        while(IsWindow(child)) {Pump();if(hang&&GetTickCount64()-start>750)Sleep(INFINITE);Sleep(5);}return 0;
    }
    if(argc>1&&wcscmp(argv[1],L"--hang")==0) {Sleep(INFINITE);return 0;}
    if(argc>1&&wcscmp(argv[1],L"--early")==0) return 23;
    if(argc>1&&wcscmp(argv[1],L"--tree")==0) {
        auto exe=Executable();auto command=L"\""+exe+L"\" --hang";
        STARTUPINFOW si{};si.cb=sizeof(si);PROCESS_INFORMATION pi{};
        Check(CreateProcessW(exe.c_str(),command.data(),nullptr,nullptr,FALSE,0,nullptr,nullptr,&si,&pi),"grandchild creation");
        CloseHandle(pi.hThread);CloseHandle(pi.hProcess);Sleep(INFINITE);return 0;
    }
    stopEvent=CreateEventW(nullptr,TRUE,FALSE,nullptr);Check(stopEvent,"stop event");
    if(argc>1&&wcscmp(argv[1],L"--nextbeta")==0){NextBeta();return 0;}
    if(argc>1&&wcscmp(argv[1],L"--advanced")==0){Advanced();return 0;}
    if(argc>1&&wcscmp(argv[1],L"--power")==0){PowerTests();CloseHandle(stopEvent);return 0;}
    if(argc>1&&wcscmp(argv[1],L"--slides")==0){Slides();return 0;}
    if(argc>1&&(wcscmp(argv[1],L"--sessions")==0||wcscmp(argv[1],L"--session-soak")==0)) {Sessions(wcscmp(argv[1],L"--session-soak")==0);return 0;}
    if(argc>1&&wcscmp(argv[1],L"--faults")==0) {
        WorkerFixture(L"--early");WorkerFixture(L"--hang");
        Run missing;missing.path=Executable()+L".missing";RunWorker(&missing);Check(missing.done&&missing.status==2&&missing.error==ERROR_FILE_NOT_FOUND,"missing file fallback");
    } else if(argc>1&&(wcscmp(argv[1],L"--containment")==0||wcscmp(argv[1],L"--host-death")==0)) {
        Run run;run.path=Executable();run.args=L"--tree";run.configuration=true;std::thread worker(RunWorker,&run);
        std::vector<HANDLE> children;Time began=GetTickCount64();
        while(children.size()!=2&&GetTickCount64()-began<3000) {
            Sleep(50);std::lock_guard lock(registryLock);
            for(auto& [job,window]:ownedJobs) {
                (void)window;ULONG_PTR ids[20]{};auto list=reinterpret_cast<JOBOBJECT_BASIC_PROCESS_ID_LIST*>(ids);
                if(QueryInformationJobObject(job,JobObjectBasicProcessIdList,list,sizeof(ids),nullptr)&&list->NumberOfProcessIdsInList==2)
                    for(DWORD i=0;i<2;++i) { children.push_back(OpenProcess(SYNCHRONIZE,FALSE,static_cast<DWORD>(list->ProcessIdList[i])));wprintf(L"CHILD %lu\n",static_cast<DWORD>(list->ProcessIdList[i])); }
            }
        }
        fflush(stdout);Check(children.size()==2,"child and grandchild contained before shutdown");
        if(wcscmp(argv[1],L"--host-death")==0) {puts("HOST-DEATH READY");fflush(stdout);Sleep(INFINITE);}
        SetEvent(stopEvent);worker.join();for(auto h:children) {Check(h&&WaitForSingleObject(h,2000)==WAIT_OBJECT_0,"descendant exits");CloseHandle(h);}
        Check(ownedJobs.empty(),"job registry empty");puts("PLATFORM containment PASS");
    } else if(argc>1&&wcscmp(argv[1],L"--emergency")==0) {
        safetyReady=CreateEventW(nullptr,TRUE,FALSE,nullptr);safetyThread=CreateThread(nullptr,0,SafetyMain,nullptr,0,nullptr);
        Check(WaitForSingleObject(safetyReady,2000)==WAIT_OBJECT_0&&safetyOK,"emergency hotkey registered");
        launchDelay=1500;Run run;run.path=Executable();run.args=L"--hang";std::thread worker(RunWorker,&run);
        Time wait=GetTickCount64();bool published=false;
        while(!published&&GetTickCount64()-wait<1000) {{std::lock_guard lock(registryLock);published=!ownedJobs.empty();}Sleep(5);}
        Check(published,"stalled launch published job");Time began=GetTickCount64();SetEvent(stopEvent);
        Check(WaitForSingleObject(safetyThread,500)==WAIT_OBJECT_0,"emergency not blocked by stalled launch");
        printf("PLATFORM emergency watcher finished during stalled launch: %llu ms\n",GetTickCount64()-began);
        worker.join();Check(run.done&&ownedJobs.empty(),"late launch cancelled before child execution");
        CloseHandle(safetyThread);safetyThread=nullptr;CloseHandle(safetyReady);safetyReady=nullptr;
    } else if(argc>1&&wcscmp(argv[1],L"--lifecycle")==0) {
        CloseHandle(stopEvent);stopEvent=nullptr;
        for(int fault=1;fault<=3;++fault) {initFault=fault;Check(!Initialize(),"partial init failure");Shutdown();Check(!ui&&!singleton&&runs.empty()&&ownedJobs.empty(),"partial init unwind");}
        initFault=0;DWORD baseline=0,final=0,guiStart=0,guiEnd=0;PROCESS_MEMORY_COUNTERS_EX memoryStart{},memoryEnd{};
        Time began=GetTickCount64();
        for(int i=0;i<52;++i) {
            Check(Initialize(),"same-source host init (tray-exempt harness)");
            DWORD_PTR result=0;Check(SendMessageTimeoutW(ui,WM_APP+100,0,0,SMTO_ABORTIFHUNG,2000,&result)&&result,"hidden settings create/destroy font cleanup");
            Sleep(10);Shutdown();Shutdown();
            Check(!ui&&!editor&&!singleton&&runs.empty()&&ownedJobs.empty()&&ownedWindows.empty(),"repeated cleanup empty ownership");
            if(i==1) {GetProcessHandleCount(GetCurrentProcess(),&baseline);guiStart=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);GetProcessMemoryInfo(GetCurrentProcess(),reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memoryStart),sizeof(memoryStart));}
        }
        GetProcessHandleCount(GetCurrentProcess(),&final);guiEnd=GetGuiResources(GetCurrentProcess(),GR_GDIOBJECTS);
        printf("HARNESS lifecycle 50 cycles after 2 warmups: %llu ms; handles %lu -> %lu; GDI %lu -> %lu; tray exempt; no actual Windhawk load\n",GetTickCount64()-began,baseline,final,guiStart,guiEnd);
        GetProcessMemoryInfo(GetCurrentProcess(),reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memoryEnd),sizeof(memoryEnd));
        printf("HARNESS private bytes %zu -> %zu; working set %zu -> %zu\n",static_cast<size_t>(memoryStart.PrivateUsage),static_cast<size_t>(memoryEnd.PrivateUsage),static_cast<size_t>(memoryStart.WorkingSetSize),static_cast<size_t>(memoryEnd.WorkingSetSize));
        Check(final<=baseline+4&&guiEnd<=guiStart,"no sustained handle/GDI growth");return 0;
    } else if(argc>1&&wcscmp(argv[1],L"--probes")==0) {
        WNDCLASSW wc{};wc.lpfnWndProc=DefWindowProcW;wc.hInstance=GetModuleHandleW(nullptr);wc.lpszClassName=L"Dac-Production-Harness-Preview";
        Check(RegisterClassW(&wc),"probe class");
        for(int i=0;i<6;++i) {
            if(!Available(i)) continue;
            wprintf(L"BEGIN stock structural probe %ls\n",savers[i]);fflush(stdout);
            Run run;run.window=CreateWindowExW(WS_EX_NOACTIVATE,wc.lpszClassName,L"",WS_POPUP,0,0,640,360,nullptr,nullptr,wc.hInstance,nullptr);
            Check(run.window,"hidden preview parent");run.preview=run.window;run.path=SaverPath(i);run.args=L"/p "+std::to_wstring(reinterpret_cast<uintptr_t>(run.window));
            std::thread worker(RunWorker,&run);Time began=GetTickCount64();
            while(!run.done&&run.status==0&&GetTickCount64()-began<6000) {Pump();Sleep(10);}
            int observed=run.status;run.cancel=true;
            while(!run.done&&GetTickCount64()-began<9000) {Pump();Sleep(10);}
            worker.join();DestroyWindow(run.window);
            Check(run.done&&ownedJobs.empty(),"stock worker cleanup");
            wprintf(L"RESULT %ls heuristic=%d cleanup=pass elapsed=%llu (hidden window; no visible rendering claim)\n",savers[i],observed,GetTickCount64()-began);fflush(stdout);
        }
        UnregisterClassW(wc.lpszClassName,wc.hInstance);
    } else if(argc>1&&wcscmp(argv[1],L"--media")==0) {
        HRESULT hr=CoInitializeEx(nullptr,COINIT_MULTITHREADED);Check(SUCCEEDED(hr),"media COM");
        Config c;auto m=ObserveMedia(c,Catalog());printf("PLATFORM read-only media sample valid=%d any=%d ambiguous=%d (no playback attribution qualification)\n",m.valid,m.any,m.ambiguous);
        Check(ImageName(GetCurrentProcessId()).find(L"platform-tests.exe")!=std::wstring::npos,"limited rights process image query");
        Check(ImageName(0xffffffff).empty(),"exited/inaccessible process query defined empty");CoUninitialize();
    } else if(argc>1&&wcscmp(argv[1],L"--storage")==0) {
        auto base=Executable();base=base.substr(0,base.find_last_of(L'\\'))+L"\\storage-\u03bb";
        Check(CreateDirectoryW(Extended(base).c_str(),nullptr)||GetLastError()==ERROR_ALREADY_EXISTS,"test directory");
        for(int i=0;i<4;++i) {base+=L"\\"+std::wstring(70,L'x');Check(CreateDirectoryW(Extended(base).c_str(),nullptr)||GetLastError()==ERROR_ALREADY_EXISTS,"long directory");}
        auto path=base+L"\\settings.ini";DWORD error=0;std::string data;
        Config c;c.monitors["unicode-\xce\xbb"]={false,2};
        auto victim=base+L"\\unrelated.txt",legacyTemp=path+L".tmp-"+std::to_wstring(GetCurrentProcessId());
        Check(WriteFileText(victim,"must survive",error),"temporary link target fixture");
        DeleteFileW(Extended(legacyTemp).c_str());
        Check(CreateHardLinkW(Extended(legacyTemp).c_str(),Extended(victim).c_str(),nullptr),"pre-existing predictable temporary hard link");
        Check(WriteFileText(path,Serialize(c),error)&&ReadFileText(path,data,error)&&data==Serialize(c),"Unicode long path roundtrip");
        Check(ReadFileText(victim,data,error)&&data=="must survive","save must not truncate temporary link target");
        Check(GetFileAttributesW(Extended(legacyTemp).c_str())!=INVALID_FILE_ATTRIBUTES,"save must not remove foreign temporary path");
        DeleteFileW(Extended(legacyTemp).c_str());DeleteFileW(Extended(victim).c_str());
        Handle held(CreateFileW(Extended(path).c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,0,nullptr));
        auto before=Serialize(c);Check(!Commit(c,Config{},[&](const std::string& s){return WriteFileText(path,s,error);})&&Serialize(c)==before,"denied replacement retains active config");
        Check(ReadFileText(path,data,error)&&data==before,"denied replacement retains saved bytes");
        Check(!WriteFileText(L"relative.ini","x",error),"relative path rejected");
        Check(!WriteFileText(base+L"\\missing\\file.ini","x",error),"missing directory write fails");
        Check(!ReadFileText(L"C:\\missing-dac-file.ini",data,error),"missing read fails");
        puts("PLATFORM storage PASS: Unicode, >260 chars, replacement denied, original intact, invalid/missing paths");
    } else if(argc>1&&wcscmp(argv[1],L"--catalog")==0) {
        BOOL wow64=FALSE;Check(IsWow64Process(GetCurrentProcess(),&wow64),"query test host architecture");
        for(int i=0;i<6;++i) if(Available(i)) {
            DWORD binary=0;Check(GetBinaryTypeW(SaverPath(i).c_str(),&binary),"installed saver PE type");
            if(wow64) Check(binary==SCS_64BIT_BINARY&&SaverPath(i).find(L"\\Sysnative\\")!=std::wstring::npos,"32-bit host selects native 64-bit savers");
        }
        printf("PLATFORM host pointer bits=%zu WOW64=%d; native saver selection verified\n",sizeof(void*)*8,wow64);
        auto list=Catalog();for(auto& d:list) wprintf(L"DISPLAY identified=%d rect=%ld,%ld,%ld,%ld\n",d.identified,d.rect.left,d.rect.top,d.rect.right,d.rect.bottom);
        for(int i=0;i<6;++i) wprintf(L"SAVER %ls available=%d\n",savers[i],Available(i));
        puts("PLATFORM catalog only; no rendering/desktop qualification");
    } else {fprintf(stderr,"Use --faults, --containment, --host-death, --storage, --catalog\n");return 2;}
    CloseHandle(stopEvent);stopEvent=nullptr;return 0;
}
