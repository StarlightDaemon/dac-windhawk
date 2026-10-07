param([string]$Archive=(Join-Path $PSScriptRoot '../tests/fixtures/rc4-source.zip'),[string]$WindhawkRoot='C:\Program Files\Windhawk',[ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9.-]*$')][string]$OutputName='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
if(!$OutputName){$OutputName=(Dac-ReleaseSpec (Dac-CurrentVersion)).output}
# Deliberately separate historical input, not bundled into the new standalone mod.
$accepted=@('bf24f21219674d519a57576f913c899a572ac0944af089245d858e90a6676378','7c1c4037f95ed326f220a8231d467016346b77fe73680544c0461c7281ae1ccc')
$expected=if(Test-Path -LiteralPath $Archive){Dac-Hash $Archive}else{''}
if($expected -cnotin $accepted){throw 'Supply the preserved RC4 source ZIP with the documented exact SHA256'}
$spec=Dac-OutputSpec $OutputName
$root=Join-Path $DacRepo ('build/research/'+$spec.output+'-rollback')
New-Item -ItemType Directory -Force $root | Out-Null
if(Test-Path -LiteralPath (Join-Path $root 'rollback.receipt.json')){Remove-Item -LiteralPath (Join-Path $root 'rollback.receipt.json')}
Add-Type -AssemblyName System.IO.Compression.FileSystem
$zip=[IO.Compression.ZipFile]::OpenRead((Resolve-Path -LiteralPath $Archive).Path)
try {
    $entry=$zip.GetEntry('windhawk/mods/liminal-oled-guard.wh.cpp')
    if(!$entry){throw 'Pinned historical source is missing'}
    $stream=$entry.Open();$memory=[IO.MemoryStream]::new()
    try{$stream.CopyTo($memory);$source=$memory.ToArray()}finally{$stream.Dispose();$memory.Dispose()}
}finally{$zip.Dispose()}
[IO.File]::WriteAllBytes((Join-Path $root 'rc4.cpp'),$source)
$fixture=@'
#define AEGIS_POLICY_ONLY
#include "rc4.cpp"
#include <filesystem>
#include <fstream>
#include <iterator>
int wmain(int argc,wchar_t** argv){
    if(argc!=2)return 2;
    std::ifstream file(std::filesystem::path(argv[1]),std::ios::binary);
    std::string bytes((std::istreambuf_iterator<char>(file)),{});
    if(bytes.starts_with("\xef\xbb\xbf"))bytes.erase(0,3); // RC4 ReadFileText does this.
    aegis::Config config;std::string error;
    if(!file||!aegis::Parse(bytes,config,error)||config.timeout!=17||config.automatic||config.monitors["A"].enabled||config.monitors["A"].saver!=2||config.monitors["A"].input!=1)return 3;
    auto before=aegis::Serialize(config);
    if(aegis::Parse("version=3\n",config,error)||aegis::Serialize(config)!=before)return 4;
    puts("ROLLBACK PASS: actual pinned RC4 parser accepts exact restored schema2 fixture and rejects schema3 without changing active preferences");
    return 0;
}
'@
[IO.File]::WriteAllText((Join-Path $root 'rollback.cpp'),$fixture,[Text.UTF8Encoding]::new($false))
$results=@()
foreach($architecture in @('x86','x86-64')) {
    $out=Join-Path $DacRepo "build/windhawk/$($spec.outputs[$architecture])"
    $build=Dac-Build $out
    $test=Get-Content -LiteralPath (Join-Path $out 'nextbeta.receipt.json') -Raw | ConvertFrom-Json
    Dac-ReceiptShape $test 'nextbeta' $build
    $restored=Join-Path $out 'platform-tests.exe.nextbeta-settings';$backup="$restored.v2-backup"
    if((Dac-Hash $restored) -cne (Dac-Hash $backup)){throw 'The production migration fixture was not restored byte-for-byte'}
    $markers=@(Get-ChildItem -LiteralPath $out -File | Where-Object {$_.Name.StartsWith('platform-tests.exe.nextbeta-settings.power-')} | ForEach-Object {[ordered]@{path=$_.FullName;sha256=(Dac-Hash $_.FullName)}})
    if(!$markers.Count){throw 'Missing isolated recovery retention fixture'}
    $archRoot=Join-Path $root $architecture;New-Item -ItemType Directory -Force $archRoot | Out-Null
    Copy-Item -LiteralPath $restored -Destination (Join-Path $archRoot 'restored-schema2.ini')
    $exe=Join-Path $archRoot 'rollback.exe';$target=$spec.targets[$architecture]
    $dependencyInputs=@(Dac-RollbackDependencyPaths $target | ForEach-Object {[ordered]@{path=$_;sha256=(Dac-Hash (Join-Path $WindhawkRoot $_))}})
    Dac-VerifyRollbackDependencies $dependencyInputs $build
    & (Join-Path $WindhawkRoot 'Compiler/bin/clang++.exe') '-std=c++23' '-O2' '-target' $target '-Wall' '-Wextra' '-Werror' '-municode' (Join-Path $root 'rollback.cpp') '-o' $exe
    if($LASTEXITCODE -ne 0){throw "Historical parser compilation failed: $architecture"}
    foreach($runtime in @('libc++','libunwind')){Copy-Item -LiteralPath (Join-Path $WindhawkRoot "Compiler/$target/bin/$runtime.dll") -Destination (Join-Path $archRoot "$runtime.whl")}
    $log=Join-Path $archRoot 'rollback.log';$err=Join-Path $archRoot 'rollback.err'
    $process=Start-Process -FilePath $exe -ArgumentList ('"'+$restored+'"') -WindowStyle Hidden -PassThru -RedirectStandardOutput $log -RedirectStandardError $err
    if(!$process.WaitForExit(10000)){$process.Kill();$process.WaitForExit();throw "Historical parser fixture timed out: $architecture"}
    if($process.ExitCode -ne 0){throw "Historical parser fixture failed: $architecture exit=$($process.ExitCode)"}
    $afterDependencies=@(Dac-RollbackDependencyPaths $target | ForEach-Object {[ordered]@{path=$_;sha256=(Dac-Hash (Join-Path $WindhawkRoot $_))}})
    Dac-VerifyRollbackDependencies $afterDependencies $build
    foreach($marker in $markers){if((Dac-Hash $marker.path) -cne $marker.sha256){throw 'Historical parser changed recovery state'}}
    Get-Content -LiteralPath $log
    $results += [ordered]@{architecture=$architecture;target=$target;buildId=$build.buildId;dependencies=$dependencyInputs;fixtureSha256=(Dac-Hash $restored);binarySha256=(Dac-Hash $exe);logSha256=(Dac-Hash $log);errSha256=(Dac-Hash $err);recoveryFilesRetained=$markers.Count}
}
$pair=Dac-VerifyReleasePair $spec
Dac-Json (Join-Path $root 'rollback.receipt.json') ([ordered]@{schema=1;status='passed';historicalArchiveSha256=$expected;historicalSourceSha256=(Dac-BytesHash $source);sourceSha256=$pair.sourceSha256;inputDigest=$pair.inputDigest;fixtureSourceSha256=(Dac-Hash (Join-Path $root 'rollback.cpp'));scriptSha256=(Dac-Hash $PSCommandPath);results=$results})
Write-Output 'PASS pinned RC4 parser rollback compatibility on x86 and x86-64; isolated preferences only'
