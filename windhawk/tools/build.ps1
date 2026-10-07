param([string]$WindhawkRoot = 'C:\Program Files\Windhawk')
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$out = Join-Path $repo 'build/windhawk'
New-Item -ItemType Directory -Force $out | Out-Null
$compiler = Join-Path $WindhawkRoot 'Compiler/bin/clang++.exe'
$engineVersion = (Get-Item (Join-Path $WindhawkRoot 'windhawk.exe')).VersionInfo.FileVersion
$engine = Join-Path $WindhawkRoot "Engine/$engineVersion/64/windhawk.lib"
$common = @('-std=c++23','-O2','-target','x86_64-w64-mingw32','-DUNICODE','-D_UNICODE','-D_WIN32_WINNT=0x0A00','-DWINVER=0x0A00','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas')
$libraries = @('-lshell32','-luser32','-lgdi32','-lshcore','-lole32','-luuid','-lwtsapi32')
function Build-Checked([string]$name,[string[]]$arguments) {
    $target=Join-Path $out $name
    if(Test-Path -LiteralPath $target) { Remove-Item -LiteralPath $target }
    & $compiler @arguments '-o' $target 2>&1 | Tee-Object -FilePath (Join-Path $out "$name.build.log")
    if($LASTEXITCODE -ne 0 -or !(Test-Path -LiteralPath $target)) { throw "Build failed: $name (exit $LASTEXITCODE)" }
    Get-FileHash -Algorithm SHA256 -LiteralPath $target | Format-List
}
& $compiler --version | Set-Content (Join-Path $out 'compiler-version.txt')
Build-Checked 'oled-aegis-prototype.dll' ($common + @('-shared','-DWH_MOD','-DWH_MOD_ID=L"oled-aegis-prototype"','-DWH_MOD_VERSION=L"0.1"','-include','windhawk_api.h',$engine,(Join-Path $repo 'windhawk/prototype/oled-aegis-prototype.wh.cpp'),'-Wl,--export-all-symbols') + $libraries)
Build-Checked 'harness.exe' ($common + @('-municode',(Join-Path $repo 'windhawk/tools/harness.cpp')) + $libraries)
# Windhawk's bundled import libraries use .whl runtime names. Local test-only
# copies; no redistributable package is produced here.
foreach($runtime in @('libc++','libunwind')) {
    Copy-Item -LiteralPath (Join-Path $WindhawkRoot "Compiler/x86_64-w64-mingw32/bin/$runtime.dll") -Destination (Join-Path $out "$runtime.whl")
}
