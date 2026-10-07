# Phase 1 feasibility report

**Phase 1 gate: NOT PASSED. Recommendation: complete a focused real-desktop feasibility session before assigning the production rebuild.** The experiment supports continuing investigation of `/p` hosting and per-session job containment. It does not yet establish two distinct animated savers on actual multiple displays, independent input, visible bounds, or Windhawk tray/load/unload behavior. No compatibility promise or V1 completion is made.

This is the bounded Phase 0–1 outcome under D-0006 (historical internal record; not bundled). The V1 plan (historical internal record; not bundled) remains unchanged. Work is reviewable and uncommitted in the original checkout on `main`, HEAD `9581aa1ea9e297c1c8e0b1f113c84ab215611ce1`. Pre-existing planning edits were preserved. One delegated agent wrote the repository; parent review was read-only. No production mod directory, new repository/worktree, publication or standalone remediation was created.

## Evidence summary

| Experiment | Actual result | What it does not establish |
| --- | --- | --- |
| Windhawk-target DLL and same-source harness build | Pass with installed clang 20.1.3, explicit x64 target, `-Wall -Wextra -Werror`; Win32/Core Audio C++ headers compile | DLL loading, COM media behavior, supported standalone MSVC build |
| Harness startup/shutdown | Pass; raw input and emergency hotkey registration succeeded; final catalog shutdown 813 ms | Actual Windhawk callbacks, user-visible tray, key delivery or unload safety |
| Tray creation in shell-accessible graphics environment | Fail: `Shell_NotifyIconW` false, reported last-error `0x80004005`; only one display enumerated | Does not prove tray impossible on the real user desktop; actual mod now rejects initialization without its tray |
| Eight installed stock saver variants, separate bounded probes | All created a child window under a 640×360 host and remained alive after six seconds; all owned child process handles signaled after cleanup; sampled foreground unchanged | Visible animation, no transient focus theft, clipping, real input, multi-display behavior, saver-specific configuration, resource/power behavior |
| Missing-file failure | `CreateProcessW` error 2; host remained, fallback flag true, no child/process; shutdown completed | Visually black fallback and error UX on the real desktop |
| Early child exit | Owned fixture exits with code 23; lifecycle timer observed exit, closed job/process, retained fallback host; no relaunch | Actual saver crash exception/WER behavior |
| Owned hung child | Fixture sleeps indefinitely without a preview child; ordinary shutdown terminated it; 828 ms recorded | Startup-health detection: prototype does **not** detect nonrendering/hung startup automatically, so this fixture had fallback=false until stop |
| Job containment before execution | Pass: child and grandchild active in job; closing job terminated both. Final test confirms harness itself was already in a job; nested assignment worked | Windhawk's actual host job constraints or every saver-created process/broker behavior |
| Abrupt host death | Exact launched harness process handle killed by test runner; retained child and grandchild handles both signaled within 2-second waits | Actual Windhawk host death, other OS/architectures |
| Emergency stop independent of lifecycle timer | Explicit named event through `harness.exe --stop` terminated owned hung child in 30 ms; later sample found no host window | Physical hotkey delivery, open tray-menu interaction, absolute worst-case OS/UI latency |

Process/window presence is deliberately not counted as rendered success. No screenshots or animation observations were obtained, no input was injected into the operator's apps, and no real multi-display experiment or simulated multi-display substitute was performed. See [COMPATIBILITY](COMPATIBILITY.md) for every candidate, exact hashes, architecture, OS identity and exposed topology.

## Build and artifact identity

From repository root, exact entry command:

```powershell
./windhawk/tools/build.ps1
```

[The checked helper](../tools/build.ps1) contains the complete compiler argument arrays and library paths. It uses installed `Compiler/bin/clang++.exe`, engine `1.7.3/64/windhawk.lib`, force-included `windhawk_api.h`, `WH_MOD`, `WH_MOD_ID=L"oled-aegis-prototype"`, C++23/O2/shared x64 linking, Unicode, Windows 10 API definitions, exports, and shell32/user32/gdi32/shcore/ole32/uuid/wtsapi32. Harness compilation defines `AEGIS_HARNESS` through its source and links the same core without Windhawk. Compiler revision: `923a5c4f83d2b3675bb88e9fe441daeaa4d69488`. No compiler installation or global setting change occurred.

Final artifacts after lifecycle review:

