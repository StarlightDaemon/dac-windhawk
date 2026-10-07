# Windhawk page and settings review

Follow-up: the operator approved the integration settings. They are implemented
in beta.4; see [WINDHAWK_INTEGRATION.md](WINDHAWK_INTEGRATION.md). The review below
records the earlier beta.3 findings and proposal. Identity selection is still open.

Reviewed 2026-10-06 against the beta.3 implementation and Windhawk's official
[mod authoring documentation](https://github.com/ramensoftware/windhawk/wiki/Creating-a-new-mod).

## Details page

The authoritative page is the `WindhawkModReadme` block in
`../mods/liminal-oled-guard.wh.cpp`. It now leads with purpose and Quick setup,
then covers capabilities, navigation, recovery and practical limitations.
Headings, emphasis, a callout and a compact table replace the previous wall of
release notes. Instructions use actual tray labels and beta.3 save behavior.
No hosted images or unavailable repository-relative hyperlinks are required.

This is a documentation-only source edit. The pinned beta.3 source and ZIP under
`build/windhawk/nextbeta3` remain the previously tested delivery; they do not yet
contain this page rewrite. Runtime code and metadata are unchanged.

## Name versus ID

- Visible name: `Monitor and Screensaver Activity Control` (`@name`).
- Technical ID: `oled-aegis` (`@id`). Windhawk treats this as the unique mod identifier.
- Config storage: `%LOCALAPPDATA%\OLED_Aegis_Windhawk\settings-v1.ini`, independently
  named in this implementation.

No replacement has been selected. A descriptive direction would be **Display
Activity Control**, with `display-activity-control` as a possible technical ID.
This is a proposal, not a checked availability or branding claim.

An ID migration must cover metadata, the `WH_MOD_ID` host-launch argument,
build-tool ID validation, artifact names and upgrade instructions. Review
Windhawk's installed old/new mod entries and stop the old host before enabling
a replacement. Preserve existing configuration, backup and hardware-recovery
identifiers until migration has been explicitly implemented and tested. An ID
edit alone is not an in-place upgrade strategy. Do not rename historical reports
or existing release artifacts to make them appear to have used a new identity.

## Why Windhawk's Settings tab is empty

There is no `WindhawkModSettings` block and no `Wh_GetIntSetting` or
`Wh_GetStringSetting` consumption in this mod. Its dialogs use a separate
validated configuration file. `Wh_ModSettingsChanged` currently only asks the
tool host to reload that file; it does not connect Windhawk settings to it.

Windhawk-native settings and custom dialogs can coexist. Native settings need a
YAML declaration, API reads, validation and change handling in the running host.
The supported 1.7.3 installation accepts basic settings with `$name`,
`$description` and `$options`. Newer 2.0-only annotations, such as dynamic monitor
selectors, must not be introduced into this compatibility target.

## Recommended ownership (proposed, not implemented)

| Surface | Ownership |
| --- | --- |
| Windhawk Settings | Small integration preferences: launch presentation (tray only or show Quick setup), left-click action, potentially appearance preference. |
| Quick setup | Display enabled state, protection style, idle time and shared Automatic control. |
| Advanced settings | Activity policy, saver options, profiles/rules/schedules, import/export, diagnostics and opt-in monitor power. |
| Tray | Immediate actions: start/stop, previews, pause/snooze and opening editors. |

Keep first-run onboarding regardless of a future tray-only launch preference.
Keep an accessible stop action regardless of a future tray-click preference.
Appearance is useful in Windhawk only if there is a real override beyond the
current follow-Windows behavior. These are candidates, not controls to add just
to fill an empty tab.

Do not duplicate protection values in two independently saved stores. Exposing
the same value in both interfaces is possible, but first requires one persistence
authority and explicit synchronization, validation and stale-draft handling.
Windhawk's global run-at-logon control should remain owned by Windhawk; Automatic
in the tool controls idle activation and is a separate concept.

If integration settings are added, verify both architectures, native settings
loading and live change delivery to the dedicated host, host restart behavior,
invalid values, open tool drafts, and compatibility with Windhawk 1.7.3.
