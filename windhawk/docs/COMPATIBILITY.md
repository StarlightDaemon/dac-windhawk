# Compatibility observations

**No stock saver is qualified as supported yet.** Installed copies passed the limited process/window probe below; visible output, full bounds, interactive behavior and simultaneous actual displays remain unverified. The plan (historical internal record; not bundled) requires those observations before a support promise. Details and exact commands are in [PHASE_1_REPORT](PHASE_1_REPORT.md).

## Environment measured on 2026-10-04

Current RC1 evidence is in [V1_RELEASE_REPORT.md](V1_RELEASE_REPORT.md), dated
2026-10-05. Both architectures pass all fifteen checks; all six native stock
savers again pass hidden structural probes. Custom .scr compatibility remains
preview-dependent and unqualified. Native photo frames and concurrent fixture
sessions pass automated checks. DDC soft-off/wake is opt-in and has no qualified
hardware entries. No support label below is promoted by these observations.
The following measurements are historical.

Beta 1 update: all six native x64 savers produced responsive owned children and
cleaned up under the strengthened worker health check in hidden hosts. Bubbles
907 ms, Mystify 890 ms, Ribbons 922 ms, 3D Text 891 ms, Photos 219 ms and Blank
156 ms were measured through cleanup. This remains structural/responsiveness
evidence. The latest catalog run found one identified 2560×1440 output, unlike
the earlier candidate observation below. Simultaneous sessions were exercised
with injected displays and contained fixture children, not actual stock savers
on multiple physical displays. All support labels remain unqualified. See the
[beta report](BETA_1_REPORT.md) for current source/build/evidence identities.

Production candidate update: all six native x64 files below were rechecked
through the production worker with hidden 640×360 test parents. Each produced
an owned child and cleaned its job; elapsed launch-through-cleanup samples were
Bubbles 907 ms, Mystify/Ribbons 891 ms, 3D Text 906 ms, Photos/Blank 156 ms.
These remain **structural probes / visually unqualified**, not supported or
limited-support labels. File hashes were re-read and match the historical
native identities below. Production only selects native System32 savers;
historical x86 evidence is not a production x86 support claim. The current
catalog at that earlier run exposed one output without a usable stable target path; production
correctly disables it rather than persisting an index. See
[production report](PRODUCTION_REPORT.md) for source/test identities and limits.

| Item | Observation |
| --- | --- |
| OS registry identity | `ProductName=Windows 10 IoT Enterprise`, `DisplayVersion=26H2`, `CurrentBuild=26300`, `UBR=9457`; these are literal registry values, not inferred from saver version |
| Process/target architecture | x64 prototype/harness; PE target AMD64 (`8664`) |
| Windhawk | Installed stable 1.7.3; actual mod loading unverified |
| Compiler | clang 20.1.3, LLVM revision `923a5c4f83d2b3675bb88e9fe441daeaa4d69488`; explicit `x86_64-w64-mingw32` target (compiler's unqualified default is i686) |
| Shell-test display catalog | One enumerated output `\\.\DISPLAY113`, bounds `[0,0,2560,1440]`, effective DPI 96 |
| Desktop limitation | Tray add returned false with reported last-error `2147500037` (`0x80004005`). A missing usable shell is a plausible explanation, not a proven cause. The tool-accessible catalog does not establish the operator's physical topology |
| Preview bounds | 640×360, PMv2 UI thread; single catalog output; no actual multiple displays, mixed DPI or negative coordinates tested |
| Targets not qualified | Other Windows builds, Windows 10 marketed releases, ARM64, other Windhawk versions, physical mixed-DPI/topology changes |

The saver executables all report file version `10.0.26100.1 (WinBuild.160101.0800)`. This is different from the measured OS build and does not override it. Windows-provided files only; no downloads, replacements, redistribution, serial numbers, EDID contents, window titles or photo contents were collected into this record.

## Installed candidate outcomes

Each installed file below launched with a fully qualified `CreateProcessW` application name and `/p <decimal HWND>`, was assigned to a kill-on-close job before its initial thread resumed, had a direct child window beneath the host, remained alive at the six-second sample, and exited after normal harness shutdown. Foreground HWND equality was true at the sample; transient focus changes were not measured. These are **partial structural passes, visual compatibility unverified**, not “limited support” or “supported.”

| Saver | Native x64 result / raw evidence | x86 result / raw evidence | Visual qualification still needed |
| --- | --- | --- | --- |
| Bubbles | Process/window probe (historical local record: `../../build/windhawk/saver-x64-0.log`; not bundled) | Unavailable | Background/transparency, full-size animation, focus/dismissal |
| Mystify | Process/window probe (historical local record: `../../build/windhawk/saver-x64-1.log`; not bundled) | Unavailable | Animation/scaling, different/same-saver concurrency |
| Ribbons | Process/window probe (historical local record: `../../build/windhawk/saver-x64-2.log`; not bundled) | Unavailable | Animation/scaling, repeated cycles, GPU cost |
| 3D Text | Process/window probe (historical local record: `../../build/windhawk/saver-x64-3.log`; not bundled) | Unavailable | Configuration, per-instance/global preference behavior, graphics lifecycle |
| Photos | Process/window probe (historical local record: `../../build/windhawk/saver-x64-4.log`; not bundled) | Process/window probe (historical local record: `../../build/windhawk/saver-x86-4.log`; not bundled) | Visible photos/errors, valid/empty/missing source, child trees, settings; no photo contents inspected |
| Blank | Process/window probe (historical local record: `../../build/windhawk/saver-x64-5.log`; not bundled) | Process/window probe (historical local record: `../../build/windhawk/saver-x86-5.log`; not bundled) | Visible blank output, full bounds and input control case |

Raw logs are ignored local evidence; this table preserves their qualified conclusions for collaborators. `System32` contains native x64 copies, `SysWOW64` contains the listed x86 copies. File identity table follows.

## Exact saver identities

| Windows directory / file | PE machine | SHA-256 |
| --- | --- | --- |
| System32/Bubbles.scr | 8664 | `4D85FAA1F28E0446494982D6993EE21BE4D336E51E96590E095E74CACEBBE04D` |
| System32/Mystify.scr | 8664 | `8959178E7BD161D5466596AAFE89006FAEB8F8018AC91FBF6BB2DD1F9A1F5340` |
| System32/Ribbons.scr | 8664 | `48A7C7BF50F44620DD34742F0C8DE68A0B76A5BFE89CBC15C869A1FECC65FE3E` |
| System32/ssText3d.scr | 8664 | `679D3C92BDAD5242B6FBA1CA7927D8D4E4274A8B93213DB2DC70B0EF25314073` |
| System32/PhotoScreensaver.scr | 8664 | `6781ED3BB31BBDFFCAE7CA4F8979050EDA2BD72A076A57692EAE586579356FDC` |
| System32/scrnsave.scr | 8664 | `497D7F1A140EB879EE9F9236E9F1413FBB8C511BFEF890FAAB37A47495FA9E14` |
| SysWOW64/PhotoScreensaver.scr | 014C | `FEDC17582B18309F93E2837E9258CB7CCC4EB310FABF8F7828282DF714638D3C` |
| SysWOW64/scrnsave.scr | 014C | `3EA91FE824700F3D0ECDB416FE37D3C1AFBE57945177414F683E6C8012DED4B7` |
