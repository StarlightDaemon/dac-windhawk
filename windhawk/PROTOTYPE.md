# OLED Aegis Windhawk feasibility prototype

Production continuation is authorized under D-0007 (historical internal record; not bundled),
using the production handoff (historical internal record; not bundled).
The code described below is still the feasibility prototype. The manual test
procedure is retained as a reference, not a prerequisite for current work;
desktop qualification remains deferred separately from implementation.

Experimental, manual-only Phase 1 work under D-0006 (historical internal record; not bundled). This is **not a supported release or unattended OLED protection**. The accepted V1 plan (historical internal record; not bundled) remains the feature contract. Start with the [Phase 1 report](docs/PHASE_1_REPORT.md), [acceptance ledger](docs/ACCEPTANCE.md), [compatibility observations](docs/COMPATIBILITY.md), [provenance](docs/PROVENANCE.md), and [audit dispositions](docs/AUDIT_DISPOSITION.md).

Inspired by OLED Aegis by spenserlee and contributors, DisplayFusion by Binary Fortress Software, and Actual Multiple Monitors by Actual Tools. Built for Windhawk. No inherited OLED Aegis implementation or assets were incorporated. This is not a clean-room claim.

## Build and repeat the isolated checks

Run from this checkout in PowerShell:

```powershell
./windhawk/tools/build.ps1
./windhawk/tools/run-check.ps1 -Name catalog -TestArguments @('--catalog')
./windhawk/tools/run-check.ps1 -Name containment -TestArguments @('--containment')
./windhawk/tools/run-check.ps1 -Name mystify -TestArguments @('--probe','1','0')
./windhawk/tools/run-check.ps1 -Name missing -TestArguments @('--probe','1','0','missing')
./windhawk/tools/run-check.ps1 -Name early -TestArguments @('--fault','early')
./windhawk/tools/run-check.ps1 -Name hang -TestArguments @('--fault','hang')
```

Dependencies: installed Windhawk 1.7.3, its clang 20.1.3 x64 target/compiler headers and engine import library, Windows Win32/COM libraries, and installed Windows saver copies. `-WindhawkRoot` overrides the default installation path. The helper copies the installed libc++/libunwind binaries to the `.whl` names required by their imports, **only into ignored `build/windhawk/` for local testing**. Do not distribute those outputs without the dependency/notices review. No large toolchain or global environment changes are needed. The standalone MSVC build is separate.

The build fails on compiler failure or missing output and removes the specific prior target before building it. Outputs are `build/windhawk/oled-aegis-prototype.dll` and `build/windhawk/harness.exe`. The harness includes the actual prototype implementation but excludes the Windhawk adapter; its success does not prove Windhawk loading or unloading. `run-check.ps1` bounds each run and kills only its own returned process handle after a timeout. Child jobs die with that host.

Saver indexes: 0 Bubbles, 1 Mystify, 2 Ribbons, 3 3D Text, 4 Photos, 5 Blank. Monitor indexes come from `--catalog`; they are temporary enumeration indexes, not persistent identities. `--probe` renders a 640×360 host for six seconds with input dismissal disabled to make process observations repeatable. Add `x86` as the fourth argument only for the installed Photos/Blank copies. No `.scr` is downloaded or bundled. Photos may display the user's configured images: inspect it only in an appropriate test environment and do not capture private content.

## Temporary Windhawk load, test, stop and remove

This procedure is supplied for the unresolved runtime gate; it was **not executed** successfully during this assignment.

1. Use an unlocked test desktop with at least two actual displays and Windhawk 1.7.3 running as the desktop user. Keep a PowerShell terminal on display A. Compile first. Do not change global Windhawk startup or Windows screensaver, lock or power settings.
2. In Windhawk, select **Create a new mod**. Replace the template with the complete contents of `prototype/oled-aegis-prototype.wh.cpp`, save, and use its **Compile Mod** action. Enable only this temporary prototype. Its target is `windhawk.exe`; never change it to `*` or Explorer. Verify a separate user-session `windhawk.exe -tool-mod ...` process and the experiment tray icon. If either is absent, stop and collect Windhawk's mod log.
3. Set `smallPreview=true`, `inputDismissal=true`, and `lifetimeSeconds=60` in the mod's Windhawk Settings. Use the tray menu to start Mystify on B. Confirm visible animation and continue typing in a harmless test document on A with the cursor on A. Then cross onto B and click; B must dismiss without intercepting A's intended input. Explicitly test keyboard with cursor on B and foreground on A (both are credited), media keys, and click recovery.
4. After recovery works, set `smallPreview=false` and repeat at full monitor bounds. Start different animated savers on A/B, then the same saver twice. Moving the cursor onto a presenting display is expected to dismiss it; use keyboard/menu timing deliberately. Record actual visual output, focus, child windows and owned PIDs. Never infer animation from process presence.
5. Use **Stop all** for graceful stop. **Session pause** stops all and blocks further manual starts until toggled; it does not persist. Tray selections update the single in-memory model; a Windhawk settings change replaces those temporary choices and stops current presentations. Persistent defaults live only in Windhawk. This avoids two durable editors, but actual callback behavior still needs testing.
6. The independent emergency exit is **Ctrl+Alt+Shift+F12**. Another explicit stop route, in the same user session, is `./build/windhawk/harness.exe --stop`. This signals only the experiment's named event. Manual previews also expire after the configured 5–120 seconds. The hotkey must register or initialization fails. Tested cleanup timings are in the report; arbitrary UI/OS hangs are not claimed bounded.
7. Test the hotkey while the tray menu is open, settings changes during presentation, ordinary tray exit, and disable/re-enable. Verify all owned children and host windows disappear. Exit is session-only; re-enable the mod to restart it. It does not disable Windhawk globally. The stable-host adapter itself remains unverified.
8. Disable the temporary mod, close its editor, and remove **only this prototype** from Windhawk. Verify no prototype tool host or owned savers remain. Leave other mods alone. No temporary mod installation was created in this run, so none was removed.

For a short harness-only manual session, run `./build/windhawk/harness.exe --interactive`; it exits after 120 seconds at most under a responsive runtime. The source's emergency watcher hides hosts and closes owned jobs; normal stop first posts close requests and allows 750 ms before job closure. Windows performs final process cleanup asynchronously.

Automatic idle activation, media suppression, padding, stable identities, configuration import, saver configuration dialogs, and production startup adaptation are later phases. Windows 10, ARM64, mixed DPI, real multi-display operation, lock/sleep, display removal and other Windhawk releases are unqualified.
