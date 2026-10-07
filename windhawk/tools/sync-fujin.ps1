param([string]$FujinRoot='',[switch]$Check)
$ErrorActionPreference='Stop'
# Consume generated outputs at the pinned tag, never the working tree or branch.
$tag='v0.1.0'
$commit='c653620262ef68fa8d58504b6c47bb21aadc5aa5'
$snapshot=Join-Path $PSScriptRoot '../theme/fujin-v0.1.0.zip'
if($FujinRoot){
    $actual=git -C $FujinRoot rev-parse "$tag^{commit}"
    if($LASTEXITCODE -ne 0 -or $actual -ne $commit){throw 'Fujin v0.1.0 tag is missing or differs from the pinned commit'}
}else{
    if(!(Test-Path -LiteralPath $snapshot) -or (Get-FileHash -LiteralPath $snapshot -Algorithm SHA256).Hash.ToLowerInvariant() -cne '021af468de3baff69c6946794eba1e7f11dec63b62e0ac049515b8ed69987456'){throw 'Bundled Fujin source snapshot is missing or changed'}
    Add-Type -AssemblyName System.IO.Compression.FileSystem
}
function Tagged([string]$path){
    if($FujinRoot){$lines=git -C $FujinRoot show "${tag}:$path";if($LASTEXITCODE -ne 0){throw "Missing Fujin output: $path"};return ($lines -join "`n")}
    $zip=[IO.Compression.ZipFile]::OpenRead($snapshot)
    try{
        $entry=$zip.GetEntry($path);if(!$entry){throw "Missing bundled Fujin output: $path"}
        $reader=[IO.StreamReader]::new($entry.Open())
        try{return $reader.ReadToEnd().Replace("`r`n","`n").TrimEnd("`n")}finally{$reader.Dispose()}
    }finally{$zip.Dispose()}
}
$resolved=(Tagged 'dist/tokens-resolved.json')|ConvertFrom-Json
$css=Tagged 'dist/tokens.css'
$license=Tagged 'LICENSE'
$roles=@('bg-base','bg-surface','bg-elevated','text-primary','text-secondary','text-on-accent','border-default','border-strong','interactive-default','interactive-hover','interactive-active','chrome-bg','chrome-text')
$names=@('base','surface','elevated','text','secondary','onAccent','border','strong','accent','hover','active','chrome','chromeText')
$lines=@('// BEGIN GENERATED FUJIN',"// Fujin $tag, commit $commit; regenerate with tools/sync-fujin.ps1.","/*`n$license`n*/",'namespace fujin {',('struct Palette { COLORREF '+($names -join ',')+'; };'))
foreach($mode in @('dark','light')){
    $colors=foreach($role in $roles){$hex=$resolved.$mode."--fujin-$role";if($hex -notmatch '^#[0-9a-fA-F]{6}$'){throw "Invalid color: $role"};'RGB(0x'+$hex.Substring(1,2)+',0x'+$hex.Substring(3,2)+',0x'+$hex.Substring(5,2)+')'}
    $lines+='constexpr Palette '+$mode+'={'+($colors -join ',')+'};'
}
foreach($entry in @(@('fontSize','font-size-sm'),@('spacing','spacing-md'),@('radius','radius-default'))){
    $match=[regex]::Match($css,'--fujin-'+$entry[1]+':\s*(\d+)px;');if(!$match.Success){throw 'Missing scalar token'}
    $lines+='constexpr int '+$entry[0]+'='+$match.Groups[1].Value+';'
}
$family=($resolved.tokens.fontFamily -split ',')[0].Trim().Trim('"');if($family -notmatch '^[a-zA-Z ]+$'){throw 'Invalid native font family'}
$lines+='constexpr wchar_t fontFamily[]=L"'+$family+'";'
$lines+='} // namespace fujin';$lines+='// END GENERATED FUJIN'
$generated=$lines -join "`n"
$source=Join-Path $PSScriptRoot '../mods/dac-windhawk.wh.cpp'
$text=[IO.File]::ReadAllText($source).Replace("`r`n","`n")
$pattern='(?s)// BEGIN GENERATED FUJIN.*?// END GENERATED FUJIN'
$match=[regex]::Match($text,$pattern);if(!$match.Success){throw 'Missing Fujin generation boundary'}
if($Check){if($match.Value -cne $generated){throw 'Embedded Fujin adapter is stale; run sync-fujin.ps1'};Write-Output "PASS Fujin $tag generated adapter matches pinned tag"}
else{$text=$text.Substring(0,$match.Index)+$generated+$text.Substring($match.Index+$match.Length);[IO.File]::WriteAllText($source,$text,[Text.UTF8Encoding]::new($false))}
