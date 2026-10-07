# Candidate dependencies and notices

The source-review package contains no stock savers, compiler binaries,
Windhawk headers/import libraries or inherited OLED Aegis source/artwork.
Installed dependency hashes and reference links are in PROVENANCE.md.

- Windhawk, Michael Maltsev / RAM Software: installed 1.7.3 API headers/import
  library are build inputs. Project terms are GPL v3 or later; no separate API
  exception is asserted. [License](https://github.com/ramensoftware/windhawk/blob/v1.7.3/LICENSE).
- LLVM/clang/libc++/libunwind, LLVM contributors: installed compiler 20.1.3;
  `Compiler/LICENSE.TXT` specifies Apache 2.0 with LLVM exceptions. Local test
  runtime copies retain original bytes under `.whl` names; excluded from this
  source package. [LLVM license](https://llvm.org/LICENSE.txt).
- MinGW-w64 project and component contributors: installed headers, runtime
  and import libraries. Component notices are in the compiler bundle's
  `x86_64-w64-mingw32/share/mingw32/COPYING.MinGW-w64-runtime.txt`. No header
  implementation is vendored. Later binary packaging needs applicable notices.
- Microsoft Windows: installed Win32/Core Audio/DWM APIs and `.scr`
  files are used under the existing Windows installation's terms; none are
  bundled or downloaded. A minimal meter COM ABI view declares only the
  documented first method and IID to bridge incomplete MinGW declarations;
  no generated Windows SDK header/implementation was copied.
  RC1 also calls installed GDI+ image rendering and Dxva2 monitor-control APIs;
  neither Windows library nor saver/photo content is bundled in the source ZIP.

This notice inventory covers actual incorporated material and build/runtime
dependencies. Optional inspiration credits were removed following the
[RC3 source assessment](SOURCE_COMPARISON.md). Historical lineage and cited
technical research remain in [PROVENANCE.md](PROVENANCE.md); those references
are not product credits or endorsements. Original DAC source is now MIT licensed; see [LICENSE.md](../LICENSE.md). This does not alter the retained standalone application's history.

## Fujin v0.1.0

Generated palettes, typography and metrics are consumed from the pinned tagged
outputs and embedded in the standalone source. See [integration provenance](FUJIN_RC3.md).
The new monitor/shield icon geometry is original.

MIT License

Copyright (c) 2026 StarlightDaemon

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
