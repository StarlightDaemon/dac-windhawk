# Monitor and Screensaver Activity Control — 1.1.0-beta.1

Current follow-up: [beta.4 integration settings](WINDHAWK_INTEGRATION.md) adds
native Windhawk preferences and rewrites Details. The original beta.1 package
and its evidence remain preserved; the feature guide below covers that series.

This is a local testing beta. It adds all I1–I7 and E1–E6 capabilities from the
[feature contract](FEATURE_OPPORTUNITIES.md). Automated policy, hidden native
windows and isolated process fixtures are distinct from visible desktop,
controller, HDR and monitor-power qualification. See the
[verification report](NEXT_BETA_REPORT.md) for exact evidence and open limits.

## Upgrade and first use

Preserve customized source before replacing the complete contents of the
existing `local@oled-aegis` editor with
liminal-oled-guard.wh.cpp (historical reference: `../mods/liminal-oled-guard.wh.cpp`). Keep its separate
`x86` and `x86-64` architecture metadata lines. Compile the existing mod rather
than enabling a second copy. This development task does not install or enable it.
Windhawk owns startup; no extra autostart service is needed.

Fresh configuration has automatic activation off. In the current working source,
**Quick setup** opens on first use and can be reopened from
**Settings & setup → Quick setup…**. It edits display enablement, native protection
style, per-display idle time and base automatic activation. In the current
[beta.3 revision](QUICK_SETUP_REFINEMENT.md), **Save setup** applies pending edits
and keeps the window open; **Advanced settings** continues editing without saving.
**Close** confirms before discarding unsaved edits. Profiles can override these
base settings. The preserved beta.1 package predates editable setup.
**Displays → Identify displays** shows
temporary number/name labels for five seconds without changing power. Choose
identified outputs deliberately in Settings, try native black, then enable
automation if wanted. Unknown output identity cannot authorize protection or DDC.

The settings draft is unsaved until Save. Status updates, theme changes, DPI moves
and topology notifications retain that draft. **Refresh display list** explicitly
stores the current monitor draft before rebuilding the list. Disconnected
preferences remain keyed by their original stable identity.

## Tray menu

The right-click menu has eight top-level choices. **Start enabled monitors**
changes to **Stop all** while a presentation is active; **Exit until mod
re-enabled** also stays directly accessible. Other commands are grouped below.

| Category | Commands |
| --- | --- |
| Presentations | Start enabled monitors (sticky); span desktop using the current monitor's saver; black all enabled monitors |
| Automation | Pause for this session; snooze for 5/15/30/60 minutes; Resume now with remaining snooze time |
| Displays | Identify displays; each display's status and its six actions: saved-assignment preview, stop/wake, configure saver, sticky toggle, black, opt-in hardware off |
| Profiles | Automatic profile selection and each saved named profile |
| Settings & setup | Settings/import and Quick setup (working source) |
| Diagnostics | Read-only conflict diagnostics and redacted diagnostics export |

The categories reorganize the existing commands; display eligibility, disabled
items, the pause checkmark, profile selection and command behavior are unchanged.
This is a local revision of beta.1, with refreshed build and package evidence.

## Activity, exceptions and temporary controls

Per-display activity choices are:

- **System input:** any real mouse or keyboard input resets all eligible outputs.
- **Legacy combined local:** mouse credits the pointer output; keyboard credits
  the pointer and foreground-window outputs. Existing schema-2 settings retain
  this behavior, including its foreground-change observation.
- **Pointer display:** real mouse and keyboard events credit the pointer output.
- **Foreground display:** real input credits outputs substantially overlapped by
  the foreground window. A window crossing outputs can credit more than one.

Missing, uncertain or late attribution conservatively credits eligible outputs.
An unchanged focused window does not continuously generate activity. Sticky
manual presentations ignore activity until Stop, their toggle shortcut, pause,
session/topology cleanup or emergency exit. Emergency exit remains
**Ctrl+Alt+Shift+F12**.

In **Profiles / application rules**, choose an action, scope and executable match.
Foreground inhibition and ignoring one observed application's audio are separate
actions. Full executable path matching is preferred and Windows ordinal case
insensitive, including Unicode. Explicit **Name only** matching is less precise.
Global scope is distinct from attributed displays. Audio rules filter matching
known sources before aggregation: an ignored browser does not waive another
player or an unattributed/ambiguous session. Owned saver playback is excluded.
Browser tabs and protected/denied process identity are not guessed.

Tray **Automation** snooze actions last 5, 15, 30 or 60 minutes. Snooze blocks automatic
activation, leaves manual actions available, shows remaining time and offers
**Resume now**. Expiry or Resume starts a fresh idle interval. Snooze is
session-only and is never restored after restart. **Pause** remains a separate
session control and retains its existing manual-start restriction.
Within the same process the snooze deadline survives suspend: resume retains
remaining time, or an expired deadline gives a fresh idle interval.

Reasons describe disabled, manual-only, paused, snoozed, session-blocked,
foreground-app, fullscreen, media, stale observation, running and fallback
states. Activation countdowns appear only when activation is eligible.
Presentation-relative black/off deadlines are labeled separately. A hardware
fault blocks further off attempts while native black remains available.

## Dimming, battery and controllers

Optional per-monitor dimming is a click-through, nonactivating software black
overlay. Its percentage is opacity, not measured panel brightness. Fade and dim
warning duration precede the assigned presentation; existing black and hardware
delay timers begin when that presentation starts. Zero dim preserves the prior
timing behavior. Input/Stop reverses the overlay promptly. HDR luminance and
exclusive-fullscreen coverage still need physical desktop testing.

