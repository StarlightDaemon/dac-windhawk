# Versioning and releases

The source `// @version` in `mods/dac-windhawk.wh.cpp` is authoritative.
`tools/version.ps1` keeps the diagnostic string in sync and checks the changelog
and release tag. Configuration schema versions are independent of product versions.

## Development compatibility policy

The operator confirms that no version is deployed to production end users.
Development releases are replaceable snapshots: breaking changes, removal of
obsolete code, configuration resets and clean installs are permitted. There is
no required version-to-version migration or backward-compatibility guarantee.
Choose the simplest correct final implementation and describe any required reset
in its release notes. Existing migration/rollback tests are retained regression
coverage, not a promise to carry those mechanisms into future versions.

Published release tags/assets remain immutable records of what was tested;
supersede them with a new version. Before supporting production end users, review
and explicitly establish any compatibility/support commitments.

| Change | Version / record |
| --- | --- |
| Each coherent change | Focused Git commit with a descriptive message |
| Fixes or small refinements ready to distribute | Next PATCH, e.g. 0.2.0 → 0.2.1 |
| A feature or substantial batch | Next MINOR, e.g. 0.2.1 → 0.3.0 |
| Reviewed moderator-submission candidate | 1.0.0; moderator acceptance is separate |
| Incompatible public behavior or configuration after 1.0 | Next MAJOR, with replacement/reset instructions as needed |

Use `MAJOR.MINOR.PATCH` with no leading zeros. This public development series uses
ordinary `0.x` numbers and the GitHub prerelease flag, rather than suffixes. Historical
internal beta/RC labels remain supported by the evidence verifier. Do not rewrite
published tags or replace released assets. Fix a bad release with a new version.

## Prepare a batch

From the repository root in PowerShell 7:

```powershell
./windhawk/tools/version.ps1 -Set 0.2.1 # choose the next appropriate version
# Add a matching section to windhawk/CHANGELOG.md.
./windhawk/tools/version.ps1
./windhawk/tools/produce-release.ps1
./windhawk/tools/prepare-assets.ps1 -Destination build/release-assets-0.2.1
```

The full pipeline compiles the standalone mod and tests for x86 and x86-64,
runs every required group, exercises the pinned historical parser, rejects
tampered evidence, and verifies the source package. It fails on stale inputs,
missing evidence, mismatched versions/architectures and changed fixtures.
It does not install the mod or operate physical monitor power.

Review and commit the complete batch using the operator's Git identity. Require
a clean worktree before pushing or tagging. Never include credentials, live settings,
generated binaries or private governance files. Push `main` and wait for
**Validate and release** to pass. Then create and push an annotated tag on that
exact commit, for example `git tag -a v0.2.1 -m "Release 0.2.1"` followed by
`git push origin v0.2.1`. The version checker rejects a mismatched tag.

## What GitHub does

Pull requests and `main` pushes run validation with a read-only token on an ephemeral
Windows runner. The offline Windhawk 1.7.3 installer is pinned by SHA-256; action
dependencies are pinned by commit. The source is tested against the bundled
compiler. The separate publishing job receives only successful run assets and
has `contents: write`; it runs only on pushed version tags. Shell arguments use
environment variables rather than interpolated event text.

A tag runs the same validation and then creates a GitHub Release with:

- `dac-windhawk.wh.cpp`: the exact tested installation source;
- `dac-windhawk-VERSION-source.zip`: source, tests, tools, dependencies, docs and
  build/test evidence, with an internal per-file checksum manifest;
- `SHA256SUMS.txt`: checksums of the two downloadable assets above.

GitHub also provides its standard tagged source archives. Those contain the
repository but not generated test receipts. Releases with major version zero are
marked prerelease. Publishing fails if that release already exists, preserving its
assets. If publishing fails after partial creation, inspect the draft/release first;
do not blindly delete it or overwrite assets. A failed validation publishes nothing.

The local packaging command requires an empty asset destination so prior bundles
are retained. The legacy `package-production.ps1` entry point now uses this same
complete package gate. Use `produce-release.ps1` for current work; prototype tools
and historical beta scripts are historical evidence, not release routes.

## 1.0 gate

Resolve the outstanding cases in [the review](ADVERSARIAL_REVIEW.md),
[acceptance checklist](ACCEPTANCE.md), and [compatibility notes](COMPATIBILITY.md).
Record actual Windhawk load/unload and Settings callbacks, visible multi-monitor
input/DPI/accessibility, media attribution, suspend/resume and opted-in DDC recovery.
No simulated pass can close a physical qualification item. Review moderator
requirements and the exact submission source before assigning 1.0.0.

References: [GitHub workflow permissions](https://docs.github.com/en/actions/reference/workflows-and-actions/workflow-syntax#permissions),
[Windhawk portable setup](https://github.com/ramensoftware/windhawk/discussions/395),
[pinned upstream release](https://github.com/ramensoftware/windhawk/releases/tag/v1.7.3).
