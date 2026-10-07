# Beta 2: local Windhawk host startup repair

2026-10-04. Operator reported an enabled local mod with no tray icon and
authorized investigation, repair and a Windows administrator prompt.

Confirmed root cause: beta 1 declared `x86-64`, while the installed Windhawk
1.7.3 `windhawk.exe` is an x86 PE (0x14c). Its engine excluded the mod before
the launcher could run. This invalidates beta 1's installability on that host;
its historical x64 harness results remain historical, not successful host tests.

Beta 2 declares both x86 and x86-64. The build helper detects the installed
host's PE architecture, rejects metadata excluding it, selects the matching
engine/compiler/runtime, and verifies the DLL machine type. Explicit x64
builds remain available. On WOW64, saver discovery uses the documented Sysnative
alias to retain native 64-bit stock savers without globally disabling redirection.
Platform tests verify that selection. Minor printf casts make the test harness
warning-clean for both targets.

The x86 build passed all twelve groups, including 116,033 policy assertions,
native saver probes and 100 hidden presentation cycles. The final cycle run
took 21,906 ms; handles 205 to 205; private bytes 2,887,680 to 3,706,880.
The x64 source also compiled. Current outputs/receipts are under ignored
`build/windhawk/beta2/` and `beta2-x64/`. Builds used `-ModId local@oled-aegis`
for the existing local registration. Source SHA-256:
`9B925E8E2BB25883E94914B1EE0C256ADE2D03DF517230129F020272B00BB7A8`.

After the operator approved elevation, the update replaced only this mod's
source, DLL registration, version and architecture. Previous DLLs remain;
source/registration backup is `build/windhawk/beta2/installed-beta1-backup/`.
Installed name is `local@oled-aegis_1.0.0-beta.2_hostfix.dll` in both architecture
folders. Windhawk's change watcher started the dedicated host without restarting
other apps. PID 18708 was observed running with the 32-bit beta 2 DLL loaded;
Windhawk's status file reported `windhawk.exe|Loaded` for that host and launcher
5900. Service 4556 correctly reported Unloaded. Successful production Initialize
requires tray registration, input/session/power controls and the safety watcher.
This establishes actual startup on this host, not visual or full lifecycle QA.

The operator stopped Computer Use with Escape before final visual inspection.
No more desktop input was issued. Visible icon/menu, saver rendering, physical
multi-monitor behavior and full Windhawk unload remain unqualified. Automatic
activation defaults off. Look under hidden tray icons for the generic application
icon with tooltip `OLED Aegis — manual only`; right-click for Settings.

Do not run lifecycle/session harness groups while the installed mod owns its
singleton/hotkey. Disable the installed mod first for those tests. The original
beta 1 ZIP is historical and contains the startup defect; use the current source.

References: [Windhawk dedicated tool host](https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process/5cb4ebb18dd742277afca3d0bd917b648c71a5fa),
[architecture filtering in Windhawk 1.7.3](https://github.com/ramensoftware/windhawk/blob/v1.7.3/src/windhawk/engine/mod.cpp),
[Microsoft WOW64 filesystem redirector](https://learn.microsoft.com/en-us/windows/win32/winprog64/file-system-redirector).
