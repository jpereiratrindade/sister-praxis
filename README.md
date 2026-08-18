# SisTer-Praxis

**Governed Evaluation and Decision Support** para o ecossistema SisTer.

SisTer-Praxis materializa uma gramática governada entre evidência e decisão:

`Evidence -> Measurement -> Indicator/Criterion -> Claim -> Assessment -> Decision -> Successor`

Princípios constitutivos do bootstrap:

- medir, avaliar e decidir são operações distintas;
- toda avaliação material referencia um `EvaluationContract` versionado;
- evidências carregam identidade e proveniência;
- mudanças materiais de critério produzem sucessores em vez de reinterpretação silenciosa;
- gates são critérios executáveis, não a totalidade da avaliação;
- perfis de avaliação especializam um núcleo comum sem codificar o domínio no core;
- REA disciplina percepção/revisão; RIT preserva integridade temporal; Reflexa observa trajetórias; SisTer-Praxis avalia e governa decisões.

## Verificação

```bash
./scripts/verify.sh
./build/sister-praxis-cli demo
```

## Estado A0

Este bootstrap entrega um núcleo C++23 compilável, teste unitário mínimo, trilha de auditoria JSONL, contratos JSON Schema, perfis de avaliação e superfícies de API/UI declarativas. Persistência SQLite, HTTP e integração Reflexa entram nas fatias verticais seguintes.

## Licença

SPDX-License-Identifier: GPL-3.0-or-later
