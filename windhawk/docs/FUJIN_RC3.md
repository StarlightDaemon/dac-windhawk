# RC3 — Fujin native theme and OLED Aegis icon

Date: 2026-10-05. Version: 1.0.0-rc.3.

The operator confirmed RC2 compiles in their editor, then requested Fujin
integration, a suitable icon and cleanup before the next testing round.

## Integration

Fujin is consumed from the local repository's tagged generated outputs:
`v0.1.0`, commit `c653620262ef68fa8d58504b6c47bb21aadc5aa5`.
`tools/sync-fujin.ps1` reads `dist/tokens-resolved.json` and `dist/tokens.css`
with `git show` at that tag; it ignores working-tree changes and rejects a
moved tag. It generates the marked native palette/font block in the complete
mod, including Fujin's MIT notice. Every build checks that block against the
pinned outputs. Values are not manually maintained copies. The shipped mod
needs neither Fujin's checkout nor JavaScript at runtime.

Maintainers need a Fujin checkout beside this repository, or can pass
`-FujinRoot` to the sync script. Clone the reference repository and fetch
`v0.1.0` if absent. Do not retag an existing ref to satisfy the check.
Windhawk users only need the complete `.wh.cpp` source.

The adapter applies Fujin's violet accent, light/dark surfaces, semantic text
colors, sharp corners and small Verdana type to both native settings windows.
Windows app-theme preference selects light/dark; high contrast overrides it
with system colors, system font and native button/checkbox painting. OS theme
broadcasts update open windows without replacing controls or resetting drafts.
Unsupported DWM caption/corner attributes leave native OS chrome in place.
File/color dialogs, tray menus and scrollbars remain Windows-managed.
Existing Win32 layout coordinates are retained; this is a native token adapter,
not a port of Fujin's React component package or every CSS layout behavior.

Original vector geometry draws a monitor and shield for tray/window icons.
The active-presentation variant adds a check mark. Icons are rasterized at
system small/large icon sizes; Windows handles shell scaling. This is a
protection-state indicator, not a guarantee of hardware-off state. Brushes,
fonts and icon handles are released with their UI owner; no stock or inherited
artwork is bundled. Standard Win32 subclassing changes painting while retaining
native control classes, check states, selection and keyboard behavior. No
common-controls v6 activation manifest or new runtime is required.

## Verification and limits

The advanced group exercises light/dark/high-contrast selection, native edit
color messages, checkbox toggle, combo choices, icon creation, retained drafts,
repeated DPI transitions, direct invalidation checks on each replaced UI
font, and resource stability after native cache warmup.
The lifecycle group repeatedly opens/closes the real settings windows.
Offscreen captures use the real painting paths, not an HTML reconstruction.
Some native edit controls omit pixels when hidden, so these captures are partial
paint evidence and are not presented as fully qualified desktop screenshots.
Physical mixed-DPI dragging, shell icon appearance, accessibility-reader use,
actual contrast-mode switching and monitor hardware remain desktop qualification.
No installed mod, Windows appearance preference or monitor power was changed.

## References and consumer registration

- [Fujin](https://github.com/StarlightDaemon/Fujin), MIT, copyright 2026 StarlightDaemon.
- [DWM attributes](https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute).
- [WM_PRINTCLIENT](https://learn.microsoft.com/en-us/windows/win32/gdi/wm-printclient).

The Fujin repository was read only. Its consumer-ledger addition is prepared
below for the Fujin maintainer; this project does not cross into another
RAIDEN Instance's write role:

`| oled_aegis (Windhawk edition) | dist/tokens-resolved.json + dist/tokens.css → generated standalone C++ adapter | v0.1.0 | 2026-10-05 | tools/sync-fujin.ps1 verifies tag commit and generated block on every build. |`

## Apply

Replace the entire existing Windhawk editor buffer with the complete RC3 source,
then compile/enable. There is exactly one metadata block/version and separate
`x86` / `x86-64` architecture lines. Settings schema and monitor assignments are
unchanged from RC2. Preserve customized source before replacement. The prior
RC2 package remains available for rollback.

## Final build evidence

Windhawk 1.7.3 / clang 20.1.3: x86 and x64 warning-clean builds.
All fifteen x86 release groups passed, including 116,056 policy assertions;
x64 advanced UI and lifecycle groups passed. The eight release-evidence
rejection cases passed, restoring the original passing receipts byte for byte.
Both 50-cycle settings lifecycle runs retained 10 GDI objects after warmup.
The 100-cycle x86 presentation soak retained 214 handles; this is a hidden
fixture soak, not GPU or monitor endurance.

Complete source SHA-256:
`89dbed1d5ed2c8b269955fd216efd11a0670536d15892296f48f8e31c3454126`.
x86 DLL SHA-256:
`9df14ec7bee34b484d12d3bcbb9aa1976ac30389566c0e10a8c3f4d879a0a01a`.
x64 DLL SHA-256:
`7a084e5f13e14bc1a32c8c2c69c0a76cbfeab34226261fe73c1f04c8dcc87f19`.
Immutable build copies and receipts: `build/windhawk/rc3` and `rc3-x64`.
Verified source package: `build/windhawk/rc3/oled-aegis-1.0.0-rc.3-source.zip`.
The earlier DPI report describes historical RC2; these identities describe RC3.
