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

## Fronteira HTTP e superfície web A0-R2

A primeira borda HTTP executável é deliberadamente mínima e somente de leitura:

- `GET /`: superfície web de visão geral, alinhada à identidade visual do ecossistema;
- `GET /health`: readiness de core, runtime e camada HTTP;
- `GET /services`: descoberta dos serviços registrados no runtime.

A página inicial consulta `/health` e `/services` na mesma origem e diferencia explicitamente capacidades já materializadas de funções futuras. Ela não introduz mutações nem antecipa persistência, runs ou avaliações ainda inexistentes.

O executável `sister-praxis-http` aceita apenas `127.0.0.1` como bind nesta fatia e usa a porta `8093` por padrão. A exposição LAN deve ocorrer exclusivamente pelo gateway do ecossistema. Recursos mutáveis e leitura persistente de runs permanecem fora desta fatia até as etapas governadas de persistência, contratos e API completa.

```bash
./scripts/http_service.sh start
curl -fsS http://127.0.0.1:8093/health
curl -fsS http://127.0.0.1:8093/services
./scripts/http_service.sh stop
```

## Verificação

```bash
./scripts/verify.sh
./build/sister-praxis-cli demo
./build/sister-praxis-cli demo-core
./build/sister-praxis-cli demo-runtime
./build/sister-praxis-http --bind 127.0.0.1 --port 8093
```

## Estado atual

A revisão v0.2.2 preserva integralmente o A0 e o A0-R1 e acrescenta uma borda HTTP mínima de conectividade e descoberta, agora com uma superfície web inicial servida em `/`. Persistência SQLite, execução de runs via HTTP, execução assíncrona, filas, cache e bridge funcional com outros subsistemas ainda não estão implementados.

A OpenAPI distingue a superfície já executável (`/health`, `/services`) dos recursos declarativos que serão materializados em fatias posteriores.

## Licença

SPDX-License-Identifier: GPL-3.0-or-later
