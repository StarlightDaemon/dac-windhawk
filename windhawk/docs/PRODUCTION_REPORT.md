# Windhawk V1 production candidate report

Operator date: **2026-10-04**. **Implementation complete with desktop qualification outstanding**, under D-0007. This delivers P01–P11/S01–S08 implementation and the available automated evidence; it does not claim a supported stable release, actual Windhawk runtime qualification, or universally compatible savers. LOOP-016 retains desktop qualification. The historical Phase 1 report/gate outcome is unchanged.

## Delivered implementation

The single-file production mod is `windhawk/mods/oled-aegis.wh.cpp`. Policy, parser, legacy importer, geometry/media helpers and Windows adapters are compiled directly into the tests, rather than replicated in a separate model. The prototype remains a separate baseline. No inherited standalone source/assets, root product/build guides, legacy CI, managed Writ, remotes, host policies or other repositories were changed. Work remains uncommitted in the original checkout on `main`.

| Contract | Implementation |
| --- | --- |
| P01 | Normalized target-device identities; disconnected preferences retained; explicit zeros preserved; conservative ineligibility for unknown/duplicate/clone identities |
| P02–P04 | Monotonic global/per-monitor idle controller, 5–3600 s timeout, 250–10000 ms polling, event input plus last-input fallback, conservative delayed attribution, keyboard cursor+foreground policy |
| P05 | Default multimedia endpoint Core Audio, mute/volume/peak and silent-session option, bounded positive grace, stale/failure suppression, visible-window attribution and silent display-request heuristic; owned job descendants excluded |
| P06 | Native black, physical-pixel 0–1024 padding, PMv2 coordinates, exact unpadded saver child host, presentation-only cursor hiding |
| P07–P09 | Derived running tray state; stable-identity menus; preview/stop/configure/settings/import; session pause/exit; shell retry/recovery; validated one-model persistence; bounded optional logging; explicit automatic preference and startup migration guard |
| P10 | Initial/query-on-connect lock state, session/power/display-off stop, topology/DPI refresh, new idle interval and invalidated media snapshots after recovery |
| P11 | Explicit flat legacy INI import, old key aliases/index handling, original-file preservation, rejected malformed input, reported unknown keys and checked save |
| S01–S03 | Durable independent native-black/six-stock-saver choices; one presentation per eligible monitor; manual previews and contained conventional configuration processes |
| S04–S05 | Installed-path discovery; unqualified labels; owned-child startup deadline and early-exit/missing/launch fallback; no automatic retry during a failed idle session |
| S06–S08 | Suspended launch assigned to per-run kill-on-close job before execution; graceful close then owned-tree termination; UI-owned windows, emergency watcher, complete worker joins; no host wake/power-policy changes |

## Ownership and policy decisions

The UI thread exclusively owns the controller, monitor catalog, configuration, windows, editor font, tray and timer. A saver worker owns each job/process and reports atomic status against a session generation. Stopped runs retain their host until the worker finishes. Stale results cannot revive a replaced/disconnected session. The registry lock only synchronizes handle/window publication and emergency access; it is never held across `CreateProcessW`, Core Audio or process identity queries. Audio uses one MTA worker, copied immutable settings/topology, and an epoch to discard obsolete observations. UI teardown joins all run workers; unload joins safety/audio/UI threads before closing their shared handles or returning to process exit. Partial initialization follows the same cleanup path.

Canonical configuration is a versioned UTF-8 file under LocalAppData. Save writes/flushed bytes to a same-directory temporary file and replaces the destination before changing live settings. Failure preserves old active/saved data. The importer is a read-only operation on the legacy file. Diagnostic messages contain categories/error codes, never titles, raw input or photo paths/contents. Logging has a fixed event cap and no unbounded private log file.

Black remains default, with auto activation off. Device paths rather than serial-number/EDID inventories identify monitors. Unidentified/cloned outputs are ineligible rather than guessed. Mixed-DPI math uses physical pixels. Padding is preserved at the inherited 0–1024 range, including adjacent-desktop coverage, but animation receives the exact monitor-sized child region. No inherited source/asset rights were assumed; implementation is original, selectively adapting the local prototype's Win32 mechanism and stable-host wrapper.

