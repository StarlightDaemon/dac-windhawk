# RC2 — preserve settings across monitor DPI changes

Date: 2026-10-05. Version: 1.0.0-rc.2.

The operator reported that settings disappear when most of the window crosses
onto another monitor. Source inspection found an explicit `DestroyWindow` in
both Settings and Saver Options `WM_DPICHANGED` handlers. This closes the dialog
and discards unsaved controls; it does not by itself establish a host-process
crash. The observed crossing behavior is consistent with a different monitor
becoming the window's DPI owner.

The fix keeps the same windows and controls alive, honors Windows' suggested
position/size, rebuilds the message font for the new scale, and lays controls out
from their original logical coordinates. Stored coordinates include the full
combo-box drop-down height. Repeated scale changes do not accumulate layout
rounding. Scroll ranges and offsets adapt to the new client size; oversized
windows remain vertically scrollable. Text, edit selection, checkboxes, selected
monitor and the parent/owned-options relationship are retained. Closing the
window remains an explicit Close/Cancel action.

Reference: [Microsoft WM_DPICHANGED contract](https://learn.microsoft.com/en-us/windows/win32/hidpi/wm-dpichanged).
The change uses the message's new DPI and suggested rectangle rather than
destroying/recreating the settings UI. Protection/saver policy is unchanged.

## Verification

The `advanced` platform group now sends repeated 96→144→192→120→96 DPI cycles
through both real window procedures while their hidden windows contain unsaved
edits and have been scrolled. It checks window/control identity, retained text
and edit selection, monitor/checkbox state, negative-coordinate placement,
control bounds, scroll offsets, modal ownership, and font/GDI retention.
The original close-on-DPI behavior would fail the first window-survival check.

Both x86 and x64 compile warning-clean against Windhawk 1.7.3 / clang 20.1.3.
The targeted `advanced` and 50-cycle `lifecycle` groups pass on both architectures.
The x86 release additionally passes all fifteen groups and the existing eight
release-evidence rejection cases. This is injected-message/hidden-window evidence;
actual dragging across physical mixed-DPI monitors remains unverified here.
The prior black hidden-window captures remain unsuitable for visual qualification.

Complete source: `windhawk/mods/oled-aegis.wh.cpp`.
Source SHA-256:
`fc8559965aacc86704eb9f67bd086f584905a4e443d9f03134c8d377dafb75f7`.
Outputs: `build/windhawk/rc2` and `build/windhawk/rc2-x64`.
Verified source ZIP: `build/windhawk/rc2/oled-aegis-1.0.0-rc.2-source.zip`.
Exact receipts are beside those outputs; the source package carries the x86
release evidence. Its final archive hash is recorded in RAIDEN WORK_LOG.

## Apply the update

Replace the complete source in the existing Windhawk local-mod editor with
`windhawk/mods/oled-aegis.wh.cpp`, then compile/enable. Preserve any local source
customizations first. No settings migration is needed from RC1. Existing settings
and saved monitor assignments are unchanged by this fix. No installed mod or
Windows display setting was modified during this repair.

Rebuild/package with `windhawk/tools/produce-release.ps1`. Earlier RC1 source
packages and the [RC1 report](V1_RELEASE_REPORT.md) are retained as historical
evidence; their hashes and measurements do not describe RC2. All remaining
hardware, screensaver and desktop qualification limits still apply.