The opt-in XInput observer uses installed system APIs, stick/trigger deadzones
and documented buttons. Held significant controls continue to count even without
a changed packet; neutral drift does not. Release counts once; disconnect clears
held state, and reconnect starts fresh. Polls occur no more often than every
250 ms for connected slots and two seconds for disconnected slots. Slow shared
media queries can delay samples; stale controller observations conservatively
inhibit automatic activation. Missing XInput is an unavailable/inactive
observation. Credible foreground outputs determine scope; unknown attribution
credits all. This is XInput support, not universal HID/Steam Input compatibility
or process ownership detection, and it never injects into a game.

**Black on battery** temporarily substitutes native black for expensive content
on DC. AC, DC and Unknown are distinct. The saved assignment stays unchanged;
returning to AC does not revive content retired in the current session. A later
activation uses current power status. Suspend/resume follows normal lifecycle
cleanup and fresh observations. Hardware off still requires its separate local
per-display grant and successful recovery state.

## Profiles and presentations

Profiles can be selected manually or triggered by a foreground executable or a
local time interval. Precedence is manual selection, first matching app profile,
first matching schedule, then base policy. Automatic changes require a stable
candidate for one second; identical effective policy does not restart sessions.
Intervals use `HH:MM`, include the start and exclude the end, and may cross
midnight. Equal start/end disables the interval. Clock or timezone changes
evaluate the current local minute without replaying missed switches.

**Capture / replace** copies the current main settings draft into the named
profile. **Load selected into settings draft** lets you review and edit a profile;
capture it back deliberately when ready. Saving the profile library and selecting
a policy are explicit actions. Base disabled outputs and current local DDC grants
cap every profile; a profile cannot grant itself authority. Running manual/sticky
presentations retain their captured assignment, timing and padding while safety
revocation remains immediate. Disconnected entries survive profile changes.

**Preview unsaved draft** opens a separate resizable contained window for native
black, photos, compatible installed `.scr` files and native scenes. It does not
save the draft, lock the workstation or issue hardware commands. Closing it or
Settings cancels its child process; it expires after two minutes. Tray **Displays
→ monitor → Preview saved assignment** continues to use the saved full-monitor
assignment. External `.scr` files are
programs and must support preview hosting; use trusted installed files.

New original native presentations are a moving dim clock and sparse animated
points. Choose dim neutral, violet or teal, optional scene rotation and maximum
seconds to black. Each monitor owns its scene and cleanup, with at most 10 frame
invalidations per second. Black/fallback stops animation invalidation. These
scenes make no burn-in prevention or power-saving measurement claim.

## Local diagnostics

Under tray **Diagnostics**, **Read-only conflict diagnostics** reports Windows screensaver enabled/timeout queries,
including failures, and advisory known controller process names. It is not an
exhaustive conflict detector. A settings link is provided; nothing is disabled
or terminated automatically.

**Diagnostics → Export redacted diagnostics** writes a local redacted text report: version, policy
summary, display aliases, bounded reason transitions and saver/hardware outcomes.
The ring retains at most 128 events with bounded labels. Export excludes paths,
usernames, hardware identities, screenshots, window titles, key contents and
recovery tickets. It remains usable after a failed operation. Private settings
and recovery files should not be shared as diagnostics.

## Preferences, import/reset and rollback

The path stays `%LOCALAPPDATA%\OLED_Aegis_Windhawk\settings-v1.ini`; schema is now 3.
Before the first successful schema-2 migration, Save preserves and verifies exact
original bytes at `settings-v1.ini.v2-backup`. An existing conflicting backup,
unreadable original or failed backup/write cancels the save without replacing the
original. Unknown future schemas fail clearly. Explicit confirmed reset can
preserve invalid/future bytes in a separate `.reset-backup-<tick>` file before
installing valid defaults. Ordinary Save does not overwrite invalid input.

Profile export excludes runtime state, snooze/deadlines, hardware grants and
recovery state. Import requires explicit source-to-local display mapping (or an
explicit preserved disconnected identity). Review the imported policy through
**Load selected into settings draft**, then deliberately renew automation and
local hardware choices. Per-monitor and all-preferences resets back up preference
bytes first. None of these actions clears unresolved recovery markers.

To return a migrated schema-2 installation to RC4:

1. Stop/wake active protection and disable the mod before changing source or
   schema. Preserve current customized editor source and a private settings copy.
2. Preserve schema-3 `settings-v1.ini` separately, then copy the verified
   schema-2 `.v2-backup` bytes over `settings-v1.ini`.
3. Restore the complete RC4 source from the preserved RC4 archive in the same
   local mod, compile and enable deliberately. RC4 cannot read schema 3;
   replacing only source is not a rollback.
4. Keep `settings-v1.ini.power-*.pending` and unresolved recovery state untouched.
   Never delete a fault merely to permit another off request. Recovery tickets
   are private and are not portable profile content.

Fresh schema-3 installations have no pre-migration `.v2-backup`. Do not substitute
schema-3 bytes or delete recovery files. Setting up fresh RC4 preferences is a
separate deliberate operation; the backup restoration procedure needs an actual
verified schema-2 original.

Automated isolated fixtures check migration, failed backup/replacement, explicit
reset, exact restored preference bytes and retained marker bytes. They do not
establish physical wake behavior. The report records separate old-parser
compatibility evidence.

## Qualification still required

Actual Windhawk enable/unload/tray behavior; visible mixed-DPI/negative-coordinate
settings and identify; dim click-through/focus/cursor, SDR/HDR and exclusive
fullscreen; actual multi-app/browser/controller attribution and Steam Input;
concurrent `.scr`/photos/scenes; actual AC/DC/suspend/resume; accessibility;
CPU/GPU/GDI endurance on real displays; separately authorized physical DDC
off/wake/failure. The existing LOOP-016 remains open. No installation, OS policy
change, real DDC/lock, commit, push or publication is part of this delivery.
