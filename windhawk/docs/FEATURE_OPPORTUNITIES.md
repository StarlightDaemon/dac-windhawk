# Monitor and Screensaver Activity Control — feature opportunities

Revised 2026-10-06. Expands the original Liminal Screen report into a
16-tool comparison. This is a prioritized proposal, not an implementation
assignment. No competitor was installed or executed, and no external code or
assets were incorporated.

## Recommendation

Build the next iteration around **clear per-monitor explanations, timed snooze,
application exceptions, better activity choices, and reliable setup/preview**.
Then add **gradual dimming, controller input and named profiles**. Those improve
daily protection without turning the mod into a wallpaper platform or a
continuous desktop-capture engine.

“Immediately” means the next development tranche, with its normal validation,
not immediately enabling new behavior on the installed machine. Existing
physical-desktop and DDC qualification remains LOOP-016 (historical internal record; not bundled).
It can proceed alongside implementation; this report does not make unavailable
hardware testing a prerequisite for starting development.

| Priority | Meaning | Recommended scope |
| --- | --- | --- |
| 1 — Add immediately | Strong benefit, bounded work in the existing architecture | I1–I7 |
| 2 — Excellent additions | High value, but needs new policy, rendering or lifecycle work | E1–E6 |
| 3 — Useful optional additions | Audience-specific or dependency-heavy | O1–O8 |
| 4 — Already covered / redundant | Keep and qualify the existing implementation | R1–R9 |
| 5 — Unnecessary or outside scope | Possible in software, poor fit for this product | X1–X7 |
| 6 — Not reasonably achievable as a general mod feature | Needs vendor support, privileged platform work or a different product | U1–U5 |

Effort estimates: **S** localized UI/adapter work; **M** new state and several
integration paths; **L** substantial subsystem or dependency work; **XL**
separate architectural project. These are relative estimates, not dates.
Feasibility and priority are our engineering judgments; advertised competitor
features are not proof of their reliability or effectiveness.

## Evidence and comparison baseline

Our baseline is the current complete source (historical reference: `../mods/liminal-oled-guard.wh.cpp`),
RC4 behavior plus the temporary descriptive header. Inspected areas include
`Preference`/`Config`, `Controller::Input/Suppress/Tick`, audio observation,
raw-input registration, presentation creation, settings, menus and session/power
messages. Source SHA-256 at review:
`5b4606a0bceaf6a7f93b120eff99b2ead2f64d9aa75d6b28cbd4aaa0004da01c`.

The [RC4 report](LIMINAL_RC4.md) and [V1 feature report](V1_RELEASE_REPORT.md)
describe existing implementation and test limits. “Already present” below means
implemented, not universal physical-monitor or third-party-saver qualification.

For Liminal Screen, retain the earlier source-level review at
`07712a13bee9234b627becde7675d5edfc42fc98`, with seventeen selected files and the
tree retained under ignored `build/research/liminal-screen`. For the other tools,
this pass examines public developer documentation, READMEs and product listings,
rather than auditing every implementation. Sources were consulted on 2026-10-06
or carried forward from the immediately preceding naming investigation.
Live repository links can change.

### Tools surveyed

The “features considered” column describes the external tool. The last column
maps those observations to our recommendations, including adaptations.

