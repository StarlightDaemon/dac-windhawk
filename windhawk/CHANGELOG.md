# Changelog

This records the Windhawk edition. Earlier version labels were internal
development labels; they are retained for evidence lookup. They do not establish
a completed 1.0 release. No historical tag or published release is fabricated.

## 0.1.6 — 2026-10-07 — local development candidate

Renumbers the former `1.1.0-beta.6` candidate. The mod metadata and diagnostic
header now report `0.1.6`; release tooling builds and verifies that exact version.
The configuration format is unchanged. The first
publication also adds the operator-selected MIT notice and pinned source fixtures
so maintainer builds work without private archives or a sibling theme checkout. Technical ID
`dac-windhawk` and `%LOCALAPPDATA%\DAC-Windhawk` storage remain the same.

### Included work

- Per-monitor enabled state, saved presentation, idle timer, input scope, media
  override and fullscreen inhibition; stable identities retain disconnected settings.
- Native black, installed/custom screensavers, independent photo slideshows,
  moving clock and sparse constellation scenes; targeted and spanning presentations.
- Automatic activity-based activation, manual/sticky control, pause/snooze,
  application rules, scheduled/manual profiles, optional controller input and
  battery-aware black fallback.
- Quick setup with independent Save and Close, repeated multi-monitor saves,
  retained drafts, display identification, responsive DPI layout and actionable
  save errors. Advanced settings adds full policy and saver configuration.
- Native Windhawk Settings for startup presentation, tray left-click action and
  appearance. Live changes retain protection drafts; high contrast takes priority.
- Categorized tray navigation, per-display explanations/countdowns, conflict
  diagnostics, redacted export, emergency stop and contained preview/process cleanup.
- Opt-in experimental DDC/CI power control with isolated workers, recovery tickets,
  fault retention and black fallback. Physical hardware qualification is incomplete.
- The selected Display Activity Controls for Windhawk identity across current
  source, runtime objects, UI, configuration folder and tools.
- Independent per-display input enabled by default, plus an **Independent display
  input** checkbox in Quick setup for existing configurations. Mouse activity on
  either of two displays leaves the unrelated third display protected; keyboard
  also credits the focused display. Explicit advanced overrides remain available.
- A **60-second** fresh global idle default. Saved timers and per-monitor
  overrides retain their values; Automatic still requires explicit enablement.
- Dual-architecture build evidence, bounded test runners, archive checksums and
  rejection tests that prevent incomplete or mismatched evidence from passing.

### Status

The operator subsequently supplied `StarlightDaemon/dac-windhawk` and authorized
pushing there, superseding the initial publication hold. Current validation is recorded in the
[repository/release plan](docs/REPOSITORY_RELEASE_PLAN.md). Operator testing
confirmed idle activation and the selected screensavers on their setup.
Broader desktop, media, accessibility, session and physical hardware qualification
remains outstanding. Original DAC source is MIT licensed.

## Historical development milestones

These labels identify preserved local evidence, not a proposed sequence of new
GitHub releases. Do not relabel their archives or recreate them as `0.1.1`–`0.1.5`.

| Original label | Work represented | Evidence |
| --- | --- | --- |
| Feasibility / prototype | Dedicated Windhawk tool host, independent monitors and saver containment | [Phase 1](docs/PHASE_1_REPORT.md) |
| 1.0.0-beta.1 / beta.2 | Initial production candidate and 32-bit Windhawk host correction | [Beta 1](docs/BETA_1_REPORT.md), [host fix](docs/BETA_2_HOST_FIX.md) |
| 1.0.0-rc.1 | Expanded per-monitor policy, savers/photos, sticky controls and experimental power handling | [Feature report](docs/V1_RELEASE_REPORT.md) |
| 1.0.0-rc.2 | Settings lifetime and draft retention across DPI changes | [DPI repair](docs/DPI_FIX_RC2.md) |
| 1.0.0-rc.3 | Fujin appearance, high contrast and original monitor/shield icon | [Fujin report](docs/FUJIN_RC3.md) |
| 1.0.0-rc.4 | Interim naming and preserved baseline evidence | [RC4 report](docs/LIMINAL_RC4.md) |
| 1.1.0-beta.1 | Explanations, rules/profiles, snooze, diagnostics, previews, dim/scenes, controller/battery policy and compact tray | [Next-beta report](docs/NEXT_BETA_REPORT.md) |
| 1.1.0-beta.2 | Informational checklist replaced by editable Quick setup | [Setup revision](docs/QUICK_SETUP_REVISION.md) |
| 1.1.0-beta.3 | Separate Save/Close, repeated monitor saves, layout and save-error refinement | [Setup refinement](docs/QUICK_SETUP_REFINEMENT.md) |
| 1.1.0-beta.4 | Windhawk integration settings and rewritten Details page | [Integration report](docs/WINDHAWK_INTEGRATION.md) |
| 1.1.0-beta.5 | DAC identity, clean configuration folder, renamed tooling and renderer-fixture isolation | [Identity report](docs/DAC_IDENTITY.md) |
| 1.1.0-beta.6 | One-minute fresh idle default and manufacturer-guidance review | [Timing report](docs/IDLE_DEFAULT_BETA6.md) |
| 0.1.6 | Corrected pre-1.0 numbering and consolidated release history | This entry |

Old reports retain their original filenames, source hashes, scope and limitations.
Their references to a contemporary source path/version describe that checkpoint.
