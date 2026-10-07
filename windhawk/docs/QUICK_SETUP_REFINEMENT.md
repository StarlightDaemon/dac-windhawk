# Quick setup review and refinement — 1.1.0-beta.3

The operator requested a separate Save action and a deeper review of the form's
organization. This revision keeps setup open after saving, so a user can adjust
and save any number of connected displays in one visit.

## Review findings and changes

| Finding in beta.2 | Revision |
| --- | --- |
| Save also closed the window, interrupting multi-display setup | Save setup retains the window, selection and success feedback. Close is independent. |
| Display-specific controls and global automation appeared as one continuous list | Separate “This display” and “All enabled displays” sections, with stronger headings and a divider. |
| Identify was separated from the display selector | Identify (5 s) sits beside the selector. |
| Monitor labels mixed friendly names with raw Windows device aliases | Friendly names are shown with display numbers; stable identities remain internal. |
| All four buttons occupied full-width rows with equal emphasis | Advanced settings, primary Save setup and Close share a compact footer. |
| Instructions, warning text and save feedback shared a single area | Guidance stays visible; a separate status area reports unsaved changes, success or the exact save failure. |
| Automatic scope was buried in a long checkbox caption | A short checkbox lives under “All enabled displays”, with a scope explanation. |
| Closing could silently discard edits | Close, Escape and the title-bar X confirm before discarding changes since the last save. |
| A legacy startup blocker was hidden behind a generic save error | Setup directly explains how to save with Automatic off or resolve the legacy startup conflict. |
| Fixed text heights could clip longer errors at smaller widths | Guidance and status heights are measured and the footer moves below them; the window resizes and scrolls. |

The existing three native protection styles remain in Quick setup. External
screensavers, photos, profiles, activity rules and hardware options remain in
Advanced settings. Adding new presentation engines is separate future work.

## Editing and saving

Select a display, edit its protection and idle time, then press **Save setup**.
Choose another display and repeat. Switching displays retains pending edits;
Save applies all pending display edits and the shared Automatic option together.
The form says this explicitly. If opened from the full editor, its draft is also
included, as stated in the persistent guidance. Save never closes the window.

After a successful Save, the configuration snapshot is refreshed so the next
Save is valid; the selected display is retained. Failed Save preserves the draft
and gives an actionable reason, including a Windows error code for file failures.
An externally changed configuration still blocks overwriting from a stale draft.
Advanced settings receives the unsaved draft without saving it.

## Installation and rollback

Use the complete `build/windhawk/nextbeta3/liminal-oled-guard.wh.cpp` source in
the existing Windhawk local mod. Preserve any custom source edits first, replace
the complete editor contents, compile and verify version `1.1.0-beta.3`. Open
**Settings & setup → Quick setup…**. Do not create a second enabled copy.

The `oled-aegis` identity and schema 3 are unchanged. Restoring your saved beta.2
source in the same editor rolls back the UI without a settings conversion.
The beta.2 package is preserved under `build/windhawk/nextbeta2/`.

## Verification scope

Regression cases exercise sequential saves across six displays, retained
selection, actual file persistence, prior-monitor preservation, unsaved draft
handoff, close/discard choices, legacy-startup rejection and manual-save recovery,
write failure, stale settings, minimum-width footer layout, themes and DPI.
Offscreen captures use native control paint paths; they are not live desktop
screenshots and may omit pixels from native controls.

A first focused run hit the existing asynchronous scene-render GDI-count check;
the unchanged binary passed on retry. No threshold was relaxed. A full run also
exposed the tray-recovery fixture's dependency on the real desktop being awake.
It now checks that injected tray failure blocks the controller, then supplies
an awake session inside the harness before verifying recovery. Production
session and power handling are unchanged. The initial failures remain in
`build/research/setup-refinement-release.log` and its resume log.

Real desktop and hardware qualification remain LOOP-016 work, including remote feedback on this
revision. The user-provided remote log confirms the legacy-startup cleanup command
completed; no separate full remote UI/hardware qualification is inferred.

## Final release evidence

2026-10-06: final-source x86 and x86-64 builds and all sixteen test groups passed
on both architectures. Both actual pinned RC4 parser rollback checks and all 32
release-tool rejection cases passed. The ZIP verifies 191 entries, including
bound build/test/tooling receipts, logs, source and documentation.

Final source SHA-256:
`e8c1c30b67484d8e0f1b075a7b0352ee9d413afb62589c2ab4ed4ee2c95dfab6`.
Release log: `build/research/setup-refinement-release-final.log`.
Ready source: `build/windhawk/nextbeta3/liminal-oled-guard.wh.cpp`.
Package: `build/windhawk/nextbeta3/monitor-screensaver-activity-control-1.1.0-beta.3-source.zip`.
The final archive checksum is in `.raiden/state/WORK_LOG.md` and the package
contains a per-entry checksum manifest. Earlier beta archives remain intact.

Light, dark and high-contrast offscreen captures are beside each architecture's
build. The dark rendering was inspected and a clipped Advanced settings label
was widened; action-label extent tests now guard against that regression. Native
edit pixels can be absent in these hidden captures, so they do not replace
visible desktop qualification. No local or remote installation, preference,
Windows startup, physical power, commit or publication was performed here.
