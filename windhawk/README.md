# Display Activity Controls for Windhawk

Activity-aware display protection, per-monitor screensavers and native scenes.
The product ID is `dac-windhawk`; configuration lives in `%LOCALAPPDATA%\DAC-Windhawk`.

See [releases](https://github.com/StarlightDaemon/dac-windhawk/releases) and the
[changelog](CHANGELOG.md) for the current pre-1.0 development candidate.
The fresh-configuration idle default is **60 seconds**. Version **1.0.0** is reserved for the candidate
submitted to Windhawk moderators, not a claim of acceptance.
See the [release guide](docs/RELEASING.md) and [adversarial review](docs/ADVERSARIAL_REVIEW.md).
**Independent display input** is on for fresh configurations. Mouse activity
wakes its display; keyboard activity credits the pointer and focused displays.
Existing configurations: enable **Independent display input** in Quick setup and
Save. In Advanced settings, use **Use global setting** for each monitor's input
scope to inherit it; explicit overrides and profiles still take precedence.
Unattributed input conservatively wakes all displays. A spanning presentation is
one shared session, so independent waking applies to separate presentations.

Saved timers and per-monitor overrides retain their values. See the
[default timing and manufacturer guidance](docs/IDLE_DEFAULT_BETA6.md).

Beta.5 formalized the selected name throughout the source, runtime UI,
configuration folder, native host identities, diagnostics and release tooling.
It is a fresh-install candidate with no automatic migration of earlier test data.
See the [identity and installation guide](docs/DAC_IDENTITY.md).

Windhawk Settings controls startup presentation, tray left-click action and
appearance. Quick setup and Advanced settings control display protection.
The [integration guide](docs/WINDHAWK_INTEGRATION.md) and
[Quick setup review](docs/QUICK_SETUP_REFINEMENT.md) describe those features.

The next-beta series is built on the preserved RC4 baseline. It reduces prolonged
static-image exposure through activity-aware protection and per-monitor
screensavers; it does not guarantee burn-in prevention.
See the [historical next-beta usage and rollback guide](docs/NEXT_BETA_RELEASE_NOTES.md) and
[next-beta evidence report](docs/NEXT_BETA_REPORT.md). The historical
[RC4 guide](docs/LIMINAL_RC4.md) retains its original evidence.

The RC3 foundation adds a native Fujin theme to both settings windows and an
original monitor-and-shield tray/window icon. Appearance follows Windows
light/dark app preference, with system high-contrast overrides. The RC2
cross-monitor settings fix is retained. See the [RC3 report](docs/FUJIN_RC3.md).

The V1 feature set provides per-monitor protection, independent screensavers and
opt-in hardware power control. Implementation and automated verification are
complete; desktop and hardware qualification remain outstanding. See the
[RC1 feature report](docs/V1_RELEASE_REPORT.md). This source update has not been
installed automatically. Earlier reports retain their historical evidence.

## Development features

Per-monitor reasons/countdowns, read-only conflict diagnostics, executable exception
rules, explicit activity modes, timed snooze, setup/identify and redacted local
diagnostics join software dim/fade, named profiles/schedules, contained unsaved
draft preview, optional XInput activity, battery-black policy and original moving
clock/sparse native scenes. Schema-3 maintenance includes verified migration
backup, explicit import mapping and backed-up reset while retaining recovery state.

## Features

- Per-monitor enabled flags, idle timeouts, input scope and media overrides,
  stored against stable display identities; disconnected preferences survive.
- Event-driven input, foreground activity, optional fullscreen inhibition and
  media detection across active audio output devices.
- Native black, installed Windows savers, a chosen local .scr, or an independent
  photo folder for each monitor, with concurrent independent presentations.
- Photo interval, shuffle, subfolders, background color and six placement modes.
- Targeted preview, stop/wake, black now, sticky mode, enabled-monitor group
  actions and one saver spanning the desktop.
- Timed saver-to-black transition and opt-in per-monitor DDC/CI soft-off.
- Configurable Ctrl+Alt+F-key sticky toggles for the pointer's monitor and all
  enabled monitors. Emergency exit: **Ctrl+Alt+Shift+F12**.
- Checked configuration persistence, legacy import, diagnostics, tray recovery,
  contained saver processes and controllable black fallback.

## Load and use

The complete standalone source is [mods/dac-windhawk.wh.cpp](mods/dac-windhawk.wh.cpp).
Copy it into Windhawk's local-mod editor and compile/enable. Preserve any local
source edits before replacing an installed mod. Architecture metadata uses
**separate x86 and x86-64 lines**. Windhawk 1.7.3 and bundled clang 20.1.3 are the
tested build toolchain; other host/compiler versions and ARM64 are unqualified.

Automatic activation defaults off. Left-click the monitor-and-shield tray icon to
start/stop enabled monitors. Right-click for the compact category menu: open
**Settings & setup → Settings / import** to edit preferences, **Automation** for
pause/snooze, **Presentations** for sticky/spanning/black, and **Displays** for
per-monitor actions. **Profiles** and **Diagnostics** hold their related commands;
Start/Stop and Exit remain directly accessible. Save the settings draft before
using full-monitor tray testing. Settings also offers a separate contained
preview of the unsaved draft.
Settings windows scroll when the content exceeds the desktop height.

Select **Custom installed .scr** or **Independent photo slideshow**, then use
**Saver files / slideshow / hotkeys** to choose its file or folder. Apply those
options to the draft, then Save the main settings. Custom savers run as programs:
choose a trusted installed file. External savers must support preview hosting;
their Windows configuration can be shared across monitors. Native slideshows
have independent preferences.

Sticky mode ignores activity until Stop, its toggle shortcut, pause,
session/topology reset or emergency exit. Spanning requires all connected
monitors to be identified and enabled. It uses the pointer monitor's assignment
and black-transition delay. Hardware power remains an individual-monitor action.
Visual protection does not lock the Windows desktop.

## Hardware power

Opt in separately for each monitor and Save. Its tray **Displays → monitor →
Hardware off now** action
or configured delay requests DDC VCP D6=04; wake requests D6=01. The delay starts
with the presentation; zero means manual only. Native black needs no hardware
opt-in.

Support depends on the monitor, adapter, cable and driver. A monitor may need
its physical power button to wake. DDC calls can fail or hang; actual hardware
compatibility is unqualified. No DDC probing occurs at startup, and no
capabilities-string query is used.

One helper loaded before off owns the off/wake cycle, including a normal
mod-disable wake request. Unconfirmed operations retain a fault marker and
prevent another attempt until **Reset hardware fault**. Confirm physical wake
before resetting. Host crashes, force termination and driver stalls can leave
hardware off; emergency exit cannot guarantee physical wake.

## Settings and installation

Install the complete `dac-windhawk.wh.cpp` in a new Windhawk local mod; see the
[fresh-install instructions](docs/DAC_IDENTITY.md). Disable earlier experimental
copies first. The configuration file is
`%LOCALAPPDATA%\DAC-Windhawk\settings-v1.ini`, with schema 3 contents.
This edition does not migrate older test folders or their settings.
Windhawk stores its integration preferences separately under `dac-windhawk`.

Pause is session-only. Exit stops the host until the mod is re-enabled.
Disable/remove the mod in Windhawk to stop it; settings are retained.
Windhawk manages its own logon startup. Automatic activation is withheld while
the legacy standalone OLED Aegis Run entry exists; that entry is never removed
silently.

## Build, verify and package

Maintainer builds verify Fujin `v0.1.0` against a bundled, hashed source snapshot.
The historical RC4 parser fixture is also bundled as source only. No sibling
checkout or private archive is required. An explicit `-FujinRoot` still allows
generation against the original pinned Git tag. See [fixture provenance](docs/FIXTURE_PROVENANCE.md).
The complete mod source embeds its generated tokens; installing it in Windhawk
needs no external Fujin files.

From the repository root in PowerShell:

```powershell
./windhawk/tools/produce-release.ps1
```

This inventories the host, builds 0.1.6 into `build/windhawk/dac-0.1.6` and
`dac-0.1.6-x64`, runs sixteen groups on each architecture, checks RC4 rollback
and 34 evidence-rejection cases, and creates a verified source ZIP.
It does not install, publish, change startup or operate physical power.
Harness events/singletons are isolated; tray and hotkey registration are
simulated so tests do not commandeer the installed mod's controls.

```powershell
./windhawk/tools/diagnose-beta.ps1 -OutputName dac-0.1.6
./windhawk/tools/build-production.ps1 -OutputName dac-0.1.6-x64 -Architecture x86-64
./windhawk/tools/verify-beta.ps1 -Archive ./build/windhawk/dac-0.1.6/dac-windhawk-0.1.6-source.zip
```

Existing *-beta.ps1 diagnostic/package tool names are retained. Use the explicit
revision output when running individual helpers; produce-release.ps1 selects
the current revision throughout. produce-beta.ps1 is historical. Packages bind all build inputs
to exact test receipts and checksums; they contain no compiled binaries or stock
savers.

Photo playlists are captured at session start: up to 2,000 JPEG/PNG/BMP/GIF files,
32 MiB per enumerated file, 16 nested folder levels, no reparse points. Images
above 40 million decoded pixels are rejected; rendered frames have a maximum
4096-pixel dimension and scale to the monitor. GIFs use their first frame.
Unusable photos/folders fall back to black. Media attribution uses process and
window heuristics, not browser-tab inspection.

The standalone edition is unchanged. See [provenance](docs/PROVENANCE.md),
[dependency notices](docs/THIRD_PARTY_NOTICES.md) and
[license status](LICENSE-STATUS.md). The selected product name is Display Activity
Controls for Windhawk; public licensing and publication remain separate decisions.

## License

Original DAC source, tests and tooling are [MIT licensed](LICENSE.md).
See [third-party notices](docs/THIRD_PARTY_NOTICES.md) for dependencies.
