# Isolated ZIP fixtures. Synthetic fixtures never become release evidence.
function Dac-FixtureJson($Payload,[string]$Name) { [Text.Encoding]::UTF8.GetString($Payload[$Name]) | ConvertFrom-Json }
function Dac-FixtureRebind($Payload,[string]$Arch,$Build,$Spec) {
    $Build.buildId=Dac-Digest $Build.identity;$Payload["evidence/$Arch/build.json"]=Dac-JsonBytes $Build
    foreach($check in $Spec.checks) {
        $name="evidence/$Arch/$check.receipt.json";$receipt=Dac-FixtureJson $Payload $name;$receipt.buildId=$Build.buildId;$Payload[$name]=Dac-JsonBytes $receipt
    }
    foreach($name in @('evidence/release.json','evidence/tooling.receipt.json')) {
        $record=Dac-FixtureJson $Payload $name;$record.buildIds.$Arch=$Build.buildId;$Payload[$name]=Dac-JsonBytes $record
    }
}
function Dac-ArchiveRejections($Payload,[string]$Directory,$Spec) {
    $null=New-Item -ItemType Directory -Path $Directory -ErrorAction Stop
    $baseline=Join-Path $Directory 'fixture-baseline.zip';$null=Dac-WriteArchive $baseline $Payload
    $null=Dac-VerifyArchive $baseline
    $passed=[Collections.Generic.List[string]]::new();$sequence=0
    function Reject-Archive([string]$Label,[scriptblock]$Mutation,[string]$Expected,[scriptblock]$ZipMutation=$null,[int]$Variants=1) {
        for($variant=0;$variant -lt $Variants;$variant++) {
            $copy=[ordered]@{};foreach($key in $Payload.Keys){$copy[$key]=$Payload[$key]}
            if($Mutation){& $Mutation $copy $variant}
            $path=Join-Path $Directory (([Guid]::NewGuid().ToString('N'))+'.zip')
            $null=Dac-WriteArchive $path $copy
            if($ZipMutation) {
                $zip=[IO.Compression.ZipFile]::Open($path,[IO.Compression.ZipArchiveMode]::Update)
                try{& $ZipMutation $zip $variant}finally{$zip.Dispose()}
            }
            $failure=$null
            try{$null=Dac-VerifyArchive $path}catch{$failure=$_.Exception.Message}
            if(!$failure -or $failure -notmatch $Expected){throw "Archive case '$Label' did not reject for $Expected`: $failure"}
        }
        $passed.Add($Label);Write-Host "PASS archive rejection: $Label"
    }
    Reject-Archive 'unknown release version' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/release.json';$r.version='../9.9.9';$p['evidence/release.json']=Dac-JsonBytes $r} 'Unsupported release version'
    if($Spec.modId -ceq 'dac-windhawk') {
        Reject-Archive 'wrong mod identity' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/x86/build.json';$r.identity.modId='oled-aegis';Dac-FixtureRebind $p 'x86' $r $Spec} 'Unexpected mod identity'
        Reject-Archive 'wrong binary name' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/x86/build.json';($r.artifacts | Where-Object {$_.name -ceq $Spec.binary}).name='oled-aegis.dll';$p['evidence/x86/build.json']=Dac-JsonBytes $r} 'build artifacts'
    }
    Reject-Archive 'unsupported build schema' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/x86/build.json';$r.schema=99;$p['evidence/x86/build.json']=Dac-JsonBytes $r} 'Unsupported build schema'
    Reject-Archive 'unsupported test receipt schema' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/x86/policy.receipt.json';$r.schema=99;$p['evidence/x86/policy.receipt.json']=Dac-JsonBytes $r} 'Unsupported test receipt schema'
    Reject-Archive 'unsupported tooling receipt schema' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/tooling.receipt.json';$r.schema=99;$p['evidence/tooling.receipt.json']=Dac-JsonBytes $r} 'Unsupported tooling receipt schema'
    Reject-Archive 'missing x64 evidence' {param($p,$v)foreach($key in @($p.Keys | Where-Object {$_ -like 'evidence/x86-64/*'})){$p.Remove($key)}} 'Required archive entry missing: evidence/x86-64/build.json'
    Reject-Archive 'mismatched architecture pair' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/x86-64/build.json';$r.identity.target='i686-w64-mingw32';Dac-FixtureRebind $p 'x86-64' $r $Spec} 'Mismatched architecture pair'
    Reject-Archive 'mixed input inventory' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/x86-64/build.json';$row=@($r.identity.inputs | Where-Object {$_.path -cne $Spec.source})[0];$row.sha256=('0'*64);Dac-FixtureRebind $p 'x86-64' $r $Spec} 'Archived build input mismatch'
    Reject-Archive 'changed archived source' {param($p,$v)$p[$Spec.source]=[Text.Encoding]::UTF8.GetBytes([Text.Encoding]::UTF8.GetString($p[$Spec.source])+"`n// fixture mutation")} 'Archived build input mismatch'
    Reject-Archive 'wrong standalone source name' {param($p,$v)$p['windhawk/mods/wrong-source.wh.cpp']=$p[$Spec.source];$p.Remove($Spec.source)} 'Archived build input mismatch'
    foreach($check in @('advanced','power','slides','nextbeta')) {
        Reject-Archive "missing $check receipt" {param($p,$v)$p.Remove("evidence/x86/$check.receipt.json")} "Required archive entry missing: evidence/x86/$check.receipt.json"
    }
    if('nativewake' -cin $Spec.checks){
        Reject-Archive 'missing nativewake receipt' {param($p,$v)$arch=if($v -eq 0){'x86'}else{'x86-64'};$p.Remove("evidence/$arch/nativewake.receipt.json")} 'Required archive entry missing: evidence/(x86|x86-64)/nativewake.receipt.json' $null 2
        Reject-Archive 'omitted nativewake declaration' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/release.json';$r.requiredChecks=@($r.requiredChecks | Where-Object {$_ -cne 'nativewake'});$p['evidence/release.json']=Dac-JsonBytes $r} 'required checks'
        Reject-Archive 'missing native wake report' {param($p,$v)$p.Remove('windhawk/docs/NATIVE_WAKE.md')} 'Required report missing'
    }
    Reject-Archive 'shortened declared check list' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/release.json';$r.requiredChecks=@($r.requiredChecks | Where-Object {$_ -cne 'nextbeta'});$p['evidence/release.json']=Dac-JsonBytes $r} 'required checks'
    Reject-Archive 'missing required report' {param($p,$v)$p.Remove($Spec.reports[0])} 'Required report missing'
    Reject-Archive 'duplicate archive path' $null 'Duplicate archive path' {param($z,$v)$e=$z.CreateEntry('SHA256SUMS.txt');$s=$e.Open();$s.Dispose()}
    Reject-Archive 'unsafe archive path' {param($p,$v)$names=@('../escape','/root','C:/bad','dir\file','dir//file','dir/./file','dir/file.');$p[$names[$v]]=[byte[]]@(1)} 'Unsafe archive/input path' $null 7
    Reject-Archive 'malformed checksum manifest' $null 'Malformed checksum row' {param($z,$v)$z.GetEntry('SHA256SUMS.txt').Delete();$e=$z.CreateEntry('SHA256SUMS.txt');$s=$e.Open();try{$b=[Text.Encoding]::UTF8.GetBytes('invalid checksum row');$s.Write($b,0,$b.Length)}finally{$s.Dispose()}}
    Reject-Archive 'unlisted archive entry' $null 'Unlisted archive entry' {param($z,$v)$e=$z.CreateEntry('unlisted.txt');$s=$e.Open();$s.Dispose()}
    Reject-Archive 'wrong tooling case set' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/tooling.receipt.json';switch($v){0{$r.cases=@($r.cases | Select-Object -Skip 1)}1{$r.cases[-1]=$r.cases[0]}2{$r.cases[-1]='invented case'}};$p['evidence/tooling.receipt.json']=Dac-JsonBytes $r} 'tooling cases' $null 3
    Reject-Archive 'missing rollback receipt' {param($p,$v)
        if($v -eq 0){$p.Remove('evidence/rollback/rollback.receipt.json');return}
        $r=Dac-FixtureJson $p 'evidence/rollback/rollback.receipt.json';$index=[int][Math]::Floor(($v-1)/5);$mode=($v-1)%5
        $names=@('rollback.cpp','x86/restored-schema2.ini','x86/rollback.log','x86/rollback.err','x86-64/restored-schema2.ini','x86-64/rollback.log','x86-64/rollback.err')
        if($mode -ne 3){$p.Remove('evidence/rollback/'+$names[$index])}
        $object=$r;$field='fixtureSourceSha256'
        if($index -gt 0){$object=$r.results[[int][Math]::Floor(($index-1)/3)];$field=@('fixtureSha256','logSha256','errSha256')[($index-1)%3]}
        switch($mode){0{$object.psobject.Properties.Remove($field)}1{$object.$field=$null}2{$object.$field=''}3{$object.psobject.Properties.Remove($field)}}
        $p['evidence/rollback/rollback.receipt.json']=Dac-JsonBytes $r
    } 'Required archive entry missing|Missing rollback evidence|Invalid SHA-256: rollback' $null 36
    Reject-Archive 'unsupported rollback schema' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/rollback/rollback.receipt.json';$r.schema=99;$p['evidence/rollback/rollback.receipt.json']=Dac-JsonBytes $r} 'Unsupported rollback receipt schema'
    Reject-Archive 'misbound rollback evidence' {param($p,$v)$r=Dac-FixtureJson $p 'evidence/rollback/rollback.receipt.json';if($v -eq 0){$r.results[1].buildId=('0'*64)}elseif($v -le 3){$r.results[1].dependencies[$v-1].sha256=('0'*64)}else{$r.results[1].dependencies=@($r.results[1].dependencies | Select-Object -Skip 1)};$p['evidence/rollback/rollback.receipt.json']=Dac-JsonBytes $r} 'Rollback build identity mismatch|Rollback toolchain dependency mismatch|rollback dependencies' $null 5
    Reject-Archive 'changed rollback log' {param($p,$v)$p['evidence/rollback/x86/rollback.log']=[Text.Encoding]::UTF8.GetBytes('changed fixture output')} 'Rollback fixture/log evidence mismatch'
    return @($passed)
}
