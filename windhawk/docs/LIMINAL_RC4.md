# Liminal OLED Guard — 1.0.0-rc.4

Local testing candidate, 2026-10-05. This is the renamed Windhawk edition,
with the complete RC3 feature set and Fujin theme. The retained standalone
application at the repository root is a separate, unchanged historical product.

## What changed

- Product name: **Liminal OLED Guard**; compact tray status: **Liminal**.
- Descriptor: **Activity-aware OLED protection and per-monitor screensavers.**
- Updated mod metadata, settings/options titles, notifications, controller and
  presentation window titles, source filename, current guide and package name.
- Removed the stale optional inspiration paragraph from the embedded mod
  readme, aligning it with the existing source-assessment disposition. Fujin's
  embedded MIT notice, dependency notices and factual provenance remain.
- Preserved the monitor/shield icon, native theme behavior and mixed-DPI fix.

The purpose is to reduce prolonged static-image exposure. No burn-in-prevention
guarantee, screen-lock behavior or universal monitor/saver support is implied.

## Upgrade the existing local mod

1. Save a copy of the existing Windhawk editor source and settings file.
2. Stop active protection from the tray; wake any monitor powered off through
   hardware control before updating.
3. In Windhawk, edit the **existing** OLED Aegis local mod. Replace the entire
   editor contents with `windhawk/mods/liminal-oled-guard.wh.cpp`, then compile.
   Do not append it to the old source or create a second enabled mod.
4. Enable it if needed. Its displayed name becomes **Liminal OLED Guard**.
   Look for the violet monitor/shield icon in the tray, including hidden icons.
5. Open Settings and confirm your monitor assignments. Existing automatic
   activation preferences are retained; a fresh configuration defaults off.

The `.wh.cpp` file is complete and self-contained. It compiles in Windhawk's
local editor without the repository or a separate Fujin checkout. Architecture
metadata has separate `x86` and `x86-64` entries. Local verification DLLs are
build outputs, not standalone applications or an installer.

## Compatibility and rollback

| Surface | RC4 treatment |
| --- | --- |
| Windhawk update/host identity | Retain `oled-aegis` and installed `local@oled-aegis` |
| Preferences | Same `%LOCALAPPDATA%\OLED_Aegis_Windhawk\settings-v1.ini`, schema 2 |
| Hardware recovery | Same ticket, fault-marker, cancellation and DDC lock identities |
| Process coordination | Same singleton, emergency event and window classes |
| Legacy import/startup | Retain original INI handling and OLED Aegis Run-entry detection |
| Source/package filenames | `liminal-oled-guard.wh.cpp` and `liminal-oled-guard-1.0.0-rc.4-source.zip` |

No settings migration is needed from RC3. To roll back, stop/wake active
protection, restore the saved RC3 editor source and compile in the same local
mod. RC3 can read the same schema 2 settings. For older beta rollback, follow
the separate schema-1 instructions in the main README. Do not reset a hardware
fault until the monitor has physically woken.

## Focused desktop testing

These are the next qualification cases, not assertions of completed testing.

| Test | Expected behavior |
| --- | --- |
| Upgrade and restart | One controller/tray icon; existing preferences and monitor assignments retained |
| Per-monitor black | An idle/selected monitor blanks; activity dismisses it according to its configured input scope |
| Concurrent savers/photos | Each monitor uses its selected presentation; stopping one leaves others running |
| Settings across monitors | Both settings windows survive mixed-DPI dragging, retaining unsaved edits and scroll position |
| Light/dark/high contrast | Readable native controls and icon; settings drafts survive theme changes |
| Media/fullscreen | Configured activity inhibition applies to the intended monitor |
| Sticky mode and emergency | Sticky presentation persists until explicitly stopped; Ctrl+Alt+Shift+F12 exits the host |
| Disable/re-enable | Owned presentations and children close; re-enable restores the single controller |
| Hardware off, only after opt-in | Selected monitor powers off and can wake; failures retain recovery/fault state |

Hardware power is optional and depends on the monitor/connection. Begin with
black blanking and savers; test hardware off only with its physical power button
accessible. Fake-DDC tests do not establish physical wake compatibility.

## Reproduction and evidence

From the repository root:

```powershell
./windhawk/tools/produce-release.ps1
./windhawk/tools/build-production.ps1 -OutputName rc4-x64 -Architecture x86-64
./windhawk/tools/test-production.ps1 -OutputName rc4-x64 -Checks policy,storage,catalog,media,faults,containment,host-death,emergency,lifecycle,probes,sessions,session-soak,advanced,power,slides -TimeoutSeconds 55
./windhawk/tools/verify-beta.ps1
```

Builds use installed Windhawk 1.7.3 / clang 20.1.3 and the pinned Fujin v0.1.0
generated tokens. The host build defaults to x86 on the current installation.
Output directories are `build/windhawk/rc4` and `build/windhawk/rc4-x64`.
Source ZIPs include checksums, build identity, tests and evidence; compiler
runtime binaries, stock savers and inherited standalone source are excluded.

Validation completed: x86 and x86-64 compile with warnings treated as errors;
all 15 automated groups pass on both architectures, including 116,056 policy
assertions, storage, lifecycle, hidden concurrent presentations, 100 presentation
cycles, mixed-DPI/draft retention, theme painting, slideshows and simulated DDC.
All eight x86 evidence-rejection cases pass. The source archive passes manifest,
input-identity and test-receipt verification. Metadata has one version entry and
separate architecture lines. A comparison against RC3 confirms only eight visible
wide-string changes; coordination, settings and hardware-recovery literals match.

Exact source SHA-256:
`ed923dfd5704e27fb10f897ccf02653d18ebe4980092a50cd5fc0a22c14d7dd4`.
Full per-architecture receipts are in each output directory; the source ZIP binds
its x86 build/test evidence to the packaged source. Source filename changes and
branding do not alter protection policy. No installed-mod update, public release,
physical monitor power operation or Windows preference change was performed.
Desktop/hardware qualification remains tracked in LOOP-016.

## Naming decision

The operator selected Liminal, accepted the Liminal OLED Guard descriptor and
authorized the rename and test build. On 2026-10-05, exact-name web search did
not surface a result for “Liminal OLED Guard”. An adjacent screensaver project
already uses [Liminal Screen](https://github.com/ScreenSaverGallery/liminal-screen),
so the full compound name is used for product identification. This limited
screen is not public name clearance; no domain or mark was reserved. Public
release and original-source licensing remain separate from this local build.

The subsequent [Liminal Screen comparison](LIMINAL_SCREEN_COMPARISON.md) finds
meaningful feature overlap and material potential for name confusion. The local
name selection stands; the base name warrants reconsideration before publication.
