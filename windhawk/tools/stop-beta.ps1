# Explicit emergency exit for this edition only. Does not alter saved settings.
$ErrorActionPreference='Stop'
if(!('DacEmergency' -as [type])) {
    Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class DacEmergency {
    [DllImport("kernel32.dll", CharSet=CharSet.Unicode, SetLastError=true)] public static extern IntPtr OpenEvent(uint access, bool inherit, string name);
    [DllImport("kernel32.dll", SetLastError=true)] public static extern bool SetEvent(IntPtr handle);
    [DllImport("kernel32.dll")] public static extern bool CloseHandle(IntPtr handle);
}
'@
}
$handle=[DacEmergency]::OpenEvent(2,$false,'Local\DAC-Windhawk-Emergency')
if($handle -eq [IntPtr]::Zero) {Write-Output 'No accessible Display Activity Controls for Windhawk host in this session.';return}
try {if(![DacEmergency]::SetEvent($handle)){throw 'Could not request emergency exit'};Write-Output 'Emergency exit requested. Final OS/COM cleanup can still wait.'} finally {$null=[DacEmergency]::CloseHandle($handle)}
