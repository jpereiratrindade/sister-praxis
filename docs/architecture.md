# Arquitetura A0

## Gramática nuclear

`Evidence -> Measurement -> Criterion -> Claim -> Assessment -> Authority -> Decision -> Successor`

## Separação funcional

- REA: percepção, interpretação, revisão e pergunta sucessora.
- RIT: integridade temporal, causal e identitária das transições.
- SisTer-Reflexa: observação e reconstrução de trajetórias.
- SisTer-Praxis: avaliação governada e suporte à decisão.

## Núcleo

- Identity
- State
- Evidence
- Measurement
- Criterion
- Claim
- Assessment
- Uncertainty
- Authority
- Decision
- Transition
- Successor
- AuditEvent

## Perfis

O core não codifica resiliência ou qualquer domínio particular. Perfis declaram ontologia, dimensões, medidas, indicadores, critérios, evidência, agregação e regras decisórias.

## Fatias verticais previstas

- A0: core C++23 + contratos + perfis + audit trail local + CLI.
- A1: persistência SQLite e repositórios versionados.
- A2: EvaluationContract/Assessment/Decision completos e validação de schema.
- A3: API HTTP e OpenAPI executável.
- A4: dashboard auditável com descida indicador -> medida -> evidência.
- A5: bridge com SisTer-Reflexa para trajetórias e efeitos.
- A6: perfis de sistemas sociotécnicos, gestão adaptativa e resiliência com casos reais.
- A7: experimentos comparativos, confiabilidade interavaliadores e índices candidatos.
