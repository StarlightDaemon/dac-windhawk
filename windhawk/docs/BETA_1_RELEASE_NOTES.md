# OLED Aegis 1.0.0-beta.1

Internal beta, 2026-10-04. The complete single-file Windhawk mod is
`windhawk/mods/oled-aegis.wh.cpp`. Automatic activation defaults off.

This beta retains independent idle timers and monitor assignments, native black,
six installed stock-saver choices, media suppression, settings/import, tray
controls, session/power cleanup and contained saver processes. It adds ongoing
owned-child responsiveness checks, rejection of unknown-only legacy imports,
hidden controller-to-presentation integration tests, and a reproducible beta
pipeline with receipts tied to the source, tests, tooling and built binaries.

## Install, upgrade and remove

Build validation uses x64 Windhawk 1.7.3 and its bundled clang 20.1.3. In the
Windhawk local mod editor, create a mod and replace its source with
`mods/oled-aegis.wh.cpp`, then compile/enable. Use the single source, not the
harness executables or the locally built DLL. Actual editor installation and
Windhawk load/settings/unload callbacks remain unqualified in this environment.

For an existing local candidate, disable it first, keep a copy of its source
and `%LOCALAPPDATA%\OLED_Aegis_Windhawk\settings-v1.ini`, replace its source and
compile/enable. Beta 1 retains version-1 settings. A fresh installation starts
with automatic activation off; an upgrade retains the saved preference.
Keep the old standalone application stopped and disable its startup before
enabling automatic activation here. The mod does not edit Windows startup.

The tray provides preview/stop, settings, import, pause and exit. The emergency
shortcut is **Ctrl+Alt+Shift+F12**. `tools/stop-beta.ps1` signals the same
session-local emergency event if the host is running. Neither mechanism has
an absolute blocked-UI/OS-call completion guarantee.

Disable/remove the local mod in Windhawk to stop its dedicated host and owned
savers. Preferences remain at the path above; remove them only if unwanted
after disabling. To roll back, disable the beta and restore the previous source
and saved settings copy.

## Beta scope

No saver is labelled supported. Hidden window/process and injected-monitor
tests do not qualify visible animation, physical input, tray interaction,
multiple real displays, mixed DPI, power effects or long elapsed GPU/display
endurance. Black fallback detects missing/exited/unresponsive preview children;
it cannot prove that a responding child draws correctly. Media attribution
remains heuristic. See [beta report](BETA_1_REPORT.md),
[compatibility](COMPATIBILITY.md) and [README](../README.md).

The beta archive contains source, documentation, tooling and recorded test
evidence. It contains no OS savers or compiler runtimes. It was produced locally
without installation, publication, commits or pushes. Original-source license
selection and public naming remain separate publication decisions in
[license status](../LICENSE-STATUS.md).
