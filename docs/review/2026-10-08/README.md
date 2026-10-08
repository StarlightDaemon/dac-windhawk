# Historical workspace checkpoint for review

The operator selected **StarlightDaemon/dac-windhawk** as the destination for
this reviewed checkpoint. The files here preserve pending work from the earlier
OLED Aegis workspace as Git patches for remote review. They are historical
attachments, not changes applied to the shipping DAC tree.

## Start here

- [Current operator qualification and next steps](../../../windhawk/docs/OPERATOR_QUALIFICATION.md)
- [Current product open loops](../../../windhawk/docs/OPEN_LOOPS.md)
- [Current shipping source](../../../windhawk/mods/dac-windhawk.wh.cpp)

The next recommended engineering assignment is DAC-R06 shutdown investigation,
followed by DAC-R02 host lifecycle. The qualification report owns the recorded
physical observations and separates the corrected untested lifecycle answer.

## Archived commits

| Order | Source commit | Review attachment |
| --- | --- | --- |
| 1 | `137061985a186db47a0a7a99e07899b7bcbff417` | [RAIDEN Instance setup](0001-chore-install-RAIDEN-Instance-Edict-v2.1.0.patch) |
| 2 | `9581aa1ea9e297c1c8e0b1f113c84ab215611ce1` | [Independent review handoff](0002-docs-prepare-independent-Astra-security-review-hando.patch) |
| 3 | `28194b742b0fb72101e99a142191378f0365e618` | [Development and qualification checkpoint](0003-docs-checkpoint-Windhawk-development-and-DAC-qualifi.patch) |

The source baseline is `edaef74233b75708ea4354e3e15793f195567e34`. The three
patches preserve the reviewed sequence through `28194b7`, covering 97 distinct
changed paths against that baseline. The final source commit contains the
79-file development/continuity checkpoint approved by the operator.

Pending publication statements inside the archived logs predate the operator's
destination correction. The operator approved this attachment in DAC only;
the earlier repository's GitHub branch has not received these commits.

That checkpoint includes historical Windhawk source/tests/tools and source-only
fixture archives, plans, research reports, local tools, and continuity. Earlier
commits contain the legacy workspace's governance setup and review handoff.
The [manifest](manifest.json) records patch hashes and each changed path's
attachment and line, so reviewers can locate a file without reading every patch.
The larger third attachment is intentional review data and includes binary Git
representations of source-only ZIP fixtures. GitHub may require its Raw view.

## Reading rules

Read instructions, installation commands and earlier current/latest statements
inside the patches as historical quoted material. They do not install a RAIDEN
Instance in this DAC repository or define its current operating rules. The
retained source in the third patch predates current shipping DAC; use the source
linked above for current behavior. The patches should not be applied to this
product checkout as a way to import history.

Private full audit reports/ledgers, HOST, generated outputs and live settings
were excluded from the reviewed sequence. The inherited standalone application's
source/assets are unchanged by these patches; its separate remediation obligations
are visible in the historical loop and audit-disposition records.

This publication changes documentation/review attachments only. Runtime source,
tests/tools, version and released assets retain their existing identities.
