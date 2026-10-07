# Windhawk V1 beta 1 production report

Version **1.0.0-beta.1**, operator date **2026-10-04**. Internal beta refinement
of the completed production candidate under D-0007 and the operator's request
to plan, build, refine and produce a first beta. P01–P11/S01–S08 remain intact.
The [candidate report](PRODUCTION_REPORT.md) is historical; this report owns
the current beta evidence. Desktop qualification remains in LOOP-016.

## Phases delivered

| Phase | Beta result |
| --- | --- |
| 2: foundation | Version-consistent metadata, build identities, output invalidation and exact source/test/tool/binary evidence binding |
| 3: parity | Existing P01–P11 retained; unknown-only legacy imports rejected; saved configuration exercised through the real UI-owner/controller path |
| 4: savers | Startup and ongoing worker-side child responsiveness sampling; late healthy results cannot revive fallback; independent contained sessions tested |
| 5: verification | Same-source policy/platform tests, hidden production integration, 100 session cycles, evidence rejection cases and archive verification |
| 6: beta | Single-file source, repeatable source ZIP, checksums, diagnostics, emergency-event helper, one-command pipeline, upgrade/removal notes |

The implementation remains in `windhawk/`; local planning and continuity remain
under `.raiden/local/` and `.raiden/state/`. The original checkout and prior work
are preserved. No inherited standalone code/assets, managed Writ, root product
or build files, legacy CI, remotes or Windows policies were changed.

## Refinements and evidence design

Each saver worker locates an owned child beneath its preview host and sends
`WM_NULL` with a 100-ms `SendMessageTimeoutW` timeout. Startup sampling runs at
100 ms with a five-second readiness deadline. Once ready, sampling runs at
one-second intervals; three consecutive missing/unresponsive samples trigger
black fallback and owned-job cleanup, without automatic retry in that session.
This is a responsiveness heuristic, not proof of rendering. The UI consumes
worker results before checking its deadline, and rejects healthy results after
fallback. OS calls and thread joins retain the documented lack of absolute
completion bounds.

The hidden integration harness runs the actual Initialize/controller/Reconcile/
AddRun/worker/cleanup path. Test-only injected displays and contained fixture
children provide two responsive savers plus native black, independent dismissal,
post-start hangs, pause, blocked sessions, saved settings, configuration ownership
and topology cleanup. Harness presentations remain hidden. Test-only seams are
excluded from the production DLL. This is stronger integration evidence without
claiming physical monitor or real Windhawk callback evidence.

Build identities hash every Windhawk C++ and PowerShell input plus key installed
compiler/host dependencies. Artifacts are hashed separately. Each passing check
records its build identity, tested binary hash and log/error/result hashes.
Packaging rejects absent, failed, stale or modified evidence and alternate-source
builds. Timed-out reruns clear old results and receipts before launch. ZIP entries
are sorted with fixed timestamps; every entry is checksummed, and archived source
and evidence are checked against the build identity. Checksums establish internal
consistency and accidental-corruption detection, not publisher authenticity.

## Reproduction and tools

From the repository root in PowerShell:

```powershell
./windhawk/tools/produce-beta.ps1
./windhawk/tools/verify-beta.ps1
```

`produce-beta.ps1` inventories prerequisites, builds, runs all twelve bounded
checks and eight negative evidence cases, packages and verifies. Individual
steps are `diagnose-beta.ps1`, `build-production.ps1 -OutputName beta1`,
`test-production.ps1 -OutputName beta1 -Checks policy,storage,catalog,media,faults,containment,host-death,emergency,lifecycle,probes,sessions,session-soak -TimeoutSeconds 55`,
`test-beta-tooling.ps1`, `package-beta.ps1` and `verify-beta.ps1`.

`diagnose-beta.ps1` reads prerequisite versions/hashes and configuration presence;
it does not collect settings contents, titles, input or photos. Its machine-local
inventory stays outside the ZIP. `stop-beta.ps1` deliberately signals the running
host's session-local emergency event when invoked; it is supplied, not invoked
during beta production. Build/test outputs are in ignored `build/windhawk/beta1/`.

The build uses installed stable Windhawk 1.7.3, clang 20.1.3, explicit
`x86_64-w64-mingw32`, C++23/O2 and warnings as errors. PE timestamps are disabled.
Toolchain binaries and OS savers are never modified or packaged. The source ZIP
includes test evidence and dependency hashes, without distributing test/mod
binaries or runtime libraries. See [release notes](BETA_1_RELEASE_NOTES.md).

## Final validation

