$ErrorActionPreference='Stop'
$root=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
. (Join-Path $root 'windhawk/tools/release-spec.ps1')
$current=Dac-CurrentVersion
$fixture=Join-Path $root ('build/research/version-'+[guid]::NewGuid().ToString('N'))
foreach($dir in @('windhawk/tools','windhawk/mods')){New-Item -ItemType Directory -Force (Join-Path $fixture $dir) | Out-Null}
foreach($file in @('windhawk/tools/version.ps1','windhawk/tools/release-spec.ps1','windhawk/mods/dac-windhawk.wh.cpp','windhawk/CHANGELOG.md')){Copy-Item -LiteralPath (Join-Path $root $file) -Destination (Join-Path $fixture $file)}
$command=Join-Path $fixture 'windhawk/tools/version.ps1'
function Reject([scriptblock]$Action,[string]$Expected){$failure='';try{& $Action | Out-Null}catch{$failure=$_.Exception.Message};if(!$failure.Contains($Expected)){throw "Expected rejection '$Expected', got '$failure'"}}
& $command -Tag "v$current"
Reject {& $command -Tag 'v99.99.99'} 'differs from source'
Reject {& $command -Set $current} 'Choose a higher'
Reject {& $command -Set '01.2.3'} 'Choose a higher'
Reject {& $command -Set '../escape'} 'Choose a higher'
$next=([version]$current).Major.ToString()+'.'+([version]$current).Minor+'.'+(([version]$current).Build+1)
& $command -Set $next
Reject {& $command} 'Add a changelog section'
Add-Content -LiteralPath (Join-Path $fixture 'windhawk/CHANGELOG.md') -Value "`n## $next — fixture"
& $command -Tag "v$next"
$source=Join-Path $fixture 'windhawk/mods/dac-windhawk.wh.cpp'
$text=[IO.File]::ReadAllText($source).Replace('constexpr char kVersion[]="'+$next+'";','constexpr char kVersion[]="wrong";')
[IO.File]::WriteAllText($source,$text)
Reject {& $command} 'Diagnostic version differs'
if((Dac-ReleaseSpec '0.3.0').checks.Count -ne 16 -or (Dac-ReleaseSpec '1.0.0').cases.Count -ne 34){throw 'Future release weakened evidence contract'}
Reject {Dac-ReleaseSpec '../1.0.0'} 'Unsupported release version'
Reject {Dac-OutputSpec '../dac-0.2.0'} 'Unsupported release output'
Write-Output 'PASS version regression: monotonic bumps, malformed versions, stale diagnostics, missing changelog, tag mismatch, fixed future evidence gates'
