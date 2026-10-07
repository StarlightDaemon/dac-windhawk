# Release readiness and open loops

DAC has the requested next-beta feature set implemented. The remaining work before
1.0 is primarily real-host, desktop and hardware qualification, plus resolution
of any defects those checks reveal. Optional feature ideas are not missing release
requirements. This report covers the shipping Windhawk product; the older standalone
OLED Aegis application is a separate codebase.

## Independent monitor wake

The operator reported that moving the mouse on the primary display wakes other
idle displays. The 0.2.1 repair removes two code paths that could cause that symptom:

- An unattributed `GetLastInputInfo` fallback, failed raw-input read or delayed
  observation previously reset every display, including independent displays.
  Polling can see activity before its raw-input message is handled. Unknown input
  now credits only explicitly shared-input policies; independent policies require
  a known target.
- A foreground-window change previously reset that window's display in the
  default independent mode. Focus restoration after dismissing an overlay is not
  new input. Foreground observations still feed media and profile rules, but no
  longer independently reset idle timers.

The new policy regression fails against the original source at the cross-display
fallback assertion. The repaired test preserves untouched display states and
generations through fallback-before/raw-input/fallback-after and delayed-input
sequences. The hidden session test uses that sequence with independent saver
children and black presentation. This reproduces the software failure mechanism;
it does not establish which path fired on the operator's physical desktop.

For a normal independent setup, enable **Independent display input**, use inherited
or pointer input scopes, and inspect profiles for shared overrides. Explicit shared
mode still wakes all displays. Keyboard activity may credit both the cursor display
and focused display; foreground-only scopes deliberately follow the focused window.
If input cannot be attributed, Stop/wake and the emergency shortcut remain available.
Spanning presentation and Windows session-wide power transitions remain global.

**Close the physical incident only after updating the installed source and testing:**
disable older renamed controllers; replace the complete source using the release
instructions; idle three displays; move the pointer inside each in turn; verify
only that display wakes and the other two retain their countdown/session. Repeat
with clicks, wheel, keyboard focus elsewhere, boundary crossings, high polling rate,
and after an intentional UI stall. Keep the pointer off display boundaries for the
basic test. Capture redacted diagnostics and exact versions if it recurs.

## Implemented features

| Area | Present in the source | Remaining evidence |
| --- | --- | --- |
| Independent display control | Per-display enable, timers, input/media overrides, start/stop, sticky sessions and Quick setup | Physical multi-monitor isolation and actual host lifecycle |
| Presentations | Native black, dim/fade stage, clock, constellation, photos, stock/custom saver hosting and contained preview | Real saver visuals, bounds, DPI, HDR and third-party compatibility |
| Activity | Pointer/keyboard attribution, media heuristics, app/fullscreen rules, optional XInput and reason/countdown explanations | Real browser/game/audio attribution and device behavior |
| Automation | Manual/app/scheduled profiles, pause, timed snooze and battery-to-black | Real profile changes, sleep/resume, AC/DC and session transitions |
| Maintenance controls | Validated settings, backups/import/reset, stale-draft protection, read-only conflict checks and redacted diagnostics | Recovery usability and long-running behavior on actual desktops |
| Optional physical power | Isolated DDC helper, per-display opt-in, cancellation and fault quarantine | Specific monitor/adapter off/wake qualification |
| Delivery | Version checks, immutable release tags, source/evidence packages, checksums, CI, security policy and compact status badges | Repeat gates on every release and final moderator review |

The original required items I1–I7, E1–E6 and maintenance M1/M2 are implemented.
[Next-beta report](NEXT_BETA_REPORT.md) contains the feature trace;
[adversarial review](ADVERSARIAL_REVIEW.md) contains the production review and fixes.
Older acceptance ledgers describe historical checkpoints and do not override this
inventory or turn completed features back into unimplemented work.

## Environment coverage

