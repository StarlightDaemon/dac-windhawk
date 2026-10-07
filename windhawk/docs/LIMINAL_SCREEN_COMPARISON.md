# Liminal Screen / Liminal OLED Guard comparison

Reviewed 2026-10-05 at the operator's request. **Functional overlap is meaningful;
the implementation and principal use case differ. Naming confusion remains a
material concern because both names identify background screensaver utilities.**

## Scope and primary evidence

Inspected the ScreenSaverGallery fork at commit
`07712a13bee9234b627becde7675d5edfc42fc98`, specifically README, AppOptions,
screensaver engine, power monitor, display manager and application integration.
The GitHub repository metadata identifies `tomaszatoo/liminal-screen` as its
parent. This is a source review, not an installation, runtime qualification,
exhaustive code-clone analysis or legal clearance. No code or assets from this
project were incorporated into Liminal OLED Guard.

Sources pinned to the inspected revision:

- [README and architecture](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/README.md)
- [Screensaver engine](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/screensaver_engine.rs)
- [Power and activity handling](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/power_monitor.rs)
- [Configuration model](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src/app/types.ts)
- [Monitor enumeration](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/display_manager.rs)

The comparison side is our RC4 source, `../mods/liminal-oled-guard.wh.cpp`,
with the existing implemented feature contract. Automated tests do not prove
real hardware or universal third-party saver behavior for either project.

## Functional comparison

| Area | Liminal Screen, inspected implementation | Liminal OLED Guard RC4 |
| --- | --- | --- |
| Primary purpose | Cross-platform host for web-based idle presentations | Windows OLED exposure reduction and independent monitor protection |
| Idle activation | One system-wide idle reading and engine state | Per-monitor activity state, timeouts and configurable input scope |
| Multiple displays | Enumerates monitors and activates a saver window on each from the same engine transition | Independent enabled flags, activation/dismissal and presentation assignments; optional spanning |
| Content | Webview URL with custom options; supports web video/audio content | Native black, installed/custom preview-compatible `.scr`, independent local photo slideshows |
| Monitor-off on Windows | `HWND_BROADCAST` / `SC_MONITORPOWER` request; not a selected physical-monitor DDC operation | Native black plus separately opt-in, selected-monitor DDC off/wake with fault recovery |
| Windows media inhibition | `is_media_active()` returns false; effective idle uses raw `GetLastInputInfo` | Core Audio/process/window heuristics and configurable media/fullscreen inhibition |
| Media inhibition elsewhere | macOS uses display-sleep assertions; not equivalent to Windows per-monitor attribution | Windows-only implementation |
| Lock and battery policy | Timed OS lock and run-on-battery configuration | Does not lock the desktop; no equivalent battery setting |
| Tray and settings | Tray, login startup, remote options, updater integration | Windhawk-managed startup, native settings, tray/manual/sticky actions and emergency exit |
| Technology | Rust + Tauri v2 + TypeScript + platform webviews | Self-contained native C++ Windhawk mod, Win32, GDI+, Fujin tokens |

In `activate_screensaver`, Liminal Screen loops over all available monitors;
`create_saver_window` obtains its content through the same global `get_saver_url`.
The reviewed options model has global timing/content fields, rather than a
per-monitor protection policy. A hosted web page can supply complex presentation
behavior, so this does not assert that every fork or hosted page shows identical
pixels or has no custom display behavior.

The README advertises configurable display-off timing. On Windows the actual
code requests display power through a broadcast, whereas our hardware path is
selected-monitor DDC/CI. These mechanisms have different behavior and failure
modes; neither source inspection nor a successful API call proves physical wake.

## Practical overlap

Both can satisfy “show something on idle screens, then turn displays off.”
The distinctive Liminal OLED Guard case is “keep using one monitor while an
unused OLED independently goes black, runs its assigned saver, or powers off.”
Liminal Screen's distinctive direction is portable, web-delivered presentation
content, with remote configuration and a fork/rebranding framework.

The stacks and inspected control paths are materially different. Shared ideas
such as idle timers, tray controls, monitor enumeration and fullscreen windows
are feature overlap, not evidence of shared source. No percentage is assigned:
a percentage would depend on arbitrary feature weighting and imply precision
that this qualitative review cannot support.

## Naming and coexistence

The name overlap is stronger than the implementation overlap. Both would often
be shortened to “Liminal”, both live in the tray, and both present screensavers
across monitors. “OLED Guard” explains our focus but does not eliminate the
possibility that users assume an edition, fork or affiliation. This is a product
and discoverability assessment, not a legal determination.

Keep the selected compound name for the already-authorized local RC4 test
artifact. Before public branding, recommend revisiting the base name or doing
a deliberate clearance review rather than treating the descriptor as sufficient.
No affiliation or integration should be implied. The user has not selected a
replacement or authorized publication during this comparison.

Running both automatic controllers together could produce competing overlays
and global display-off actions. This is an inference from their control paths,
not a tested coexistence defect. Compare them separately during desktop tests.
