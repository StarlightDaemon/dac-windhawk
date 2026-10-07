# Windhawk integration settings — 1.1.0-beta.4

Windhawk's native Settings tab now owns three preferences. Display protection
remains in Quick setup and Advanced settings, with no duplicated protection values.

| Setting | Choices | Default / application |
| --- | --- | --- |
| On tool startup | Tray only; Show Quick setup | Tray only; next tool start. Missing configuration always opens first-run setup. |
| Tray icon left-click | Start or stop protection; Open Quick setup; Open Advanced settings | Start/stop; immediate. Right-click retains the full menu and Stop all. |
| Tool appearance | Follow Windows; Light; Dark | Follow Windows; immediate. Windows high contrast takes priority. |

Changing these settings refreshes integration preferences on the tool's UI
thread. It does not reload the protection configuration, discard open edits,
enable Automatic, start protection, or change Windows run-at-logon preferences.
Unknown or missing option values use the existing safe defaults. Windhawk owns
these three settings; the tool's INI import/export/reset owns protection settings
only. Windhawk settings must be configured separately when moving to another PC.

The release includes the rewritten Details page. The YAML uses only annotations
supported by Windhawk 1.7.3, following the
[official authoring documentation](https://github.com/ramensoftware/windhawk/wiki/Creating-a-new-mod#settings).

## Installation

Replace the entire source in the existing local Windhawk mod with
`build/windhawk/nextbeta4/liminal-oled-guard.wh.cpp`, compile, and verify version
`1.1.0-beta.4`. Preserve custom source edits before replacement. Reopen the mod's
Settings tab to see the new options. Avoid enabling a second copy of the tool.
Select **Show Quick setup**, save, then disable/re-enable the mod to check startup.
Appearance and click behavior should apply as soon as Windhawk saves its settings.

To roll back, restore the preserved beta.3 source in the same local mod. Its
protection configuration is compatible; the older implementation ignores the
new Windhawk preferences. No live installation is performed by release tooling.

## Naming remains a separate selection

- **MSAC:** Monitor and Screensaver Activity Control; proposed ID `msac`.
- **SMAC:** Screensaver and Monitor Activity Control; proposed ID `smac`.
- Other descriptive options: Display Activity Control (`dac`), Monitor Idle
  Control (`mic`), Display and Screensaver Control (`dsc`).

These are naming proposals, not availability claims. This revision retains
`oled-aegis` and `%LOCALAPPDATA%\OLED_Aegis_Windhawk\settings-v1.ini`.
The folder is independently named; changing `@id` does not rename it. A selected
identity needs coordinated Windhawk upgrade, tooling, configuration and recovery
migration rather than a directory replacement.

## Validation

The platform harness exercises missing/invalid values, first-run/startup policy,
tray routing, stopping active protection, immediate appearance changes, high
contrast precedence and preservation of open Quick setup/Advanced drafts.
Harness settings are injected; no operator settings are read or written by tests.
Both x86 and x86-64 compiled against the installed Windhawk 1.7.3 API and
completed all sixteen test groups. The first x64 final-group run hit the existing
asynchronous native-renderer GDI-count check (49 to 56); the unchanged binary
passed the retry (49 to 50). No threshold was relaxed. The original failure and
retry remain in `build/research/integration-release.log` and
`integration-release-retry.log`. This intermittent test remains a qualification
limitation rather than evidence of a clean first-pass suite.

Both historical RC4 parser rollback checks and all 32 tooling rejection cases
passed. The source ZIP is verified against its archived evidence and checksums.

Actual Windhawk Settings-tab rendering and live engine callback delivery, remote
desktop appearance and physical monitor behavior still need operator validation.
Compilation and isolated tests do not establish those results.
