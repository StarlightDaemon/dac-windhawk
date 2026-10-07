# Compatibility entry point: current packages always require both architectures,
# rollback evidence, fixture inputs, and all rejection checks.
$ErrorActionPreference='Stop'
& (Join-Path $PSScriptRoot 'package-beta.ps1')
