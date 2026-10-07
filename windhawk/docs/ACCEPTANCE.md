# Acceptance evidence ledger

This ledger retains historical test checkpoints. Current review/remediation is
in [Adversarial review](ADVERSARIAL_REVIEW.md), and the
[development compatibility policy](RELEASING.md#development-compatibility-policy)
permits replacement versions and clean configurations without migration work.
Historical migration observations below are not current release requirements.
Physical runtime/desktop/hardware gaps remain distinct from automated checks.

## RC3 Fujin and icon refinement — 2026-10-05

Both architectures compile warning-clean; all 15 x86 release groups pass,
plus x64 advanced UI/lifecycle checks and 8 packaging-evidence rejection
cases. Theme switches preserve drafts, native checkbox/combo behavior and
DPI window identity. The generated adapter matches Fujin's pinned tag.
See [RC3 report](FUJIN_RC3.md) for exact hashes and paint limitations.
Operator reports RC2 compiles; RC3 has not been installed by this work.
Physical cross-monitor dragging, actual contrast changes and assistive-reader
use still require desktop qualification alongside existing hardware/saver gaps.

Current repair: [RC2 DPI fix](DPI_FIX_RC2.md). Settings now survive injected
monitor-DPI transitions on both architectures, retaining draft controls and
scrolling. RC2 passes all fifteen x86 groups and x64 advanced/lifecycle groups.
The older RC1 evidence below remains historical; physical mixed-DPI dragging
is still unqualified.

The V1 plan (historical internal record; not bundled) is authoritative for the
original P/S requirements. D-0007 separates implementation from deferred desktop
qualification. [V1_RELEASE_REPORT.md](V1_RELEASE_REPORT.md) now owns current
RC1 coverage, source identity, measured results and residual limits. Earlier
candidate/beta reports remain historical. No P/S row is fully desktop-qualified.

## RC1 extension evidence (2026-10-05)

All fifteen groups pass on both x86 and x64. The current report maps the original
contract and added per-monitor time/input/media/fullscreen controls, sticky
shortcuts, black transitions, native photos, custom savers, spanning and opt-in
hardware power. Power tests use fake DDC plus real child-process transport and
acknowledgement checks; no physical off/wake command was executed. Native photo
frames were rendered and sampled. Settings capture returned black images and
does not establish UI quality. Physical multi-monitor, actual RC1 Windhawk
lifecycle/guardian injection and hardware qualification remain LOOP-016.
Beta 2 remains the installed version.

## Beta 1 evidence (2026-10-04)

All twelve current-build checks pass, including 116,033 policy assertions,
hidden controller-to-presentation integration with two concurrent saver fixtures
and native black, independent dismissal, post-start unresponsive-child fallback,
saved settings/configuration ownership and pause/session/topology cleanup.
One hundred hidden presentation cycles retained 187 process handles before/after;
private bytes rose from 2,592,768 to 3,280,896. This is bounded integration evidence
for P07/P08/P10 and S01–S06, not physical input/rendering/endurance qualification.
Eight evidence rejection cases pass; four intentionally malformed archives were
also rejected. Exact source/tool/build/test binding and raw receipts accompany
the beta archive. No deferred desktop row below is promoted to fully passed.

## Production candidate evidence (2026-10-04)

| Contract | Available result | Still deferred |
| --- | --- | --- |
| P01 | Pass policy/parser: all-disabled, retained disconnected Unicode IDs, reorder/reconnect; conservative clone/unknown handling implemented | Physical identity/clone transitions |
| P02–P04 | Pass controller: 48 mode/poll combinations, boundaries, delayed/between-poll input, global/per-display dismissal, virtual monotonic clock | Physical input attribution/latency/media keys |
| P05 | Pass injected audio/mute/peak/endpoint, ambiguity/stale/failure and grace policies; read-only COM sample succeeds | Actual players, devices and browser placement |
| P06 | Pass geometry and negative coordinates; PMv2 black padding/exact saver child compiled | Visible mixed-DPI bounds/cursor behavior |
| P07 | Derived tray state, stable menu intent/IDs, pause/settings/manual/exit implemented; hidden editor resources checked | Actual shell/tray restart and interaction |
| P08 | Pass validated model/import, failed-write rollback, Unicode/long paths; bounded diagnostics | Actual Windhawk log UI and user-facing error UX |
| P09 | Default-off automatic preference, legacy Run guard and documented migration implemented | Real startup sequence |
| P10 | Generation/epoch/reset policy tested; WTS/power/DPI/topology handlers compiled; same-source lifecycle passes | Actual lock/suspend/display power/topology/device changes |
| P11 | Pass legacy field/range/disabled/alias/malformed fixtures and storage failures; importer opens source read-only | Real legacy UI migration |
| S01–S03 | Durable assignments, independent controller sessions, preview/configure/stop code complete | Visible simultaneous same/different savers and config UI |
| S04 | Six native production structural probes clean up; historical x86 probes retained | All visual compatibility; none supported |
| S05 | Pass missing/early/hung-no-child failures, no-retry policy and stale results | Post-start nonrendering/focus failures |
| S06 | Pass production owned-tree stop/host-death, three partial-init cuts, 50 lifecycle cycles and emergency launch-stall injection | Actual Windhawk unload, hotkey/window-hide latency on desktop |
| S07 | Exact unpadded child region, no arbitrary reparenting, no input interception | Visible clipping/focus/independent use |
| S08 | No host wake request/policy writes; virtual 8-hour controller and short resource/lifecycle measurements | Elapsed saver CPU/GPU/display soak and power effectiveness |

## Historical Phase 0/1 evidence

Labels below retain the original experiment's meaning. **Not run** means no execution; **unavailable** names a missing environment; **pass (partial)** applies only to that experiment. Runtime observations remain in [PHASE_1_REPORT.md](PHASE_1_REPORT.md).

| Plan ID | Verification still required | Actual Phase 0/1 evidence | Result / limits |
| --- | --- | --- | --- |
| P01 | Restart, all-disabled, reorder/reconnect, ambiguous/cloned display identity | Temporary catalog has one display; indexes deliberately nonpersistent | Not run; production identity deferred |
| P02 | Global activation/dismissal, 5–3600 s timeouts | No automatic activation in prototype | Not run; later phase |
| P03 | A input leaves B running; mouse vs keyboard attribution and crossings | Raw-input sink implemented; no input interception flags; no real event test | Unavailable: actual multi-display interaction |
| P04 | 250–10000 ms polling, between-poll input and measured scheduling tolerance | Prototype dismissal is event-driven; lifecycle timer fixed at 50 ms | Not run for V1 policy; no polling-parity claim |
| P05 | All media/mute/window/device cases in plan | Core Audio headers compile under installed C++ toolchain | Compile-only; media behavior not implemented |
| P06 | Full bounds, negative coordinates, padding and mixed DPI | Small 640×360 hosts on one 96-DPI display | Not run for full coverage/padding |
| P07 | Tray, controls/settings, shell restoration, pause/exit | Lifecycle harness initializes/cleans; tray add fails in test graphics environment; restore handler compiled | Fail in available environment / actual shell unverified |
| P08 | Durable validation/persistence failures, all-disabled, bounded optional logging | Shared clamped Config; no private file persistence; Wh_Log off unless host logging enabled | Compile/static only; durability failures not run |
| P09 | Explicit per-mod auto-activation, global startup dependence and legacy Run transition | Prototype never auto-activates or writes startup; manual-only adaptation documented | Not run; production startup choice deferred |
| P10 | Resume/topology/DPI/audio-device transitions without orphan windows | Suspend/lock/display-change stop handlers compiled; re-enable required after topology change | Not run; not full recovery implementation |
| P11 | Deliberate validated legacy import, original preservation | No import or legacy file access | Not run; later phase |
| S01 | Distinct savers on A/B, black C, durable assignments | Eight stock binaries individually create preview children; no simultaneous real displays | Unavailable for required scenario |
| S02 | Independent dismissal, typing/focus/media/cursor crossing | Input path compiled; sampled foreground equality during individual probes | Not run for actual input; sampled equality is insufficient |
| S03 | Selected-monitor preview/stop and configure interface | Small-host preview launch and stop tested; configuration action absent | Pass (partial) for launch/stop; configure not run |
| S04 | Every installed candidate visually qualified, same-saver concurrency, exact versions/hashes | Full installed inventory and eight individual process/window probes | Partial evidence; **no saver labelled supported** |
| S05 | Missing, launch error, hung startup, early exit/crash controllable fallback without relaunch | Missing-file and early-exit fallback tested; owned hang cleaned on shutdown | Pass (partial); no visual fallback or hung-startup detector proof |
| S06 | Stop/disable/unload/sleep/lock/removal/host-death cleanup including descendants | Individual shutdown, child+grandchild job close and abrupt host death tested | Pass (partial), harness only; actual mod lifecycle unverified |
| S07 | Visible containment, focus and noninterception on independent displays | /p parent serialization, child window presence, sampled foreground equality | Partial structural evidence; visible bounds/input unverified |
| S08 | Display power-off unaffected, locked/suspended work stops, separate resource measurements | No display-power request API used; handlers compiled | Not run; no CPU/GPU/memory qualification |

Phase 0's checklist, provenance, findings curation and inventory are reviewable. Phase 1's required actual multi-display gate remains **not passed**. There is no “not applicable” waiver of a requested feature. Windows x86 animated binaries absent on this host are explicitly unavailable, not failed compatibility tests.
