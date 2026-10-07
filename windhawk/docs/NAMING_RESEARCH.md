# Naming research — first refinement round

Historical research round, 2026-10-05. **Superseded by the operator selection
of Liminal OLED Guard.** See [RC4 naming and migration](LIMINAL_RC4.md).
The research below records the earlier unselected stage.
This round establishes the naming brief, examines the adjacent market,
rejects crowded word directions and defines the next candidate-refinement gate.

## Phase 1 — define what needs a name (completed)

The product is a Windows/Windhawk utility for independent monitor idle
protection: activity/media-aware blanking, installed screensavers, photo
presentation and optional hardware sleep/wake. It also offers manual/sticky
control and multi-monitor coordination. That combination is the identity to
communicate; the implementation is no longer accurately described as only an
OLED black-screen application.

Working positioning sentence, not a proposed product name:

> Independent monitor rest and screensaver control for Windows.

Naming constraints derived from the actual product:

- Suggest controlled rest, display stewardship or coordination.
- Work for LCD as well as OLED; avoid making panel technology the brand.
- Avoid promising burn-in prevention, guaranteed physical wake, a screen lock,
  privacy enforcement or universal saver compatibility.
- Keep the brand distinct from the upstream project's name; dropping only
  “OLED” while keeping “Aegis” would leave that confusion unresolved.
- Keep Windhawk as an edition/integration descriptor rather than a permanent
  brand dependency if a standalone edition may later exist.
- Aim for easy English pronunciation/spelling, two to four syllables and a
  compact tray/menu label. These are provisional design criteria, not facts
  about an audience we have interviewed.
- The monitor/shield icon can remain during research. A future softer rest
  identity might warrant adjusting its security-like shield emphasis.

## Phase 2 — adjacent-market probe (completed)

These are comparison references, not credits or dependencies.

| Observed name | Primary evidence | Implication for this product |
| --- | --- | --- |
| Twinkle Tray | [Official project](https://twinkletray.com/) describes multi-monitor brightness, hotkeys, idle schedules and DDC controls | A light/tray name competes with brightness tools; our descriptor should foreground idle protection and savers |
| Monitorian | [Maintainer repository](https://github.com/emoacht/Monitorian) describes multi-monitor brightness adjustment | Generic monitor-root names are crowded; a bare monitor prefix adds little distinctiveness |
| Luma | [Official app](https://getluma.app/) markets display brightness and dimming | Do not advance this exact seed; it is already adjacent display software |
| Lumen | [Epic documentation](https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-global-illumination-and-reflections-in-unreal-engine) names its rendering technology | Light vocabulary has major graphics/search collisions; do not advance this exact seed |
| Sentinel | [Thales documentation](https://docs.sentinel.thalesgroup.com/index.htm) identifies existing software licensing products | Avoid this exact seed; it is crowded software/security language and can misstate the utility's purpose |
| ScreenSentry | [Maintainer repository](https://github.com/FaxanaduHacks/screen_sentry) uses the name for screen redaction | Avoid this exact seed; it also suggests privacy/security behavior we do not provide |

This is a qualitative web/maintainer-source screen, not a trademark clearance
search. An observed collision is reason to deprioritize a seed; it does not
establish legal infringement. An unobserved collision would not prove availability.
No domain, package name, repository handle or mark has been reserved.

## Phase 3 — refine naming directions (completed)

Scores are design judgments on a 1–5 scale, not measured market results.
Weights: product fit 30%, distinction from upstream 25%, avoiding misleading
claims 20%, ease of use 15%, room for future scope 10%. Collision clearance
is deliberately excluded: it must be tested for actual candidate names later.

| Direction, not a candidate name | Fit | Distinction | Truthful meaning | Ease | Future scope | Weighted /5 | Disposition |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Coined word around rest + coordination | 5 | 5 | 5 | 3 | 5 | 4.70 | Strong direction to explore; coined spellings need spoken testing |
| Descriptive panel/display + rest concept | 5 | 4 | 5 | 5 | 3 | 4.55 | Strong clarity; likely weaker uniqueness/searchability |
| Abstract calm/night word with explanatory subtitle | 4 | 5 | 4 | 4 | 5 | 4.35 | Useful alternate direction; avoid circadian/sleep-health implications |
| Protection/guardian/security metaphor | 3 | 2 | 2 | 5 | 4 | 2.95 | Deprioritize: too close to Aegis and security products |
| Light/luminance/brightness metaphor | 2 | 4 | 2 | 4 | 4 | 3.00 | Deprioritize: describes brightness control better than this utility |

Refinement result: explore the first two directions, retain the third as a
contrast set. Do not choose a winner, infer an available trademark from these
scores, or mint a replacement name in source at this stage.

## Phase 4 — candidate generation and probing (prepared, not selected)

The next naming round can produce 12–18 raw candidates across those three
directions, then reduce to roughly six with the following evidence per name:

1. Say it aloud and spell it without seeing it. Check obvious alternate
   spellings, abbreviations, singular/plural forms and phonetic neighbors.
2. Search exact and near names with Windows, display, monitor, screensaver,
   software and app. Check maintainer repositories, Windhawk's catalog,
   Microsoft Store and package registries. Record the query/date and primary
   result, including adverse matches.
3. Reject names that imply guaranteed burn-in prevention, system sleep,
   security/privacy, brightness control or a vendor relationship we lack.
4. Test remaining names in the actual strings “Settings”, “Pause protection”,
   “Stop all monitors”, “for Windhawk”, a tray tooltip and the small icon.
5. Rank with the stated criteria, show unresolved conflicts, and bring a short
   list to the operator. Operator selection remains separate from research.

The present instruction explicitly asks not to pick a name, so this document
does not manufacture a favored candidate or imply a selection has occurred.

## Phase 5 — clearance and technical rename (deferred)

Before public adoption, check the relevant markets' trademark and common-law
use, phonetic similarity, related software, domains and handles. Geographic
release plans are not specified; this research does not assume US-only use.
The [USPTO clearance guidance](https://www.uspto.gov/trademarks/search/comprehensive-clearance-search-similar-trademarks)
explains why a register-only or exact-name web check is incomplete. No registry
clearance result is claimed in this round.

After a name is selected, plan compatibility before a bulk replacement:

| Surface | Rename treatment to decide and test |
| --- | --- |
| Window titles, tray labels, readme, visible mod name | Update to the selected brand; review actual clipping and pronunciation |
| Windhawk `@id` and installed `local@` identity | Prefer retaining update identity initially; a new ID can create a second enabled mod |
| Settings path and schema | Read/migrate existing settings deliberately; retain backup and rollback |
| Singleton/emergency events and window-class names | Preserve old/new coexistence protection during migration; avoid two competing controllers |
| DDC helper tickets/fault markers | Keep identities compatible with outstanding wake/recovery operations |
| Legacy INI keys and original Run-entry detection | Keep compatibility strings even when the visible brand changes |
| Build/package paths, tests and documentation | Update deliberately with identity-bound evidence; retain historical reports/hashes |
| Author metadata and notices | Identify the new edition accurately; do not use branding to erase dependency rights or original history |

No renaming, purchase, registration, external message or publication occurred.
