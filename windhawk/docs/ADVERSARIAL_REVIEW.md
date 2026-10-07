# Pre-1.0 adversarial review

Review date: 2026-10-07. Baseline: public repository commit `8372324` (0.1.6).
Remediation candidate: 0.2.0. This is a source review plus automated regression
exercise, not an independent penetration test or a claim that no defects remain.

## Scope and method

Reviewed the complete shipping `mods/dac-windhawk.wh.cpp`, including policy,
configuration parsing/import/export, Win32 storage, rendering, executable launch,
process ownership, audio/input attribution, hardware helpers, settings/UI and
Windhawk callbacks. Reviewed the production build/evidence/package tools and
their tests, fixture guards, documentation and release setup. The prototype and
its harness are historical, not the shipping product. Pinned fixture archives
are validated by their existing hashes; dependency internals and Windows/Windhawk
themselves are outside this source audit.

The older OLED Aegis standalone application is absent from this public repository.
Its previous findings remain in [the inherited audit disposition](AUDIT_DISPOSITION.md).
They are not repaired or certified by DAC changes. The development workspace's
inherited source/build workflow remains a separate product and qualification scope.

Read paths were traced from inputs to effects. Specific probes target malformed
configuration, file replacement, stale settings drafts, child lifecycle, stale or
altered release evidence, version identity and archive completeness. Tests run in
isolated harness processes; no live installation/settings, real DDC off, or lock
operation is part of this review.

## Findings and disposition

| ID / severity | Trigger and consequence | Disposition |
| --- | --- | --- |
| DAC-001 / Medium | `WriteFileText` reused `destination.tmp-PID` with `CREATE_ALWAYS`. A pre-existing file or hard link at that path was truncated, including an unrelated target in a writable export directory. This is a same-account/shared-directory integrity problem; no cross-account exploit is claimed. | Fixed: GUID temporary name, `CREATE_NEW`, and no deletion unless creation succeeded. Regression pre-creates the old temporary hard link and verifies target bytes and link survival. Existing replacement-failure and long-Unicode-path cases remain. |
| DAC-002 / Medium | Open Advanced settings, change saved settings through another surface, then save the old draft. Advanced/profile Save silently overwrote the newer values; Quick setup already rejected this. | Fixed: compare the editor's baseline before Advanced or profile Save. Reject stale drafts, preserve active state, and explain how to reopen. Hidden UI regression exercises both save paths. |
| DAC-003 / Medium | The legacy `package-production.ps1` omitted ZIP fixtures and top-level instructions/license from its source package. A maintainer following that route could distribute a bundle that could not reproduce its build. | Fixed: route it through the complete dual-architecture package gate; include root usage/license/security documents and require them for the new release contract. |
| DAC-004 / Medium | Versions were repeated in source diagnostics and tool defaults, and the release specification accepted only enumerated historical versions. No public CI/release workflow enforced a new batch's identity or publication. | Fixed: version command, source-derived defaults, fixed evidence requirements for strict public versions, changelog/tag checks, pinned toolchain CI and successful-tag publication. |

Severity is qualitative engineering triage, not a CVSS assessment. No confirmed
critical/high defect was established in this review. That does not establish absence.

## Coverage and retained protections

| Surface | What was assessed / exercised |
| --- | --- |
| Parser and policy | 256 KiB bound, UTF-8/NUL rejection, duplicate/canonical keys, schema and integer validation, profile depth/count and display/rule limits; input scopes, stale media, schedules, profile permissions, battery, dim/black transitions |
| Storage | Known-folder failure, Unicode/long paths, atomic same-directory replacement, denied replacement, preserved migration backups, unsupported schemas, recovery markers, exclusive temporary creation |
| Launch and lifecycle | Explicit executable path, quoted command line, no shell execution or inherited handles; suspended launch then job assignment; exact owned process cleanup, emergency path, host death, fallback, late generations, final thread joins |
| Media and input | Limited-rights process query, owned-process exclusion, ambiguous/stale observations, raw-input fallback, controller attribution; real application behavior remains heuristic |
| Hardware | Base permission caps, nonce-bound markers, one-target checks, helper containment, cancellation/deadlines, quarantine and fake recovery tests; no physical power qualification inferred |
| UI and rendering | Draft/save boundaries, native command paths, topology/DPI retention, themes, resource lifetime, scenes/slides and contained previews; hidden-window checks do not establish appearance/accessibility |
| Release integrity | Same-source builds on both architectures, fixture hashes, rollback parser, required receipt identities, archive path/duplicate/size/checksum checks and tampered-evidence rejection; version/tag/changelog checks |