| Ignored local path | SHA-256 |
| --- | --- |
| `build/windhawk/oled-aegis-prototype.dll` | `8D380DF328F1E287D7995281E466821AA63D01817BDCEE0C3D71ADFA4A641A26` |
| `build/windhawk/harness.exe` | `CAED45B5DCF1A2C8E73AB7ABE37929762E26CC03CEF02903F6D1AFCA9E1D8F1F` |

The full saver/missing/early/hang probes used prior harness hash `2F0930C26A12A3F377D93DF311B39539FE75DCBD2775918A7686323E63F61BDE`. The final review changed mod-only exit/tray-failure handling and added a containment diagnostic; the harness retained its shell-less tray behavior and saver lifecycle. Final catalog, nested containment and emergency tests ran on the final hash. Thus the per-saver data is not misrepresented as a full retest of the final artifact.

First bounded executable launch failed with `0xC0000135`. PE import inspection identified `libc++.whl` and `libunwind.whl`; adding the installed compiler DLL directory to PATH did not satisfy those names. The helper now copies the installed binaries byte-for-byte under their imported names beside the harness. This fixed loader startup. Those local dependencies and their terms are in [PROVENANCE](PROVENANCE.md); no redistributable package was produced.

## Runtime commands and raw evidence

The [runner](../tools/run-check.ps1) defaults to a nine-second limit, redirects stdout/stderr under ignored `build/windhawk/`, records the exit result, and terminates only the exact process object it launched if that limit expires. All completed runs below returned 0. The normal saver shutdown measurements were 812–859 ms, including the intended 750 ms graceful interval and UI scheduling; these are samples, not a worst-case guarantee.

```powershell
./windhawk/tools/run-check.ps1 -Name final-catalog -TestArguments @('--catalog')
./windhawk/tools/run-check.ps1 -Name final-containment -TestArguments @('--containment')
0..5 | ForEach-Object { ./windhawk/tools/run-check.ps1 -Name "saver-x64-$_" -TestArguments @('--probe',"$_",'0') }
4..5 | ForEach-Object { ./windhawk/tools/run-check.ps1 -Name "saver-x86-$_" -TestArguments @('--probe',"$_",'0','x86') }
./windhawk/tools/run-check.ps1 -Name missing -TestArguments @('--probe','1','0','missing')
./windhawk/tools/run-check.ps1 -Name early-exit -TestArguments @('--fault','early')
./windhawk/tools/run-check.ps1 -Name owned-hang -TestArguments @('--fault','hang')
```

Raw logs: catalog (historical local record: `../../build/windhawk/final-catalog.log`; not bundled), nested containment (historical local record: `../../build/windhawk/final-containment.log`; not bundled), missing (historical local record: `../../build/windhawk/missing.log`; not bundled), early exit (historical local record: `../../build/windhawk/early-exit.log`; not bundled), hang (historical local record: `../../build/windhawk/owned-hang.log`; not bundled), host death (historical local record: `../../build/windhawk/host-death.log`; not bundled), host-death result (historical local record: `../../build/windhawk/host-death-result.log`; not bundled), emergency (historical local record: `../../build/windhawk/emergency.log`; not bundled), emergency result (historical local record: `../../build/windhawk/emergency-result.log`; not bundled). Per-saver links are in COMPATIBILITY. These ignored files are local corroboration; the qualified measured conclusions above are preserved in this tracked-intended report.

For host-death reproduction, launch `harness.exe --host-death` with PowerShell `Start-Process -WindowStyle Hidden -PassThru`, redirected output, retain process handles for the two PIDs printed by that new harness, then call `.Kill()` on the **returned host object**, never a filename-wide process search. The child/grandchild handles must signal within 2 seconds. The harness creates only test descendants and a kill-on-close job. Use a bounded outer timeout. For emergency reproduction, similarly launch `--fault hang`, retain its logged child handle, run `harness.exe --stop` in the same session, and time its exit; let the harness finish its six-second observation and shutdown. Neither test installs a mod.

## Desktop access and interruption

Computer Use's deferred JavaScript bridge initialized and its app listing contained no Windhawk match. A subsequent combined call intended to run checks then launch/list Windhawk stalled for about 652 seconds and was interrupted by the parent. It returned no partial output and left no expected check logs; the precise nested operation at which it stalled cannot be established. No successful UI launch, prototype loading, desktop access or test is inferred from that call. After interruption only the two previously observed Windhawk processes were visible, with no harness. Fresh isolated runtime calls were explicitly bounded and returned the results above. UI automation was not retried indefinitely. No temporary Windhawk mod was installed, and no settings were written through its UI.

