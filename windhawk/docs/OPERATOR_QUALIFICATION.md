# Operator qualification and next steps

## 2026-10-08 physical observations

The operator reports **Display Activity Controls for Windhawk 0.3.0**
(`dac-windhawk`) running in **Windhawk v1.7.3** (`windhawk.exe`). The
questionnaire confirms three or more local active displays. Exact count,
Windows version, monitor models, resolutions/scaling and exported input/profile
scopes were not provided. Live installed source bytes were not independently
hashed during this review.

These are operator-reported physical observations, separate from hidden harness
measurements and the release's build/package evidence.

| Case | Result | Scope |
| --- | --- | --- |
| Constellation starts | Working | All intended displays in the tested configuration |
| Constellation rendering/layout | Working | No blank/frozen/misplaced/clipped windows reported; layout usable with the operator's arrangement |
| Independent pointer wake | Working | Target woke; untouched displays retained protection/timers/sessions |
| Click and wheel wake | Working | Same independent behavior reported |
| Keyboard wake | Working as expected | Exact pointer/focus scope undocumented |
| Unexpected wake-all | No recurrence reported | Tested run only |
| Previous 5–15-second delay | No recurrence reported | No measured input-to-hide timing or absolute bound |
| Wake after pause or UI stall | Working response | Combined question does not establish which condition or a specific induced stall |
| Windhawk enable | Working | Actual enable reported |
| Settings open/render/save/reopen | Working | Values retained; exact settings surface/callback coverage unspecified |
| Disable active presentations | Working | Presentations stopped; full unload timing/resource coverage unmeasured |
| Display scaling usability | Working | Tested scaling; mixed-DPI transitions/small work areas not separately established |
| Tray/menu controls | Working | Tested controls |
| Re-enable/reload/restart without duplicates/orphans | Untested | Initial No explicitly corrected by operator; no duplicate-controller failure reported |
| Emergency Stop/wake | Untested | Explicitly not tested |
| Sleep/resume, lock/unlock, docking/reconnection | Untested | No to whether tested; no failure inferred |
| HDR, high contrast, screen reader, media/games/XInput, stock/custom savers, DDC | Untested | Combined environment question answered No to whether tested |

The operator approved recording multi-monitor Constellation as physically
observed and working for this configuration. Other native styles, hardware and
host environments require their own evidence.

## Field-incident disposition

The Constellation retest provides positive field evidence for independent wake
and the native wake-delay incident. The reported symptoms did not recur in this
run. Broader matrices remain pending in [Open loops](OPEN_LOOPS.md) and the
[native wake checklist](NATIVE_WAKE.md#remaining-operator-field-matrix).

Not established: separate black/clock trials; exact one/two/three-display
combinations; held-button/boundary-crossing cases; sticky/shared/foreground/spanning
scope checks; manual-activation timing; reproducible induced stalls; measured
visible latency; long-running endurance. These limits retain the successful
Constellation observation without expanding it into all-environment support.

## Recommended execution order

| Order | Work | Completion evidence |
| --- | --- | --- |
| 1 | DAC-R06 shutdown investigation | Inventory blocking calls/owning threads; isolated supervised stalls; measure visible recovery, child cleanup, controller exit and unload separately; recommend safe bounded design or documented limitation |
| 2 | DAC-R02 host lifecycle | Real enable/disable, Settings callbacks, restart/reload, emergency exit, host death, singleton behavior and helper/child cleanup in an isolated environment |
| 3 | DAC-R03/R04 desktop/activity cases | Small independent questions and recorded tests when a desktop is available; distinguish untested, failed and working |
| 4 | DAC-R05 hardware, DAC-R07 ARM, DAC-R08 Windhawk 2.0 | Named monitor/adapter recovery, native ARM execution and pinned alpha-toolchain/host evidence |
| 5 | DAC-R09 submission review | Current docs/licenses/source agree; full release gates; selected support claims qualified before 1.0.0 |

The immediate engineering recommendation is shutdown investigation. Version
0.3.0 joins OS-facing workers; cancellation between calls cannot bound an OS
call already in progress. Final joins protect DLL lifetime, so abandoning running
threads is not a safe unload strategy. Disposable helpers may be warranted after
measurement of filesystem, photo, audio and driver paths.

Continue static review and test preparation while physical testing is unavailable.
Keep optional feature expansion behind reliability work. Changing the documented
1.0 gate or support scope needs an explicit product decision.

This documentation changes no runtime source, version or released asset. Retained
standalone remediation and repository governance remain separate queues.
