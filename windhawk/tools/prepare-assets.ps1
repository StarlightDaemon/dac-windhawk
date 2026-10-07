# Prepare only a fully verified source package and its exact compiled source.
param([string]$Destination='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
. (Join-Path $PSScriptRoot 'archive-evidence.ps1')
& (Join-Path $PSScriptRoot 'version.ps1')
$spec=Dac-ReleaseSpec (Dac-CurrentVersion)
$out=Join-Path $DacRepo ('build/windhawk/'+$spec.output)
$null=Dac-VerifyReleasePair $spec
$archive=Join-Path $out $spec.archive
$verified=Dac-VerifyArchive $archive
if($verified.version -cne $spec.version -or $verified.sourceSha256 -cne (Dac-Hash (Join-Path $DacRepo $spec.source))){throw 'Archive does not match current source'}
if(!$Destination){$Destination=Join-Path $DacRepo 'build/release-assets'}
if(Test-Path -LiteralPath $Destination){if(@(Get-ChildItem -LiteralPath $Destination -Force).Count){throw 'Asset destination must be empty; preserve previous releases separately'}}
New-Item -ItemType Directory -Force $Destination | Out-Null
Copy-Item -LiteralPath $archive -Destination (Join-Path $Destination $spec.archive)
Copy-Item -LiteralPath (Join-Path $DacRepo $spec.source) -Destination (Join-Path $Destination $spec.sourceName)
$rows=foreach($name in @($spec.archive,$spec.sourceName)){(Dac-Hash (Join-Path $Destination $name))+'  '+$name}
[IO.File]::WriteAllText((Join-Path $Destination 'SHA256SUMS.txt'),($rows -join "`n")+"`n",[Text.UTF8Encoding]::new($false))
Write-Output "Verified release assets: $Destination"
