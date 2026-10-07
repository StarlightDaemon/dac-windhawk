<p align="center">
  <img src="docs/assets/dac-banner.svg" alt="Display Activity Controls — Your displays. Their own timing." width="900">
</p>

<p align="center">
  <a href="https://github.com/StarlightDaemon/dac-windhawk/actions/workflows/release.yml?query=branch%3Amain+event%3Apush"><img src="https://img.shields.io/github/actions/workflow/status/StarlightDaemon/dac-windhawk/release.yml?branch=main&amp;event=push&amp;label=CI&amp;style=flat" alt="CI: build and test status on main"></a>
  <a href="https://github.com/StarlightDaemon/dac-windhawk/releases"><img src="https://img.shields.io/github/v/release/StarlightDaemon/dac-windhawk?include_prereleases&amp;color=blue&amp;style=flat" alt="Latest release including prereleases"></a>
  <a href="CONTRIBUTING.md#build-and-test-status"><img src="https://img.shields.io/badge/Windows-x86%20%7C%20x64-0078D4?style=flat" alt="Windows builds: x86 and x64"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue?style=flat" alt="MIT license"></a>
</p>

<p align="center">
  <a href="#install-and-get-started">Get started</a> ·
  <a href="windhawk/README.md">User guide</a> ·
  <a href="https://github.com/StarlightDaemon/dac-windhawk/releases">Downloads</a> ·
  <a href="windhawk/CHANGELOG.md">What's changed</a> ·
  <a href="CONTRIBUTING.md">Build & contribute</a>
</p>

**Display Activity Controls (DAC)** is a Windhawk tool mod that gives each monitor
its own idle timer, presentation and activity rules. Keep a working display awake
while another shows black, a moving scene, photos or an installed screensaver.

> **Development release:** DAC is in the `0.x` series. Automatic activation is
> opt-in. New configurations use a **60-second idle timer** and **independent display
> input**. [Review status and remaining v1 work](windhawk/docs/ADVERSARIAL_REVIEW.md).

## What you can do

| Area | Controls |
| --- | --- |
| Each display | Enable protection, set a timer, choose a presentation and override input/media behavior |
| Presentations | Native black, dim warning, moving clock, sparse constellation, photo slideshow, installed or custom `.scr` |
| Activity | Pointer and keyboard attribution, media inhibition, fullscreen/app rules, optional XInput |
| Automation | Manual or scheduled profiles, app-triggered selection, pause and snooze |
| Everyday use | Quick setup, display identification, tray start/stop, sticky shortcuts, contained preview |
| Appearance | Native settings using the Fujin palette; system/light/dark choices with high-contrast priority |
| Optional power | Experimental per-display DDC/CI off/wake, explicit opt-in and failure quarantine |

Mouse activity wakes its display; keyboard activity also credits the focused
display. In independent mode, unattributed or delayed input does not wake unrelated
displays; focus changes alone do not count as input. Application/media
attribution is heuristic; advanced overrides and profiles can change the policy.

## Install and get started

**Requirements:** Windows with [Windhawk](https://windhawk.net/) installed. The
build tools and host adapter target **Windhawk 1.7.3**; other versions and
ARM64 runtime behavior are unqualified. ARM64 has a separate compile-only probe;
see the [readiness and open loops report](windhawk/docs/OPEN_LOOPS.md).
Maintainer tools are unnecessary for installation.

1. Open [Releases](https://github.com/StarlightDaemon/dac-windhawk/releases) and
   download **`dac-windhawk.wh.cpp`** from the release you want to test.
2. In Windhawk, create a local mod. Replace the complete editor template with the
   downloaded source, compile it, then enable it.
3. Open **Settings & setup → Quick setup** from the monitor/shield tray icon.
   Choose a display, protection style and idle time; select **Save setup**.
4. Repeat for other displays. Enable **Automatic** when ready and save again.
   **Close** is separate, so you can save repeatedly without leaving setup.

Advanced settings adds screensaver/photo paths, profiles and detailed rules.
Existing configurations retain their saved values. Enable **Independent display
input** in Quick setup if an older configuration has it off; check Advanced
input scopes for intentional per-display overrides.

<details>
<summary><strong>Updating, settings location and source packages</strong></summary>

Replace the complete local mod source with the new release's `.wh.cpp`, compile,
and confirm the version in Windhawk. Read the changelog before updating. The
technical ID is `dac-windhawk`; settings live in `%LOCALAPPDATA%\DAC-Windhawk`.
Development releases may require a clean configuration; migration is not guaranteed.
Follow each release's reset instructions. An optional backup lets you keep a test
configuration for your own reference. Disable older renamed
test mods and the standalone OLED Aegis controller before enabling DAC.

The `-source.zip` release asset includes source, docs, tests, pinned fixtures and
validation evidence. `SHA256SUMS.txt` beside the assets checks the downloads; the
ZIP has its own manifest for its contents. GitHub's standard source archives are
snapshots of the tagged repository and omit generated validation evidence.

</details>

## Start, stop and recover

| Action | Where |
| --- | --- |
| Start or stop enabled displays | Left-click the tray icon (default action) |
| Stop one display | **Displays → Stop / wake this monitor** |
| Pause or snooze automatic protection | **Automation** in the tray menu |
| Inspect activity reasons or export diagnostics | **Diagnostics** and Advanced settings |
| Emergency exit | **Ctrl+Alt+Shift+F12** |
| Exit until re-enabled | Tray menu; disable/re-enable the mod to restart |

**Native black and screensavers do not lock Windows or guarantee burn-in
prevention.** A custom `.scr` is a program: choose trusted files. Hardware power
control is experimental and monitor-dependent; wake failure can require the
physical power button. See [security and trust boundaries](SECURITY.md).

## Find your next step

| I want to… | Read |
| --- | --- |
| Configure everyday behavior | [Controls and usage](windhawk/README.md) |
| See changes and release numbering | [Changelog](windhawk/CHANGELOG.md) · [Release guide](windhawk/docs/RELEASING.md) |
| Build or contribute | [Contributor guide](CONTRIBUTING.md) |
| Assess readiness and known limits | [Open loops](windhawk/docs/OPEN_LOOPS.md) · [Adversarial review](windhawk/docs/ADVERSARIAL_REVIEW.md) · [Compatibility](windhawk/docs/COMPATIBILITY.md) |
| Understand source/dependency history | [Provenance](windhawk/docs/PROVENANCE.md) · [Notices](windhawk/docs/THIRD_PARTY_NOTICES.md) |
| Report a problem | [Issues](https://github.com/StarlightDaemon/dac-windhawk/issues) · [Security reporting](SECURITY.md) |

Include the DAC/Windows/Windhawk versions, expected behavior, and reproducible
steps in bug reports. Review diagnostic attachments before sharing them.

## Development and license

DAC ships as a self-contained C++ source mod. Maintainers use PowerShell 7 and
the pinned Windhawk toolchain. Validation builds x86 and x86-64 and exercises
policy, storage, process containment, lifecycle, native UI and packaging failures.
Physical multi-monitor and hardware qualification is tracked separately.

Version **1.0.0** is reserved for the reviewed moderator-submission candidate;
public availability does not imply moderator acceptance. See the
[release guide](windhawk/docs/RELEASING.md) for the exact gates.

[MIT](LICENSE), copyright 2026 StarlightDaemon. Dependency terms remain in their
notices. This repository contains the Windhawk edition; it does not ship the older
OLED Aegis standalone application or artwork.
