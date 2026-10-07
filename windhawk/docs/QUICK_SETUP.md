# Quick setup review and revision

2026-10-06. This implementation is now delivered as
[1.1.0-beta.2](QUICK_SETUP_REVISION.md). The focused test identities below record
the initial implementation before the version bump; the beta.2 package carries
fresh full-release evidence. The installed mod is unchanged.

## Purpose and review

Setup should let someone choose which displays to protect, what appears when
they are idle, and when protection starts. The previous checklist was only a
static instruction block: it neither tracked completion nor adjusted settings.
Its Windows screensaver button actually opened `ms-settings:lockscreen` and did
not configure this tool. The shared positioning helper anchored the window to
the work area's top-left, and the theme's default text was 12 logical pixels.

The working source replaces that checklist with **Quick setup**:

- A short introduction, followed by a display selector and enable checkbox.
- Native black, moving dim clock, or sparse constellation. Existing external
  saver/photo assignments can be retained and edited in Advanced settings.
- Per-display idle seconds, with 0 explicitly inheriting the displayed global time.
- Automatic activation for enabled displays, still off on fresh configuration.
- Save setup and close; continue the unsaved draft in Advanced settings;
  Identify displays for five seconds; Close without saving.

The window is centered in the monitor work area, has 28-pixel content margins,
larger controls and spacing, and an 18-pixel minimum font (50% larger than the
previous default). Font and geometry scale with display DPI; content width
follows the window and vertical scrolling keeps controls reachable on shorter
desktops. Theme changes retain the larger type. The Windows shortcut is removed.

## Draft and persistence behavior

Quick setup uses the existing checked save transaction and a separate draft.
Save also includes an existing settings draft when setup was opened from it;
the window says so. The main editor is disabled while Quick setup is open.
Closing setup without saving leaves that editor's earlier draft intact.
Advanced settings receives the setup draft without saving it.

Profiles, rules, hardware opt-ins, assets and disconnected display preferences
are preserved. Setup edits base settings; a notice explains that profiles can
override them. Display selection remains bound to the identity captured when
setup opened, even if display order changes. Missing/unidentified outputs have
disabled display controls. If live saved configuration changes concurrently,
the setup draft cannot overwrite it: setup asks the user to close and reopen.
Failed saves retain both the window's draft and the previous active settings.

## Verification

Both x86 and x86-64 standalone DLL and native harness builds passed with
Windhawk's clang, `-Wall -Wextra -Werror`, and the pinned Fujin adapter check.
The `nextbeta`, `advanced`, and `lifecycle` groups passed on both architectures.
New native setup cases exercise invalid input, save/cancel, failed persistence,
stale configuration rejection, preserved advanced/disconnected preferences,
draft transfer and modal ownership, display reorder, no displays, larger fonts,
high contrast, and repeated DPI changes with text and selection retention.
Lifecycle checks cover 50 cycles after warmup on each architecture.

Reproduction script: `build/research/quick-setup-check.ps1` (ignored, local).
Builds and logs: `build/research/quick-setup/{i686,x86_64}/`.
Before-edit source/test copies: `build/research/setup-before/`.

Source SHA-256:
`09bb0f6061ed6a92ddaf9b379a6a969bf423f7e801873ddd0c76315d6cbda6cf`.
Platform test SHA-256:
`df8d66982f9516a85ce93de1fb7dfd775a9aeb6eb5654d099f428d998094b9e2`.

These are hidden native-window and isolated persistence checks. Actual visible
layout, keyboard/accessibility use, mixed-monitor dragging and installed
Windhawk behavior still need desktop qualification under LOOP-016.
No live installation/settings, Windows policy, physical power, commit or
publication changed. The existing beta ZIP remains unchanged at SHA-256
`c5265bebb22c8b37567907bfd3a383570eddaa45756f83040ffef67fd366c296`.
