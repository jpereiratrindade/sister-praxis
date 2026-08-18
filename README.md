# SisTer-Praxis

**Núcleo reflexivo, temporal e governado de computação, avaliação e suporte à decisão** para o ecossistema SisTer.

O SisTer-Praxis parte de uma gramática já materializada entre evidência e decisão:

`Evidence -> Measurement -> Criterion -> Claim -> Assessment -> Authority -> Decision -> Successor`

A partir da revisão arquitetural v0.2, esse conjunto passa a ser explicitamente tratado como **Praxis Core**. Capacidades adicionais podem integrar-se a esse núcleo por um runtime compartilhado de transformações governadas, sem ampliar o core até que ele domine a semântica dos demais subsistemas.

## Fronteira constitutiva

O `sister_praxis_core` preserva conceitos fundamentais e estáveis:

- identidade de estados e referências;
- evidência e proveniência;
- medidas, critérios e avaliações;
- estados epistêmicos;
- autoridade, decisão e sucessão;
- trilha de auditoria;
- integridade temporal como princípio de transição.

O `sister_praxis_runtime` acrescenta mecanismos compartilháveis:

- registro de serviços de transformação;
- execução de transformações identificadas;
- preservação de `owner` semântico externo ao core;
- contrato e implementação versionados;
- estado predecessor e estado sucessor;
- contexto temporal;
- inputs, parâmetros, outputs e proveniência;
- recibo reconstruível da execução.

**Centralizar mecanismos compartilhados não implica centralizar o domínio.**

Um serviço pode ser semanticamente propriedade de SisTer-Atmos, SisTer-Memória, SisTer-Reflexa ou outro componente, enquanto reutiliza o runtime comum do Praxis. O core não precisa conhecer precipitação, documentos, trajetórias ou qualquer ontologia particular para preservar as propriedades fundamentais de uma transformação.

## Regra de integração

Uma capacidade é candidata ao runtime compartilhado quando a execução precisa preservar uma ou mais destas propriedades:

- identidade do estado derivado;
- proveniência;
- contrato e versão;
- contexto temporal;
- relação predecessor/sucessor;
- reprodutibilidade;
- auditabilidade;
- composição com outras transformações;
- reutilização transversal entre domínios.

A semântica especializada, os invariantes científicos e a propriedade do serviço permanecem no domínio que os define.

## Verificação

```bash
./scripts/verify.sh
./build/sister-praxis-cli demo
./build/sister-praxis-cli demo-core
./build/sister-praxis-cli demo-runtime
```

## Estado atual

A revisão v0.2 preserva integralmente o A0 de avaliação governada e acrescenta uma prova arquitetural mínima de **core + governed transformation runtime**. Ainda não implementa persistência SQLite, servidor HTTP, execução assíncrona, filas, cache ou bridge funcional com outros subsistemas.

As superfícies OpenAPI continuam declarativas e agora incluem o contrato futuro de catálogo e execução de serviços.

## Licença

SPDX-License-Identifier: GPL-3.0-or-later
