# Inherited audit dispositions

Production update under D-0007: the new edition now has independent checked
Unicode persistence, nonfatal bounded diagnostics, limited-rights process
identity, event/between-poll input, persistent all-disabled/disconnected
preferences, fail-closed build outputs, UI resource ownership and derived tray
state. Their automated evidence and remaining limitations are in
[PRODUCTION_REPORT.md](PRODUCTION_REPORT.md). This supersedes the future-work
wording for the Windhawk edition in the historical Phase 0 table below only.
No retained standalone finding or LOOP-005 through LOOP-014 is closed by this
implementation. Assessment curation is not reopened.

Source assessment: ignored local audit report (historical local record: `../../audit-reports/audit-2026-10-04-9581aa1.md`; not bundled), reviewed HEAD `9581aa1ea9e297c1c8e0b1f113c84ab215611ce1`. Its findings are static unless it explicitly says otherwise. This curation is authorized by the Phase 0 handoff. It does not rerun the audit, change its protocol ledgers, repair the retained standalone program, or establish runtime severity/frequency.

All ten Medium findings now have reserved numbered loops in OPEN_LOOPS (historical internal record; not bundled). That ledger owns status and acceptance criteria. Production Windhawk treatments are regression requirements, not completed fixes.

| Finding | Loop | Windhawk treatment / validation focus |
| --- | --- | --- |
| OA-001 AppData construction | LOOP-005 | Avoid legacy path helper; checked Unicode executable construction in prototype; production persistence path failures still to test |
| OA-002 logging failure | LOOP-006 | Prototype only optional Windhawk log; production diagnostics must remain bounded/nonfatal under storage faults |
| OA-003 process rights | LOOP-007 | Prototype uses limited-information queries only for ownership checks; future media identity needs correct query API and inaccessible/exited handling |
| OA-004 input loss | LOOP-008 | Dedicated raw-input sink observes event kinds without freshness-window polling; actual mixed input test still required |
| OA-005 disabled selections | LOOP-009 | Persistent monitor settings not implemented; retain all-disabled and disconnected preferences in production |
| OA-006 false build success | LOOP-010 | New build checks exit/output and removes stale target; deliberate compiler-failure test still required; standalone scripts untouched |
| OA-007 inconsistent build routes | LOOP-011 | Separate installed-Windhawk compiler helper; old supported MSVC verification remains LOOP-002 |
| OA-008 prerequisites | LOOP-012 | Discovered and documented `.whl` runtime imports; no unsupported dependency-free claim |
| OA-009 action versions | LOOP-013 | No new CI/release workflow introduced; review supported immutable actions in later release work |
| OA-010 permissions | LOOP-014 | No inherited implementation/assets reused; retain credits and unresolved grant before such reuse/distribution |

Relevant Low findings remain explicit in the rebuild checklist:

| Finding | Production verification still required |
| --- | --- |
| OA-011 environment cache | No whole-environment file in new helper. Test harmless canary exclusion if a future toolchain environment cache is added |
| OA-012 release credentials | Pin reviewed action SHAs; minimize/segregate credentials; preserve separate publication approval |
| OA-013 tooltip ownership | Own/destroy every custom UI resource; repeated dialog/topology changes must return USER/GDI counts to baseline |
| OA-014 stale tray state | Derive any-running from monitor states. Prototype has explicit start/stop commands rather than a global toggle; production toggle still needs regression evidence |
| OA-015 output hygiene | New output stays under existing ignored `build/`; do not add binaries/runtime copies or private evidence to tracked sources |
| OA-016 persistence failures | Single model does not prove durable saving. Fault-inject save/write/replace failures and show live-vs-saved distinction |
| OA-017 startup serialization | No Run key or Explorer-by-basename launch in prototype. Production startup/import must be explicit and validate arguments without changing global settings |

All retained standalone defects remain unresolved. LOOP-004 can close its assessment/curation tracking criterion once this mapping is reviewed; that closure must never be described as remediation or product certification.
