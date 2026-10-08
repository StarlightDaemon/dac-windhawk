# Native multi-monitor wake refinement (0.3.0)

DAC now uses the queued mouse event's position and timestamp to select its display.
A movement that occurred during an existing native presentation remains usable
when the UI resumes after a queue delay; it needs no second movement. The other
independent displays retain their session generation and idle history. The native
black, clock and constellation presentations have no external saver to stop.

## Behavior and boundaries

The main message loop preserves the full signed `MSG.pt` and `MSG.time`; raw input
uses the current thread's signed message-position/time fallback inside nested
Windows modal loops. It never substitutes the current pointer for an old pointer
event. Timestamp expansion handles the DWORD wrap and rejects future/half-range
ambiguity. Topology/reset boundaries reject older and same-millisecond ambiguous
events. New manual and automatic sessions reject input preceding activation.
A policy update that preserves a manual session also preserves its queued wake.

Keyboard focus is a separate observation: fresh input can credit the event's
pointer display and the current foreground display. Events older than 250 ms
cannot establish historical foreground focus, so only their pointer attribution
and explicit shared policy remain usable. Foreground-only scope intentionally
rejects these aged observations. Unknown input cannot wake all independent
monitors. Sticky sessions ignore activity; spanning sessions and explicit shared
scopes retain their deliberate global semantics. Disabled displays, session
blocks, power opt-in and fault quarantine remain enforced.

Raw input immediately cancels stale presentation owners and hides their UI-owned
native shells. Cleanup, new presentation work and notices run outside this wake
path. The 50-ms UI timer no longer queries marker files or calls the shell.

One joined maintenance worker refreshes a fault cache and updates the tray from
bounded snapshots. It holds no controller lock across filesystem or shell calls.
Topology, power start/completion and fault reset invalidate earlier cache results.
A reset reserves its target until deletion finishes, so a new power ticket cannot
be deleted by a late reset. Failed marker queries remain quarantined; only explicit
file/path absence establishes a clear marker. The power worker still performs
its own final marker check before any hardware action.

Tray changes are sent when the displayed state changes. A successful unchanged
tray has a 30-second health check; unavailable tray state retries after one second.
An in-flight attempt is not replaced by periodic retries. Explorer notifications
invalidate stale responses, and the worker tracks its actual shell-icon state.
No helper outlives the DLL: shutdown joins it before destroying its HWND and icons.
A blocking OS call can still delay final unload; there is no forced thread stop
or absolute unload deadline.

## Diagnostics and timing

Use **Diagnostics → Export redacted diagnostics**. The export includes fixed-size
aggregate counts and maximum queue age, UI heartbeat gap, raw-input execution,
wake reconciliation, timer execution, file refresh and tray-call duration.
Queue/heartbeat values are milliseconds; execution values are microseconds.
Rejection categories distinguish timestamp ambiguity, session/topology boundaries,
unknown scope, sticky state and disabled/paused/blocked state. Historical focus
unavailability and stale maintenance results have separate counts. Fault status
separates refresh pending, query failed, marker retained and clear.

No per-packet disk writes, input values, coordinates, display identities, file
paths or window titles are recorded. The existing event ring remains capped at
128 entries; input timing uses a fixed set of counters. Maxima accumulate within
the host lifetime and can describe different events, so they are diagnostic clues,
not a synchronized trace or proof that one measured operation caused another.
Exporting itself is an explicit filesystem operation outside the raw-input path.

## Automated evidence and its limits

The durable `nativewake` group exercises the shipping raw-input wrapper, actual
posted `WM_INPUT` message-loop capture, signed nested-loop fallback, controller
and native hide path. It includes 72 synthetic samples: black/clock/constellation,
one/two/three displays, every target and 0/300/1500/5000-ms event ages. Separate
posted callbacks sit behind nine real UI sleeps across all three native styles;
these must wake once when processing resumes. No physical input is injected.

The final local release run recorded these hidden-window measurements:

