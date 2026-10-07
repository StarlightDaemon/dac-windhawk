# Building and contributing

Use `main` as the mainline branch. Keep changes focused and include relevant
validation and release notes. This development series uses 0.x versions;
1.0.0 is the moderator-submission milestone.

For version bumps, annotated tags and automated GitHub packages, follow
[Releasing](windhawk/docs/RELEASING.md). Use focused commits (`fix:`, `feat:`,
`docs:`, `test:`, `build:` or `ci:` prefixes are useful); update the changelog for
each released batch. Product/schema versions are independent. Report vulnerabilities
according to [SECURITY.md](SECURITY.md).

## Build and test status

The README's **build & tests** badge reports the latest `main` push through
[Validate and release](https://github.com/StarlightDaemon/dac-windhawk/actions/workflows/release.yml?query=branch%3Amain+event%3Apush).
It changes with the workflow result; click it for the exact commit and logs.
The architecture and test-count badges describe the required suite. They are
not separate live results or a code-coverage percentage.

| Gate | Required for a passing run |
| --- | --- |
| Compile | x86 and x86-64 mod/harness builds with warnings treated as errors, using the pinned Windhawk 1.7.3 toolchain |
| Runtime tests | 16 groups per architecture, including storage, policy, process containment, lifecycle and hidden native UI checks |
| Regression and packaging | Version checks, both historical-parser checks, 34 evidence-rejection cases and complete archive verification |
| Release publication | A matching version tag repeats validation, then publishes source, evidence ZIP and download checksums |

A red historical run belongs to its recorded commit. It remains in Actions even
after a later fix passes. The first two hosted failures were DPI fixture assertions
after successful compilation, fixed before 0.2.0 was published; see the
[review's validation record](windhawk/docs/ADVERSARIAL_REVIEW.md#validation-record).
Within a run, expand the failed step and find the first compiler diagnostic or
`FAIL` assertion. A failed test in the combined build/test step is not itself a
compiler failure. Badge images may briefly lag the run because they are cached.

Automated results do not qualify physical displays, actual host callbacks or DDC
recovery; those remaining checks are listed in the review. The release badge
includes development prereleases and links to their packages and notes.

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