## Available verification

Exact entry commands, from repository root:

```powershell
./windhawk/tools/build-production.ps1
./windhawk/tools/test-production.ps1 -Checks policy,storage,catalog,media,faults,containment,host-death,emergency,lifecycle,probes -TimeoutSeconds 55
./windhawk/tools/package-production.ps1
```

Build: stable Windhawk 1.7.3, clang 20.1.3 revision `923a5c4f83d2b3675bb88e9fe441daeaa4d69488`, explicit `x86_64-w64-mingw32`, C++23/O2, `-Wall -Wextra -Werror`. Complete argument arrays are in `tools/build-production.ps1`; libraries are parsed from source metadata. It compiles the copied standalone distribution, with installed force-included Windhawk headers/import library. No local include or installed toolchain modification is needed. An incomplete installed Core Audio meter declaration was bridged with a minimal documented COM ABI view, separately recorded in provenance.

| Evidence class | Result and scope |
| --- | --- |
| Pure policy/configuration | **PASS: 116,028 assertions.** 48 combinations of input/media scope/muted preference/poll interval; 128 injected audio active/mute/peak/volume/endpoint cases; exact idle boundaries, stale/ambiguous/failed media, quiet-grace expiry, delayed trigger clicks, between-poll input, regression of injected clock, disabled persistence, Unicode identities, import/ranges, reconnect/reorder, generations and no-retry fallback |
| Geometry policy | Negative coordinates, physical padding independent of DPI scale, substantive media overlap vs invisible border; simulated math, not physical DPI evidence |
| Controller endurance | Eight **virtual hours**, 250-ms ticks with input/topology churn and bounded state; not elapsed display/GPU soak |
| Platform storage | **PASS:** Unicode and >260-character paths, file roundtrip, denied final replacement with original bytes/live settings intact, missing directory/read and relative path rejection |
| Build negative case | **PASS:** appended deliberate `#error` via `-SourceFile` test copy; helper failed and left no stale DLL/success manifest. Normal final build passed afterward |
| Platform process query/media | Correct current-process limited-rights image query and defined missing-PID result. Read-only Core Audio sample: valid=true, any=false, ambiguous=false. No player/tab attribution claim |
| Worker failures | **PASS:** early child exit (94 ms), missing file error 2, owned non-window hang timeout/cleanup (5891 ms total). UI deadline/OS-call limits below still apply |
| Containment | **PASS:** production worker's child and grandchild both in job; retained handles signalled after stop; abrupt exact host termination also signalled both within 2-second checks |
| Emergency injection | **PASS:** watcher completed in the same 15.6-ms tick (reported 0 ms) while launch worker was deliberately held 1500 ms after job publication. Late launch was cancelled before child execution. Not a measured physical hotkey/blocked-UI window-hide bound |
| Lifecycle harness | **PASS:** three partial-init cut points, 2 warmups + 50 init/shutdown cycles, repeated shutdown, hidden settings create/destroy and font cleanup. 1625 ms total; handles 174→176; GDI 0→0; private bytes 2,523,136→2,801,664; working set 14,979,072→15,269,888. This short sample is not long-run leak/CPU/GPU qualification |
| Stock structural probes | All six native savers created owned preview children in hidden 640×360 hosts and cleaned up. Bubbles 907 ms, Mystify/Ribbons 891 ms, 3D Text 906 ms, Photos/Blank 156 ms through cleanup. Window presence, not animation, was observed |
| Real Windhawk/desktop | **Deferred:** actual load/settings/unload, tray/shell behavior, visible rendering, physical inputs/multiple displays/DPI/power, same-saver/different-saver concurrency and eight-hour elapsed display soak |

The available graphics catalog exposed one `[0,0,2560,1440]` output without a usable stable target identity. Production correctly makes it ineligible. Harness lifecycle explicitly exempts absent tray, uses a test-only settings path and keeps automatic activation off; it does not pretend that this is a successful product initialization on the user's desktop. Hidden stock probes directly exercise the production worker on owned test parents. No operator playback, photo contents, desktop input or policies were manipulated.

