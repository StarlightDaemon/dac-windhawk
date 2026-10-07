# Provenance and reuse register

Current edition: **Display Activity Controls for Windhawk**, version **0.1.6**,
source `windhawk/mods/dac-windhawk.wh.cpp`. Original DAC source is MIT licensed
under [LICENSE.md](../LICENSE.md). The Windhawk-only repository preserves the
implementation history below; the inherited standalone application and assets
are not part of its export.

Pinned build fixtures now ship with the source; see
[fixture provenance](FIXTURE_PROVENANCE.md). Older naming/licensing statements
below describe their original checkpoints and are superseded for current use.

## Current attribution assessment — 2026-10-05

[SOURCE_COMPARISON.md](SOURCE_COMPARISON.md) records the hashed RC3/baseline
comparison, exact and structural measurements, manual review and attribution
decision. Current Windhawk notices retain actual dependency terms and omit
optional inspiration credits. This register preserves factual source lineage
and research citations; it is not a product credit roll. Earlier no-copy
statements below describe implementation records, not proof of clean-room
development or a legal conclusion. The retained standalone source remains
unchanged upstream work. [NAMING_RESEARCH.md](NAMING_RESEARCH.md) records the
first naming probe; the current selection is recorded above.

## RC3 Fujin integration

Fujin generated design tokens (MIT, StarlightDaemon, 2026) are consumed at
tag v0.1.0, commit c653620262ef68fa8d58504b6c47bb21aadc5aa5. The generator
and build drift check are tools/sync-fujin.ps1. Full license travels inside
the standalone source and THIRD_PARTY_NOTICES.md. Native painting adapter
and monitor/shield icon geometry are original. See [RC3 report](FUJIN_RC3.md).

## Beta 1 continuation

Beta refinements, fixture tests and build/evidence/package/diagnostic tools are
original work in this checkout. Primary API references and exact current
artifact identities are in [BETA_1_REPORT.md](BETA_1_REPORT.md). Existing
attribution and dependency terms remain unchanged. The internal beta ZIP adds
hash-bound build/test evidence; it does not bundle savers, DLLs, compiler
runtimes or inherited assets. Original-source licensing and public naming
remain separate publication decisions.

## Production continuation under D-0007

Production source, tests and tools are original work in this checkout,
informed by the same behavioral references. Selective adaptation from the
local original prototype includes suspended launch/job containment,
nonactivating host-window flags, raw-input registration, tray/shell messages,
and the independently expressed stable 1.7.3 dedicated-host adapter. Baseline
prototype source SHA-256:
`0E0B6AF05DF8594F1F8F7E67EE21085A68055729DB219F766530312BF959E1D8`.
Destination: `windhawk/mods/oled-aegis.wh.cpp`. The prototype source itself is
unchanged. Production replaces its indexed sessions, launch lock, temporary
configuration and absent idle/media policy. Tests and differences are recorded
in [PRODUCTION_REPORT.md](PRODUCTION_REPORT.md).

