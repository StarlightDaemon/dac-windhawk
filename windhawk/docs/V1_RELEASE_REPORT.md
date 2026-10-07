# V1 release candidate 1 — implementation and evidence

Date: 2026-10-05. Scope: the operator's production continuation and explicit
choice of both black blanking and opt-in hardware power, focused on protection
and screensavers. This is an original Windhawk implementation. Retained native
source/assets and RAIDEN Writ were not changed.

**Outcome:** 1.0.0-rc.1 implements the requested protection/screensaver expansion.
Both x86 and x64 builds pass all fifteen available test groups. It is a local
production candidate, not a fully qualified stable hardware release. D-0007
permits implementation/delivery without hands-on testing; LOOP-016 retains
physical-desktop qualification. The running beta 2 installation was not updated.

## Development phases

| Phase | Delivered | Evidence boundary |
| --- | --- | --- |
| Requirements/research | Original P01–P11/S01–S08 parity retained; Actual Tools protection/screensaver features mapped; hardware scope clarified | Behavioral references, no inherited implementation copied |
| Foundation | Separate architecture metadata lines; WOW64 native savers retained; recoverable missing tray; input attribution; configuration exit errors; isolated harness objects | Same-source compilation and policy/platform tests |
| Monitor controls | Individual deadlines, input/media overrides, fullscreen inhibition, live state, targeted black/start/stop/wake, sticky mode, configured shortcuts | Policy, hidden UI, simulated topology/input and hotkey dispatch |
| Screensavers | Concurrent stock/custom previews, per-monitor native photos, spanning, saver-to-black delays | Owned fixture children, native frame pixel checks and hidden stock probes |
| Hardware power | Explicit opt-in, manual/timed soft-off, loaded guardian owns wake, exact identity/nonce checks, cancellation, fault quarantine | Fake DDC actions plus real child-process transport; no physical power commands |
| Qualification/tooling | Fifteen checks on both architectures, source/build/test binding, rejection tests, diagnostics and package verification | Available automated evidence only |
| Delivery | Complete single-file mod, reproducible build/package entry point, source archive, usage/rollback guidance and current report | Local, uncommitted and unpublished |

## Expanded behavior

Monitor preferences now include an override for idle seconds (0 inherits), input
scope and media suppression (inherit/global/local where appropriate), a
saver-to-black delay and a hardware-off delay. Both delays begin with the
presentation, and 0 disables that timed transition. Hardware remains false by
default. Fullscreen inhibition is optional and independent of audio suppression.
Media observation enumerates active output endpoints; process/window attribution
remains a heuristic, including conservative same-executable browser attribution.

Manual sticky sessions ignore input until explicitly stopped/toggled or reset by
pause, session, topology or shutdown. Optional Ctrl+Alt+F1–F12 assignments toggle
the pointer monitor or all enabled monitors. Conflicts are reported; emergency
Ctrl+Alt+Shift+F12 remains separate. Tray creation failure temporarily blocks
protection and retries instead of permanently terminating the host.

Custom installed .scr files use the same contained preview/configuration worker
as stock savers. Photos use independent folder playlists, interval, shuffle,
subfolders, background color, fit/fill/stretch/center/tile/fit-without-enlargement
placement. GIF is first-frame only. Playlists are captured per session, bounded
to 2,000 files and 16 directory levels; junctions/reparse points are skipped.
Enumeration excludes files above 32 MiB; decoded images above 40 million pixels
are rejected. Rendered frames have a maximum 4096-pixel dimension. Unusable
files/folders keep a controllable black fallback. Slow OS/codec calls are not
claimed to have an absolute cancellation bound.

Spanning creates one virtual desktop canvas using the pointer monitor's saved
assignment, assets and black transition. All physical outputs must be identified
and enabled, avoiding unrequested coverage of disabled screens. Input anywhere
dismisses a normal span. Spanning does not implicitly power off physical outputs.
Enabled-monitor group actions remain available for independent presentations.

