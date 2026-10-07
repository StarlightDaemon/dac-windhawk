# Security policy

Report reproducible security problems through
[GitHub private vulnerability reporting](https://github.com/StarlightDaemon/dac-windhawk/security/advisories/new)
when available. If that form is unavailable, open an issue asking for a private
contact without including exploit details, personal paths, or configuration files.
Include the affected version, Windows/Windhawk versions, impact, and minimal steps.

During development, fixes target the latest `0.x` release. Older development
snapshots and the retained prototype do not receive a separate maintenance branch.

## Trust boundaries

- DAC runs with the account and privileges of its Windhawk tool host. Use ordinary
  user privileges. Settings are per user; another process with equivalent account
  access is not isolated from your settings or display session.
- A custom `.scr` is executable code. Only select trusted installed programs.
  Job objects constrain lifetime and descendant cleanup; they are **not a sandbox**.
  Review custom paths in imported profiles before activating them.
- Photos are decoded by Windows GDI+. Use trusted local content and keep Windows
  patched. Size/playlist bounds do not guarantee a decoder or disk operation will
  finish promptly. Mapped drives and ancestor junctions can still reach remote storage.
- Native black, dimming and screensavers do not lock Windows. Hardware DDC/CI is
  opt-in and experimental; failed wake may require the monitor's physical button.
- Diagnostic exports redact application paths and display identities. Configuration
  exports, build receipts and test logs can contain paths; review them before sharing.

See [the adversarial review](windhawk/docs/ADVERSARIAL_REVIEW.md) for findings,
coverage, and unresolved qualification. Passing tests is not a security certification.