| Environment | Evidence and build route | Qualification remaining |
| --- | --- | --- |
| Windhawk 1.7.3, x86/x64 on the available x64 Windows host | Production build, hidden harness execution and verified source packaging; hosted release gate on Windows Server 2022 | Actual Windhawk callbacks, injection/tool startup, reload/unload and physical desktop behavior |
| Windhawk 1.7.3, ARM64 target | `probe-compatibility.ps1` compiles and links the production adapter and both harnesses, checks ARM64 PE headers; CI repeats the cross-compile | No ARM execution: no native ARM device is available |
| Windhawk on an ARM64 PC, including emulated x86/x64 processes | Upstream supports ARM64; source portability is plausible and cross-compilation passes | Host architecture selection, emulation, hooks, saver/helper paths and actual ARM execution |
| Windhawk 2.0.0-alpha.6 | Official pinned portable payload inspected separately; its extracted configuration has an empty CompilerPath and no clang executable | Obtain the matching compiler, reproduce flags, compile, then qualify dedicated host and custom helper behavior |
| Older Windhawk, ARM32, other Windows versions/editions and remote sessions | No additional support promise | Add only a deliberate test matrix; do not infer OS compatibility from a target triple |

Windhawk's [architecture metadata](https://github.com/ramensoftware/windhawk/wiki/Creating-a-new-mod#architecture)
gives `x86-64` special ARM behavior; DAC's existing metadata is not an ARM exclusion.
The [stable 1.7.3 compiler implementation](https://github.com/ramensoftware/windhawk/blob/v1.7.3/src/vscode-windhawk/src/utils/compilerUtils.ts)
includes the aarch64 target. A passing compile does not prove that a DLL loads in
the selected host or that the host can execute the saver/helper architecture.

The [2.0 alpha release](https://github.com/ramensoftware/windhawk/releases/tag/2.0.0-alpha.6)
and [tool-host documentation](https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process)
introduce a separate compatibility task for DAC's custom legacy adapter and
`-dac-power` helper dispatch. Do not promote alpha or ARM to supported just by
relaxing the stable build's version guard. Hosted ARM execution could add evidence
later, but cannot qualify physical displays or DDC.

## Prioritized open work

P1 items gate the corresponding 1.0 behavior or support claim. P2 items are planned
maintenance or extensions; they can remain deferred with explicit scope.

| ID | Priority and owner | Next action and closure evidence |
| --- | --- | --- |
| DAC-R01 | P1, maintainer + desktop operator | Install the repaired source and complete the independent-wake matrix above. Record versions, input scopes, monitor topology and observed untouched sessions; code regression alone does not close the field report. |
| DAC-R02 | P1, maintainer | Run actual Windhawk enable/disable, Settings callback, reload, tool startup, emergency exit, host crash and unload. Prove one controller, correct helper dispatch and no stranded owned children. Harness compilation excludes the real host entry adapter from execution. |
| DAC-R03 | P1, desktop operator | Qualify three-monitor layouts, negative coordinates, mixed DPI, small work areas, docking, HDR, high contrast and screen reader use. Exercise real stock/custom savers; keep per-saver support labels unqualified until observed. |
| DAC-R04 | P1, desktop operator | Test browser/audio sources, muted/quiet media, games/fullscreen, XInput, app/profile transitions, suspend/resume, lock/unlock and Windows display power. Record where heuristics inhibit more than one monitor. |
| DAC-R05 | P1 for hardware support, operator with recovery access | Test explicit DDC off/wake on named monitor/adapter/driver combinations, cancellation, host death and failure recovery. Keep experimental label and physical-button recovery until qualified. |
| DAC-R06 | P1 decision, maintainer | Measure blocked file/photo decode, audio enumeration and driver paths during shutdown. Decide whether v1 requires a strict unload bound. If so, move blocking work to disposable helpers and verify timeout cleanup; otherwise document the residual limitation. |
| DAC-R07 | P2, maintainer | Add native ARM execution when hardware or hosted ARM capacity is available. Check IsWow64Process/Sysnative saver selection, real tool process architecture, engine loading and helper execution before publishing ARM support. |
| DAC-R08 | P2, maintainer | Qualify a complete pinned Windhawk 2.0 toolchain and dedicated host adapter. Preserve the known stable release route until a new route has equivalent evidence. |
| DAC-R09 | P1 before submission, maintainer | Reconcile current acceptance/docs, review licenses and exact source, rerun security/build/package gates, assign 1.0.0 only for the reviewed moderator-submission candidate. Moderator acceptance remains separate. |

Custom `.scr` files and same-account settings remain trusted inputs, not security
sandboxes. See [SECURITY](../../SECURITY.md). No confirmed High/Critical finding
was established by the previous review; that is not a guarantee of absence.

## Features not implemented

These are optional opportunities from [the feature survey](FEATURE_OPPORTUNITIES.md),
not promised work for 1.0. Select them individually after the compatibility gates.

| Candidate | Status and prerequisite |
| --- | --- |
| OLED model suggestions | Deferred; metadata can be ambiguous, so user choice remains authoritative |
| Hardware brightness dim/restore | Deferred; requires original-value ownership, external adjustment handling and physical DDC qualification |
| Explicit global Windows lock/all-displays-off | Deferred; these are session-wide actions, distinct from independent monitor idle |
| Disable/restore Windows screensaver settings | Deferred; read-only conflict diagnostics already exist; mutation requires ownership/restore semantics |
| Local automation interface | Deferred; design an authenticated per-user command boundary; existing hotkeys already work |
| Video, animated GIF playback and audio-reactive scenes | Deferred renderer/decoder work; current photo GIF handling uses the first frame |
| Web, Lively or Rainmeter integration | Separate prototype; new runtime/security/cleanup requirements and no universal preview compatibility |
| Static-region/HUD analysis or exposure counters | Research only; capture/GPU cost, HDR, privacy and measurement need definition |

Cloud accounts, marketplaces, a second updater/service, general monitor OSD control
and unrelated desktop customization are outside current scope. Universal burn-in
prevention/repair, lossless whole-desktop pixel shifting, guaranteed hardware wake
and secure locking by overlay are not product promises.

## Maintenance plan

The hosted run exposed deprecated Node 20 action runtimes. The workflow pins are
updated to the reviewed Node 24 releases of
[checkout](https://github.com/actions/checkout/releases/tag/v7.0.1),
[upload-artifact](https://github.com/actions/upload-artifact/releases/tag/v7.0.1)
and [download-artifact](https://github.com/actions/download-artifact/releases/tag/v8.0.1).
The download action's default digest-mismatch failure remains enabled.

The 2026-10-07 local 0.2.1 validation passed warning-as-error x86/x64 builds,
all sixteen groups on each architecture, both historical-parser runs, version
regressions and 34 packaging rejection cases. The source archive verified with
214 entries. The final-source ARM64 probe passed compilation, linking and PE
checks for the mod and both harnesses; its receipt explicitly records no execution
and no release eligibility. An unsupported alpha recipe produced a failed receipt
as intended. GitHub results remain tied to their exact commit in
[Actions](https://github.com/StarlightDaemon/dac-windhawk/actions/workflows/release.yml).

| Trigger | Maintainer work |
| --- | --- |
| Every behavior/tooling change | Add a meaningful regression, review trust boundaries, run affected checks and the full release gate before publication |
| Every release | Check version/changelog/tag agreement; inspect diff; require clean Git state; observe CI; verify published checksums/source; retain hardware qualifications separately |
| Monthly and on upstream security releases | Review Windhawk stable/alpha changes, compiler and action pins, Windows changes, Fujin/parser fixture provenance and dependency notices; update pins only with validation |
| After driver/monitor/saver changes | Repeat affected desktop/media/DDC cases; record exact equipment and withdraw obsolete compatibility claims |
| After a field report | Reproduce against installed version/settings first; add a failing test where possible; publish a new patch rather than replace released assets |

The older standalone application's audit findings and MSVC build gaps are not
fixed by DAC. [Audit disposition](AUDIT_DISPOSITION.md) tracks that boundary;
its AppData/logging/process-rights/input/persistence/build/CI/provenance work needs
a separate remediation effort. Private repository governance work is likewise
outside this product release report.