## Residual risks and release gates

1. **Actual host and hardware qualification is incomplete.** Record actual
   Windhawk load/unload, integration settings, multi-monitor attribution, mixed DPI,
   high contrast/screen reader, small work areas at high DPI, SDR/HDR, AC/DC, suspend/resume, and DDC off/wake/fault
   recovery before claiming v1 support for those combinations. Existing positive
   operator observations are useful but narrower than this matrix.
2. **Custom screensavers execute with host privileges.** Job containment is lifetime
   management, not a security sandbox. Treat `.scr` files and imported custom paths
   as trusted code. This is an explicit product trust boundary.
3. **Synchronous OS calls can stall.** Photo decoding, filesystem calls, audio COM
   and driver calls may hang despite logical size/time limits. Shutdown must join
   workers to avoid unloading executing DLL code. The emergency path hides owned
   windows and kills owned saver jobs, but no absolute unload deadline is claimed.
   Consider moving photo decoding and audio collection to disposable helpers if
   bounded unload becomes a v1 requirement. Large directory scans are cancellation
   aware but the playlist cap does not bound all non-image entries visited.
4. **Same-account settings are trusted.** Exclusive temporary creation fixes the
   concrete overwrite flaw; it does not isolate against malicious replacement of
   parent directories or arbitrary programs running as the same account. Export
   only to trusted directories and avoid running the tool elevated.
5. **Hardware control remains experimental.** Physical DDC can fail after an off
   command. Quarantine and retained recovery markers support diagnosis, not a
   guarantee of waking every display.

## Validation record

The 2026-10-07 local run passed warning-as-error builds and all 16 groups on each
architecture, both pinned historical-parser runs and all 34 named packaging
rejection cases. The verified bundle contained 212 entries at that checkpoint.
Version regression checks passed malformed/non-increasing bumps, mismatched tags,
missing changelog entries and stale diagnostics. PowerShell syntax and local links
in the new operator-facing documentation passed; the banner was rendered and inspected.

As negative controls, the new tests were compiled against the unchanged `8372324`
source. `storage` failed specifically at "save must not truncate temporary link
target" and `nextbeta` failed at "stale Advanced draft cannot overwrite live
settings". Both pass against the repaired source on both architectures.

An initial packaging run failed because its missing-report mutation assumed an
old report name. The fixture now removes a report required by the selected trusted
specification; the complete fresh rerun above passed. Requirements were not relaxed.

The first hosted run reached the Advanced group and exposed a DPI fixture that
requested widths beyond Windows' tracking limit on its 1024-pixel desktop. The
fixture now respects that limit and Quick setup's explicit 620-logical-pixel
minimum (which Windows gives precedence when synthetic DPI and real desktop
constraints conflict). It retains every geometry, draft and font assertion,
and logs actual/requested bounds on failure. The changed
Advanced and nextbeta groups passed locally. The complete local rerun and
[hosted run 37588586026](https://github.com/StarlightDaemon/dac-windhawk/actions/runs/37588586026)
at commit `bb311e4` then passed both architectures and every required gate.

The [GitHub Actions history](https://github.com/StarlightDaemon/dac-windhawk/actions/workflows/release.yml)
records hosted results against exact commits. The source bundle carries exact
input, binary and test evidence identities. Hosted validation and tag publication
must succeed independently of the local checks above.