| Ref | Tool / primary evidence | Features considered | Where it matters |
| --- | --- | --- | --- |
| T1 | [Liminal Screen](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/README.md), plus pinned sources below | Web presentations, preview, battery preference, OS-saver handling, lock/power timers, options synchronization, reset, onboarding, feeds/updater | I1/I2/I6, E3/E5/E6, O3/O4/O6/O7, X1/X4 |
| T2 | [IdleDimmer](https://github.com/UnDadFeated/IdleDimmer) | Per-display overlays, idle fade, foreground/audio app bypass, fullscreen exclusion, optional light wash, startup and update check | I3, E1, R2/R5, X1/X6 |
| T3 | [OLED Sleeper](https://github.com/Quorthon13/OLED-Sleeper) | Mouse, focused-application and system-input modes; independent blackout; DDC brightness dimming; setup wizard | I4/I6, E4, R1/R2 |
| T4 | [OLED Saver](https://github.com/esleghel/oled-saver) | Black/dim actions, timed pause, night schedule, media exceptions, advertised EDID-based OLED recognition, multiple OS backends | I5, E1/E2, O1, X5/U4 |
| T5 | [OLED Guard Pro](https://oledguard.com/) | Advertised GPU static-region analysis, learned game HUDs, per-app presets, snooze, heatmap/statistics, DDC controls and tinted/vignette finishes | I3/I5, E2, O2/O8, X6 |
| T6 | [Actual Multiple Monitors / Save Idle Screens](https://www.actualtools.com/windowmanager/help/features/save_idle_screens.php), [screen saver panel](https://www.actualtools.com/windowmanager/help/userinterface/screensaver.php) | Idle activation by monitor, foreground/fullscreen criteria, persistent manual hotkey activation, independent or spanning savers; requires preview-compatible savers | I4, R1/R3/R4/R6 |
| T7 | [gamescope-idle](https://github.com/gehhilfe/gamescope-idle) | Controller-aware input with noise filtering, dim warning before black, inhibitors, manual wake, optional CEC adapter support | E1/E4, O5, U3 |
| T8 | [Twinkle Tray](https://twinkletray.com/) | Per-monitor brightness, schedules/idle adjustment, shortcuts, scripting, normalization, extra DDC controls and localization | E2, O2/O5, X3 |
| T9 | [IdleScreen / Idle Overlay](https://github.com/EatKFCinMc/IdleScreen) | Configurable idle-triggered fullscreen black overlay | R2 |
| T10 | [IdleStyle](https://github.com/Jax-Core/IdleStyle) | Custom animations, image/video content, skin groups and multiple monitors | E6, O6/O7, X4 |
| T11 | [Idle Webview](https://github.com/EnhancedJax/IdleWebview) | Local/remote URL savers, multiple displays, hotkey/timer; Rainmeter and WebView2 dependencies | O7; not a drop-in native renderer |
| T12 | [drift](https://github.com/phlx0/drift) | Terminal animation scenes, themes, automatic scene rotation, showcase; automatic shell integration is unavailable on Windows | E6, O6; no terminal framework needed |
| T13 | [Drift Clock](https://github.com/AnshTandon05/drift-clock) | Windows ambient clock, shader backgrounds, themes, input dismissal, Edge-kiosk-based presentation | E6; its renderer is not an established preview-compatible saver for us |
| T14 | [Lively screensaver documentation](https://github.com/lively-community/lively/wiki/Screen-Saver) | Video/image/GIF/web savers, multiple monitors, Windows saver and command-line integration; dependencies vary by distribution | O5/O6/O7, R3; integration requires testing |
| T15 | [idle by Kamina.io](https://play.google.com/store/apps/details?id=com.idle.app&hl=en) | Android charging-triggered clock, pixel shifting and ambient-light brightness | E6, U2/U4; phone workflow not a Windows requirement |
| T16 | [Idle Display](https://github.com/nool-valai/idle-display) | Android music artwork/metadata, moving clock UI, audio-reactive visuals, notification display | E6, O6, X4; adapt concepts, not Android APIs |

Actual Tools pages intermittently returned HTTP 502; the relevant claims above
are also present in the publisher's indexed manual excerpts. Broader taskbar,
wallpaper and window-management features are excluded under the operator's
earlier monitor-protection/screensaver scope.

Do not import marketing promises into our requirements. Reported low overhead,
complete burn-in prevention, seamless HDR or universal wake must not be treated
as measured facts. T5 is a product-description review; T15/T16 claims about
preventing burn-in are not validated here.

## 1 — Add immediately

| ID | Addition and origin | Current gap and proposed behavior | Fit / effort | Acceptance gate |
| --- | --- | --- | --- | --- |
| I1 | **Why this monitor is awake**, with action countdown. T1 explanation concept; our adaptation | Current status is coarse. Show input age, media/fullscreen inhibition, pause, invalid observation, saver fallback and hardware fault separately. Countdown only when an action has a valid deadline. | Native policy/UI, M | Correct reason per display; no invented app identity; unknown/stale media remains explicit; no misleading countdown during indefinite inhibition. |
| I2 | **Competing-controller diagnostics.** T1 | Read Windows screensaver enable/timeout and explain overlap. Add a small advisory for known running controllers where reliably identifiable; not an exhaustive detector. Current legacy Run-entry check is narrower. | Read-only adapter/UI, S–M | Detection never edits OS settings or kills another tool; unavailable/managed settings are reported honestly. |
| I3 | **Application exception rules.** T2/T5 | We have general media/fullscreen policies, no user-configurable app list. Start with “keep this app's display awake while foreground” and “ignore audio from this app for protection.” | Extend observation and precedence, M | Prefer executable path identity; preserve conservative fallback when attribution is unknown. Test multiple browser windows and process exit. Exceptions cannot override emergency stop or hardware faults. |
| I4 | **Explicit activity modes.** T3/T6 | Current local mode combines cursor/foreground attribution; it is not three distinct choices. Offer system input, pointer activity on the display, and foreground-window input, with clear keyboard semantics. | Existing input adapter/policy, M | Test stationary reading, typing with pointer on another display, window movement, spanning windows and unavailable attribution. Merely focused must not automatically mean endless activity unless the user chooses that inhibition rule. |
| I5 | **Timed snooze with automatic resume.** T4/T5 | Current pause lasts for the session. Offer 5/15/30/60 minutes and a visible resume time, initially global; per-display snooze can follow. | Native timer/policy, S–M | Define suspend/restart behavior; expiry begins a fresh idle interval rather than immediately blacking the desktop; stopping and wake remain available. |
| I6 | **First-run checklist and identify displays.** T1/T3 plus our adaptation | Explain the tray, show a brief number/name overlay, select outputs, try black, then choose automation. Do not silently identify every display as OLED. | Existing native/Fujin UI, S–M | Works with negative coordinates and mixed DPI; number overlays clear themselves; no real power-off in onboarding. |
| I7 | **Useful diagnostics export.** Our synthesis of T1 status and current support needs | Existing bounded events and test tooling are not an accessible support bundle. Export version, configuration summary, display identity aliases, reason transitions and saver/hardware outcomes locally. | Native bounded logging/UI, M | Redact usernames/paths by default; exclude screenshots, keystrokes and titles; keep recovery tickets/nonces out; no automatic upload. |

I3 should begin as a small rule set, not an arbitrary scripting engine. I7 is
an original proposed adaptation, not a claim that all surveyed products already
export such bundles.

## 2 — Excellent additions

| ID | Addition and origin | Proposed behavior / distinction | Fit / effort | Acceptance gate |
| --- | --- | --- | --- | --- |
| E1 | **Gradual dimming and a pre-blank warning.** T2/T4/T7 | Add a selected-monitor partial black overlay, short fade and optional dim → saver → black sequence. Existing black and saver-to-black cover only part of this. Retain separate opt-in hardware timing. | Native overlay plus policy, M–L | Input reverses the fade promptly; no focus theft/input trapping; test SDR/HDR and fullscreen behavior; label percentage as overlay opacity, not measured panel brightness. |
| E2 | **Named profiles and schedules.** T4/T5/T8 | Save Work/Gaming/Presentation preferences; switch manually first, then time-of-day or foreground-app rules. Profile names/examples are ours. | Config/schema plus rule evaluation, M–L | Stable display identity mapping, disconnected outputs, overnight schedules, clock changes and deterministic precedence. Import/switch never silently grants hardware opt-in. |
| E3 | **Contained preview using the settings draft.** T1 | A small resizable window for black/photos and supported .scr content, separate from full-display testing. Current Preview uses the saved assignment on a physical monitor. | Existing containment adapted, M–L | Draft remains unsaved; closing cleans up; preview cannot issue hardware commands, lock the workstation or cover unrelated outputs. |
| E4 | **Controller-aware activity.** T7; complements T3's input modes | Begin with XInput controllers; apply stick/trigger deadzones and button handling. Attribute to the foreground game display where credible, with an explicit conservative fallback. | Native observation adapter, M | Idle drift cannot inhibit forever; a held real control can count as activity; unplug/reconnect and Steam Input tested. XInput support is not all-controller/HID support. |
| E5 | **Battery-aware protection.** T1 | Default to cheaper black rendering instead of photos/animation on battery if selected. Existing suspend/display-state handling does not provide an AC/battery policy. | Windows power adapter/policy, M | Handle desktops/unknown battery status; changes do not abandon cleanup; plugging in never enables hardware-off implicitly. |
| E6 | **Small native ambient saver collection.** T12/T13/T15/T16 concepts | One moving dim clock and one sparse animation, then optional rotation/theme choices. Maintain a configurable maximum duration before black. No terminal or browser needed for these simple scenes. | Native renderer, M | No stationary bright clock/logo, bounded frame rate, low measured resource use, independent placement, readable preview and deterministic cleanup. No burn-in guarantee. |

E1 is the best visual upgrade. E4 closes a meaningful observation gap: our
source registers mouse and keyboard raw input, not game controllers. Blanket
fullscreen inhibition can avoid interruptions but also keeps a forgotten game
awake; controller activity offers a more deliberate choice.

E6 is attractive, but black remains the most direct way this tool avoids
displaying static content. A moving clock is an optional presentation, not an
improvement over black on the core protection metric.

## 3 — Useful optional additions

| ID | Addition / origin | Feasibility and limits | Effort / recommendation |
| --- | --- | --- | --- |
| O1 | OLED model suggestions from display metadata. T4 | Feasible as a curated hint list with an explicit Unknown state. Do not equate EDID identity or an HDR flag with guaranteed OLED technology; adapters can obscure information. User selection stays authoritative. | M; onboarding enhancement after I6. |
| O2 | Hardware brightness dim/restore and normalization. T3/T8 | DDC brightness differs from our existing D6 power control. Reuse isolation/identity discipline, but add original-value ownership, external-change handling and recovery. A previous brightness must not overwrite a user's later OSD adjustment. | L; after software dim and physical DDC qualification; one controller should own brightness. |
| O3 | Global Windows lock and all-displays-off. T1 | Technically feasible, but session-wide. Use Windows locking rather than a simulated lock overlay. Broadcast off must be a separately labeled action, never marketed as selected-monitor power control. | M; opt-in, separate from per-monitor idle. |
| O4 | Disable/restore the Windows screensaver. T1 | Add only after read-only I2, with explicit action, saved prior values and restore that respects external changes. Do not modify managed policy. | M; diagnostics may be enough for most users. |
| O5 | Local automation commands. T7/T8/T14 | A small authenticated local command interface for pause/resume/start/stop/profile. Hotkeys already exist; arbitrary command execution is not needed. External saver managers need ownership and stop semantics, not just a launch shortcut. | M–L; later, with a per-user boundary and no remote listener. |
| O6 | Local video/GIF playlists and audio-reactive scenes. T10/T12/T14/T16 | New decoder/rendering work; our GIF support is first frame only. Start with muted local video, bounded resources and black fallback. Exclude owned playback from our own media inhibition. | L; require measured value over installed .scr plus native photos. |
| O7 | Web content / Lively / Rainmeter integration. T1/T11/T14 | Possible via optional renderer or supported external interface, not automatically compatible with per-monitor preview hosting. WebView2 adds runtime/loader distribution; arbitrary pages add navigation, audio, pop-up, network and cleanup requirements. | L–XL; separate feasibility prototype. A successful launch is not compatibility evidence. |
| O8 | Continuous static-region/HUD analysis and exposure visualization. T5 | Technically possible in native Windows code, but needs desktop capture, GPU analysis, masks, exclusion of our own overlay and display-specific recovery. Approximate exposure is not a panel-health measurement. | XL; research branch or companion subsystem, not a near-term addition. |

A modest session counter such as “minutes blacked out” could accompany I7.
Predicted panel life, “burn-in repaired” or watts saved without measurement should
not. O8 is expensive, not inherently prohibited by Windhawk.

## 4 — Already covered or redundant

| ID | Capability seen elsewhere | What we already implement / important boundary |
| --- | --- | --- |
| R1 | Independent monitor idle protection. T3/T6 | Stable identities, per-monitor enable/timeout/input/media overrides and conservative unidentified-output handling. I4 improves choices rather than inventing per-monitor detection. |
| R2 | Blackout and manual blank-now. T2/T4/T9 | Native black and selected-monitor start/stop are present. Partial-opacity dimming and DDC brightness are E1/O2, not already covered. |
| R3 | Independent/custom screensavers and spanning. T1/T6/T10/T14 | Installed/custom preview-compatible .scr processes, concurrent displays and one virtual-desktop span. Arbitrary executable/web savers are not implicitly supported. |
| R4 | Persistent manual activation and shortcuts. T6 | Sticky presentations, pointer-monitor/all-enabled shortcuts, group actions and emergency exit. Timed snooze is the new gap. |
| R5 | Media and fullscreen inhibition. T1/T2/T3/T4 | Active audio endpoints, mute option, process/window attribution and foreground-fullscreen inhibition. App lists, browser-tab precision and controller input are separate capabilities. |
| R6 | Photo slideshow controls | Per-display folder, interval, shuffle, subfolders, six placements and background. Video, animated GIF, scene rotation and multiple-folder playlist editing are not currently claimed. |
| R7 | Staged black/power behavior. T1/T7/T8 | Saver-to-black and opt-in DDC off delays exist and begin at presentation start. E1 adds dimming; O2 adds brightness. Preserve timing semantics during migration. |
| R8 | Tray, theme, icon, storage and lifecycle | Native Fujin appearance/high contrast, draft/save, stable preferences, fault handling, owned child cleanup and unload paths are implemented. Live draft-safe synchronization remains a refinement. |
| R9 | Startup, deployment and automated checks | Windhawk owns startup/mod lifecycle; we already have reproducible source packaging and policy/platform tests. Keep physical qualification separate; a second autostart/updater adds no protection. |

A full preferences reset, validated export/import and live settings synchronization
are reasonable M-sized maintenance work to accompany E2, rather than headline
features. Back up before reset, retain unsaved drafts when status refreshes, and
never erase unresolved hardware fault/recovery state. Cross-machine import must
map monitors explicitly and require renewed hardware opt-in.

## 5 — Unnecessary or outside the agreed scope

| ID | Feature family | Disposition and reason |
| --- | --- | --- |
| X1 | Another updater, autostart service or installer inside the mod | Duplicates Windhawk ownership. Improve guidance and release evidence instead. External runtime servicing, if O7 is selected, is a distinct dependency concern. |
| X2 | Taskbars, wallpapers, window tiling, forced focus retention or audio-device routing | Some are feasible as other Windhawk mods; not monitor protection/screensaver work. Avoid injecting this controller into games simply to reproduce a suite's unrelated features. |
| X3 | Full monitor OSD replacement: contrast, volume, input-source switching | T8 demonstrates the category, but these are general monitor controls with added hardware conflicts. Brightness/power alone have a direct purpose here. |
| X4 | Content marketplace, remote options SDK, publisher feeds, cloud accounts, notification dashboard | T1/T10/T16 ideas would change the product into a content platform. No current user need justifies hosting, authentication, content moderation or persistent user IDs. |
| X5 | Automatic enabling on every “detected OLED” | Technically implementable but a poor default. Identification can be ambiguous, and the user may intentionally leave a display unmanaged. Use O1 suggestions. |
| X6 | Bright white wash, decorative tint/vignette as “protection”, exact copies of competitor themes | Comfort/aesthetics can conflict with the black-first purpose. Keep simple neutral dimming; existing Fujin UI already handles appearance. |
| X7 | Permanent external-saver pooling and broad platform rewrites for parity | Liminal's webview parking solves a different runtime problem. Do not keep arbitrary .scr processes alive or replace native UI with WPF/Tauri/Rainmeter just for feature parity. |

## 6 — Not reasonably achievable as a general Windhawk addition

| ID | Promise / feature | Actual boundary and practical alternative |
| --- | --- | --- |
| U1 | Guaranteed burn-in prevention, repair, or vendor pixel-refresh control for every panel | A desktop mod cannot establish that guarantee. Vendor compensation/maintenance is not a universal Windows facility. Keep claims limited to reducing static exposure. |
| U2 | Shift every pixel of the real desktop losslessly without side effects | Moving our own clock is straightforward; moving arbitrary apps/output is not equivalent. Desktop magnification/capture has clipping, input, HDR and fullscreen implications. Do not treat overlay padding as desktop pixel shift. |
| U3 | Universal physical off/wake, HDMI-CEC through any GPU, or disconnecting a display with no topology changes | Hardware, driver and adapter dependent. Retain opt-in tested DDC and native black; CEC would need supported hardware/software integration. Do not guarantee wake after host failure. |
| U4 | Android charging/notification integrations, Linux gamescope/evdev/Wayland or macOS Spaces inside this Windows mod | Their concepts can inform Windows features; the platform implementation needs a separate port or product. |
| U5 | Security lock by covering the desktop, or universal capture/overlay coverage over protected content and secure desktops | Ordinary presentation windows are not an OS security boundary. Use Windows lock for security; treat unsupported capture/presentation paths as unavailable rather than bypassing them. |

These are boundary statements, not a claim that all underlying ideas are
impossible in all software. Vendor-specific integrations or a separately scoped
platform component could change an individual assessment.

## Windhawk feasibility and architecture

Windhawk supports native C++ mods loaded into selected processes. Its documented
[dedicated-process tool pattern](https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools%3A-Running-mods-in-a-dedicated-process/5cb4ebb18dd742277afca3d0bd917b648c71a5fa)
is consistent with our existing host approach. A feature does not need to hook
Explorer or games to be a Windhawk tool. Keep observation, policy, rendering and
slow hardware work separated; keep x86-host and x64 compilation coverage.

Concrete feasibility anchors:

- **Dimming:** Windows supports per-window alpha via
  [SetLayeredWindowAttributes](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setlayeredwindowattributes).
  This supports E1's basic mechanism; it does not certify HDR accuracy,
  fullscreen coverage, input pass-through or panel luminance.
- **Controllers:** [XInputGetState](https://learn.microsoft.com/en-us/windows/win32/api/xinput/nf-xinput-xinputgetstate)
  exposes controller state; Microsoft's [XInput guidance](https://learn.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput)
  explains deadzones. It supports an initial bounded controller implementation,
  not universal controller attribution to a monitor.
- **Battery:** [GetSystemPowerStatus](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getsystempowerstatus)
  supplies AC/DC and battery information. Extend our power adapter without
  changing saved preferences on each transition.
- **OS conflicts:** [SystemParametersInfo](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-systemparametersinfoa)
  provides screensaver queries. Query failures and policy-managed values need
  visible unavailable states.
- **DDC:** Microsoft's [monitor brightness documentation](https://learn.microsoft.com/en-us/windows/win32/api/highlevelmonitorconfigurationapi/nf-highlevelmonitorconfigurationapi-getmonitorbrightness)
  explicitly notes incomplete MCCS implementations and the need for physical
  validation. Existing power isolation is useful groundwork, not proof that
  arbitrary brightness writes are safe or reversible.
- **Screen analysis:** [Windows capture documentation](https://learn.microsoft.com/en-us/windows/apps/develop/media-authoring-processing/screen-capture)
  covers support checks, frame lifetime, resizing/device loss and HDR formats.
  O8 must handle these rather than assuming a uniform SDR screenshot loop.
- **Web renderer:** [WebView2 distribution](https://learn.microsoft.com/microsoft-edge/webview2/concepts/distribution)
  requires the runtime and appropriate loader arrangement. A self-contained
  mod source does not make its future browser runtime dependency disappear.

Proposed rule precedence for new work: emergency/session block and fault
constraints first; user enable/disable and explicit manual commands next;
timed pause/inhibitors next; profile selection and ordinary idle action last.
Resolve exact manual-versus-inhibitor behavior explicitly against current
semantics before changing it. No schedule or imported profile may override a
hardware fault or silently grant opt-in.

## Phasing and validation

| Phase | Work | Exit evidence |
| --- | --- | --- |
| 0 — Establish baseline | Retain current tests; pursue existing desktop qualification alongside development | Record actual monitor/DPI/saver/DDC results where available, with limits explicit. |
| 1 — Explain and control | I1, I2, I5, I6, then I3/I4/I7 | Reason precedence, stale data, pause expiry, input attribution and rule matching tested; read-only checks do not mutate Windows. |
| 2 — Improve protection | E1 and E4, then E5 | Noise/held-controller cases, fade reversal, battery transitions and session/topology cleanup; measured desktop/GPU behavior for supported configurations. |
| 3 — Improve configuration and presentation | E2/E3, safe reset/import/live-status maintenance, then E6 | Draft fidelity, monitor remapping, schedule boundaries, preview containment and rendering/resource checks. |
| 4 — Select optional work | O1–O5 individually; prototype O6/O7 only if wanted | Demonstrate a real gap over existing .scr/photos and prove stop/fallback/ownership before claiming compatibility. |
| 5 — Separate research | O8 only with explicit expanded scope | Capture/HDR/device-loss feasibility and performance measured; no learned-HUD or lifetime claim without evidence. |

For each implemented slice, test policy with simulated time and observations,
then adapter failures and actual lifecycle behavior. UI work needs visible
mixed-DPI review. Hardware work needs physical qualification. Existing tests
should be extended around real failure modes, not duplicated simply to raise
test counts.

Recommended first delivery: **I1 + I2 + I5**, with the small I6 checklist.
Recommended next substantial feature: **E1**, followed by E4 for controller
users. Do not bundle web rendering or GPU content analysis into that release.

## Reconciliation with the original report

The original opportunity IDs remain traceable:

| Original ID | Updated disposition |
| --- | --- |
| A1 explanations | I1, still immediate; Windows blocker attribution remains our adaptation. |
| A2 OS-saver detection | I2, expanded to advisory controller diagnostics. |
| A3 battery | E5, excellent addition after core usability. |
| A4 contained preview | E3, expanded to preview the unsaved draft. |
| A5 safe reset | Maintenance alongside E2; recovery markers remain protected. |
| A6 onboarding | I6, expanded with monitor identification. |
| B1 OS disable/restore | O4, optional explicit action. |
| B2 global lock / B3 broadcast off | O3; both remain session-wide decisions. |
| B4 web / B5 local-video adaptation | O7/O6; separate rendering work. |
| B6 live synchronization | Maintenance alongside E2/E3, retaining drafts. |
| Original covered/excluded groups | Consolidated into R, X and U above. |

Retained Liminal-specific source qualifications: blocker-name detection is
macOS-specific in the inspected code, not a Windows capability we can port.
Battery-to-black and per-monitor countdown are our adaptations. A notification
URL field was unused in the inspected service. Startup invokes
`update_silent()`, which can install/restart with a configured feed despite
documentation recommending explicit updates. These do not change the priority
of adding a second updater or notification feed: both remain excluded.

Pinned sources for those details and the original inventory:
[engine](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/screensaver_engine.rs),
[power](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/power_monitor.rs),
[integration/reset/preview](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/lib.rs),
[API](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/packages/liminal-api/docs/API.md),
[security](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/packages/liminal-api/docs/SECURITY.md),
[notifications](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/notification_service.rs),
[updater](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/updater.rs),
[frontend](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src/main.ts),
[autoplay](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/autoplay_media.rs),
[speech](https://github.com/ScreenSaverGallery/liminal-screen/blob/07712a13bee9234b627becde7675d5edfc42fc98/src-tauri/src/speech.rs).

Research references to commercial tools establish comparison evidence, not
product attribution or affiliation. Any later reuse of actual source/assets
needs its own provenance review. This report changes no feature contract,
runtime source, release artifact, installed mod or Windows setting.
