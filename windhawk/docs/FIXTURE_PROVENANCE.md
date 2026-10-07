# Pinned source fixtures

These archives contain source material only. Their exact hashes are checked
before use, included in the build input inventory, and retained in source bundles.
They remove dependencies on the original development checkout's ignored files.

## Fujin v0.1.0

`theme/fujin-v0.1.0.zip` contains only `dist/tokens-resolved.json`, `dist/tokens.css`
and `LICENSE`, read from Git tag `v0.1.0` at commit
`c653620262ef68fa8d58504b6c47bb21aadc5aa5` in
[StarlightDaemon/Fujin](https://github.com/StarlightDaemon/Fujin).
It retains the original MIT license. Archive SHA-256:
`021af468de3baff69c6946794eba1e7f11dec63b62e0ac049515b8ed69987456`.

`tools/sync-fujin.ps1 -Check` compares the embedded C++ adapter with these exact
tokens. Pass `-FujinRoot` explicitly to regenerate/check against the pinned Git
tag instead. The standalone mod has no runtime dependency on the archive.

## Historical parser fixture

`tests/fixtures/rc4-source.zip` contains only the original Windhawk RC4 source at
`windhawk/mods/liminal-oled-guard.wh.cpp`, extracted byte-for-byte from the preserved
local RC4 archive. It contains no logs, host information or original standalone
OLED Aegis application. The retained old source is compiled only for its parser
regression test; it is not installed as a mod or linked into the current product.

- New source-only archive SHA-256:
  `7c1c4037f95ed326f220a8231d467016346b77fe73680544c0461c7281ae1ccc`.
- Exact historical source SHA-256:
  `ed923dfd5704e27fb10f897ccf02653d18ebe4980092a50cd5fc0a22c14d7dd4`.
- Original complete evidence archive SHA-256:
  `bf24f21219674d519a57576f913c899a572ac0944af089245d858e90a6676378`.

The runner and verifier accept the two explicitly pinned archive hashes and
require the same historical source hash. A new source-only receipt records the
new archive's actual hash. Old complete-archive receipts remain verifiable.

The snapshots were written with stable ZIP timestamps and contain no executables,
installed Windows screensavers, compiler runtimes, credentials or live settings.
