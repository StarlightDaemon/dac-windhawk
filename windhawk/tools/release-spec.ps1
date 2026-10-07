# Trusted requirements: archived manifests describe evidence, never choose it.
$DacBaselineChecks=@('policy','storage','catalog','media','faults','containment','host-death','emergency','lifecycle','probes','sessions','session-soak','advanced','power','slides')
$DacChecks=@($DacBaselineChecks)+@('nextbeta')
$DacLegacyCases=@('failed receipt','stale build receipt','missing receipt','edited log','changed binary','alternate-source eligibility','changed test/tool input','actual timed-out rerun invalidates previous pass')
$DacNextCases=@($DacLegacyCases)+@(
    'unknown release version','unsupported build schema','unsupported test receipt schema','unsupported tooling receipt schema',
    'missing x64 evidence','mismatched architecture pair','mixed input inventory','changed archived source','wrong standalone source name',
    'missing advanced receipt','missing power receipt','missing slides receipt','missing nextbeta receipt','shortened declared check list',
    'missing required report','duplicate archive path','unsafe archive path','malformed checksum manifest','unlisted archive entry','wrong tooling case set',
    'missing rollback receipt','unsupported rollback schema','misbound rollback evidence','changed rollback log')
$DacInputExtensions=@('.cpp','.ps1','.h','.hpp','.inl','.rc','.zip')
function Dac-CurrentVersion {
    $text=Get-Content -LiteralPath (Join-Path $PSScriptRoot '../mods/dac-windhawk.wh.cpp') -Raw
    $match=[regex]::Matches($text,'(?m)^// @version[^\S\r\n]+([^\s]+)[^\S\r\n]*\r?$')
    if($match.Count -ne 1){throw 'Exactly one source version is required'}
    $match[0].Groups[1].Value
}
function Dac-ReleaseSpec([string]$Version) {
    $spec=[ordered]@{version=$Version;layout='legacy';modId='oled-aegis';binary='oled-aegis.dll';sourceName='oled-aegis.wh.cpp';checks=@($DacBaselineChecks);cases=@($DacLegacyCases);reports=@('windhawk/docs/BETA_1_REPORT.md','windhawk/docs/BETA_1_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md')}
    switch -CaseSensitive -Exact ($Version) {
        '1.0.0-beta.1' {$spec.output='beta1';$spec.checks=@($DacBaselineChecks[0..11])}
        '1.0.0-beta.2' {$spec.output='beta2';$spec.checks=@($DacBaselineChecks[0..11])}
        '1.0.0-rc.1' {$spec.output='rc1';$spec.reports+=@('windhawk/docs/V1_RELEASE_REPORT.md')}
        '1.0.0-rc.2' {$spec.output='rc2';$spec.reports+=@('windhawk/docs/DPI_FIX_RC2.md')}
        '1.0.0-rc.3' {$spec.output='rc3';$spec.reports+=@('windhawk/docs/FUJIN_RC3.md')}
        '1.0.0-rc.4' {$spec.output='rc4';$spec.sourceName='liminal-oled-guard.wh.cpp';$spec.reports+=@('windhawk/docs/LIMINAL_RC4.md')}
        '1.1.0-beta.1' {
            $spec.output='nextbeta';$spec.layout='dual-architecture-v1';$spec.specId='nextbeta-v1'
            $spec.sourceName='liminal-oled-guard.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)
            $spec.outputs=[ordered]@{'x86'='nextbeta';'x86-64'='nextbeta-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        '1.1.0-beta.2' {
            $spec.output='nextbeta2';$spec.layout='dual-architecture-v1';$spec.specId='nextbeta2-v1'
            $spec.sourceName='liminal-oled-guard.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)
            $spec.outputs=[ordered]@{'x86'='nextbeta2';'x86-64'='nextbeta2-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/docs/QUICK_SETUP_REVISION.md','windhawk/docs/QUICK_SETUP.md','windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        '1.1.0-beta.3' {
            $spec.output='nextbeta3';$spec.layout='dual-architecture-v1';$spec.specId='nextbeta3-v1'
            $spec.sourceName='liminal-oled-guard.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)
            $spec.outputs=[ordered]@{'x86'='nextbeta3';'x86-64'='nextbeta3-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/docs/QUICK_SETUP_REFINEMENT.md','windhawk/docs/QUICK_SETUP_REVISION.md','windhawk/docs/QUICK_SETUP.md','windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        '1.1.0-beta.4' {
            $spec.output='nextbeta4';$spec.layout='dual-architecture-v1';$spec.specId='nextbeta4-v1'
            $spec.sourceName='liminal-oled-guard.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)
            $spec.outputs=[ordered]@{'x86'='nextbeta4';'x86-64'='nextbeta4-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/docs/WINDHAWK_INTEGRATION.md','windhawk/docs/QUICK_SETUP_REFINEMENT.md','windhawk/docs/QUICK_SETUP_REVISION.md','windhawk/docs/QUICK_SETUP.md','windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        '1.1.0-beta.5' {
            $spec.output='dac-beta5';$spec.layout='dual-architecture-v1';$spec.specId='dac-beta5-v1'
            $spec.modId='dac-windhawk';$spec.binary='dac-windhawk.dll';$spec.sourceName='dac-windhawk.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)+@('wrong mod identity','wrong binary name')
            $spec.outputs=[ordered]@{'x86'='dac-beta5';'x86-64'='dac-beta5-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/docs/DAC_IDENTITY.md','windhawk/docs/WINDHAWK_INTEGRATION.md','windhawk/docs/QUICK_SETUP_REFINEMENT.md','windhawk/docs/QUICK_SETUP_REVISION.md','windhawk/docs/QUICK_SETUP.md','windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        '1.1.0-beta.6' {
            $spec.output='dac-beta6';$spec.layout='dual-architecture-v1';$spec.specId='dac-beta6-v1'
            $spec.modId='dac-windhawk';$spec.binary='dac-windhawk.dll';$spec.sourceName='dac-windhawk.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)+@('wrong mod identity','wrong binary name')
            $spec.outputs=[ordered]@{'x86'='dac-beta6';'x86-64'='dac-beta6-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/docs/IDLE_DEFAULT_BETA6.md','windhawk/docs/DAC_IDENTITY.md','windhawk/docs/WINDHAWK_INTEGRATION.md','windhawk/docs/QUICK_SETUP_REFINEMENT.md','windhawk/docs/QUICK_SETUP_REVISION.md','windhawk/docs/QUICK_SETUP.md','windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        '0.1.6' {
            $spec.output='dac-0.1.6';$spec.layout='dual-architecture-v1';$spec.specId='dac-0.1.6-v1'
            $spec.modId='dac-windhawk';$spec.binary='dac-windhawk.dll';$spec.sourceName='dac-windhawk.wh.cpp';$spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)+@('wrong mod identity','wrong binary name')
            $spec.outputs=[ordered]@{'x86'='dac-0.1.6';'x86-64'='dac-0.1.6-x64'}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('windhawk/LICENSE.md','windhawk/docs/FIXTURE_PROVENANCE.md','windhawk/CHANGELOG.md','windhawk/docs/REPOSITORY_RELEASE_PLAN.md','windhawk/docs/IDLE_DEFAULT_BETA6.md','windhawk/docs/DAC_IDENTITY.md','windhawk/docs/WINDHAWK_INTEGRATION.md','windhawk/docs/QUICK_SETUP_REFINEMENT.md','windhawk/docs/QUICK_SETUP_REVISION.md','windhawk/docs/QUICK_SETUP.md','windhawk/docs/NEXT_BETA_REPORT.md','windhawk/docs/NEXT_BETA_RELEASE_NOTES.md','windhawk/LICENSE-STATUS.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
        default {
            if($Version -cnotmatch '^(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)$'){throw "Unsupported release version: $Version"}
            # A fixed evidence contract, independent of claims inside an archive.
            $spec.output="dac-$Version";$spec.layout='dual-architecture-v1';$spec.specId="dac-$Version-v2"
            $spec.modId='dac-windhawk';$spec.binary='dac-windhawk.dll';$spec.sourceName='dac-windhawk.wh.cpp'
            $spec.checks=@($DacChecks);$spec.cases=@($DacNextCases)+@('wrong mod identity','wrong binary name')
            $spec.outputs=[ordered]@{'x86'="dac-$Version";'x86-64'="dac-$Version-x64"}
            $spec.targets=[ordered]@{'x86'='i686-w64-mingw32';'x86-64'='x86_64-w64-mingw32'}
            $spec.reports=@('README.md','LICENSE','CONTRIBUTING.md','SECURITY.md','windhawk/CHANGELOG.md','windhawk/docs/RELEASING.md','windhawk/docs/ADVERSARIAL_REVIEW.md','windhawk/docs/FIXTURE_PROVENANCE.md','windhawk/docs/PROVENANCE.md','windhawk/docs/THIRD_PARTY_NOTICES.md')
        }
    }
    $spec.source='windhawk/mods/'+$spec.sourceName
    $prefix=if($spec.modId -ceq 'dac-windhawk'){'dac-windhawk'}elseif($Version -cin @('1.1.0-beta.1','1.1.0-beta.2','1.1.0-beta.3','1.1.0-beta.4')){'monitor-screensaver-activity-control'}elseif($Version -ceq '1.0.0-rc.4'){'liminal-oled-guard'}else{'oled-aegis'}
    $spec.archive="$prefix-$Version-source.zip"
    return [pscustomobject]$spec
}
function Dac-OutputSpec([string]$OutputName) {
    if($OutputName -cmatch '^dac-((?:0|[1-9][0-9]*)\.(?:0|[1-9][0-9]*)\.(?:0|[1-9][0-9]*))(?:-x64)?$'){return Dac-ReleaseSpec $Matches[1]}
    $version=switch -CaseSensitive -Exact ($OutputName) {
        'beta1' {'1.0.0-beta.1'} 'beta2' {'1.0.0-beta.2'} 'beta2-x64' {'1.0.0-beta.2'}
        'rc1' {'1.0.0-rc.1'} 'rc1-x64' {'1.0.0-rc.1'} 'rc2' {'1.0.0-rc.2'} 'rc2-x64' {'1.0.0-rc.2'}
        'rc3' {'1.0.0-rc.3'} 'rc3-x64' {'1.0.0-rc.3'} 'rc4' {'1.0.0-rc.4'} 'rc4-x64' {'1.0.0-rc.4'}
        'nextbeta' {'1.1.0-beta.1'} 'nextbeta-x64' {'1.1.0-beta.1'} 'nextbeta2' {'1.1.0-beta.2'} 'nextbeta2-x64' {'1.1.0-beta.2'} 'nextbeta3' {'1.1.0-beta.3'} 'nextbeta3-x64' {'1.1.0-beta.3'} 'nextbeta4' {'1.1.0-beta.4'} 'nextbeta4-x64' {'1.1.0-beta.4'} 'dac-beta5' {'1.1.0-beta.5'} 'dac-beta5-x64' {'1.1.0-beta.5'} 'dac-beta6' {'1.1.0-beta.6'} 'dac-beta6-x64' {'1.1.0-beta.6'} 'dac-0.1.6' {'0.1.6'} 'dac-0.1.6-x64' {'0.1.6'} default {throw "Unsupported release output: $OutputName"}
    }
    Dac-ReleaseSpec $version
}
function Dac-Schema($Value,[int]$Expected,[string]$Label) {
    if(($Value -isnot [int] -and $Value -isnot [long]) -or $Value -ne $Expected){throw "Unsupported $Label schema"}
}
function Dac-AssertHash($Value,[string]$Label) {
    if($Value -isnot [string] -or $Value -cnotmatch '^[0-9a-f]{64}$'){throw "Invalid SHA-256: $Label"}
}
function Dac-AssertPath([string]$Name) {
    if(!$Name -or $Name -match '(^/|\\|:|[\x00-\x1f\x7f])'){throw "Unsafe archive/input path: $Name"}
    foreach($segment in $Name.Split('/')) {
        if(!$segment -or $segment -in @('.','..') -or $segment -match '[. ]$'){throw "Unsafe archive/input path: $Name"}
    }
}
function Dac-AssertNames($Actual,$Expected,[string]$Label) {
    $set=[Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
    foreach($name in @($Actual)) {
        if($name -isnot [string] -or !$name -or !$set.Add($name)){throw "Invalid or duplicate $Label"}
    }
    if($set.Count -ne @($Expected).Count){throw "Incorrect $Label set"}
    foreach($name in @($Expected)){if(!$set.Contains($name)){throw "Missing $Label`: $name"}}
}
function Dac-CheckBinary([string]$Check) {
    if($Check -cnotin $DacChecks){throw "Unknown check: $Check"}
    if($Check -ceq 'policy'){'policy-tests.exe'}else{'platform-tests.exe'}
}
