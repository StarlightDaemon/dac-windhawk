param([string]$WindhawkRoot='C:\Program Files\Windhawk',[string]$SourceFile='',[ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9.-]*$')][string]$OutputName='production',[ValidateSet('host','x86','x86-64')][string]$Architecture='host',[string]$ModId='dac-windhawk',[string]$FujinRoot='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$out=Join-Path $repo "build/windhawk/$OutputName"
New-Item -ItemType Directory -Force $out | Out-Null
# Invalidate success before any validation/launch can fail, including toolchain lookup.
foreach($name in @('build.json','identities.json','dac-windhawk.dll')) { $p=Join-Path $out $name;if(Test-Path -LiteralPath $p){Remove-Item -LiteralPath $p} }
$compiler=Join-Path $WindhawkRoot 'Compiler/bin/clang++.exe'
$version=(Get-Item (Join-Path $WindhawkRoot 'windhawk.exe')).VersionInfo.FileVersion
if($version -ne '1.7.3') { throw "Toolchain not qualified by this build: $version" }
$hostMachine=Dac-PeMachine (Join-Path $WindhawkRoot 'windhawk.exe')
$hostArchitecture=switch($hostMachine){0x14c {'x86'} 0x8664 {'x86-64'} default {throw 'Unsupported Windhawk host PE architecture'}}
if($Architecture -eq 'host'){$Architecture=$hostArchitecture}
$bits=if($Architecture -eq 'x86'){32}else{64}
$target=if($bits -eq 32){'i686-w64-mingw32'}else{'x86_64-w64-mingw32'}
$engine=Join-Path $WindhawkRoot "Engine/$version/$bits/windhawk.lib"
$common=@('-std=c++23','-O2','-target',$target,'-DUNICODE','-D_UNICODE','-D_WIN32_WINNT=0x0A00','-DWINVER=0x0A00','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wl,--no-insert-timestamp')
$source=Join-Path $repo 'windhawk/mods/dac-windhawk.wh.cpp'
if($SourceFile) { $source=(Resolve-Path -LiteralPath $SourceFile).Path }
if(Test-Path -LiteralPath (Join-Path $out 'identities.json')) {Remove-Item -LiteralPath (Join-Path $out 'identities.json')}
$text=Get-Content -LiteralPath $source -Raw
if($ModId -notmatch '^(local@)?dac-windhawk$'){throw 'Unexpected mod identity'}
$architectures=@([regex]::Matches($text,'(?m)^// @architecture[^\S\r\n]+([^\r\n]+)') | ForEach-Object {$_.Groups[1].Value.Trim()})
foreach($entry in $architectures){if($entry -notin @('x86','x86-64','amd64','arm64')){throw "Invalid Windhawk architecture metadata value: $entry (use separate lines)"}}
if($hostArchitecture -notin $architectures -or $Architecture -notin $architectures){throw "Metadata excludes the installed $hostArchitecture Windhawk host or the requested build target"}
if($text -match '#include\s+"' -or $text -notmatch '// ==WindhawkMod==' -or $text -notmatch '@id\s+dac-windhawk\s') { throw 'Invalid standalone Windhawk source format' }
$optionLine=($text -split "`n" | Where-Object { $_ -match '^// @compilerOptions ' })
$libraries=($optionLine -replace '^// @compilerOptions\s+','').Trim() -split '\s+'
$versionMatches=[regex]::Matches($text,'(?m)^// @version\s+([^\r\n ]+)\s*$')
if($versionMatches.Count -ne 1) {throw 'Exactly one version is required'}
$modVersion=$versionMatches[0].Groups[1].Value
if($OutputName -cne 'production') {
    $releaseSpec=Dac-OutputSpec $OutputName
    if($modVersion -cne $releaseSpec.version){throw 'Release output/version mismatch'}
    if($releaseSpec.layout -ceq 'dual-architecture-v1' -and $OutputName -cne $releaseSpec.outputs[$Architecture]){throw 'Release output/architecture mismatch'}
}
& (Join-Path $PSScriptRoot 'sync-fujin.ps1') -FujinRoot $FujinRoot -Check
$inputs=Dac-InputInventory
Copy-Item -LiteralPath $source -Destination (Join-Path $out 'dac-windhawk.wh.cpp')
function Build-Checked([string]$name,[string[]]$arguments) {
    $target=Join-Path $out $name
    if(Test-Path -LiteralPath $target) { Remove-Item -LiteralPath $target }
    Set-Content -LiteralPath (Join-Path $out "$name.build.log") -Value ''
    & $compiler @arguments '-o' $target 2>&1 | Tee-Object -FilePath (Join-Path $out "$name.build.log")
    if($LASTEXITCODE -ne 0 -or !(Test-Path -LiteralPath $target) -or (Get-Item -LiteralPath $target).Length -eq 0) { throw "Build failed: $name (exit $LASTEXITCODE)" }
}
& $compiler --version | Set-Content (Join-Path $out 'compiler-version.txt')
# Compile the exact copied standalone distribution, using its own metadata libraries.
Build-Checked 'dac-windhawk.dll' ($common+@('-shared','-DWH_MOD',('-DWH_MOD_ID=L"'+$ModId+'"'),('-DWH_MOD_VERSION=L"'+$modVersion+'"'),'-include','windhawk_api.h',$engine,(Join-Path $out 'dac-windhawk.wh.cpp'),'-Wl,--export-all-symbols')+$libraries)
Build-Checked 'policy-tests.exe' ($common+@((Join-Path $repo 'windhawk/tests/policy.cpp')))
Build-Checked 'platform-tests.exe' ($common+@('-municode',(Join-Path $repo 'windhawk/tests/platform.cpp'))+$libraries+@('-lpsapi'))
foreach($runtime in @('libc++','libunwind')) {
    Copy-Item -LiteralPath (Join-Path $WindhawkRoot "Compiler/$target/bin/$runtime.dll") -Destination (Join-Path $out "$runtime.whl")
}
Get-FileHash -Algorithm SHA256 -LiteralPath $source,(Join-Path $out 'dac-windhawk.dll'),(Join-Path $out 'policy-tests.exe'),(Join-Path $out 'platform-tests.exe') | ConvertTo-Json | Set-Content (Join-Path $out 'identities.json')
Get-Content (Join-Path $out 'identities.json')
if((Dac-Digest $inputs) -ne (Dac-Digest @(Dac-InputInventory))) {throw 'Inputs changed during compilation; rebuild required'}
$dependencies=@('windhawk.exe','Compiler/bin/clang++.exe','Compiler/include/windhawk_api.h','Compiler/include/windhawk_api_internal.h',"Engine/$version/$bits/windhawk.lib","Compiler/$target/bin/libc++.dll","Compiler/$target/bin/libunwind.dll")
foreach($binary in @('dac-windhawk.dll','policy-tests.exe','platform-tests.exe')) {
    if((Dac-PeMachine (Join-Path $out $binary)) -ne $(if($bits -eq 32){0x14c}else{0x8664})){throw "Compiled binary architecture mismatch: $binary"}
}
$identity=[ordered]@{version=$modVersion;modId=$ModId;hostVersion=$version;hostArchitecture=$hostArchitecture;target=$target;inputs=$inputs;dependencies=@($dependencies | ForEach-Object { [ordered]@{path=$_;sha256=(Dac-Hash (Join-Path $WindhawkRoot $_))} })}
$artifacts=@('dac-windhawk.wh.cpp','dac-windhawk.dll','policy-tests.exe','platform-tests.exe','libc++.whl','libunwind.whl') | ForEach-Object {[ordered]@{name=$_;sha256=(Dac-Hash (Join-Path $out $_))}}
Dac-Json (Join-Path $out 'build.json') ([ordered]@{schema=1;buildId=(Dac-Digest $identity);releaseEligible=(!$SourceFile);identity=$identity;artifacts=@($artifacts)})