The graphics topology exposed to the shell is one output, not evidence about the user's physical desktop. Tray failure plus absent observed interactive UI prevents P07 and the actual multi-display gate here. No approval denial or prohibited operation is being disguised as a technical incompatibility.

## Architecture, lifecycle and settings findings

The smallest architecture still supported by evidence is one dedicated user-session host, one UI thread owning catalog/windows/tray, one emergency watcher, one configuration model, and one kill-on-close job per saver. Launch uses a full path, decimal pointer-width `/p` argument, suspended creation and assignment before resume. No forced reparenting, filename-based termination, inherited process handles, display-power request, or OS policy change is used. Ordinary stop hides the host, posts close requests to job-owned windows, waits 750 ms through the UI timer, then closes the job. Early exit closes the remaining job tree and retains the host without relaunching.

The original stable adapter follows the official tool-mod mechanism, targeting only `windhawk.exe`, filtering session 0 and service/control invocations, spawning a dedicated `-tool-mod` process, and replacing its entry point. This was compiled and reviewed, not actually loaded. Unload joins the safety/UI workers before returning to an exiting dedicated process; session exit cleans its windows/jobs before process exit. The configured safety interval is bounded, but shutdown retains infinite final joins to avoid returning with code still executing in the DLL. A pathological blocked UI/OS call could delay exit; do not claim an absolute bounded unload. The watcher also takes the session mutex, so a blocked launch API can delay its intervention. Production needs explicit launch cancellation/time budgets if that failure is in the guarantee.

The tray's settings help is nonmodal, and emergency exit ends an open tray menu before destroying the controller. Actual menu-open hotkey testing remains missing. Raw input distinguishes mouse and keyboard without suppression flags; current cursor/foreground mapping at dispatch can be conservative under queue delay. Consumer HID media keys and physical interactions remain unverified. There is no idle scheduler/media inference in this prototype.

Windhawk settings and tray selection both feed the same validated `Config`. Tray choices are session overrides, never a second durable file; settings callbacks replace them and stop presentations. No custom persistent monitor/saver editor was added. This resolves the intended data ownership model but does not prove callback delivery or durability. Session pause blocks starts and stops current work. Exit ends only the experiment; re-enable restarts it. Production must add the plan's explicit per-mod automatic-activation preference without changing global Windhawk startup; that feature was deliberately not implemented here.

## Remaining work and recommendation

Phase 0 has its complete requirement ledger, reuse inventory, candidate identities, audit dispositions and numbered Medium loops. The retained standalone defects are not repaired. Original implementation and OS icons avoid unresolved inherited source/asset reuse; actual toolchain inputs and release licensing work are recorded separately.

The next assignment should be **finish the Phase 1 desktop gate**, with the existing source, not Phase 2 production. The smallest required operator action is to make an unlocked Windhawk test desktop with at least two actual displays available, or manually perform the [temporary load/test/unload procedure](../README.md#temporary-windhawk-load-test-stop-and-remove). First prove the tray and emergency stop, then visibly animate Mystify on B while typing on A, then Ribbons/Mystify concurrently and two Mystify instances. Include one launch failure, disable/re-enable, settings update and exact owned-process cleanup. Record observations and process identities; do not change sleep/lock policies or restart Explorer for this first gate.

Still unverified: visible rendering for every candidate; actual monitor B versus A input/cursor/media behavior; simultaneous distinct/same savers; full monitor bounds and mixed DPI; real Windhawk loading/settings/unload; saver configuration UI/global preference effects; Photos valid/empty/missing sources; actual saver crash/hung rendering; display removal; shell restart; sleep/resume/lock; display power-off; separate CPU/GPU/memory/resource soak; Windows 10/ARM64/other versions. Disruptive shell/power/topology tests need a disposable environment or the exact separately authorized manual action. No requested capability is removed to conceal these gaps.

If that focused experiment passes, reuse the **approach and tests**, and review the disposable implementation before production: stable monitor identities, canonical durable assignments, asynchronous launch health, reliable dismissal/power/session handling and tested shutdown remain necessary. There is currently no evidence requiring a narrower saver promise; there is also no evidence justifying any supported saver list. Stop here under D-0006; the operator decides the production assignment after real runtime evidence.
