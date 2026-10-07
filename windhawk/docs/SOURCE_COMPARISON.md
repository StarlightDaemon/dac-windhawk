# RC3 source comparison and attribution assessment

Assessment date: 2026-10-05. Scope: the delivered Windhawk implementation,
its local prototype/tests, and the retained upstream application. No runtime
code, installed mod, name, or historical release archive is changed by this review.

## Finding

The evidence supports describing the Windhawk edition as a substantially new
implementation informed by the original project's behavior and reviewed source.
It does **not** establish a clean-room origin, absence of every possible
adaptation, or a percentage of legally independent authorship.

The only exact contiguous matches of at least 20 lexical tokens in RC3 are
Windows header declarations. No such match remains after directives are
excluded. Broader syntax-shape matches are short, generic Win32/C++ patterns.
The product's behavior, temporary name and compatibility importer visibly
retain its development relationship to the original project.

The **retained standalone application is still upstream code**. Its C source,
resource file, images, root product/build guides, build scripts and legacy CI
have no diff against the inherited baseline. The low Windhawk similarity figures
must never be applied to that application or to the repository as a whole.
The verified RC3 source ZIP contains the Windhawk edition and evidence, not
the retained `src/` application or inherited `images/` assets.

## Inputs and reproducibility

| Input | Identity |
| --- | --- |
| Upstream baseline | `spenserlee/oled_aegis`, commit `edaef74233b75708ea4354e3e15793f195567e34`, `src/oled_aegis.c` |
| Baseline source SHA-256, git blob bytes | `d59ff0bdfbef6e47b3e9a089ca8de008953d1bd626367978b273aef76330499d` |
| Baseline size | 2,702 lines; 14,860 lexical code tokens |
| Retained working-tree source SHA-256 | `d1797297695050b497dea91e729bba32741706b41234a57d6d83771125588cd0` |
| Working-tree difference | Newline representation only; text equals the baseline after CRLF/LF normalization |
| Current mod | `windhawk/mods/oled-aegis.wh.cpp`, version `1.0.0-rc.3` |
| Current mod SHA-256 | `89dbed1d5ed2c8b269955fd216efd11a0670536d15892296f48f8e31c3454126` |
| Current mod size | 1,710 lines; 32,141 lexical code tokens |
| Analysis script SHA-256 | `1b4a399e0b0d82914f4a9b85d0dcbdf9c958dbe84d117f44d98b6b4d19c2088c` |

Run from the checkout root:

```powershell
python .raiden/local/tools/compare_sources.py
```

The standard-library-only script writes `build/analysis/source-comparison.json`.
It includes all match locations, both directional coverage figures, input
hashes and the script identity. Its self-checks compare the matching algorithm
with an exhaustive reference on 100 deterministic fixtures and verify comment,
whitespace, renaming, literal and no-match cases. This is a lexical research
tool, not a copyright detector or a full C++ parser.

## Measurements

Each percentage below uses **current-mod tokens** as its denominator. Comments
and whitespace do not count. Exact-token matching preserves identifiers and
literal spelling. Shape matching replaces non-keyword identifiers and literals
with placeholders while retaining language keywords and punctuation. It can
match unrelated code and is a sensitivity probe, not evidence of adaptation.

We find every maximal contiguous match at each minimum length and count the
union of covered tokens, so repeated or overlapping matches are not added
multiple times. Reordered code, semantic rewrites and fragments below the
threshold can escape these measures; manual subsystem review addresses those
limitations without inventing a semantic similarity percentage.

| Scope and denominator | Exact runs ≥20 tokens | Exact runs ≥50 tokens | Shape runs ≥20 tokens | Shape runs ≥50 tokens |
| --- | ---: | ---: | ---: | ---: |
| Complete mod, 32,141 tokens | 49 / **0.1525%** | 0 / **0%** | 286 / **0.8898%** | 91 / **0.2831%** |
| Excluding generated Fujin block, 31,833 tokens | 49 / **0.1539%** | 0 / **0%** | 286 / **0.8984%** | 91 / **0.2859%** |
| Excluding Fujin and preprocessor directives, 31,504 tokens | 0 / **0%** | 0 / **0%** | 195 / **0.6190%** | 0 / **0%** |

