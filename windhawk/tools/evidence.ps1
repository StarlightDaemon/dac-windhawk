# Shared evidence primitives. Importing never runs tests or writes files.
$DacRepo=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
. (Join-Path $PSScriptRoot 'release-spec.ps1')
function Dac-Hash([string]$Path) { (Get-FileHash -Algorithm SHA256 -LiteralPath $Path).Hash.ToLowerInvariant() }
function Dac-BytesHash([byte[]]$Bytes) {
    $sha=[Security.Cryptography.SHA256]::Create()
    try {([BitConverter]::ToString($sha.ComputeHash($Bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
}
function Dac-PeMachine([string]$Path) {
    $bytes=[IO.File]::ReadAllBytes($Path)
    if($bytes.Length -lt 64 -or [BitConverter]::ToUInt16($bytes,0) -ne 0x5a4d){throw 'Invalid DOS header'}
    $pe=[BitConverter]::ToInt32($bytes,60)
    if($pe -lt 64 -or $pe+6 -gt $bytes.Length -or [BitConverter]::ToUInt32($bytes,$pe) -ne 0x4550){throw 'Invalid PE header'}
    [BitConverter]::ToUInt16($bytes,$pe+4)
}
function Dac-InputInventory {
    @(Get-ChildItem -LiteralPath (Join-Path $DacRepo 'windhawk') -File -Recurse |
        Where-Object {$_.Extension -in $DacInputExtensions} | Sort-Object FullName |
        ForEach-Object {[ordered]@{path=$_.FullName.Substring($DacRepo.Length+1).Replace('\','/');sha256=(Dac-Hash $_.FullName)}})
}
function Dac-Digest($Value) { Dac-BytesHash ([Text.Encoding]::UTF8.GetBytes(($Value | ConvertTo-Json -Depth 12 -Compress))) }
function Dac-JsonBytes($Value) { ,([Text.UTF8Encoding]::new($false).GetBytes(($Value | ConvertTo-Json -Depth 12)+"`n")) }
function Dac-Json([string]$Path,$Value) { [IO.File]::WriteAllBytes($Path,(Dac-JsonBytes $Value)) }
function Dac-InventoryDigest($Rows) {
    $paths=[string[]]@($Rows | ForEach-Object {$_.path})
    [Array]::Sort($paths,[StringComparer]::Ordinal)
    $byPath=@{};foreach($row in $Rows){$byPath[$row.path]=$row.sha256}
    Dac-Digest @($paths | ForEach-Object {[ordered]@{path=$_;sha256=$byPath[$_]}})
}
function Dac-VerifyRollback($Record,$Pair,$Hashes,$Spec) {
    Dac-Schema $Record.schema 1 'rollback receipt'
    foreach($name in @('rc4.cpp','rollback.cpp','x86/restored-schema2.ini','x86/rollback.log','x86/rollback.err','x86-64/restored-schema2.ini','x86-64/rollback.log','x86-64/rollback.err')){
        if(!$Hashes.ContainsKey("evidence/rollback/$name")){throw "Missing rollback evidence: $name"}
    }
    foreach($field in @('historicalArchiveSha256','historicalSourceSha256','sourceSha256','inputDigest','fixtureSourceSha256','scriptSha256')){Dac-AssertHash $Record.$field "rollback $field"}
    if($Record.status -cne 'passed' -or $Record.sourceSha256 -cne $Pair.sourceSha256 -or $Record.inputDigest -cne $Pair.inputDigest){throw 'Rollback source/input identity mismatch'}
    if($Record.historicalArchiveSha256 -cnotin @('bf24f21219674d519a57576f913c899a572ac0944af089245d858e90a6676378','7c1c4037f95ed326f220a8231d467016346b77fe73680544c0461c7281ae1ccc') -or $Record.historicalSourceSha256 -cne 'ed923dfd5704e27fb10f897ccf02653d18ebe4980092a50cd5fc0a22c14d7dd4' -or $Record.historicalSourceSha256 -cne $Hashes['evidence/rollback/rc4.cpp']){throw 'Rollback historical input mismatch'}
    if($Record.scriptSha256 -cne $Hashes['windhawk/tools/test-rollback.ps1'] -or $Record.fixtureSourceSha256 -cne $Hashes['evidence/rollback/rollback.cpp']){throw 'Rollback executable input mismatch'}
    Dac-AssertNames @($Record.results.architecture) @('x86','x86-64') 'rollback architectures'
    foreach($row in $Record.results){
        $arch=$row.architecture
        foreach($field in @('buildId','binarySha256','fixtureSha256','logSha256','errSha256')){Dac-AssertHash $row.$field "rollback $arch $field"}
        if($row.buildId -cne $Pair.buildIds[$arch] -or $row.target -cne $Spec.targets[$arch]){throw 'Rollback build identity mismatch'}
        Dac-VerifyRollbackDependencies @($row.dependencies) $Pair.builds[$arch]
        if(($row.recoveryFilesRetained -isnot [int] -and $row.recoveryFilesRetained -isnot [long]) -or $row.recoveryFilesRetained -lt 1){throw 'Rollback recovery evidence missing'}
        if($row.recoveryFilesRetained -lt 1 -or $row.fixtureSha256 -cne $Hashes["evidence/rollback/$arch/restored-schema2.ini"] -or $row.logSha256 -cne $Hashes["evidence/rollback/$arch/rollback.log"] -or $row.errSha256 -cne $Hashes["evidence/rollback/$arch/rollback.err"]){throw 'Rollback fixture/log evidence mismatch'}
    }
}
function Dac-RollbackDependencyPaths([string]$Target){@('Compiler/bin/clang++.exe',"Compiler/$Target/bin/libc++.dll","Compiler/$Target/bin/libunwind.dll")}
function Dac-VerifyRollbackDependencies($Rows,$Build){
    $required=@(Dac-RollbackDependencyPaths $Build.identity.target)
    Dac-AssertNames @($Rows.path) $required 'rollback dependencies'
    foreach($row in $Rows){
        Dac-AssertHash $row.sha256 'rollback dependency'
        $expected=@($Build.identity.dependencies | Where-Object {$_.path -ceq $row.path})
        if($expected.Count -ne 1 -or $expected[0].sha256 -cne $row.sha256){throw 'Rollback toolchain dependency mismatch'}
    }
}
function Dac-BuildShape($Build,$Spec) {
    Dac-Schema $Build.schema 1 'build'
    if($Build.releaseEligible -isnot [bool] -or !$Build.releaseEligible){throw 'Build is not eligible: alternate source'}
    Dac-AssertHash $Build.buildId 'build identity'
    if((Dac-Digest $Build.identity) -cne $Build.buildId){throw 'Build identity is corrupt'}
    if($Build.identity.version -cne $Spec.version){throw 'Release version mismatch'}
    $paths=[Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
    foreach($row in @($Build.identity.inputs)) {
        Dac-AssertPath $row.path;Dac-AssertHash $row.sha256 'input'
        if(!$paths.Add($row.path)){throw 'Duplicate build input'}
    }
    if(!$paths.Count){throw 'Empty build input inventory'}
    $names=[Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
    foreach($artifact in @($Build.artifacts)) {
        Dac-AssertPath $artifact.name;Dac-AssertHash $artifact.sha256 'artifact'
        if($artifact.name.Contains('/') -or !$names.Add($artifact.name)){throw 'Invalid or duplicate build artifact'}
    }
    if($Spec.layout -ceq 'dual-architecture-v1') {
        if($Build.identity.modId -cnotin @($Spec.modId,('local@'+$Spec.modId))){throw 'Unexpected mod identity'}
        Dac-AssertNames @($Build.artifacts.name) @($Spec.sourceName,$Spec.binary,'policy-tests.exe','platform-tests.exe','libc++.whl','libunwind.whl') 'build artifacts'
        $source=@($Build.identity.inputs | Where-Object {$_.path -ceq $Spec.source})
        $compiled=@($Build.artifacts | Where-Object {$_.name -ceq $Spec.sourceName})
        if($source.Count -ne 1 -or $compiled[0].sha256 -cne $source[0].sha256){throw 'Compiled source input mismatch'}
    }
}
function Dac-Build([string]$Out) {
    $build=Get-Content -LiteralPath (Join-Path $Out 'build.json') -Raw | ConvertFrom-Json
    $spec=Dac-ReleaseSpec $build.identity.version;Dac-BuildShape $build $spec
    if((Dac-InventoryDigest @(Dac-InputInventory)) -cne (Dac-InventoryDigest @($build.identity.inputs))){throw 'Source, tests or tooling changed after build'}
    foreach($artifact in $build.artifacts) {
        if((Dac-Hash (Join-Path $Out $artifact.name)) -cne $artifact.sha256){throw "Build artifact changed: $($artifact.name)"}
    }
    return $build
}
function Dac-ReceiptShape($Receipt,[string]$Check,$Build) {
    Dac-Schema $Receipt.schema 1 'test receipt'
    if($Receipt.check -cne $Check -or $Receipt.status -cne 'passed' -or $Receipt.buildId -cne $Build.buildId){throw "Missing/current pass required: $Check"}
    $binary=@($Build.artifacts | Where-Object {$_.name -ceq (Dac-CheckBinary $Check)})
    if($binary.Count -ne 1 -or $Receipt.binarySha256 -cne $binary[0].sha256){throw "Tested binary mismatch: $Check"}
    foreach($extension in @('log','err','result')){Dac-AssertHash $Receipt.evidence.$extension "test evidence $Check.$extension"}
}
function Dac-VerifyEvidence([string]$Out) {
    $build=Dac-Build $Out;$spec=Dac-ReleaseSpec $build.identity.version
    foreach($check in $spec.checks) {
        $receipt=Get-Content -LiteralPath (Join-Path $Out "$check.receipt.json") -Raw | ConvertFrom-Json
        Dac-ReceiptShape $receipt $check $build
        foreach($extension in @('log','err','result')) {
            if($receipt.evidence.$extension -cne (Dac-Hash (Join-Path $Out "$check.$extension"))){throw "Test evidence changed: $check.$extension"}
        }
    }
    return $build
}
function Dac-PairShape($Builds,$Spec) {
    $first=$Builds['x86'];$second=$Builds['x86-64']
    foreach($arch in @('x86','x86-64')) {
        $build=$Builds[$arch];Dac-BuildShape $build $Spec
        if($build.identity.target -cne $Spec.targets[$arch]){throw "Mismatched architecture pair: $arch"}
    }
    $digest=Dac-InventoryDigest @($first.identity.inputs)
    if($digest -cne (Dac-InventoryDigest @($second.identity.inputs))){throw 'Mixed input inventory in architecture pair'}
    foreach($field in @('version','modId','hostVersion','hostArchitecture')) {
        if($first.identity.$field -cne $second.identity.$field){throw "Architecture pair identity mismatch: $field"}
    }
    foreach($dependency in @('windhawk.exe','Compiler/bin/clang++.exe','Compiler/include/windhawk_api.h','Compiler/include/windhawk_api_internal.h')) {
        $a=@($first.identity.dependencies | Where-Object {$_.path -ceq $dependency});$b=@($second.identity.dependencies | Where-Object {$_.path -ceq $dependency})
        if($a.Count -ne 1 -or $b.Count -ne 1 -or $a[0].sha256 -cne $b[0].sha256){throw "Architecture pair toolchain mismatch: $dependency"}
    }
    $source=@($first.identity.inputs | Where-Object {$_.path -ceq $Spec.source})[0].sha256
    [pscustomobject]@{builds=$Builds;inputDigest=$digest;sourceSha256=$source;buildIds=[ordered]@{'x86'=$first.buildId;'x86-64'=$second.buildId}}
}
function Dac-VerifyReleasePair($Spec) {
    if($Spec.layout -cne 'dual-architecture-v1'){throw 'Architecture pair requires a dual-architecture release'}
    $builds=[ordered]@{}
    foreach($arch in @('x86','x86-64')) {$builds[$arch]=Dac-VerifyEvidence (Join-Path $DacRepo ('build/windhawk/'+$Spec.outputs[$arch]))}
    Dac-PairShape $builds $Spec
}
function Dac-ReleaseManifest($Pair,$Spec) {
    [ordered]@{schema=1;specId=$Spec.specId;version=$Spec.version;source=$Spec.source;sourceSha256=$Pair.sourceSha256;inputDigest=$Pair.inputDigest;requiredChecks=@($Spec.checks);architectures=@('x86','x86-64');buildIds=$Pair.buildIds}
}
function Dac-ToolingReceipt($Pair,$Spec,$Cases) {
    [ordered]@{schema=2;status='passed';specId=$Spec.specId;version=$Spec.version;buildIds=$Pair.buildIds;inputDigest=$Pair.inputDigest;sourceSha256=$Pair.sourceSha256;cases=@($Cases)}
}
function Dac-VerifyTooling($Tooling,$Spec,$Pair,$Build) {
    $schema=if($Spec.layout -ceq 'dual-architecture-v1'){2}else{1};Dac-Schema $Tooling.schema $schema 'tooling receipt'
    if($Tooling.status -cne 'passed'){throw 'Tooling regressions must pass'}
    Dac-AssertNames @($Tooling.cases) @($Spec.cases) 'tooling cases'
    if($schema -eq 1) {if($Tooling.buildId -cne $Build.buildId){throw 'Stale tooling build receipt'};return}
    if($Tooling.specId -cne $Spec.specId -or $Tooling.version -cne $Spec.version -or $Tooling.inputDigest -cne $Pair.inputDigest -or $Tooling.sourceSha256 -cne $Pair.sourceSha256){throw 'Tooling release identity mismatch'}
    Dac-AssertNames @($Tooling.buildIds.psobject.Properties.Name) @('x86','x86-64') 'tooling architectures'
    foreach($arch in @('x86','x86-64')){if($Tooling.buildIds.$arch -cne $Pair.buildIds[$arch]){throw "Stale tooling build receipt: $arch"}}
}
