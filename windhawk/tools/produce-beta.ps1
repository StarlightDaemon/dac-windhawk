param([string]$WindhawkRoot='C:\Program Files\Windhawk')
$ErrorActionPreference='Stop'
Write-Output 'Beta 2: read-only prerequisite inventory'
& (Join-Path $PSScriptRoot 'diagnose-beta.ps1') -WindhawkRoot $WindhawkRoot
Write-Output 'Beta 2: build production source and same-source tests'
& (Join-Path $PSScriptRoot 'build-production.ps1') -WindhawkRoot $WindhawkRoot -OutputName beta2
. (Join-Path $PSScriptRoot 'evidence.ps1')
Write-Output 'Beta 2: bounded policy/platform/hidden integration verification'
& (Join-Path $PSScriptRoot 'test-production.ps1') -OutputName beta2 -Checks $DacChecks -TimeoutSeconds 55
Write-Output 'Beta 2: release-evidence rejection tests'
& (Join-Path $PSScriptRoot 'test-beta-tooling.ps1')
Write-Output 'Beta 2: package and verify'
& (Join-Path $PSScriptRoot 'package-beta.ps1')
& (Join-Path $PSScriptRoot 'verify-beta.ps1')
