param([Parameter(Mandatory=$true)][string]$BaselineSource,[ValidateSet('x86','x86-64')][string]$Architecture='x86-64',[string]$WindhawkRoot='C:\Program Files\Windhawk')
$ErrorActionPreference='Stop'
$root=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$baseline=(Resolve-Path -LiteralPath $BaselineSource).Path
$expected='c5b7a7148b27cbff63acf1e73332ff332ded7ab02951d7c80cfe45bf731281a4'
if((Get-FileHash -Algorithm SHA256 -LiteralPath $baseline).Hash.ToLowerInvariant() -cne $expected){throw 'Negative control must be unchanged published 0.2.1 source'}
$out=Join-Path $root "build/investigation/nativewake-control-$Architecture"
New-Item -ItemType Directory -Force $out | Out-Null
$compiler=Join-Path $WindhawkRoot 'Compiler/bin/clang++.exe'
$target=if($Architecture -eq 'x86'){'i686-w64-mingw32'}else{'x86_64-w64-mingw32'}
$current=Join-Path $root 'windhawk/mods/dac-windhawk.wh.cpp'
$text=Get-Content -Raw -LiteralPath $current
$libraries=(($text -split "`n" | Where-Object {$_ -match '^// @compilerOptions '}) -replace '^// @compilerOptions\s+','').Trim() -split '\s+'
foreach($runtime in @('libc++','libunwind')){Copy-Item -LiteralPath (Join-Path $WindhawkRoot "Compiler/$target/bin/$runtime.dll") -Destination (Join-Path $out "$runtime.whl")}
foreach($case in @(@{name='baseline';source=$baseline;exit=1},@{name='repaired';source=$current;exit=0})){
    $exe=Join-Path $out ($case.name+'.exe')
    $define='-DDAC_WAKE_SOURCE="'+$case.source.Replace('\','/')+'"'
    & $compiler '-std=c++23' '-O2' '-target' $target '-municode' '-DUNICODE' '-D_UNICODE' '-D_WIN32_WINNT=0x0A00' '-DWINVER=0x0A00' '-Wall' '-Wextra' '-Werror' '-Wno-unknown-pragmas' $define (Join-Path $root 'windhawk/tests/nativewake-negative.cpp') @libraries '-o' $exe 2>&1 | Tee-Object -FilePath (Join-Path $out ($case.name+'.build.log'))
    if($LASTEXITCODE -ne 0){throw "Control must compile before behavioral comparison: $($case.name)"}
    $stdout=Join-Path $out ($case.name+'.log');$stderr=Join-Path $out ($case.name+'.err')
    $p=Start-Process -FilePath $exe -WindowStyle Hidden -RedirectStandardOutput $stdout -RedirectStandardError $stderr -PassThru
    if(!$p.WaitForExit(10000)){$p.Kill();throw 'Control timed out'}
    if($p.ExitCode -ne $case.exit){throw "Unexpected control exit $($case.name): $($p.ExitCode)"}
    $failure=Get-Content -Raw -LiteralPath $stderr
    $positive=Get-Content -Raw -LiteralPath $stdout
    if($positive -notmatch 'POSITIVE control queue-age=0 ms: wake A and preserve B passed'){throw 'Fresh positive control did not pass'}
    if($case.name -eq 'repaired' -and $positive -notmatch 'POSITIVE control queue-age=300 ms: wake A and preserve B passed'){throw 'Repaired delayed positive control did not pass'}
    if($case.name -eq 'baseline' -and $failure -notmatch 'FAIL queued 300-ms mouse movement must wake intended existing native session without second input'){throw 'Baseline failed for wrong reason'}
    $record="$($case.name) architecture=$Architecture sourceSha256=$((Get-FileHash -Algorithm SHA256 -LiteralPath $case.source).Hash.ToLowerInvariant()) exit=$($p.ExitCode)"
    $record | Tee-Object -FilePath (Join-Path $out ($case.name+'.result'))
    Get-Content -LiteralPath $stdout,$stderr
    $p.Dispose()
}
Write-Output 'PASS negative control: unchanged 0.2.1 compiles and misses queued wake; repaired source accepts same input and preserves peer'
