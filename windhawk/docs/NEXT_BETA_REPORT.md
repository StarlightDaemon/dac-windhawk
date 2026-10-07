# 1.1.0-beta.1 local testing report

The working source has a subsequent [Quick setup revision](QUICK_SETUP.md).
The release identities and full-suite evidence below describe the preserved
tray-menu package, not that subsequent source edit.

This local beta.1 revision groups the tray menu into eight top-level choices.
Fourteen of the sixteen former top-level actions moved into categories; Start/Stop
and Exit remain direct. All command IDs, enable/check states, stable-display
selection and dispatch are unchanged. The source diff is confined to menu
construction. Independent static review confirmed command coverage and submenu
ownership/cleanup; actual visible tray behavior remains physical qualification.
No new persistent tests or release-tool changes were needed for this layout edit.
All existing validation below was rerun against the revised source.

The original delivery's source, reports, ZIP and receipts remain in400 hashed
files under `build/research/tray-menu-baseline/`. Its source SHA-256 is
`8af200569812ffd1e8c35d88c695e59b10e4a95718bb4d4dfcb3887c40dca51d`
and ZIP SHA-256 is
`72321d34a60850c7cf9a34833200fa09ebe50b706f890fc25cefe69a1052e648`.
The identities in Final evidence below identify the refreshed delivery.

Monitor and Screensaver Activity Control implements the next-beta feature scope
I1–I7, E1–E6 and supporting configuration maintenance. Final validation passed
on 2026-10-06: both architectures passed all sixteen groups, both historical
RC4 parser rollback checks passed, and all 32 named tooling rejection cases
passed. Visible desktop and physical hardware qualification remain open.

Source is the complete standalone
liminal-oled-guard.wh.cpp (historical reference: `../mods/liminal-oled-guard.wh.cpp`). The mod ID remains
`oled-aegis`; the product name is temporary. The source ZIP includes both x86 and
x86-64 evidence manifests. No installed mod, live preference file, Windows policy,
real DDC command or workstation lock was changed by this work. No commit, push
or publication was performed. Preserved RC4 output remains historical evidence.

## Feature and regression traceability

| ID | Production path | Available automated evidence |
| --- | --- | --- |
| I1 | Controller::Explain, ExplanationText, timer/tray status | policy reason/deadline/precedence and native identity/ticket fixtures |
| I2 | ConflictText, SetupProc | nextbeta read-only query/error text; no mutation path |
| I3 | MatchApp, AddAudioContribution, ObserveEndpoint, workspace rule form | Unicode/full-path/name-only, two-source audio and uncertainty, UI form validation |
| I4 | AttributeInput, Controller::Activity, ObserveForeground | policy legacy migration and explicit input/uncertain attribution cases |
| I5 | Controller::Snooze, tray Resume action | policy exact expiry/fresh idle/manual retention/pause boundaries |
| I6 | ShowSetup, IdentifyDisplays, startup | nextbeta checklist/reopen/DPI/label lifetime and no-power fixtures |
| I7 | Record, DiagnosticText, ExportDiagnostics | nextbeta bounded ring, default redaction, saver fallback and fake hardware off/wake outcomes |
| E1 | State::Dim, DimAlpha, AddRun/Reconcile | policy timer origins/opacity/fade, hidden layered styles/negative bounds/input reversal |
| E2 | ResolveProfile, EffectivePolicy, SelectPolicy, WorkspaceProc | policy precedence/debounce/schedules/caps and native draft capture/load/mapping |
| E3 | StartDraftPreview, contained Run lifecycle | delayed/hung child, editor/preview close, invalid photos, timeout, independent production output |
| E4 | XInputAdapter, ControllerActive, ConsumeAdapters | injected held/deadzone/release/disconnect/reconnect/missing API and polling bounds |
| E5 | ReadPowerSource, Controller::Tick, retained native shell | pure AC/DC/Unknown and native saver-to-black/late-result/cleanup checks |
| E6 | PaintScene, Run, Reconcile | concurrent native scenes, actual hidden GDI rendering sample, bounded resources, repaint-stop assertions |
| M1 | Parse/Commit/Save, Portable/MapImported, native drafts | schema2 backup, Unicode/bounds/future schemas, atomic failure, reset, exact rollback/recovery retention |
| M2 | release-spec/evidence/archive/package tooling | exact release/check/case authority, dual-target binding, historical verifier and rejection suite |

