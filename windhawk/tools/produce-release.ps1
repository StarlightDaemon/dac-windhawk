param([string]$WindhawkRoot='C:\Program Files\Windhawk',[string]$HistoricalArchive=(Join-Path $PSScriptRoot '../tests/fixtures/rc4-source.zip'),[string]$ReleaseVersion='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
if(!$ReleaseVersion){$ReleaseVersion=Dac-CurrentVersion}
$spec=Dac-ReleaseSpec $ReleaseVersion
& (Join-Path $PSScriptRoot 'version.ps1') -Tag "v$ReleaseVersion"
& (Join-Path $PSScriptRoot '../tests/version.ps1')
& (Join-Path $PSScriptRoot 'diagnose-beta.ps1') -WindhawkRoot $WindhawkRoot -OutputName $spec.output
foreach($architecture in @('x86','x86-64')) {
    & (Join-Path $PSScriptRoot 'build-production.ps1') -WindhawkRoot $WindhawkRoot -OutputName $spec.outputs[$architecture] -Architecture $architecture
    & (Join-Path $PSScriptRoot 'test-production.ps1') -OutputName $spec.outputs[$architecture] -Checks $spec.checks -TimeoutSeconds 55
}
& (Join-Path $PSScriptRoot 'test-rollback.ps1') -Archive $HistoricalArchive -WindhawkRoot $WindhawkRoot -OutputName $spec.output
& (Join-Path $PSScriptRoot 'test-beta-tooling.ps1') -OutputName $spec.output
& (Join-Path $PSScriptRoot 'package-beta.ps1') -OutputName $spec.output
& (Join-Path $PSScriptRoot 'verify-beta.ps1') -Archive (Join-Path $DacRepo ('build/windhawk/'+$spec.output+'/'+$spec.archive))
