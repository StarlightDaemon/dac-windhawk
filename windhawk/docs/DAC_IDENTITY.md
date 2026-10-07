# Display Activity Controls for Windhawk — 1.1.0-beta.5

The operator selected this name and explicitly authorized a clean break because
the tool is not deployed. This revision implements the new identity throughout
the active Windhawk product and tooling. It does not migrate prior test settings.

## Identity

| Surface | Value |
| --- | --- |
| Product name | Display Activity Controls for Windhawk |
| Short name | DAC for Windhawk |
| Windhawk mod ID | `dac-windhawk` (`local@dac-windhawk` in a local editor) |
| Standalone mod source | `windhawk/mods/dac-windhawk.wh.cpp` |
| Compiled DLL | `dac-windhawk.dll` |
| Configuration folder | `%LOCALAPPDATA%\DAC-Windhawk` |
| Settings file | `settings-v1.ini` (schema 3) |
| Source archive | `dac-windhawk-1.1.0-beta.5-source.zip` |
| Build directories | `build/windhawk/dac-beta5` and `dac-beta5-x64` |
| C++ namespace / test macros | `dac`, `DAC_POLICY_ONLY`, `DAC_HARNESS` |
| PowerShell helper prefix | `Dac-` |

All current window titles, tray text, Details, authorship label, diagnostics,
window classes/control properties, singleton/emergency/power event names and
power-helper command flag use the new identity. The emergency-stop tool targets
the renamed event. Release validation binds mod ID, source filename and binary
filename to the release specification and rejects the previous identities.

The file basename retains its schema-compatible meaning; the new parent folder
provides the product identity. Existing old folders are never read, moved,
merged or deleted. Fresh configuration opens Quick setup with Automatic off.
Windhawk owns its three integration preferences under the new mod ID.

## Installation

1. Disable any earlier experimental copy in Windhawk.
2. Create a new local mod and replace its complete template with the delivered
   `dac-windhawk.wh.cpp` source.
3. Compile and enable it. Verify **Display Activity Controls for Windhawk**,
   version **1.1.0-beta.5**, and ID **local@dac-windhawk**.
4. Configure monitors in Quick setup and integration preferences in Windhawk's
   Settings tab. Save before enabling Automatic.

There is no separate installer executable. Windhawk compiles and hosts the
self-contained mod source. No installer, installed mod, registry configuration
or live user folder was modified by this development task.

## Build and package

From the checkout root:

```powershell
./windhawk/tools/produce-release.ps1
```

The default is beta.5. It builds both architectures, runs sixteen groups on each,
retains historical parser regression checks, runs 34 packaging rejection cases,
and verifies the completed source ZIP. Historical parser checks validate the
existing import/serialization behavior; they do not imply an upgrade commitment.

## Deliberately retained history

Earlier ZIPs/reports, the disposable feasibility prototype, third-party notices,
and the inherited standalone OLED Aegis application retain their real names.
The repository checkout remains the original development checkout; it also holds that
inherited application and is registered with RAIDEN at that path. It is not a
shipped product folder. Renaming the checkout/fleet registration is separate
from the Windhawk product identity and crosses the Instance's role boundary.

References to external OLED Aegis startup/process names remain in the legacy
conflict detector and explicit legacy INI importer because those must identify
the actual external application. No old runtime identity is needed for DAC's
own host, events, storage, source or release tools.

## Validation

The release uses Windhawk 1.7.3's bundled compiler for x86 and x86-64.
Builds treat warnings as errors. Both builds passed all sixteen test groups,
both historical RC4 parser regression checks passed, and all 34 release-tool
rejection cases passed. The completed archive verifies 194 entries against its
checksums, compiled source and bound evidence. Final release log:
`build/research/dac-identity-release-verified.log`.

Compiled/distributed source SHA-256:
`aad9557583f9db08c9309e863761dbdee065e6747db59597665c8c272e284e97`.
The final archive hash is recorded in `.raiden/state/WORK_LOG.md`; it is outside
the archive to avoid a self-referential checksum.

The first release attempt and a later repeat exposed a timing-dependent GDI
count failure in the renderer fixture. Warming the font alone was insufficient.
Process-wide resource counts were being sampled after starting unrelated UI and
media workers. The test now measures the production renderer before host startup,
with the original +2 growth bound, warmup and explicit font/DC/bitmap cleanup.
A separate integrated pass still renders both real scene owners and checks their
pixels and cleanup. Production renderer code was not changed. Initial failures
and subsequent runs are retained in `build/research/dac-identity-release*.log`
and `dac-render-repeat-*.log`. A first cleanup assertion also relied on querying
a deleted font handle; it was replaced with flushed GDI counts around owner
destruction. Three consecutive targeted x64 runs passed in
`build/research/dac-isolated-check-{1,2,3}.log`: each isolated 400-frame sample
held handles at 140 and GDI objects at 3, then verified font release. The integrated
samples still showed background GDI variation, supporting the decision to keep
process-wide growth assertions in the isolated renderer fixture. The separate
50-cycle host lifecycle test retains its process-wide cleanup assertion.

Actual Windhawk load, Settings-tab display and physical monitor/remote desktop
qualification remain separate from compilation and isolated harness testing.