At a 100-token minimum all six exact/shape comparisons above are zero.
The longest exact match is 25 tokens. The longest shape match is 91 tokens
with headers included, and 25 tokens with directives excluded. Excluded
regions retain barriers so removal cannot manufacture matches across gaps.

As a deliberately naive cross-check, **195 of 1,696 nonblank current lines
(11.4976%)** also occur upstream after trimming surrounding whitespace.
Every match belongs to just fifteen short line texts: seven common includes,
closing braces, `};`, `break;`, `do {`, three return statements, and
`case WM_POWERBROADCAST:`. No trimmed line of 30 or more characters matches
(0 / 1,204 eligible current lines). Neither line measure is an authorship
percentage; RC3's compact multi-statement formatting also makes lines a poor
unit for comparing the two implementations.

## Manual match review

Line numbers refer to the input hashes above. Token blocks can start or end
partway through the reported first/last line.

| Match | Upstream | RC3 | Assessment |
| --- | --- | --- | --- |
| Exact 24 tokens | Lines 1–4 | Lines 306–309 | Common `windows.h`, `shellapi.h`, `shlobj.h` include sequence and boundary tokens |
| Exact 25 tokens | Lines 11–15 | Lines 310–314 | Common Core Audio include sequence and boundary tokens |
| Shape maximum, 91 tokens | Header region, lines 1–15 | Lines 306–318 | Erasing header-name identifiers makes different include lists look alike |
| Shape 25 tokens | `ShowSettingsDialog`, lines 1891–1895 | `ShowSaverOptions`, line 1306 | Generic window styles, default placement and scaled `CreateWindowEx` arguments |
| Remaining body shape matches, 20–23 tokens | Window procedures, control initialization and menu calls | Painting, window procedures and settings initialization | Common callback signatures, assignments, call punctuation and short control-flow fragments; some pair unrelated operations |

No matched block identified in this review establishes copied original
application logic. That is a bounded assessment of these files, not a claim
that every short shared expression was invented independently.

## Architecture and behavioral cross-comparison

| Area | Original application | Current Windhawk implementation | Relationship |
| --- | --- | --- | --- |
| Idle/protection state | Global structures, fixed monitor arrays and `HandleTimeout` | `Controller`/`Node` state machine, stable-ID map, generations and monotonic time | Same purpose; different representation and lifecycle |
| Per-monitor input | Polls global last-input age, cursor and foreground geometry | Raw input with cursor/foreground attribution, stale-event conservatism and fallback | Strong behavioral continuity; event handling rewritten |
| Media inhibition | Default audio endpoint, process-name/browser/title heuristics, 30-second audio grace | All active render endpoints, PID/name window association, endpoint/session mute and volume, bounded polling-based grace | Shared Core Audio problem and APIs; different implementation and some deliberately different behavior |
| Window overlap | Area/intersection and center fallback; 10,000 pixels / 10% | `MediaOverlap`, center fallback; 4,096 pixels / 5% | Similar geometric approach, different expression and thresholds; ordinary geometry is not a provenance test |
| Configuration | ANSI paths, `fopen`/`sscanf`/`atoi`, legacy INI keys and fixed arrays | Validated UTF-8 schema, transactional replacement, stable preferences and explicit importer | Legacy key strings intentionally shared for interoperability; parser/persistence rewritten |
| Black presentation | Nonactivating topmost black windows, per-monitor padding | Owned nonactivating host windows and black fallback with monitor preferences | Same visible baseline behavior and conventional Win32 flags |
| External savers | No installed `.scr` preview-host pipeline found in the baseline | Independent preview processes, contained jobs, health checks and configuration sessions | Added implementation |
| Photos and spanning | No corresponding independent photo pipeline found | GDI+ folder slideshow, six placements, per-monitor frames and explicit span session | Added implementation |
| Hardware off/wake | No DDC VCP helper implementation found | Opt-in DDC operations, operation-bound tickets, persistent helper and fault quarantine | Added implementation |
| UI and assets | Native settings and two resource-linked upstream icons | Two DPI-preserving settings windows, generated Fujin theme and original monitor/shield geometry | Different UI implementation/assets; basic native controls shared |
| Startup and host | Standalone `WinMain`, own Run-key management | Windhawk dedicated host, singleton/emergency coordination and legacy coexistence check | Different deployment and lifecycle |

