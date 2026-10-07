# Quick setup revision — 1.1.0-beta.2

This report describes the preserved beta.2 release. The current
[beta.3 refinement](QUICK_SETUP_REFINEMENT.md) separates Save from Close and
reorganizes Quick setup. Use that guide for the current installation source.

This local testing revision turns the informational checklist into editable
Quick setup. It includes the compact tray menu from beta.1, with the same mod
identity and schema-3 configuration. Implementation details and initial focused
test evidence are in [QUICK_SETUP](QUICK_SETUP.md).

## Install the ready source

1. Save a copy of the source currently in your existing Windhawk local mod,
   particularly any personal edits.
2. Open that local mod's source editor. Replace its entire contents with
   liminal-oled-guard.wh.cpp (historical reference: `../mods/liminal-oled-guard.wh.cpp`).
3. Compile the existing mod and enable it if necessary. Its header must say
   `1.1.0-beta.2`; keep the `oled-aegis` ID and both architecture lines.
4. Open the tray menu → **Settings & setup → Quick setup…**. Existing users
   open it here; automatic first-use display is only for an empty configuration.
5. Choose each display's enablement, style and idle seconds. Choose automatic
   activation if desired, then **Save setup and close**.

The source is complete and self-contained; no project checkout or external
Fujin files are needed in the Windhawk editor. Update the existing mod in place
instead of creating a second enabled copy. This delivery does not install it.

## What changed

- Centered setup window, more spacing, and 18-pixel minimum text that scales
  with DPI. Short desktops can scroll the form.
- Controls for display enablement, native protection style, per-display idle
  time and automatic activation; the Windows settings shortcut is removed.
- Advanced settings continues the unsaved draft for external savers, photos,
  activity rules, profiles and hardware power. Identify and Close remain.
- Checked Save, cancel, stale-settings rejection, and preservation of existing
  profiles, advanced options and disconnected display preferences.

Setup changes base settings; profiles may override them. Automatic activation
still defaults off for new users. Hardware power remains a separate opt-in.
Quick setup opened from the main editor includes that editor's draft on Save.

## Rollback and compatibility

Beta.1 and beta.2 both use schema 3. Restore your saved beta.1 source in the
same local-mod editor and compile to roll back; no settings conversion is needed.
Keep a separate settings-file copy if you also want to undo preferences changed
after upgrading. Configuration remains in
`%LOCALAPPDATA%\OLED_Aegis_Windhawk\settings-v1.ini`.

The preserved beta.1 source ZIP is under `build/windhawk/nextbeta/`.
Older RC4 rollback requires restoring its schema-2 backup as described in
[the next-beta guide](NEXT_BETA_RELEASE_NOTES.md); RC4 cannot read schema 3.

## Build and verification

Verified 2026-10-06: both architecture builds, all sixteen test groups on each,
both actual RC4 parser rollback checks, and all 32 release-tool rejection cases
passed. The source archive verifies 190 entries. The release log is
`build/research/quick-setup-release.log`; per-check receipts/logs are beside the
builds. The prior beta.1 and RC4 archives also still pass the updated verifier.

Ready standalone source: `build/windhawk/nextbeta2/liminal-oled-guard.wh.cpp`.
Source SHA-256:
`6ab752f463692c89c011734921a037c1232302752bef4aa141d450eeabfe8b19`.
Package: `build/windhawk/nextbeta2/monitor-screensaver-activity-control-1.1.0-beta.2-source.zip`.
Its exact archive checksum is recorded in `.raiden/state/WORK_LOG.md`; the archive
contains its own per-entry checksum manifest and bound release evidence.

Run `./windhawk/tools/produce-release.ps1` from the repository root. It targets
beta.2 in separate `build/windhawk/nextbeta2` and `nextbeta2-x64` directories,
builds both architectures, runs all sixteen groups on each, checks migration
rollback with the pinned RC4 parser, and runs the 32 release-tool rejection cases
before packaging and verifying the source ZIP. Receipts bind the compiled source,
test binaries, compiler dependencies and logs to this release.

Actual visible desktop layout, keyboard/accessibility use, physical mixed-DPI
dragging and hardware compatibility remain LOOP-016 qualification work. Hidden
native-window checks are not a claim of installed Windhawk or physical-monitor
qualification. No Windows policy or installed settings are changed by delivery.
