# Building and contributing

Use `main` as the mainline branch. Keep changes focused and include relevant
validation and release notes. This development series uses 0.x versions;
1.0.0 is the moderator-submission milestone.

## Build and validate

Prerequisites: Windows, PowerShell 7, Git and Windhawk **1.7.3** with its bundled
clang **20.1.3** compiler installed at the default path. The tools support a
`-WindhawkRoot` override. Other toolchain versions and ARM64 are unqualified.
No sibling theme checkout or private historical archive is required: the
[hashed source fixtures](windhawk/docs/FIXTURE_PROVENANCE.md) are included.

From the checkout root in PowerShell 7:

```powershell
./windhawk/tools/produce-release.ps1
```

This builds both x86 and x86-64, runs sixteen test groups on each, validates the
historical parser regression fixtures, runs 34 package rejection cases, and
verifies the generated source bundle. Outputs are ignored under `build/`.
The process does not install the mod or execute physical monitor-power tests.

For a focused development build/check:

```powershell
./windhawk/tools/build-production.ps1 -Architecture x86-64
./windhawk/tools/test-production.ps1 -Checks policy,storage
```

Use `windhawk/tools/sync-fujin.ps1 -Check` to verify embedded theme tokens.
Changing a fixture requires a reviewed provenance and hash update, not disabling
the guard. Do not commit generated binaries, runtime DLLs, credentials or live
configuration. Review any evidence files before making them public; local build
receipts can contain machine paths.

Installation in Windhawk uses the complete mod source directly and does not
require maintainer build tools. Record real desktop/hardware observations
separately from hidden-window or simulated test results.
