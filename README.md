# Display Activity Controls for Windhawk

**Development version 0.1.6** — activity-aware display protection and per-monitor
screensavers, running as a Windhawk tool mod.

Choose native black, installed screensavers, photos or built-in scenes for each
monitor. Quick setup provides independent Save and Close actions; Advanced
settings adds activity rules, profiles and optional hardware controls. New
configurations use a **60-second idle timeout** and **independent display input**.
Automatic activation is opt-in. Mouse activity wakes its display; keyboard activity
also credits the focused display.

## Install

1. Install Windhawk, then create or open a local mod in its editor.
2. Replace the complete template with
   [dac-windhawk.wh.cpp](windhawk/mods/dac-windhawk.wh.cpp).
3. Compile and enable it. Confirm version **0.1.6**, configure displays in Quick
   setup, enable Automatic if desired, and **Save setup**.
4. For an existing test configuration, enable **Independent display input** in
   Quick setup and save. Each monitor's Advanced **Input scope** should use the
   global setting unless you deliberately want an override.

Existing test configurations retain their saved timers and presentation choices.
The intentional change from internal 1.x labels to 0.1.6 requires explicitly
compiling the source; it is not an automatic upgrade. The technical ID is
`dac-windhawk`, and settings live in `%LOCALAPPDATA%\DAC-Windhawk`.

**Emergency stop: Ctrl+Alt+Shift+F12.** Default native black covers a display;
it does not lock Windows or imply that the monitor is physically powered off.

## Documentation

- [Controls, settings and usage](windhawk/README.md)
- [Changelog and historical milestones](windhawk/CHANGELOG.md)
- [Version policy, build validation and remaining qualification](windhawk/docs/REPOSITORY_RELEASE_PLAN.md)
- [Provenance](windhawk/docs/PROVENANCE.md) and [third-party notices](windhawk/docs/THIRD_PARTY_NOTICES.md)
- [Building and contributing](CONTRIBUTING.md)

This is a pre-1.0 development project. Version 1.0.0 is reserved for the reviewed
candidate submitted to Windhawk moderators. Public source availability does not
mean moderator acceptance or complete physical-monitor qualification.

## License

[MIT](LICENSE), copyright 2026 StarlightDaemon. The self-contained mod embeds its
license notice. Dependency terms and the MIT-licensed Fujin tokens retain their
own notices. This repository contains the Windhawk edition, not the inherited
OLED Aegis standalone application or artwork; its development relationship is
recorded in the provenance document.
