param([ValidateSet('beta1','beta2','rc1','rc2','rc3','rc4','nextbeta','nextbeta2','nextbeta3','nextbeta4','dac-beta5','dac-beta6','dac-0.1.6')][string]$OutputName='dac-0.1.6')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
. (Join-Path $PSScriptRoot 'archive-evidence.ps1')
. (Join-Path $PSScriptRoot 'tooling-fixtures.ps1')
$spec=Dac-OutputSpec $OutputName;$out=Join-Path $DacRepo "build/windhawk/$OutputName"
$resultPath=Join-Path $out 'tooling.receipt.json'
# Any attempted rerun invalidates the previous aggregate pass before validation.
if(Test-Path -LiteralPath $resultPath){Remove-Item -LiteralPath $resultPath}
$pair=$null
if($spec.layout -ceq 'dual-architecture-v1'){$pair=Dac-VerifyReleasePair $spec;$build=$pair.builds['x86']}else{$build=Dac-VerifyEvidence $out;if($build.identity.version -cne $spec.version){throw 'Release version mismatch'}}
$cases=[Collections.Generic.List[string]]::new()
function Restore-EvidenceBytes([string]$Path,[byte[]]$Bytes){
    $deadline=[DateTime]::UtcNow.AddSeconds(5)
    while($true){
        try{[IO.File]::WriteAllBytes($Path,$Bytes);return}
        catch [IO.IOException]{$cause=$_.Exception;while($cause.InnerException){$cause=$cause.InnerException};if(($cause.HResult -band 0xffff) -notin @(32,33) -or [DateTime]::UtcNow -ge $deadline){throw};Start-Sleep -Milliseconds 50}
    }
}
function Reject-Mutation([string]$Label,[string]$Name,[scriptblock]$Mutation) {
    $path=Join-Path $out $Name;$backup=[IO.File]::ReadAllBytes($path)
    try {
        & $Mutation $path
        $rejected=$false;try{$null=Dac-VerifyEvidence $out}catch{$rejected=$true}
        if(!$rejected){throw "Verifier accepted $Label"}
        $cases.Add($Label);Write-Output "PASS rejection: $Label"
    } finally {[IO.File]::WriteAllBytes($path,$backup)}
}
Reject-Mutation 'failed receipt' 'policy.receipt.json' {param($p)$r=Get-Content $p -Raw|ConvertFrom-Json;$r.status='failed';Dac-Json $p $r}
Reject-Mutation 'stale build receipt' 'policy.receipt.json' {param($p)$r=Get-Content $p -Raw|ConvertFrom-Json;$r.buildId='old';Dac-Json $p $r}
Reject-Mutation 'missing receipt' 'policy.receipt.json' {param($p)Remove-Item -LiteralPath $p}
Reject-Mutation 'edited log' 'policy.log' {param($p)[IO.File]::AppendAllText($p,'corruption')}
Reject-Mutation 'changed binary' 'policy-tests.exe' {param($p)$b=[IO.File]::ReadAllBytes($p);$b[$b.Length-1]=$b[$b.Length-1] -bxor 1;[IO.File]::WriteAllBytes($p,$b)}
Reject-Mutation 'alternate-source eligibility' 'build.json' {param($p)$b=Get-Content $p -Raw|ConvertFrom-Json;$b.releaseEligible=$false;Dac-Json $p $b}
Reject-Mutation 'changed test/tool input' 'build.json' {param($p)$b=Get-Content $p -Raw|ConvertFrom-Json;$row=@($b.identity.inputs | Where-Object {$_.path -ceq 'windhawk/tests/policy.cpp'})[0];$row.sha256=('0'*64);$b.buildId=Dac-Digest $b.identity;Dac-Json $p $b}
$backups=@{};foreach($extension in @('log','err','result','receipt.json')){$p=Join-Path $out "faults.$extension";$backups[$p]=[IO.File]::ReadAllBytes($p)}
try {
    $timeoutFailure=$null
    try{& (Join-Path $PSScriptRoot 'test-production.ps1') -OutputName $OutputName -Checks faults -TimeoutSeconds 1}catch{$timeoutFailure=$_.Exception.Message}
    if($timeoutFailure -cne 'TIMEOUT faults; exact owned harness terminated'){throw "Expected confirmed owned-harness timeout, got: $timeoutFailure"}
    if((Test-Path -LiteralPath (Join-Path $out 'faults.receipt.json')) -or (Test-Path -LiteralPath (Join-Path $out 'faults.result'))){throw 'Failed rerun retained passing evidence'}
    $cases.Add('actual timed-out rerun invalidates previous pass');Write-Output 'PASS timeout invalidation'
} finally {$restoreErrors=@();foreach($p in $backups.Keys){try{Restore-EvidenceBytes $p $backups[$p]}catch{$restoreErrors+=$_.Exception.Message}};if($restoreErrors.Count){throw ($restoreErrors -join '; ')}}
if($spec.layout -ceq 'dual-architecture-v1') {
    $pair=Dac-VerifyReleasePair $spec
    $payload=Dac-ArchivePayload $spec $pair $out
    # This synthetic receipt exists only in isolated fixture ZIPs. It is never
    # copied to the real evidence directory or promoted as a package.
    $payload['evidence/tooling.receipt.json']=Dac-JsonBytes (Dac-ToolingReceipt $pair $spec $spec.cases)
    $directory=Join-Path $out ('tooling-fixtures/'+[Guid]::NewGuid().ToString('N'))
    foreach($case in @(Dac-ArchiveRejections $payload $directory $spec)){$cases.Add($case)}
    $pair=Dac-VerifyReleasePair $spec
    Dac-AssertNames @($cases) @($spec.cases) 'completed tooling cases'
    Dac-Json $resultPath (Dac-ToolingReceipt $pair $spec @($cases))
} else {
    $null=Dac-VerifyEvidence $out;Dac-AssertNames @($cases) @($spec.cases) 'completed tooling cases'
    Dac-Json $resultPath ([ordered]@{schema=1;status='passed';buildId=$build.buildId;cases=@($cases)})
}
Write-Output "PASS beta tooling: $($cases.Count) named negative cases, original passing evidence restored byte-for-byte"