No inherited standalone implementation, artwork or product prose was copied.
No commercial/example source snippet was incorporated. The small `MeterPeak`
ABI view independently declares the documented first IAudioMeterInformation
method after IUnknown and its IID because the installed MinGW header has only
a forward declaration. Method order/IID were checked against Microsoft's
[SDK metadata header](https://github.com/microsoft/win32metadata/blob/main/generation/WinSDK/RecompiledIdlHeaders/um/endpointvolume.h)
and [API reference](https://learn.microsoft.com/en-us/windows/win32/api/endpointvolume/nn-endpointvolume-iaudiometerinformation);
no generated SDK header is vendored. Other production API semantics were
checked against Microsoft's QueryDisplayConfig, GetProcessId,
CallNtPowerInformation and WTSINFOEX documentation. No third-party package was
downloaded or new CI dependency introduced.

The installed compiler/API/runtime inputs below remain unchanged. Production
also links installed DWM, common-dialog, registry and power import libraries;
these are OS/toolchain interfaces, not bundled implementations. Exact output
identities are in PRODUCTION_REPORT. The source ZIP excludes runtime binaries
and savers. [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) and
[LICENSE-STATUS.md](../LICENSE-STATUS.md) accurately retain public licensing
and naming as unselected operator publication decisions; no unknown rights
were assigned a license label. The following historical inventory is retained.

## Historical experiment inventory

The new experiment is original implementation informed by reviewed source and documentation, **not a clean-room reconstruction**. No existing OLED Aegis source, icons, screenshot, product prose, DisplayFusion/Actual Tools code or assets, or Monitor Sleep Button snippets were copied/adapted into the new implementation. The generic Windows application icon is loaded from the OS, not packaged. No license for the original new source is selected in this feasibility assignment; operator naming/license and dependency-notice review precede publication.

## Behavioral and technical references (not incorporated source)

| Reference | Revision / scope | Historical relationship / technical evidence |
| --- | --- | --- |
| [OLED Aegis](https://github.com/spenserlee/oled_aegis) | Local HEAD `9581aa1ea9e297c1c8e0b1f113c84ab215611ce1`, README and `src/oled_aegis.c`; inherited product baseline `edaef74233b75708ea4354e3e15793f195567e34` | spenserlee and contributors; behavioral reference. Missing source/asset grant remains LOOP-014; no reuse based on assumed permission |
| [DisplayFusion screensaver development](https://www.displayfusion.com/HelpGuide/ScreenSaverDevelopment/) | Technical reference linked by accepted plan | Documentation consulted for preview-host behavior; no incorporated code/assets or product credit |
| [Actual Multiple Monitors](https://www.actualtools.com/multiplemonitors/), [compatibility guidance](https://www.actualtools.com/windowmanager/help/features/multi_monitor_screen_saver.php) | Technical references linked by accepted plan | Documentation consulted for compatibility behavior; no incorporated code/assets or product credit |
| [Windhawk tool-mod documentation](https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process) | Retrieved 2026-10-04; stable 1.7.3 mechanism, 2.0 alpha distinguished | RAM Software / Michael Maltsev. Adapter is independently expressed using documented dedicated-host flag, entry-point hook and process launch mechanism; wiki wrapper was read but not pasted |
| [Windhawk mod API](https://github.com/ramensoftware/windhawk/wiki/Creating-a-new-mod) | Retrieved 2026-10-04 | Lifecycle/settings packaging reference |
| [Monitor Sleep Button](https://github.com/ramensoftware/windhawk-mods/blob/main/mods/monitor-sleep-button.wh.cpp) | Plan's reference, no source selected for incorporation | SilverAmd; feasibility precedent only. No claim that its MIT notice grants rights to other work |
| [Microsoft job objects](https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects), [raw input registration](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerrawinputdevices) | Retrieved 2026-10-04 | API semantics; original calls, no sample source pasted |

## Actual build/runtime dependencies

Installed-file SHA-256 identities are recorded below and in ignored `build/windhawk/dependency-identities.json`; those bytes, not an assumed latest checkout, define the local inputs. The engine API upstream reference is tag [`v1.7.3`, `src/windhawk/engine/mods_api.h`](https://github.com/ramensoftware/windhawk/blob/v1.7.3/src/windhawk/engine/mods_api.h). The installation renames that header to `windhawk_api.h`. No API header/import library is vendored into tracked source.

| Item | Source / author / terms | Destination / modifications |
| --- | --- | --- |
| `windhawk_api.h`, `windhawk_api_internal.h`, `Engine/1.7.3/64/windhawk.lib` | Installed Windhawk 1.7.3; Michael Maltsev / RAM Software. Project [license](https://github.com/ramensoftware/windhawk/blob/v1.7.3/LICENSE) is GPL v3 or later; no separate API licensing exception is asserted here | Force-included and linked into local DLL; unchanged dependency inputs. Distribution terms need reconciliation with the original-code license before release |
| clang/libc++/libunwind | Installed LLVM 20.1.3, compiler revision `923a5c4f83d2b3675bb88e9fe441daeaa4d69488`; LLVM contributors; installed `Compiler/LICENSE.TXT` says Apache 2.0 with LLVM exceptions | Compiler used unchanged; libc++.dll/libunwind.dll copied byte-for-byte to ignored `build/windhawk/libc++.whl` and `libunwind.whl` for the harness loader; filenames only changed |
| MinGW-w64 headers/runtime/import libraries | Installed Windhawk compiler bundle; mingw-w64 project and individual contributors; installed `x86_64-w64-mingw32/share/mingw32/COPYING.MinGW-w64-runtime.txt` contains component notices (including overall 2009–2013 project copyright) | Toolchain inclusion/linkage only; no header source vendored. Preserve applicable component notices in any later binary package |
| Windows stock `.scr`, Win32/COM DLLs and stock icon | Microsoft installed OS components, Windows terms; exact saver hashes in COMPATIBILITY | Executed/loaded from existing Windows installation; not copied, modified, downloaded or bundled |

This local feasibility output is not a release package or license adjudication. Unknown inherited rights block that inherited reuse; they do not block this original experiment. Dependency notices, exact upstream source resolution for the installed bundle, original-source licensing, and public attribution remain release work rather than fabricated permission.

## Installed dependency identities

RC1 adds original calls to Windows GDI+ for photo rendering and Dxva2 physical
monitor/DDC APIs for opt-in power control. Those are installed Windows components,
not bundled dependencies. Actual Tools and Microsoft references and the Windhawk
1.7.3 helper-loading analysis are linked in the
[current release report](V1_RELEASE_REPORT.md). No reference implementation was
copied. Current x86/x64 source/toolchain identities are in each RC1 build receipt;
the older inventory below retains its historical meaning.

| Installed path relative to Windhawk root | SHA-256 |
| --- | --- |
| Compiler/include/windhawk_api.h | `220C51A303408826114F512D9C889B2E655676AAFF67315809F54FC32C35CBD0` |
| Compiler/include/windhawk_api_internal.h | `3BF72BA5E844A1EDF297CF14B7E949FBCB8EE5F00C5737F93DD22DDA8C905B27` |
| Engine/1.7.3/64/windhawk.lib | `18EE735994961A1C8C33B51459F614D87A256A2CF630A34577502C5C8699C4D3` |
| Compiler/x86_64-w64-mingw32/bin/libc++.dll | `74C541F76732D738642072E90F26343E44D66D97BB258DB76F2F3BEE7E30060C` |
| Compiler/x86_64-w64-mingw32/bin/libunwind.dll | `6E6D1E74D17EE361B3F3661FAA3D07FA7597257DF1CD3DD360719222E54AA36C` |

## Next local beta 1.1.0-beta.1

The next-beta source adds original policy, configuration, native UI, dim layering,
XInput observation and native clock/sparse-scene rendering. Research concepts are
mapped in FEATURE_OPPORTUNITIES.md; no competitor source, art, animation assets or
runtime was copied. XInput is dynamically resolved from installed Windows system
DLLs; no SDK redistributable or browser runtime is bundled. Existing Fujin pin,
original icon lineage and dependency notices remain unchanged. Exact per-target
identities and validation limits are in NEXT_BETA_REPORT.md. This work preserves
the public-distribution licensing limitations in LICENSE-STATUS.md.
