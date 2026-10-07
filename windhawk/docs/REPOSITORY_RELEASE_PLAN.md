# Repository and release record — 0.1.6

The operator authorized publication to
[StarlightDaemon/dac-windhawk](https://github.com/StarlightDaemon/dac-windhawk)
on 2026-10-07, superseding the earlier hold on commits and pushes. The destination
is an independent public repository on `main`. Its initial commit and selected
MIT license are preserved. The original `StarlightDaemon/oled_aegis` fork and
its remotes/history remain intact.

## Version policy

| Stage | Numbering and purpose |
| --- | --- |
| Current development snapshot | `0.1.6`, renumbered from internal `1.1.0-beta.6` |
| Fixes/refinements | `0.1.7`, `0.1.8`, etc. |
| Larger development milestones | `0.2.0`, `0.3.0`, etc., with defined release notes |
| Submission preparation | Continue `0.x` while completing qualification and submission review |
| Moderator submission | `1.0.0`, the reviewed candidate submitted to Windhawk moderators; acceptance is a separate event |

Use annotated tags such as `v0.1.6` on the exact validated commit. Development
GitHub releases should be marked prereleases. Do not fabricate older `0.x` tags
or backdate commits. The [changelog](../CHANGELOG.md) preserves the original
internal milestones and their actual labels.

## Repository scope

The export contains the Windhawk edition, tests, release tools, selected product
and historical documentation, and source-only fixtures. It excludes the inherited
standalone application, its artwork/build scripts/CI, the old Git history,
private governance state, live settings and local logs. The independent
implementation's relationship to the older project remains documented in
[provenance](PROVENANCE.md); this is not a claim of clean-room authorship.

The operator-selected [MIT license](../LICENSE.md) applies to original DAC code.
Dependency licenses and Fujin's notice remain in [third-party notices](THIRD_PARTY_NOTICES.md).
Pinned Fujin tokens and the historical parser are bundled with checksums; see
[fixture provenance](FIXTURE_PROVENANCE.md). No sibling checkout or private
historical archive is required for current maintainer builds.

Historical reports may reference original internal records and old file/version
names. Those references are labelled as unavailable historical context; the
private records are not copied into this repository. Historical reports are not
current installation or release instructions.

## Current build and installation

Install the complete `windhawk/mods/dac-windhawk.wh.cpp` in Windhawk's local-mod
editor, compile and verify `0.1.6`. The lower number is intentional; compile it
explicitly rather than relying on an update comparison. The technical ID and
configuration location remain `dac-windhawk` and `%LOCALAPPDATA%\DAC-Windhawk`.
Saved preferences are not reset. This first publication also enables independent
input by default for new configurations. Existing users can enable it in Quick
setup and Save; advanced per-display overrides still apply. All six ordered pairs
of three displays, keyboard attribution, and Quick setup save/discard are covered
by the updated tests.

From a Windows checkout with Windhawk 1.7.3 installed:

```powershell
./windhawk/tools/produce-release.ps1
```

The script builds x86 and x86-64, runs sixteen groups on each, verifies both
historical parser fixtures and 34 packaging rejection cases, then produces and
verifies `build/windhawk/dac-0.1.6/dac-windhawk-0.1.6-source.zip`.
The tests use hidden windows, fixture processes and simulated hardware. No
physical monitor-power tests or installation are performed by that command.

Validation results for the clean publication checkout will be recorded here.

## Remaining qualification

The operator reported successful idle activation and selected screensavers on
their setup. That observation and automated harness passes do not cover all
mixed-DPI/accessibility, media/browser/controller attribution, actual Windhawk
load/unload/settings callbacks, suspend/resume, or physical DDC recovery cases.
Keep development numbering until the documented submission milestone is ready.