The same product name, overlapping setting concepts/ranges, monitor protection
contract and legacy keys are intentional history, not something a token score
erases. Prior authors reviewed original code; this is not clean-room work.
The original browser/video-name and site-title tables were not found in RC3;
that is one concrete example of related functionality expressed differently.

## Supporting files and whole-repository scope

| Compared against original C baseline | Exact ≥20-token coverage | Shape ≥20-token coverage |
| --- | ---: | ---: |
| Local Windhawk prototype, 4,644 tokens | 0.5383% | 2.0887% |
| Policy tests, 3,046 tokens | 0% | 0% |
| Platform tests, 8,178 tokens | 0% | 0.2446% |
| Initial harness, 1,278 tokens | 0% | 1.7214% |

These are separate comparisons, not weighted into the production-mod result.
The prototype is part of the new implementation's local lineage, not the
upstream standalone application. Build scripts are a separate language and
were not assigned a misleading C-token similarity score. Inherited source,
assets, guides and build files remain unchanged in the checkout. No single
whole-repository "percent original" is asserted.

## Attribution decision

Use **5% current implementation-token coverage in reviewed, non-boilerplate
matches of at least 50 tokens** as an engineering trigger for presumptively
retaining an upstream implementation credit and examining reuse closely.
It is a screening convention, not a legal safe harbor. Any identifiable copied
material or actual license condition takes precedence even below 5%.
RC3 is below this threshold; manual inspection finds no substantive matched
implementation block. The report therefore supports removing optional
inspiration credits from the current Windhawk notices.

Applied changes:

- Remove the optional inspiration-credit paragraph for the original project,
  DisplayFusion, Actual Multiple Monitors and the reference-only mod from
  `THIRD_PARTY_NOTICES.md`. None is an incorporated runtime/source dependency
  of this implementation on the evidence reviewed.
- Retain Fujin's MIT notice and genuine Windhawk/toolchain/Windows dependency
  notices. A similarity result against one project does not waive other terms.
- Retain the factual original-project lineage and technical citations in
  `PROVENANCE.md` and historical reports. Those are research records, not a
  current product credit roll or an endorsement. Commercial documentation
  references are labeled as documentation evidence, not product credits.
- Preserve upstream files, author history and their existing identification.
  The retained standalone application is not covered by the low mod score.
- Do not rename the product, change author identity, erase migration strings,
  relabel a license, or rebuild a functioning mod just for a documentation review.

The reviewed upstream commit has no license file or source grant identified
in its tree. Public availability is not itself an open-source license. The
U.S. Copyright Office explains that there is no fixed permitted percentage and
that changing someone else's work is not a numerical route to ownership.
This assessment does not resolve a legal derivative-work question or supply
missing upstream permission. [Copyright Office FAQ](https://www.copyright.gov/help/faq/faq-fairuse.html),
[GitHub's no-license guidance](https://choosealicense.com/no-permission/).

The historical RC3 ZIP is left intact with its original documentation and
checksums; the current notices govern future packaging. The current mod source
already contains no optional third-party product credit block requiring removal.

## Limits

The numerical baseline is the inherited commit, not every historical/future
upstream revision. Matching is lexical, not compiler-AST, control-flow or
legal substantial-similarity analysis. No closed-source product implementation
was available or compared; no source-similarity percentage is claimed for one.
No code was rewritten merely to lower a score. No outside repository, installed
mod, settings, Git commit or public release was modified.