Hardware uses VCP D6=04 (DPM off) and D6=01 (on), never a global monitor-power
broadcast. No startup scan or capabilities-string query is issued. Each request
requires an exact stable identity, a single physical target, saved opt-in and an
operation-specific ticket. It refuses to take ownership of an already-off display.
One dedicated helper stays loaded for the entire off/wake cycle. The parent
accepts only a matching `awake`/`untouched` acknowledgement; an ordinary Windhawk
process exiting zero is not treated as success. DDC runs outside the tray thread.
An initial five-second query deadline requests cancellation; a separate five-
second recovery grace precedes helper termination. Confirmed process termination
is awaited to avoid racing a remaining writer; a kernel/driver stall can still
make shutdown unbounded. These are soft deadlines, not a hard OS guarantee.

Failed/uncertain cycles retain a per-display marker and refuse another off request
until the user confirms physical wake and resets the fault. Normal activity,
Stop, settings/session/topology reset and disable request wake. Host death closes
the helper's kill-on-close job and may leave the monitor off; physical wake is
required in that case. Hardware/connection compatibility is not established by
DDC-read success or the tests in this report. A buggy driver can fail outside
process containment; no claim of guaranteed hardware wake is made.

## Verification on final source

Toolchain: installed Windhawk 1.7.3, clang 20.1.3, Windows APIs targeting Windows
10. The installed Windhawk host is x86; both requested binary architectures were
verified as PE x86/x64. The read-only catalog exposed one identified 2560×1440
output. Multiple display cases use explicit fixture topology.

| Check | Result on x86 and x64 | What it establishes |
| --- | --- | --- |
| policy | Pass, 116,056 assertions each | Existing 48 mode combinations, eight-hour virtual clock, new per-display policy, attribution, serialization and independent power-recovery deadlines |
| storage | Pass | Unicode/long paths, atomic replacement failure and prior configuration retention |
| catalog | Pass | Identity observation and architecture-correct installed saver discovery; x86 uses native x64 savers through Sysnative |
| media | Pass | Read-only all-active-endpoint sample and limited-rights process identity; no playback matrix qualification |
| faults | Pass | Missing file, early exit and hung-startup fallback |
| containment / host-death | Pass | Exact owned child/grandchild termination; these are saver fixtures, not proof of monitor wake after host death |
| emergency | Pass | Event-driven cancellation while launch stalls; real global hotkey registration is intentionally not exercised |
| lifecycle | Pass, 50 cycles after warmup | UI/font/handle cleanup; x86 handles 191→193, x64 218→220; GDI 3→3 both |
| probes | Pass | All six installed native stock savers produced responsive owned preview children and cleaned up; hidden structural observation only |
| sessions | Pass | Concurrent independent fixture savers/black, hung-after-start fallback, input/settings/pause/session/topology cleanup |
| session-soak | Pass, 100 cycles each | x86 22,343 ms, handles 214→214; x64 21,359 ms, handles 196→196; hidden processes, not GPU/display endurance |
| advanced | Pass | Recoverable tray startup, settings/options draft, span bounds/disabled exclusion, configuration failure notification and sticky shortcut dispatch |
| power | Pass | Shared guardian protocol with fake DDC; off/wake/cancel, failed wake quarantine, serialized generations; real fixture process/event transport; missing acknowledgement rejected despite exit zero |
| slides | Pass | Actual native frame rendering, six placements/background pixel checks, corrupt image rejection, concurrent independent photo sessions and missing-folder fallback |

The settings capture API produced black PNGs in the hidden harness despite a
successful API return. Those images were inspected and **do not count as visual
UI qualification**. Control creation/draft/font tests remain valid. Actual
high-DPI layout, keyboard navigation and the new tray menus remain LOOP-016 work.

Eight release-evidence rejection cases passed for the x86 deliverable: failed,
stale or missing receipts; edited logs/binaries; alternate source; changed
test/tool inputs; and an actual deliberately timed-out rerun. Original passing
evidence was restored byte-for-byte after that negative test. Artifact receipts
bind all C++/PowerShell inputs to the build identity. Documentation is included
and independently checksummed in the source ZIP.

Final source SHA-256:
`616be4c3ea441d2dc9d548b9aeaf5349b9051be7fb89824eea37470beb688b04`

