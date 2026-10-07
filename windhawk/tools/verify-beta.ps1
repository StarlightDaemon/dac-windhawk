param([string]$Archive='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
. (Join-Path $PSScriptRoot 'archive-evidence.ps1')
if(!$Archive){$spec=Dac-ReleaseSpec (Dac-CurrentVersion);$Archive=Join-Path $DacRepo ('build/windhawk/'+$spec.output+'/'+$spec.archive)}
$result=Dac-VerifyArchive $Archive
"PASS beta archive: $($result.version); $($result.layout); $($result.entries) verified entries; compiled source $($result.sourceSha256)"
