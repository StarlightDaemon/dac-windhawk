# Shared archive reader/writer. Import is read-only; callers choose explicit paths.
function Dac-ArchivePayload($Spec,$Pair,[string]$Out) {
    $payload=[ordered]@{}
    Get-ChildItem -LiteralPath (Join-Path $DacRepo 'windhawk') -File -Recurse |
        Where-Object {$_.Extension -in (@($DacInputExtensions)+@('.md'))} | Sort-Object FullName | ForEach-Object {
            $name=$_.FullName.Substring($DacRepo.Length+1).Replace('\','/');$payload[$name]=[IO.File]::ReadAllBytes($_.FullName)
        }
    if($Spec.layout -ceq 'dual-architecture-v1') {
        $payload['evidence/release.json']=Dac-JsonBytes (Dac-ReleaseManifest $Pair $Spec)
        foreach($arch in @('x86','x86-64')) {
            $directory=Join-Path $DacRepo ('build/windhawk/'+$Spec.outputs[$arch])
            foreach($name in @('build.json')+@($Spec.checks | ForEach-Object {"$_.receipt.json";"$_.log";"$_.err";"$_.result"})) {
                $payload["evidence/$arch/$name"]=[IO.File]::ReadAllBytes((Join-Path $directory $name))
            }
        }
        $rollbackRoot=Join-Path $DacRepo ('build/research/'+$Spec.output+'-rollback')
        foreach($name in @('rollback.receipt.json','rc4.cpp','rollback.cpp','x86/restored-schema2.ini','x86/rollback.log','x86/rollback.err','x86-64/restored-schema2.ini','x86-64/rollback.log','x86-64/rollback.err')){
            $payload["evidence/rollback/$name"]=[IO.File]::ReadAllBytes((Join-Path $rollbackRoot $name))
        }
    } else {
        foreach($name in @('build.json')+@($Spec.checks | ForEach-Object {"$_.receipt.json";"$_.log";"$_.err";"$_.result"})) {
            $payload["evidence/$name"]=[IO.File]::ReadAllBytes((Join-Path $Out $name))
        }
    }
    return $payload
}
function Dac-WriteArchive([string]$Path,$Payload) {
    $stream=[IO.File]::Open($Path,[IO.FileMode]::CreateNew)
    $zip=[IO.Compression.ZipArchive]::new($stream,[IO.Compression.ZipArchiveMode]::Create)
    $manifest=[Text.StringBuilder]::new()
    try {
        foreach($name in @($Payload.Keys | Sort-Object)) {
            if($name -ceq 'SHA256SUMS.txt'){throw 'Payload cannot provide checksum manifest'}
            [byte[]]$bytes=$Payload[$name];$null=$manifest.Append((Dac-BytesHash $bytes)+'  '+$name+"`n")
            $entry=$zip.CreateEntry($name,[IO.Compression.CompressionLevel]::Optimal);$entry.LastWriteTime=[DateTimeOffset]::new(1980,1,1,0,0,0,[TimeSpan]::Zero)
            $dest=$entry.Open();try{$dest.Write($bytes,0,$bytes.Length)}finally{$dest.Dispose()}
        }
        $entry=$zip.CreateEntry('SHA256SUMS.txt');$entry.LastWriteTime=[DateTimeOffset]::new(1980,1,1,0,0,0,[TimeSpan]::Zero)
        $bytes=[Text.Encoding]::UTF8.GetBytes($manifest.ToString());$dest=$entry.Open();try{$dest.Write($bytes,0,$bytes.Length)}finally{$dest.Dispose()}
    } finally {$zip.Dispose();$stream.Dispose()}
    return $manifest.ToString()
}
function Dac-EntryText($Entries,[string]$Name) {
    if(!$Entries.ContainsKey($Name)){throw "Required archive entry missing: $Name"}
    $reader=[IO.StreamReader]::new($Entries[$Name].Open(),[Text.UTF8Encoding]::new($false,$true))
    try{$reader.ReadToEnd()}finally{$reader.Dispose()}
}
function Dac-EntryJson($Entries,[string]$Name) { (Dac-EntryText $Entries $Name) | ConvertFrom-Json }
function Dac-VerifyArchivedBundle($Entries,$Hashes,[string]$Prefix,$Spec) {
    $build=Dac-EntryJson $Entries ($Prefix+'build.json');Dac-BuildShape $build $Spec
    foreach($row in $build.identity.inputs) {if($Hashes[$row.path] -cne $row.sha256){throw "Archived build input mismatch: $($row.path)"}}
    if($Spec.layout -ceq 'dual-architecture-v1') {
        $archivedInputs=@($Hashes.Keys | Where-Object {$_.StartsWith('windhawk/',[StringComparison]::Ordinal) -and [IO.Path]::GetExtension($_) -in $DacInputExtensions})
        Dac-AssertNames @($build.identity.inputs.path) $archivedInputs 'archived build inputs'
    }
    foreach($check in $Spec.checks) {
        $receipt=Dac-EntryJson $Entries ($Prefix+"$check.receipt.json");Dac-ReceiptShape $receipt $check $build
        foreach($extension in @('log','err','result')) {
            if($Hashes[$Prefix+"$check.$extension"] -cne $receipt.evidence.$extension){throw "Archived test evidence mismatch: $check.$extension"}
        }
    }
    $compiled=@($build.artifacts | Where-Object {$_.name -ceq $Spec.sourceName})
    if(!$Hashes[$Spec.source] -or $compiled.Count -ne 1 -or $compiled[0].sha256 -cne $Hashes[$Spec.source]){throw 'Packaged source differs from build identity'}
    return $build
}
function Dac-VerifyArchive([string]$Archive) {
    $zip=[IO.Compression.ZipFile]::OpenRead((Resolve-Path -LiteralPath $Archive).Path)
    try {
        $entries=[Collections.Generic.Dictionary[string,object]]::new([StringComparer]::Ordinal)
        $names=[Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
        $total=0L
        foreach($entry in $zip.Entries) {
            Dac-AssertPath $entry.FullName
            if(!$names.Add($entry.FullName)){throw 'Duplicate archive path'}
            $total+=$entry.Length
            if($entry.Length -gt 64MB -or $total -gt 512MB -or $names.Count -gt 4096){throw 'Archive exceeds evidence size bounds'}
            $entries.Add($entry.FullName,$entry)
        }
        $manifest=Dac-EntryText $entries 'SHA256SUMS.txt'
        $seen=[Collections.Generic.Dictionary[string,string]]::new([StringComparer]::Ordinal)
        foreach($line in ($manifest -split "`n")) {
            if(!$line){continue}
            if($line -cnotmatch '^([0-9a-f]{64})  (.+)$'){throw 'Malformed checksum row'}
            $expected=$Matches[1];$name=$Matches[2];Dac-AssertPath $name
            if($name -ceq 'SHA256SUMS.txt' -or $seen.ContainsKey($name) -or !$entries.ContainsKey($name)){throw 'Duplicate/missing manifest entry'}
            $stream=$entries[$name].Open();$sha=[Security.Cryptography.SHA256]::Create()
            try{$actual=([BitConverter]::ToString($sha.ComputeHash($stream))).Replace('-','').ToLowerInvariant()}finally{$stream.Dispose();$sha.Dispose()}
            if($actual -cne $expected){throw "Archive corruption: $name"};$seen.Add($name,$actual)
        }
        if($seen.Count -ne $entries.Count-1){throw 'Unlisted archive entry'}
        if($entries.ContainsKey('evidence/release.json')) {
            $release=Dac-EntryJson $entries 'evidence/release.json';Dac-Schema $release.schema 1 'release manifest'
            $spec=Dac-ReleaseSpec $release.version
            if($spec.layout -cne 'dual-architecture-v1' -or $release.specId -cne $spec.specId -or $release.source -cne $spec.source){throw 'Invalid release manifest identity'}
            Dac-AssertNames @($release.requiredChecks) @($spec.checks) 'required checks'
            Dac-AssertNames @($release.architectures) @('x86','x86-64') 'release architectures'
            Dac-AssertNames @($release.buildIds.psobject.Properties.Name) @('x86','x86-64') 'release build IDs'
            $builds=[ordered]@{}
            foreach($arch in @('x86','x86-64')){$builds[$arch]=Dac-VerifyArchivedBundle $entries $seen "evidence/$arch/" $spec}
            $pair=Dac-PairShape $builds $spec
            if($release.inputDigest -cne $pair.inputDigest -or $release.sourceSha256 -cne $pair.sourceSha256){throw 'Release source/input identity mismatch'}
            foreach($arch in @('x86','x86-64')){if($release.buildIds.$arch -cne $pair.buildIds[$arch]){throw "Release build ID mismatch: $arch"}}
            $tooling=Dac-EntryJson $entries 'evidence/tooling.receipt.json';Dac-VerifyTooling $tooling $spec $pair $null
            Dac-VerifyRollback (Dac-EntryJson $entries 'evidence/rollback/rollback.receipt.json') $pair $seen $spec
            $text=Dac-EntryText $entries $spec.source
            $versions=[regex]::Matches($text,'(?m)^// @version[^\S\r\n]+([^\r\n ]+)[^\S\r\n]*\r?$')
            $ids=[regex]::Matches($text,'(?m)^// @id[^\S\r\n]+([^\r\n ]+)[^\S\r\n]*\r?$')
            if($versions.Count -ne 1 -or $versions[0].Groups[1].Value -cne $spec.version -or $ids.Count -ne 1 -or $ids[0].Groups[1].Value -cne $spec.modId){throw 'Standalone source metadata mismatch'}
            $architectures=@([regex]::Matches($text,'(?m)^// @architecture[^\S\r\n]+([^\r\n]+)') | ForEach-Object {$_.Groups[1].Value.Trim()})
            Dac-AssertNames $architectures @('x86','x86-64') 'source architectures'
        } else {
            $build=Dac-EntryJson $entries 'evidence/build.json';$spec=Dac-ReleaseSpec $build.identity.version
            if($spec.layout -cne 'legacy'){throw 'New release requires both architecture evidence bundles'}
            $build=Dac-VerifyArchivedBundle $entries $seen 'evidence/' $spec
            $tooling=Dac-EntryJson $entries 'evidence/tooling.receipt.json';Dac-VerifyTooling $tooling $spec $null $build
        }
        foreach($required in $spec.reports){if(!$seen.ContainsKey($required)){throw "Required report missing: $required"}}
        [pscustomobject]@{version=$spec.version;layout=$spec.layout;entries=$seen.Count;sourceSha256=$seen[$spec.source]}
    } finally {$zip.Dispose()}
}