| Baseline obligation | Retained checks |
| --- | --- |
| R1 identity/independent settings/input | policy, catalog, sessions, nextbeta |
| R2 native black/manual | policy, sessions, advanced |
| R3 independent/custom savers/span | faults, containment, host-death, probes, sessions, advanced |
| R4 sticky/shortcuts/emergency | policy, emergency, advanced |
| R5 media/fullscreen | policy, media, nextbeta |
| R6 photos/placement | slides, nextbeta |
| R7 timer/DDC guardian/quarantine | policy, power, nextbeta |
| R8 theme/high contrast/DPI/persistence/lifecycle | storage, lifecycle, advanced, nextbeta |
| R9 host startup/evidence | lifecycle, tooling rejection suite, archive verifier |

## Final evidence

The final source SHA-256 is
`aae592132a2b166486de3e8ab6ebd306eb67b5d8eb4533621bca91ddaf8a5f6b`.
The common executable-input inventory digest is
`209e643ea69ac1894e81db507434018456cca292e769acfafde5139e1ea34a07`.
These identities supersede every intermediate run.

| Architecture / target | Final build ID |
| --- | --- |
| x86 / i686-w64-mingw32 | `2ee467ce1465599e198f98f68a04228e92059a425e0f981f38b66569dd7f80e9` |
| x86-64 / x86_64-w64-mingw32 | `f3ae6f5218e0524fb7aa0ed00775c0ee13200fd5039375d47c78ccef37dc5111` |

| Required check | x86 | x86-64 |
| --- | --- | --- |
| policy | passed | passed |
| storage | passed | passed |
| catalog | passed | passed |
| media | passed | passed |
| faults | passed | passed |
| containment | passed | passed |
| host-death | passed | passed |
| emergency | passed | passed |
| lifecycle | passed | passed |
| probes | passed | passed |
| sessions | passed | passed |
| session-soak | passed | passed |
| advanced | passed | passed |
| power | passed | passed |
| slides | passed | passed |
| nextbeta | passed | passed |

Both builds passed warning-as-error compilation and the pinned Fujin check.
Each policy run passed 116185 assertions, 48 input/media/mute/poll combinations
and an eight-hour virtual-time controller soak. No subset or package-declared
shortened check list is accepted.

| Measured hidden-process sample | x86 | x86-64 |
| --- | --- | --- |
| Lifecycle, 50 cycles after two warmups | 3031 ms; handles 236→238; GDI 10→10 | 2422 ms; handles 217→219; GDI 10→10 |
| Presentation, 100 fixture cycles | 21922 ms; handles 251→251; private bytes 3784704→4493312 | 21375 ms; handles 233→233; private bytes 3493888→4251648 |
| Original scenes, 400 rendered frames | 34.335 ms wall / 31.250 ms CPU; handles 266→266; GDI 23→24 | 30.194 ms wall / 31.250 ms CPU; handles 249→249; GDI 23→24 |

The scene GDI increment is the retained scene font, released on stop. These
small hidden rendering samples establish bounded fixture behavior, not GPU,
visible display, HDR or long-duration endurance qualification. Fake DDC tests
also verify permission-revocation recovery and redacted terminal outcomes.

Local build manifests and all receipts are in `build/windhawk/nextbeta/` and
`build/windhawk/nextbeta-x64/`. The deliverable is
`build/windhawk/nextbeta/monitor-screensaver-activity-control-1.1.0-beta.1-source.zip`.
The final ZIP hash is recorded outside the archive in
`.raiden/state/WORK_LOG.md`, avoiding a self-referential archive hash. The archive
contains the complete source and evidence; compiled binaries stay in the local
build directories and their exact hashes are retained in its build manifests.

The static `nextbeta-v1` release specification binds version `1.1.0-beta.1`,
source filename, source/input hashes, x86 and x86-64 targets, required reports,
exact named 32-case tooling evidence and per-architecture build/test receipts.
The source ZIP carries `evidence/release.json`, `evidence/x86/`,
`evidence/x86-64/` and aggregate tooling receipt. A SHA256SUMS inventory covers
every payload entry; duplicate, unsafe and unlisted archive paths are rejected.
These checks detect stale, corrupt or misbound local evidence; they are not a
signature or adversarial authenticity guarantee.