The complete `produce-beta.ps1` run passed. Twelve current-build receipts and
the tooling receipt are embedded in the archive, alongside their raw evidence.

| Check | Result and scope |
| --- | --- |
| Policy/configuration | **116,033 assertions passed**, 48 input/media/mute/poll combinations and eight virtual hours; includes startup/ongoing responsiveness boundaries and unknown-only import rejection |
| Storage/catalog/media | Unicode/long-path persistence and failure rollback passed; read-only process/Core Audio query passed; six native savers present; one 2560×1440 output identified in this run |
| Worker failures | Early exit 93 ms; non-window hang fallback/cleanup 5,844 ms; missing path rejected |
| Containment/host death | Child and grandchild owned-job membership and retained process-handle termination checks passed |
| Emergency | Watcher completed within the same timer tick (reported 0 ms) during the injected launch stall; no physical hotkey or blocked-UI bound claimed |
| Lifecycle | 50 init/shutdown cycles after two warmups: 1,547 ms; handles 170→172; GDI 0→0; private bytes 2,404,352→2,609,152; working set 14,258,176→14,471,168; tray exemption explicit |
| Stock probes | All six created responsive owned children and cleaned up in hidden hosts: Bubbles 907 ms, Mystify 890 ms, Ribbons 922 ms, 3D Text 891 ms, Photos 219 ms, Blank 156 ms through cleanup; no visible-rendering claim |
| Production integration | Two concurrent hidden fixture savers and native black, independent dismissal, post-start hang fallback with no retry, saved settings, pause, blocked session, configuration ownership and topology cleanup passed |
| Presentation resource cycles | 100 hidden cycles: 21,438 ms; handles **187→187**, private bytes 2,592,768→3,280,896; windows/jobs cleared each cycle; short process sample, not GPU/display endurance |
| Evidence rejection | Eight cases passed: failed/stale/missing receipts, altered log/binary/input, alternate-source ineligibility and an actual timed-out rerun clearing previous success; successful fixture evidence restored byte-for-byte |
| Archive verification | All 79 included entries verified; deliberately altered source, unsafe path, unlisted file and duplicate entry each rejected; consecutive packaging runs produced identical SHA-256 |

The current catalog has a usable identity, unlike the earlier candidate run.
This read-only observation does not supply physical multi-monitor evidence or
remove the harness tray/input exemptions. Private-memory increases are recorded
without treating short-cycle results as proof of leak freedom.

| Artifact | SHA-256 |
| --- | --- |
| Single-file source | `9106c5dfddb535eda8c0e7ef625e9e9292d933c571d087656f9d911310fef274` |
| Local compiled DLL (not in ZIP) | `0f297bf626e4591c6fec82d21a094ead84202ea890f8f05d9276d8a564bac61d` |
| Policy test executable | `8508c967c0520f4575dd5ea1828f919ea2cee53768f0b4fa31b7f0cc68cdb7cc` |
| Platform test executable | `ba38ed636a18e2f7984d2d9ed628955ee5d0c9fa5e3fd5488063b62dcee0ca10` |

Build identity: `d41270b5b601ca0629b848709e33a674c79a00e2df9ce8481969ed58e5c22b90`.
The final ZIP SHA-256 is recorded in `.raiden/state/WORK_LOG.md`, avoiding a
self-referential hash inside the ZIP. Current build/evidence validation must pass
before repackaging. Repackaging identical inputs is deterministic; a fresh test
run naturally changes timing/process logs and therefore the evidence/archive hash.

## Research and remaining qualification

Primary references informed the changes:
[SendMessageTimeoutW](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendmessagetimeoutw)
documents timeout behavior and flags; probes originate on the worker and target
the foreign child queue. This avoids placing saver health calls on the UI thread.
[CreateProcessW](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw)
supports the existing fully qualified suspended-launch approach.
[GetLastInputInfo](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getlastinputinfo)
documents nonmonotonic input timestamps; the existing conservative change-based
fallback is retained. The [Windhawk mod format](https://github.com/ramensoftware/windhawk/wiki/Creating-a-new-mod)
supports the self-contained source/metadata/readme distribution.

Actual Windhawk import/load/settings/unload, tray UI, visible stock animation,
physical multi-display input/concurrency/DPI, session/power effects and elapsed
CPU/GPU/display endurance remain unqualified. The environment exposes only one
display; product logic disables unidentified or cloned outputs when encountered.
No stock saver is promoted to supported. No manual testing is requested as a
condition of this beta. Original-source license and public naming remain
unselected publication decisions; see [license status](../LICENSE-STATUS.md).
