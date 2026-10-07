param(
    [string]$WindhawkRoot='C:\Program Files\Windhawk',
    [ValidateSet('x86','x86-64','arm64')][string[]]$Architecture=@('x86','x86-64','arm64')
)
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
# Unique directories prevent a failed probe from exposing an old success receipt.
# These outputs deliberately have no release build.json or test receipts.
$out=Join-Path $DacRepo ('build/compatibility/'+[guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $out -Force | Out-Null
$receipt=[ordered]@{schema=1;status='failed';scope='compile-only';releaseEligible=$false;runtimeTested=$false;architectures=@();error=$null}
Write-Output "Compatibility evidence: $out"
try {
    if(!$Architecture.Count){throw 'At least one architecture is required'}
    $version=(Get-Item -LiteralPath (Join-Path $WindhawkRoot 'windhawk.exe')).VersionInfo.FileVersion
    if($version -ne '1.7.3'){throw "No compiler recipe qualified for Windhawk $version; release support is unchanged"}
    $compiler=Join-Path $WindhawkRoot 'Compiler/bin/clang++.exe'
    $inputs=Dac-InputInventory
    $source=Join-Path $DacRepo 'windhawk/mods/dac-windhawk.wh.cpp'
    $sourceText=Get-Content -LiteralPath $source -Raw
    $modVersion=Dac-CurrentVersion
    $libraries=([regex]::Match($sourceText,'(?m)^// @compilerOptions[^\S\r\n]+([^\r\n]+)').Groups[1].Value.Trim() -split '\s+')
    if(!$libraries.Count -or @($libraries | Where-Object {$_ -notmatch '^-l[a-zA-Z0-9_]+$'}).Count){throw 'Probe expects library-only compiler metadata; review the recipe'}
    $receipt.hostVersion=$version
    $receipt.sourceSha256=Dac-Hash $source
    $receipt.inputDigest=Dac-Digest $inputs
    $receipt.compilerSha256=Dac-Hash $compiler
    $receipt.apiHeaders=@('windhawk_api.h','windhawk_api_internal.h') | ForEach-Object {[ordered]@{name=$_;sha256=(Dac-Hash (Join-Path $WindhawkRoot "Compiler/include/$_"))}}
    & $compiler --version | Set-Content -LiteralPath (Join-Path $out 'compiler-version.txt')
    if($LASTEXITCODE -ne 0){throw 'Compiler version query failed'}
    foreach($arch in ($Architecture | Select-Object -Unique)) {
        $target,$folder,$machine=switch($arch){'x86' {'i686-w64-mingw32';'32';0x14c} 'x86-64' {'x86_64-w64-mingw32';'64';0x8664} 'arm64' {'aarch64-w64-mingw32';'arm64';0xaa64}}
        $dir=Join-Path $out $arch
        New-Item -ItemType Directory -Path $dir | Out-Null
        $engine=Join-Path $WindhawkRoot "Engine/$version/$folder/windhawk.lib"
        $common=@('-std=c++23','-O2','-target',$target,'-DUNICODE','-D_UNICODE','-D_WIN32_WINNT=0x0A00','-DWINVER=0x0A00','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wl,--no-insert-timestamp')
        $builds=[ordered]@{
            'dac-windhawk.dll'=($common+@('-shared','-DWH_MOD','-DWH_MOD_ID=L"dac-windhawk"',('-DWH_MOD_VERSION=L"'+$modVersion+'"'),'-include','windhawk_api.h',$engine,$source,'-Wl,--export-all-symbols')+$libraries)
            'policy-tests.exe'=($common+@((Join-Path $DacRepo 'windhawk/tests/policy.cpp')))
            'platform-tests.exe'=($common+@('-municode',(Join-Path $DacRepo 'windhawk/tests/platform.cpp'))+$libraries+@('-lpsapi'))
        }
        $row=[ordered]@{architecture=$arch;target=$target;engineSha256=(Dac-Hash $engine);artifacts=@();commands=@()}
        foreach($name in $builds.Keys) {
            $arguments=$builds[$name]+@('-o',(Join-Path $dir $name))
            $row.commands+=,@{binary=$name;arguments=$arguments}
            & $compiler @arguments 2>&1 | Tee-Object -FilePath (Join-Path $dir "$name.log")
            if($LASTEXITCODE -ne 0){throw "Compile failed: $arch/$name ($LASTEXITCODE)"}
            if((Dac-PeMachine (Join-Path $dir $name)) -ne $machine){throw "Wrong PE machine: $arch/$name"}
            $row.artifacts+=@{name=$name;sha256=(Dac-Hash (Join-Path $dir $name));machine=$machine}
        }
        $receipt.architectures+=,$row
        Write-Output "PASS compile/link/PE $arch (no execution or host loading)"
    }
    if((Dac-Digest @(Dac-InputInventory)) -ne $receipt.inputDigest){throw 'Inputs changed during probe; rerun required'}
    $receipt.status='passed'
} catch {
    $receipt.error=$_.Exception.Message
    throw
} finally {
    Dac-Json (Join-Path $out 'compatibility.json') $receipt
}
