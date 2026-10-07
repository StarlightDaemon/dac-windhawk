param([string]$WindhawkRoot='C:\Program Files\Windhawk',[ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9.-]*$')][string]$OutputName='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
if(!$OutputName){$OutputName=(Dac-ReleaseSpec (Dac-CurrentVersion)).output}
$out=Join-Path $DacRepo "build/windhawk/$OutputName"
New-Item -ItemType Directory -Force $out | Out-Null
$hostFile=Join-Path $WindhawkRoot 'windhawk.exe'
$compiler=Join-Path $WindhawkRoot 'Compiler/bin/clang++.exe'
$os=Get-ItemProperty -LiteralPath 'HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion'
$savers=@('Bubbles.scr','Mystify.scr','Ribbons.scr','ssText3d.scr','PhotoScreensaver.scr','scrnsave.scr') | ForEach-Object {
    $path=Join-Path ([Environment]::SystemDirectory) $_
    $exists=Test-Path -LiteralPath $path -PathType Leaf
    [ordered]@{name=$_;installed=$exists;fileVersion=$(if($exists){(Get-Item -LiteralPath $path).VersionInfo.FileVersion}else{$null});sha256=$(if($exists){Dac-Hash $path}else{$null});qualification='unqualified'}
}
$config=Join-Path ([Environment]::GetFolderPath('LocalApplicationData')) 'DAC-Windhawk/settings-v1.ini'
$report=[ordered]@{schema=1;purpose='read-only beta prerequisites; no saver launch or desktop changes';os=[ordered]@{product=$os.ProductName;build=$os.CurrentBuild;ubr=$os.UBR};is64BitOS=[Environment]::Is64BitOperatingSystem;windhawkInstalled=(Test-Path -LiteralPath $hostFile);windhawkVersion=$(if(Test-Path -LiteralPath $hostFile){(Get-Item -LiteralPath $hostFile).VersionInfo.FileVersion}else{$null});compilerAvailable=(Test-Path -LiteralPath $compiler);configurationPresent=(Test-Path -LiteralPath $config);savers=@($savers);desktopQualification='outstanding'}
$report['hardwarePendingCount']=@(Get-ChildItem -LiteralPath (Split-Path $config) -Filter 'settings-v1.ini.power-*.pending' -File -ErrorAction SilentlyContinue).Count
$report['v1RollbackPresent']=Test-Path -LiteralPath ($config+'.v1-backup')
Dac-Json (Join-Path $out 'diagnostics.json') $report
$report | ConvertTo-Json -Depth 8
