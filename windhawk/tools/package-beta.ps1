param([ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9.-]*$')][string]$OutputName='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
if(!$OutputName){$OutputName=(Dac-ReleaseSpec (Dac-CurrentVersion)).output}
. (Join-Path $PSScriptRoot 'archive-evidence.ps1')
$spec=Dac-OutputSpec $OutputName;$out=Join-Path $DacRepo "build/windhawk/$OutputName"
$pair=$null;$build=$null
if($spec.layout -ceq 'dual-architecture-v1'){$pair=Dac-VerifyReleasePair $spec}else{$build=Dac-VerifyEvidence $out;if($build.identity.version -cne $spec.version){throw 'Release version mismatch'}}
$tooling=Get-Content -LiteralPath (Join-Path $out 'tooling.receipt.json') -Raw | ConvertFrom-Json
Dac-VerifyTooling $tooling $spec $pair $build
$payload=Dac-ArchivePayload $spec $pair $out
$payload['evidence/tooling.receipt.json']=Dac-JsonBytes $tooling
$zipPath=Join-Path $out $spec.archive;$temp=$zipPath+'.tmp'
if(Test-Path -LiteralPath $temp){Remove-Item -LiteralPath $temp}
$manifest=Dac-WriteArchive $temp $payload
$verified=Dac-VerifyArchive $temp
if($spec.layout -ceq 'dual-architecture-v1'){$null=Dac-VerifyReleasePair $spec}else{$null=Dac-VerifyEvidence $out}
Move-Item -LiteralPath $temp -Destination $zipPath -Force
[IO.File]::WriteAllText((Join-Path $out 'SHA256SUMS.txt'),$manifest,[Text.UTF8Encoding]::new($false))
Write-Output "Beta source package: $zipPath"
Write-Output "SHA-256: $(Dac-Hash $zipPath)"