Raw ignored logs/results: `build/windhawk/production/{policy,storage,catalog,media,faults,containment,host-death,emergency,lifecycle,probes}.{log,err,result}`. Builds and source/binary identities are beside them. This report retains the meaningful results so ignored logs are not the only record.

## Artifact identities and prerequisites

| Artifact | SHA-256 |
| --- | --- |
| `windhawk/mods/oled-aegis.wh.cpp` | `7500B3D1060DE956CF0DC468D25E7809EE92B36ACD24890FFA02B22094071C92` |
| `build/windhawk/production/oled-aegis.dll` | `E39F96CF559216033E3E0830D7546155F3EEAAF7AB176267D4FD476E80E8809E` |
| `policy-tests.exe` | `6AEAAC14148A858D353799653A9D67925E1E7DDB1C55054BC88193E81600ECE1` |
| `platform-tests.exe` | `BBF70CC82B1FDAFCEFBF99305C5B1DB4090199EE042AFCCDDB295E8307CD08EE` |
| Unmodified prototype source | `0E0B6AF05DF8594F1F8F7E67EE21085A68055729DB219F766530312BF959E1D8` |

The source ZIP is `build/windhawk/production/oled-aegis-v1-candidate-source.zip`; `SHA256SUMS.txt` inside it identifies every included file. Fixed timestamps/sorted entries permit repeatable archive bytes for identical inputs. Archive identity is recorded in WORK_LOG rather than embedded recursively in this report. This package includes source/docs/tools only and is a review artifact, not a publication.

Measured DLL imports: `windhawk.dll`, `libc++.whl`, `libunwind.whl`, `SHELL32`, `USER32`, `GDI32`, `ole32`, `WTSAPI32`, `comdlg32`, `ADVAPI32`, `POWRPROF`, `dwmapi`, `msvcrt`, `KERNEL32`. The installed Windhawk host supplies its runtime. Tests use byte-identical local renamed runtime copies, excluded from the ZIP. This is not a standalone dependency-free DLL, and no clean-machine deployment was tested.

## Review fixes and residual limits

Read-only review plus tests corrected the worker status-consumption race, waiter/handle-close race at unload, initialization publication race, locked-session reconnect handling, stale tray-menu indexes/intent, case normalization/duplicate identity handling, invisible-window-border media attribution, initial error notices, emergency window publication, editor font ownership, and external animation padding. Sole repository write ownership was maintained.

Remaining limits are explicit:

- Win32/COM/storage calls have no absolute bound. `CreateProcessW` can stall beyond the five-second UI startup deadline; cancellation prevents a late result/resume from reviving the session. Final joins may wait indefinitely rather than execute unloaded code. No forced thread termination is used. `ShowWindowAsync` cannot hide a black window while its owner queue is stalled. The emergency measurement covers the launch-worker stall scenario only.
- Startup health detects process exit/owned-child absence, not a saver hanging or drawing incorrectly after creating its child. Black fallback cannot diagnose every graphics failure. Exact focus, clipping, display sleep and saver resource behavior are unqualified.
- Media attribution is deliberately heuristic. Muted display-required-only activity that starts while a saver job exists is not observed through the aggregate signal. Inaccessible, minimized or ambiguous sessions suppress globally; sparse peak sampling may miss sound, and active-but-paused streams may suppress when muted mode is on. Default-endpoint-only coverage is retained.
- Unknown/clone identities remain unavailable for independent operation; port changes may require selecting the new identity. Actual multi-monitor and mixed-DPI hardware evidence is still absent. UI screenshots were not fabricated from the shell-less harness.
- Source build and package format are checked; actual Windhawk import/compile/load callbacks and UI still need qualification. Resource figures are short harness measurements, not saver CPU/GPU/display endurance or an absolute leak-free claim.
- Public naming/original-source license are recorded as **unselected**, consistent with their operator publication gate. Dependency terms are inventoried, not adjudicated. No inherited permissions are inferred. No release should be represented as licensed for public redistribution until those choices are resolved.

No known reproducible crash/data-loss defect remained in the executed checks. These limits remain in the candidate rather than being counted as passed qualification. Next work is the separately deferred LOOP-016 evidence in a suitable automated desktop environment, then public naming/license review if publication is separately authorized. No hands-on testing is requested as a condition of this completed implementation assignment.