| Architecture | Maximum matrix handler | Resume handlers after injected UI stalls | Handler during 1,500-ms background file stall | Pending-worker shutdown join |
| --- | --- | --- | --- | --- |
| x86 | 18 microseconds | 12–71 microseconds | 6 microseconds | 266 ms |
| x64 | 25 microseconds | 11–77 microseconds | 3 microseconds | 250 ms |

Actual queued callback ages reached 5,016 ms. These are hidden-process
handler/queue measurements, **not input-to-photon, monitor
wake, compositor or visible rendering measurements**. Broad responsiveness checks
avoid fragile CI microbenchmarks. The shipping timer is also exercised while the
maintenance worker is stalled.

Additional cases cover cursor crossings, negative and full-width coordinates,
timestamp wrap/future rejection, display replacement, activation boundaries,
raw-read failure, fresh versus delayed keyboard focus, clicks/wheel/held motion,
shared/sticky/spanning/disabled policy, slow/failed/stale file refresh, reset races,
slow/failing tray and stale successful tray responses, 50,000 packets with no
retained handles/GDI/log packets, and shutdown with a pending worker.

`tests/nativewake-negative.cpp` is a source-compatible behavioral control. With
`tools/test-nativewake-regression.ps1 -BaselineSource <unchanged-0.2.1-source>` it
compiles both versions using the same API adapters. Both must pass an age-zero
positive control on A and preserve B; v0.2.1 must then fail specifically because
one 300-ms queued movement does not wake the existing A presentation. The repaired
source must pass that same assertion. The baseline is pinned to SHA-256
`c5b7a7148b27cbff63acf1e73332ff332ded7ab02951d7c80cfe45bf731281a4`.
The script retains compiler logs, stdout/stderr and source-hash/exit records under
`build/investigation/nativewake-control-ARCHITECTURE`. Both x86 and x86-64
controls passed this comparison against the 0.3.0 source; the old source passed
fresh input and failed only the required delayed-wake assertion.

The current release contract requires 17 groups per architecture and 37 named
packaging rejection cases, including missing native-wake receipts on either
architecture, omission from the declared checklist and a missing report.
The final local release gate passed warning-as-error x86/x64 builds, all 17 groups
on each architecture, both historical-parser runs, version regressions and all 37
packaging rejection cases. The resulting archive verified 226 entries. The new
verifier also accepted the existing 0.2.1 archive under its historical contract.
The ARM64 mod and both harnesses passed compile/link/PE checks without execution.
Release evidence and hosted Actions results remain bound to their exact source
and commit. ARM64 execution and Windhawk 2.0 alpha qualification remain separate.

## Remaining operator field matrix

The [operator qualification record](OPERATOR_QUALIFICATION.md) contains the
positive physical Constellation retest. The checklist below covers the remaining
expanded matrix; installation and every listed case are not newly presumed absent.

1. Save a redacted export from the existing incident/version if available. Disable
   older renamed controller instances, then replace the complete installed source
   with the standalone `dac-windhawk.wh.cpp` from the new release. No configuration
   reset is required. This automated delivery does not install or enable the mod.
2. Verify Independent display input and each display/profile scope. Test one, two
   and three protected displays for each native style: black, clock, constellation.
   Move once inside each intended target and then keep the pointer stationary.
   Record visible input-to-hide time separately from diagnostics queue/handler values.
   The normal real-desktop target is below 100 ms; it has not been measured here.
3. Verify the other displays keep their sessions/countdowns. Repeat clicks, wheel,
   held-button motion, boundary crossings and negative-coordinate displays. Check
   keyboard with focus elsewhere, explicit shared/pointer/foreground scopes,
   manual start immediately around activity, sticky sessions and spanning mode.
4. Export diagnostics after any delayed wake. Record DAC/Windhawk/Windows versions,
   native style, number of displays, input scope, visible delay and whether another
   movement was needed. Note tray/Explorer loss, session changes and any unusual
   heartbeat/file/tray maxima. Do not induce hardware power or OS lock for this test.
5. Preserve the incident if delay recurs. The software tests establish the queued
   input loss mechanism and its repair; they do **not** identify the operator's
   actual 5–15-second desktop stall source. Physical observations are recorded
   separately in the qualification record; the full matrix remains pending.
