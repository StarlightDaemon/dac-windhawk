param([string]$Set='', [string]$Tag='')
$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'release-spec.ps1')
$root=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$path=Join-Path $root 'windhawk/mods/dac-windhawk.wh.cpp'
$current=Dac-CurrentVersion
if($Set) {
    if($Set -cnotmatch '^(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)$' -or [version]$Set -le [version]$current){throw 'Choose a higher MAJOR.MINOR.PATCH version (no leading zeros)'}
    $text=[IO.File]::ReadAllText($path)
    $text=[regex]::Replace($text,'(?m)^(// @version[^\S\r\n]+)[^\s]+',('${1}'+$Set))
    $text=$text.Replace('constexpr char kVersion[]="'+$current+'";','constexpr char kVersion[]="'+$Set+'";')
    [IO.File]::WriteAllText($path,$text,[Text.UTF8Encoding]::new($false))
    Write-Output "Version set to $Set. Add windhawk/CHANGELOG.md notes, then run this script without -Set."
    return
}
$null=Dac-ReleaseSpec $current
$text=[IO.File]::ReadAllText($path)
if(!$text.Contains('constexpr char kVersion[]="'+$current+'";')){throw 'Diagnostic version differs from source metadata'}
if($Tag -and $Tag -cne "v$current"){throw "Tag $Tag differs from source version v$current"}
$notes=[IO.File]::ReadAllText((Join-Path $root 'windhawk/CHANGELOG.md'))
if($notes -cnotmatch ('(?m)^## '+[regex]::Escape($current)+'(?:\s|$)')){throw "Add a changelog section for $current"}
Write-Output "PASS version $current, diagnostics, changelog and tag identity"