Required `evidence/rollback/` additionally contains the actual pinned RC4 source,
generated parser driver, both restored schema-2 fixtures and parser logs. Its
receipt binds the historical ZIP/source hashes, current source/input/build IDs,
rollback tool/driver hashes, tested executable hashes and fixture/log hashes.
Both actual old-parser runs passed: restored schema-2 bytes were accepted,
schema 3 was rejected without changing the parse destination, and the isolated
recovery marker remained byte-identical. Missing, misbound, unsupported-schema
or changed-log rollback evidence is rejected. The 32-case tooling suite includes
36 missing-rollback-evidence variants and five rollback-binding variants. Its
actual timeout case confirms termination of the exact owned harness, invalidates
the old pass, and restores every saved evidence file byte-for-byte. Restoration
failure withholds aggregate success. Separate temporary-file smoke verified a
215 ms transient-lock recovery, bounded 5065 ms persistent-lock rejection and
immediate unrelated-I/O rejection; no trusted receipt is generated by that smoke.

Reproduce from the repository root with the existing qualified toolchain:

```powershell
./windhawk/tools/sync-fujin.ps1 -Check
./windhawk/tools/diagnose-beta.ps1 -OutputName nextbeta
./windhawk/tools/build-production.ps1 -OutputName nextbeta -Architecture x86
./windhawk/tools/build-production.ps1 -OutputName nextbeta-x64 -Architecture x86-64
. ./windhawk/tools/evidence.ps1
./windhawk/tools/test-production.ps1 -OutputName nextbeta -Checks $AegisChecks -TimeoutSeconds 55
./windhawk/tools/test-production.ps1 -OutputName nextbeta-x64 -Checks $AegisChecks -TimeoutSeconds 55
./windhawk/tools/test-rollback.ps1 -Archive ./build/windhawk/rc4/liminal-oled-guard-1.0.0-rc.4-source.zip
./windhawk/tools/test-beta-tooling.ps1 -OutputName nextbeta
./windhawk/tools/package-beta.ps1 -OutputName nextbeta
./windhawk/tools/verify-beta.ps1 -Archive ./build/windhawk/nextbeta/monitor-screensaver-activity-control-1.1.0-beta.1-source.zip
```

Windhawk 1.7.3 and its bundled clang 20.1.3 compile both targets. Fujin v0.1.0 pin
`c653620262ef68fa8d58504b6c47bb21aadc5aa5` is checked before builds. No new runtime
dependency was installed. Build and receipt JSON files record exact toolchain,
source, test and tool identities. Any executable input edit requires rebuilding
and rerunning receipts; documentation may be finalized before packaging.

The rollback command has an explicit historical dependency: the preserved RC4
ZIP SHA-256 must be
`bf24f21219674d519a57576f913c899a572ac0944af089245d858e90a6676378`.
Its source SHA-256 is
`ed923dfd5704e27fb10f897ccf02653d18ebe4980092a50cd5fc0a22c14d7dd4`.
Provide that historical archive separately when reproducing from an extracted
next-beta ZIP; the new source ZIP does not contain the entire old archive.
`produce-release.ps1 -HistoricalArchive <path>` accepts its location and runs
the same full sequence. Historical RC4 output directories are never overwritten.

## Qualification boundaries and operation

Policy tests execute production parsing and controller logic with simulated
time and observations. Native tests use hidden owned windows and isolated
executable-local preferences, real owned fixture process trees, injected XInput
and fake DDC results. Read-only catalog/media queries and hidden stock-saver
structural probes do not establish visible rendering or attribution behavior.
Lifecycle and presentation soaks measure process resources, not panel endurance.

Physical qualification is still required for actual Windhawk enable/unload,
tray visibility, mixed-DPI/negative-coordinate usability, dim focus/click-through
and HDR/fullscreen behavior, real controller/Steam Input and media attribution,
AC/DC/suspend transitions, physical DDC off/wake/failure and long CPU/GPU/display
endurance. Keep LOOP-016 open. Missing physical equipment is not represented as
a passing test or as an unfinished implementation feature.

Use the [release notes](NEXT_BETA_RELEASE_NOTES.md) for input/rule/profile
semantics, contained preview, new scenes, schema migration and exact rollback
procedure. RC4 requires restoring the verified schema-2 backup as well as old
source; preference rollback never deletes unresolved `.power-*.pending` state.

No source, art, animation or runtime from researched applications was copied.
The clock and sparse scenes are original native rendering. Existing Fujin
attribution and dependency notices remain in [PROVENANCE](PROVENANCE.md),
[THIRD_PARTY_NOTICES](THIRD_PARTY_NOTICES.md) and
[LICENSE-STATUS](../LICENSE-STATUS.md). Local testing does not resolve the
remaining public-distribution licensing work.
