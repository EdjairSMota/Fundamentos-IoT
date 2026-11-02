# ICC316 — Tópico 7: Armazenamento Persistente

Exemplos Python correspondentes aos slides 31–59, destinados à atividade
**Do Gateway ao InfluxDB 2: Armazenamento Persistente de Dados IoT**.

## Pré-requisitos

InfluxDB 2 instalado e em execução no Raspberry Pi 3 Modelo B; organização
e bucket configurados; Python 3 com `venv`.

## Preparação

```bash
cd Topico_7_Armazenamento_Persistente
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp .env.example .env
```

Edite `.env` com os parâmetros reais. **Nunca publique `.env` ou tokens no GitHub.**

## Executar na sequência

```bash
python exemplos/01_conexao_influxdb.py
python exemplos/02_gravar_ponto.py
python exemplos/04_processar_mensagem.py
python exemplos/03_importar_historico.py dados/exemplo_medicoes.csv
```

No Data Explorer, consulte o bucket `dados_agua`. O CSV ilustrativo
contém registros de **18/10/2026**; ajuste a faixa temporal da consulta.

## Correspondência com os slides

| Arquivo | Objetivo | Slides |
| --- | --- | --- |
| `01_conexao_influxdb.py` | Verificar a saúde do InfluxDB 2 | 31–33 |
| `02_gravar_ponto.py` | Criar e gravar um Point | 43–45 |
| `03_importar_historico.py` | Importar CSV histórico | 46–50 |
| `04_processar_mensagem.py` | Interpretar JSON e construir Point | 53–55, 58 |
| `05_ingestao_continua.py` | Estrutura do gateway com gravação | 56–59 |

`exemplos/comum.py` contém as funções compartilhadas.

## Atividade dos grupos

1. Adaptar a importação ao CSV real do datalogger ou gateway.
2. Preservar timestamps originais e não inventar campos de sensores ausentes.
3. Completar `receber_mensagem()` com o receptor Wi-Fi ou Bluetooth já criado.
4. Verificar os registros no Data Explorer.
5. Investigar dados inválidos, reinicialização e indisponibilidade do banco.

**Atenção:** `gateway/05_ingestao_continua.py` é propositalmente incompleto:
o `NotImplementedError` marca a integração a ser feita pelos alunos.

Estes exemplos didáticos não implementam filas persistentes ou retransmissão
automática. Escrita sem erro não substitui verificação por consulta. Pontos com
mesma measurement, tags e timestamp podem coincidir no InfluxDB.
