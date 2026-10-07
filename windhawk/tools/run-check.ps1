param([Parameter(Mandatory)][string]$Name,[string[]]$TestArguments=@('--catalog'),[int]$TimeoutSeconds=9)
$ErrorActionPreference='Stop'
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$out=Join-Path $repo 'build/windhawk'
if($Name -notmatch '^[a-zA-Z0-9-]+$') { throw 'Name must be a simple evidence label' }
$p=Start-Process -FilePath (Join-Path $out 'harness.exe') -ArgumentList $TestArguments -WindowStyle Hidden -RedirectStandardOutput (Join-Path $out "$Name.log") -RedirectStandardError (Join-Path $out "$Name.err") -PassThru
if(!$p.WaitForExit($TimeoutSeconds*1000)) {
    # Exact process handle returned by this launch, never a saver-name kill.
    $p.Kill(); $null=$p.WaitForExit(2000)
    "TIMEOUT $Name" | Set-Content (Join-Path $out "$Name.result")
    throw "Owned harness timed out: $Name"
}
"EXIT $($p.ExitCode)" | Tee-Object -FilePath (Join-Path $out "$Name.result")
Get-Content (Join-Path $out "$Name.log"),(Join-Path $out "$Name.err")
if($p.ExitCode -ne 0) { throw "Harness failed: $Name" }
