# Arquitetura — Praxis Core e Governed Transformation Runtime

## 1. Decisão arquitetural

O que foi implementado no A0 passa a ser tratado explicitamente como **Praxis Core**.

O core não é o ecossistema inteiro e não deve absorver ontologias ou algoritmos específicos dos demais subsistemas. Ele fornece os princípios e tipos fundamentais que tornam estados, evidências, avaliações, decisões e sucessões identificáveis e auditáveis.

Ao redor do core existe um **Governed Transformation Runtime**. Esse runtime permite que serviços de análise, processamento, derivação, comparação, agregação, avaliação, projeção ou conversão compartilhem as propriedades fundamentais do Praxis sem transferir sua propriedade semântica para o core.

Regra protetora:

> **Centralizar mecanismos compartilhados não implica centralizar o domínio.**

## 2. Gramática nuclear preservada

`Evidence -> Measurement -> Criterion -> Claim -> Assessment -> Authority -> Decision -> Successor`

A gramática de avaliação permanece válida e não é substituída pela abstração de transformação.

## 3. Abstração de transformação

A camada imediatamente externa ao core trata computações relevantes como transformações identificadas:

`Input State -> Governed Transformation -> Output/Successor State`

Uma execução deve poder preservar, quando aplicável:

- `run_id`;
- `service_id`;
- `owner` semântico;
- `kind`;
- `contract_ref`;
- `implementation_ref`;
- `version`;
- `predecessor_state_id`;
- `successor_state_id`;
- `input_refs`;
- parâmetros;
- contexto temporal;
- `output_refs`;
- `provenance_ref`.

Assim, uma transformação deixa de ser apenas uma chamada de função: torna-se um evento computacional reconstruível e temporalmente identificável.

## 4. Separação de camadas

```text
┌──────────────────────────────────────────────┐
│                PRAXIS CORE                   │
│                                              │
│ Identity / State / Evidence                  │
│ Measurement / Criterion / Assessment         │
│ Authority / Decision / Transition            │
│ Successor / Audit / Temporal Integrity       │
└───────────────────┬──────────────────────────┘
                    │ fundamenta
                    ▼
┌──────────────────────────────────────────────┐
│       GOVERNED TRANSFORMATION RUNTIME        │
│                                              │
│ Service Registry                             │
│ Transformation Request                       │
│ Transformation Execution                     │
│ Transformation Receipt                       │
└───────────────────┬──────────────────────────┘
                    │ recebe extensões
        ┌───────────┼───────────┐
        ▼           ▼           ▼
   domínio A    domínio B    domínio C
   owner=A      owner=B      owner=C
```

O runtime depende do core. O core não depende do runtime nem de domínios externos.

## 5. Propriedade semântica

Um serviço registrado no runtime declara um `owner`. O owner identifica o componente responsável por sua semântica, seus invariantes e sua evolução de domínio.

Exemplo conceitual:

```yaml
service: climate.area_weighted_mean
owner: sister-atmos
runtime: sister-praxis
contract: climate.area_weighted_mean@v1
implementation: git:sister-atmos@<commit>
```

O exemplo não significa que o Praxis conhece climatologia. Significa que uma operação definida por um domínio pode reutilizar mecanismos comuns de identidade, temporalidade, proveniência e execução.

## 6. Critério de admissão no runtime

Uma funcionalidade não pertence ao runtime apenas por ser útil ou reutilizável. Ela é candidata quando sua execução exige propriedades constitutivas do Praxis, por exemplo:

- produz estado derivado que precisa permanecer identificável;
- transforma evidência;
- compara estados ou trajetórias;
- produz medidas ou indicadores;
- compõe resultados de múltiplas fontes;
- depende de versão, parâmetros ou janela temporal para reprodução;
- pode fundamentar alegação, avaliação ou decisão;
- precisa preservar proveniência e auditoria;
- é uma capacidade computacional transversal reutilizada por múltiplos domínios.

Funções puramente locais, utilitárias ou constitutivas de um único domínio não migram automaticamente para o Praxis.

## 7. Relação com REA, RIT e Reflexa

- **REA** disciplina percepção, interpretação, revisão e pergunta sucessora.
- **RIT** preserva identidade temporal, causal e histórica das transições.
- **SisTer-Praxis Core** materializa as primitivas governadas que tornam transformações e avaliações identificáveis.
- **SisTer-Praxis Runtime** oferece mecanismos compartilhados de execução sem possuir a semântica dos domínios.
- **SisTer-Reflexa** observa e reconstrói trajetórias, inclusive trajetórias computacionais produzidas por sucessivas transformações.

A relação entre Praxis e Reflexa é, portanto, acoplada e não hierárquica:

```text
Praxis:  S0 --T1--> S1 --T2--> S2
                     │
                     ▼
Reflexa: torna a trajetória e suas relações cognoscíveis
                     │
                     ▼
          contexto para novas transformações
```

## 8. Implementação v0.2

### `sister_praxis_core`

Permanece composto por:

- `domain.cpp`;
- `audit.cpp`;
- tipos de evidência, medida, critério, avaliação e decisão;
- cálculo mínimo de cobertura de evidência;
- avaliação mínima por critério;
- audit trail JSONL.

### `sister_praxis_runtime`

Nova biblioteca separada com:

- `TransformationKind`;
- `TemporalContext`;
- `ServiceDescriptor`;
- `TransformationRequest`;
- `TransformationOutput`;
- `TransformationReceipt`;
- `ServiceRegistry`.

O teste de runtime registra um serviço cujo `owner` é externo ao core e verifica que a execução preserva contrato, implementação, predecessor, sucessor e proveniência.

## 9. Fatias verticais revisadas

- **A0**: Praxis Core C++23 + contratos de avaliação + perfis + audit trail local + CLI.
- **A0-R1**: separação explícita Core/Runtime + transformação governada mínima + service registry local.
- **A1**: persistência SQLite para estados, serviços, runs, receipts e avaliações.
- **A2**: contratos completos e validação executável de schemas.
- **A3**: API HTTP executável para core e runtime.
- **A4**: superfície de inspeção auditável: resultados, transformações, avaliações e proveniência.
- **A5**: primeira integração real com um subsistema externo preservando `owner/runtime`.
- **A6**: composição/pipelines e perfis reais de avaliação.
- **A7**: experimentos comparativos, reprodução, confiabilidade e candidatos a promoção metodológica.

A ordem futura deve continuar orientada por evidência: a arquitetura não exige antecipar execução assíncrona, filas, cache ou distribuição antes de um caso real demonstrar sua necessidade.