x86 DLL SHA-256:
`d877512c80cebddc0b46e0bd9744b539056ffc2f221ca7636227c42a26cf97af`

x64 DLL SHA-256:
`608b6627827bd91e9693fdcda54a5c7643f515fef0fb6d217eceaddeed50e2f0`

Build/test outputs are under ignored `build/windhawk/rc1` and `rc1-x64`.
The source package is `build/windhawk/rc1/oled-aegis-1.0.0-rc.1-source.zip`.
Its package identity belongs in RAIDEN WORK_LOG, avoiding a self-referential ZIP
hash in an included document. The archive carries the x86 release receipts;
the supplemental x64 receipts remain in its local build directory.

## Reproduction and upgrade

Use `windhawk/tools/produce-release.ps1` for the host architecture. To repeat
the supplementary architecture check:

```powershell
./windhawk/tools/build-production.ps1 -OutputName rc1-x64 -Architecture x86-64
. ./windhawk/tools/evidence.ps1
./windhawk/tools/test-production.ps1 -OutputName rc1-x64 -Checks $AegisChecks -TimeoutSeconds 55
```

See the [current README](../README.md) for loading, controls, opt-in power,
settings schema 1→2 migration, rollback and removal. Existing normal beta
settings are backed up before the first schema-2 save. Live installation source
and settings were not changed by this development/verification run.

## Deferred qualification and scope

No stock/custom saver is promoted to supported from these probes. Actual RC1
Windhawk enable/disable/unload, persistent helper injection/unload, visible
rendering/focus/containment, physical independent input, mixed DPI, Explorer
restart, actual media/player movement, sleep/resume, monitor reconnect and DDC
off/wake remain unqualified. Beta 2's observed real host startup does not prove
these new RC1 paths. OS/codec/driver calls may stall; CPU/GPU/display endurance
has not been measured. No secure-lock integration or arbitrary named monitor
groups is claimed. Taskbars, wallpapers and window-management features are
outside the operator's clarified scope. The photo feature uses folder playlists,
not an editor for multiple independent image-source lists.

Public licensing/naming and any public binary/source distribution remain
separate decisions under [LICENSE-STATUS.md](../LICENSE-STATUS.md). This report
does not certify the unchanged inherited standalone edition.

## Primary research references

- [Actual Tools independent screen savers](https://www.actualtools.com/windowmanager/help/features/multi_monitor_screen_saver.php)
  and [idle-screen protection](https://www.actualtools.com/windowmanager/help/features/save_idle_screens.php): independent/whole-desktop previews, activity and sticky controls.
- [Actual Tools screen saver settings](https://www.actualtools.com/windowmanager/help/userinterface/screensaver.php): slideshow and monitor configuration reference; no code copied.
- [Microsoft DDC/CI guidance](https://learn.microsoft.com/en-us/windows/win32/monitor/using-the-low-level-monitor-configuration-functions)
  and [SetVCPFeature](https://learn.microsoft.com/en-us/windows/win32/api/lowlevelmonitorconfigurationapi/nf-lowlevelmonitorconfigurationapi-setvcpfeature): hardware API semantics.
- [Microsoft PowerToys power constants](https://github.com/microsoft/PowerToys/blob/main/src/modules/powerdisplay/PowerDisplay.Lib/Drivers/NativeConstants.cs)
  and [Power Display cautions](https://learn.microsoft.com/en-us/windows/powertoys/power-display): D6 mapping and monitor/driver limitations; capability-string probing deliberately avoided.
- [GDI+ image loading](https://learn.microsoft.com/en-us/windows/win32/api/gdiplusheaders/nf-gdiplusheaders-image-fromfile): installed Windows image support.
- [Windhawk 1.7.3 loading](https://github.com/ramensoftware/windhawk/blob/v1.7.3/src/windhawk/engine/mod.cpp),
  [process injection](https://github.com/ramensoftware/windhawk/blob/v1.7.3/src/windhawk/engine/new_process_injector.cpp)
  and [application dispatch](https://github.com/ramensoftware/windhawk/blob/v1.7.3/src/windhawk/app/app.cpp): persistent guardian and explicit acknowledgement design. Source reasoning, not a live RC1 bootstrap observation.
