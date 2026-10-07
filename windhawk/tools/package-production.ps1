$ErrorActionPreference='Stop'
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$out=Join-Path $repo 'build/windhawk/production'
. (Join-Path $PSScriptRoot 'evidence.ps1')
$verifiedBuild=Dac-VerifyEvidence $out
$identity=Get-Content -LiteralPath (Join-Path $out 'identities.json') -Raw | ConvertFrom-Json
$source=Join-Path $repo 'windhawk/mods/dac-windhawk.wh.cpp'
if((Get-FileHash -LiteralPath $source).Hash -ne $identity[0].Hash) {throw 'Source changed since successful build'}
foreach($test in @('policy','storage','catalog','media','faults','containment','host-death','emergency','lifecycle','probes')) {
    $result=Join-Path $out "$test.result"
    if(!(Test-Path -LiteralPath $result) -or ((Get-Content -LiteralPath $result -Raw).Trim() -notmatch '^(EXIT 0|PASS host death:)')) {throw "Missing passing evidence: $test"}
    if((Get-Item -LiteralPath $result).LastWriteTimeUtc -lt (Get-Item -LiteralPath $source).LastWriteTimeUtc) {throw "Stale test evidence: $test"}
}
$files=Get-ChildItem -LiteralPath (Join-Path $repo 'windhawk') -File -Recurse | Where-Object { $_.Extension -in @('.cpp','.md','.ps1') } | Sort-Object FullName
$manifest=($files | ForEach-Object { "{0}  {1}" -f (Get-FileHash -LiteralPath $_.FullName).Hash.ToLowerInvariant(),($_.FullName.Substring($repo.Length+1).Replace('\','/')) }) -join "`n"
$manifest+="`n"
$zipPath=Join-Path $out "dac-windhawk-$($verifiedBuild.identity.version)-source.zip"
if(Test-Path -LiteralPath $zipPath) {Remove-Item -LiteralPath $zipPath}
$stream=[IO.File]::Open($zipPath,[IO.FileMode]::CreateNew)
$zip=[IO.Compression.ZipArchive]::new($stream,[IO.Compression.ZipArchiveMode]::Create)
try {
    foreach($file in $files) {
        $entry=$zip.CreateEntry($file.FullName.Substring($repo.Length+1).Replace('\','/'),[IO.Compression.CompressionLevel]::Optimal)
        $entry.LastWriteTime=[DateTimeOffset]::new(1980,1,1,0,0,0,[TimeSpan]::Zero)
        $dest=$entry.Open();try {$bytes=[IO.File]::ReadAllBytes($file.FullName);$dest.Write($bytes,0,$bytes.Length)} finally {$dest.Dispose()}
    }
    $entry=$zip.CreateEntry('SHA256SUMS.txt');$entry.LastWriteTime=[DateTimeOffset]::new(1980,1,1,0,0,0,[TimeSpan]::Zero)
    $dest=$entry.Open();try {$bytes=[Text.Encoding]::UTF8.GetBytes($manifest);$dest.Write($bytes,0,$bytes.Length)} finally {$dest.Dispose()}
} finally {$zip.Dispose();$stream.Dispose()}
[IO.File]::WriteAllText((Join-Path $out 'SHA256SUMS.txt'),$manifest,[Text.UTF8Encoding]::new($false))
Get-FileHash -LiteralPath $zipPath | Format-List
