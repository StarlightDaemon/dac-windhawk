// Same-source test entry point; does NOT emulate a successful Windhawk load.
#define AEGIS_HARNESS
#include "../prototype/oled-aegis-prototype.wh.cpp"

int wmain(int argc,wchar_t** argv) {
    using namespace prototype;
    if(argc>1 && wcscmp(argv[1],L"--stop")==0) {
        HANDLE event=OpenEventW(EVENT_MODIFY_STATE,FALSE,kStopName);
        if(!event) return 2; BOOL ok=SetEvent(event); CloseHandle(event); return ok?0:3;
    }
    if(argc>1 && wcscmp(argv[1],L"--hang")==0) { Sleep(INFINITE); return 0; }
    if(argc>1 && wcscmp(argv[1],L"--exit-child")==0) return 23;
    if(argc>1 && wcscmp(argv[1],L"--tree")==0) {
        wchar_t exe[32768]; GetModuleFileNameW(nullptr,exe,32768);
        std::wstring command=L"\""+std::wstring(exe)+L"\" --hang";
        STARTUPINFOW si{}; si.cb=sizeof(si); PROCESS_INFORMATION pi{};
        if(!CreateProcessW(exe,command.data(),nullptr,nullptr,FALSE,0,nullptr,nullptr,&si,&pi)) return 1;
        Note(L"grandchild",pi.dwProcessId); CloseHandle(pi.hThread); CloseHandle(pi.hProcess); Sleep(INFINITE); return 0;
    }
    if(argc>1 && wcscmp(argv[1],L"--containment")==0) {
        BOOL inJob=FALSE; IsProcessInJob(GetCurrentProcess(),nullptr,&inJob); Note(L"harness already in a job",inJob);
        wchar_t exe[32768]; GetModuleFileNameW(nullptr,exe,32768);
        Session s{};
        if(!Launch(s,exe,L"--tree")) return 1;
        Sleep(500);
        JOBOBJECT_BASIC_ACCOUNTING_INFORMATION accounting{};
        QueryInformationJobObject(s.job,JobObjectBasicAccountingInformation,&accounting,sizeof(accounting),nullptr);
        Note(L"active job processes before close",accounting.ActiveProcesses);
        ULONG_PTR ids[18]{}; auto list=reinterpret_cast<JOBOBJECT_BASIC_PROCESS_ID_LIST*>(ids);
        if(!QueryInformationJobObject(s.job,JobObjectBasicProcessIdList,list,sizeof(ids),nullptr)) return 2;
        std::vector<HANDLE> children;
        for(DWORD i=0;i<list->NumberOfProcessIdsInList;++i) children.push_back(OpenProcess(SYNCHRONIZE,FALSE,static_cast<DWORD>(list->ProcessIdList[i])));
        auto before=GetTickCount64(); CloseHandle(s.job); s.job=nullptr;
        bool all=children.size()==2;
        for(auto child:children) { if(!child || WaitForSingleObject(child,2000)!=WAIT_OBJECT_0) all=false; if(child) CloseHandle(child); }
        Note(L"close-to-tree-exit milliseconds",static_cast<DWORD>(GetTickCount64()-before));
        CloseHandle(s.process); Note(all?L"containment PASS":L"containment FAIL"); return all?0:3;
    }
    if(argc>1 && wcscmp(argv[1],L"--host-death")==0) {
        wchar_t exe[32768]; GetModuleFileNameW(nullptr,exe,32768); Session s{};
        if(!Launch(s,exe,L"--tree")) return 1; Sleep(500);
        // External test runner terminates this exact process handle.
        Note(L"host-death ready",GetCurrentProcessId()); Sleep(INFINITE); return 0;
    }
    if(argc>1 && wcscmp(argv[1],L"--fault")==0) fixtureArgs=argc>2 && wcscmp(argv[2],L"early")==0?L"--exit-child":L"--hang";
    Config cfg; cfg.input=false; cfg.lifetime=10; ApplyConfig(cfg);
    if(!Initialize()) { fwprintf(stderr,L"initialization failed\n"); return 1; }
    for(size_t i=0;i<monitors.size();++i) {
        auto m=monitors[i]; wprintf(L"MONITOR %zu %ls [%ld,%ld,%ld,%ld] dpi=%u\n",i,m.name,m.rect.left,m.rect.top,m.rect.right,m.rect.bottom,m.dpi);
    }
    fflush(stdout);
    int result=0;
    if(argc>1 && wcscmp(argv[1],L"--catalog")==0) { }
    else if(argc>1 && (wcscmp(argv[1],L"--probe")==0 || wcscmp(argv[1],L"--fault")==0)) {
        int saver=fixtureArgs.empty() && argc>2?_wtoi(argv[2]):1, monitor=argc>3?_wtoi(argv[3]):0;
        bool x86=argc>4 && wcscmp(argv[4],L"x86")==0;
        bool missing=argc>4 && wcscmp(argv[4],L"missing")==0;
        HWND foreground=GetForegroundWindow();
        PostMessageW(uiWindow,kStart,monitor,saver|(x86?256:0)|(missing?512:0));
        Sleep(6000);
        { std::lock_guard guard(sessionLock);
          auto& s=sessions[std::clamp(monitor,0,15)];
          Note(L"foreground unchanged",GetForegroundWindow()==foreground);
          Note(L"host exists",!!s.window); Note(L"fallback",s.fallback);
          Note(L"direct child window",s.window && GetWindow(s.window,GW_CHILD)!=nullptr);
          Note(L"process still running",s.process && WaitForSingleObject(s.process,0)==WAIT_TIMEOUT);
        }
    } else if(argc>1 && wcscmp(argv[1],L"--interactive")==0) {
        cfg.input=true; cfg.lifetime=60; ApplyConfig(cfg);
        Note(L"manual interactive run expires in 120 seconds"); WaitForSingleObject(exitEvent,120000);
    } else { fwprintf(stderr,L"Use --catalog, --probe saver monitor [x86|missing], --interactive, --containment, --host-death, --stop\n"); result=2; }
    HANDLE ownedChild=nullptr;
    { std::lock_guard guard(sessionLock); if(sessions[0].process) DuplicateHandle(GetCurrentProcess(),sessions[0].process,GetCurrentProcess(),&ownedChild,SYNCHRONIZE,FALSE,0); }
    auto shutdownStart=GetTickCount64(); Shutdown();
    if(ownedChild) { bool exited=WaitForSingleObject(ownedChild,2000)==WAIT_OBJECT_0; Note(L"owned child exited after shutdown",exited); if(!exited) result=4; CloseHandle(ownedChild); }
    Note(L"shutdown milliseconds",static_cast<DWORD>(GetTickCount64()-shutdownStart));
    Note(L"harness shutdown complete"); return result;
}
