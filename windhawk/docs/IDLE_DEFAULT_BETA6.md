# One-minute idle default — 1.1.0-beta.6

The fresh-configuration global idle timeout is now **60 seconds**, down from
300 seconds. Per-monitor zero continues to inherit the saved global timeout.
Existing saved global timers, individual overrides and selected screensavers
keep their values. Automatic activation still requires explicit enablement.
The screensaver-to-black timer is a separate setting and is unchanged.

For an existing configuration, open **Settings & setup > Settings / import**,
set the first **Timeout seconds** field to **60**, and Save. Leave a monitor's
idle field at **0** to inherit that global value, or keep a deliberate override
such as the operator's 30-second side-monitor timers.

## Manufacturer guidance reviewed 2026-10-07

- [MSI laptop guidance](https://us.msi.com/faq/faq-5394) recommends a screen-off
  interval of approximately five minutes or less for OLED, with a moving
  screensaver starting sooner. This is laptop guidance, not a monitor standard.
- [ASUS XG27AQDMGR OLED Care](https://rog.asus.com/us/monitors/27-to-31-5-inches/rog-strix-oled-xg27aqdmg-gen2-xg27aqdmgr/)
  documents a screen-saver feature that dims after two minutes of inactivity.
  Its presence sensor can also switch to a black image when the user leaves.

These sources do not establish a universal one-minute requirement. The selected
60-second default is an operator-approved product preference that acts sooner
than those examples; it is not a certified burn-in prevention threshold.
Software black/screensavers do not replace a panel's own maintenance features.

## Operator observation

The operator reported that the monitors did idle and entered the selected
screensavers after the proposed side-monitor test. This adds a successful
automatic-activation/presentation observation for that setup. The exact saved
configuration and primary-media attribution were not supplied, so it does not
qualify every media, topology, hardware or session case in LOOP-016.

## Installation and validation

Ready source: `windhawk/mods/dac-windhawk.wh.cpp`. The mod ID remains
`dac-windhawk`, with `%LOCALAPPDATA%\DAC-Windhawk` storage. Compile the complete
source in Windhawk's local-mod editor. The release outputs are `dac-beta6` and
`dac-beta6-x64`; the bundle is `dac-windhawk-1.1.0-beta.6-source.zip`.
Earlier beta.5 artifacts are preserved. Run `./windhawk/tools/produce-release.ps1`
from the checkout root to reproduce the current release.

Both warning-as-error builds (x86 and x86-64) passed all sixteen test groups.
Both historical parser regression checks and all 34 release-tool rejection cases
passed. The completed archive verifies 195 entries, compiled source, checksums
and bound evidence. The preserved beta.5 archive also still verifies. Existing
tests were used for this small default change; no test-only source changes were
needed. Production source comparison against beta.5 is confined to the idle
default, Details explanation and version labels.

Release log: `build/research/dac-idle-beta6-release.log`.
Source SHA-256:
`0032fcca0f5df57c96077f8f71d9a5bb1dcf70253addb74a16824a165679452a`.
The final ZIP hash is recorded outside the archive in `.raiden/state/WORK_LOG.md`.
No installation or live preference changes were made by this development task.
