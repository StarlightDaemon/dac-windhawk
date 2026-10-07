param([string[]]$Checks=@('policy','storage','catalog','faults','containment'),[ValidateRange(1,300)][int]$TimeoutSeconds=20,[ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9.-]*$')][string]$OutputName='production')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'evidence.ps1')
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$out=Join-Path $repo "build/windhawk/$OutputName"
$build=Dac-Build $out
foreach($check in $Checks) {
    if($check -notin $DacChecks) { throw "Unknown check $check" }
    foreach($extension in @('result','receipt.json')) { $path=Join-Path $out "$check.$extension";if(Test-Path -LiteralPath $path){Remove-Item -LiteralPath $path} }
    $watch=[Diagnostics.Stopwatch]::StartNew()
    function Save-Receipt {
        $evidence=[ordered]@{};foreach($extension in @('log','err','result')) {$evidence[$extension]=Dac-Hash (Join-Path $out "$check.$extension")}
        $cpuMs=$null;try {$cpuMs=$p.TotalProcessorTime.TotalMilliseconds} catch {}
        Dac-Json (Join-Path $out "$check.receipt.json") ([ordered]@{schema=1;check=$check;status='passed';buildId=$build.buildId;binarySha256=(Dac-Hash $exe);elapsedMs=$watch.ElapsedMilliseconds;hostCpuMs=$cpuMs;evidence=$evidence})
    }
    $exe=Join-Path $out (Dac-CheckBinary $check)
    $args=@{FilePath=$exe;WindowStyle='Hidden';RedirectStandardOutput=(Join-Path $out "$check.log");RedirectStandardError=(Join-Path $out "$check.err");PassThru=$true}
    if($check -ne 'policy') { $args.ArgumentList=@("--$check") }
    Write-Output "BEGIN $check (bounded at $TimeoutSeconds seconds)"
    $p=Start-Process @args
    if($check -eq 'host-death') {
        $children=@()
        try {
            $deadline=[DateTime]::UtcNow.AddSeconds(5)
            do {
                Start-Sleep -Milliseconds 50
                $log=Get-Content (Join-Path $out "$check.log") -Raw -ErrorAction SilentlyContinue
            } until(($log -match 'HOST-DEATH READY') -or [DateTime]::UtcNow -ge $deadline -or $p.HasExited)
            if($log -notmatch 'HOST-DEATH READY') { throw 'Host-death fixture did not become ready' }
            foreach($match in [regex]::Matches($log,'CHILD (\d+)')) {
                $child=Get-Process -Id ([int]$match.Groups[1].Value)
                $null=$child.Handle # retain exact process handle before host termination
                $children+=,$child
            }
            if($children.Count -ne 2) { throw 'Expected exactly two owned fixture descendants' }
            $p.Kill(); $null=$p.WaitForExit(2000)
            foreach($child in $children) { if(!$child.WaitForExit(2000)) {throw 'Owned descendant survived host death'} }
            'PASS host death: retained child and grandchild handles signaled' | Tee-Object -FilePath (Join-Path $out "$check.result")
            Save-Receipt
        } finally {
            if(!$p.HasExited) {$p.Kill();$null=$p.WaitForExit(2000)}
            foreach($child in $children) {$child.Dispose()}
        }
        continue
    }
    if(!$p.WaitForExit($TimeoutSeconds*1000)) { $p.Kill();if(!$p.WaitForExit(2000)){throw "TIMEOUT $check; exact harness termination not confirmed"};$p.Dispose();throw "TIMEOUT $check; exact owned harness terminated" }
    if($check -eq 'session-soak') {Get-Content (Join-Path $out "$check.log") -Tail 3;Get-Content (Join-Path $out "$check.err")}
    else {Get-Content (Join-Path $out "$check.log"),(Join-Path $out "$check.err")}
    "EXIT $($p.ExitCode)" | Set-Content (Join-Path $out "$check.result")
    if($p.ExitCode -ne 0) { throw "FAILED $check exit=$($p.ExitCode)" }
    Save-Receipt
    $p.Dispose()
}
