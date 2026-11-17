# Controladora MIDI de baixo custo

Controladora MIDI USB com 8 botões e 4 potenciômetros, construída com Arduino Pro Micro (ATmega32U4) e *enclosure* impresso em 3D. Ao ser conectada ao computador, é reconhecida como dispositivo MIDI genérico (*plug-and-play*), sem *drivers* nem fonte externa.

> Projeto desenvolvido no IFG – Câmpus Formosa. Veja o texto completo da modelagem e construção em `docs/`.

## Sumário

- [Funcionalidades](#funcionalidades)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Lista de materiais](#lista-de-materiais)
- [1. Montagem do circuito](#1-montagem-do-circuito)
- [2. Impressão 3D](#2-impressão-3d)
- [3. Firmware](#3-firmware)
- [4. Testando e usando na DAW](#4-testando-e-usando-na-daw)
- [Personalização](#personalização)
- [Limitações](#limitações)
- [Solução de problemas](#solução-de-problemas)
- [Autoria](#Autoria)

## Funcionalidades

- **7 botões de nota** na escala de Dó maior (Dó, Ré, Mi, Fá, Sol, Lá, Si), enviando *Note On* e *Note Off*.
- **1 botão de oitava** que percorre ciclicamente as oitavas 2 a 6 (padrão: oitava 4, Dó central = nota MIDI 60).
- **4 potenciômetros** enviando *Control Change* (CC 20 a 23), números sem função predefinida no padrão MIDI. Eles só atuam no som depois de mapeados na DAW.
- *Debounce* por contagem de tempo (sem `delay()`), garantindo baixa latência.
- Registro da nota acionada, o que evita notas presas se a oitava mudar com uma tecla pressionada.
- Canal MIDI 1 e velocidade fixa em 100.

## Estrutura do repositório

```
.
├── Firmware/
│   └── MIDI_Livre.ino
├── Esquema Fritzing/
│   ├── MIDI Livre.fzz
│   └── MIDI Livre.jpg
├── Modelos 3D/
│   └──Arquivos STL
│       ├── Caixa.stl
│       ├── Tampa Deslizante.stl
│       └── Suporte Arduino.stl
│   └──Imagens
│       ├── Caixa Interior.png
│       ├── Caixa.png
│       ├── Suporte Arduino.png
│       ├── Tampa Deslizante
└── README.md
```

## Lista de materiais

| Qtd. | Componente | Observação |
|:---:|---|---|
| 1 | Arduino Pro Micro ATmega32U4 **5 V** | USB nativo |
| 8 | Chave botão R13-507 | Rosca M16 × 1,0, acionamento momentâneo |
| 4 | Potenciômetro linear 10 kΩ | Eixo para *knob* |
| 1 | Cabo micro USB (30 cm) | Dados + alimentação |
| 1 | Cabo *flat* 20 vias 26 AWG (1 m) | Ligação entre placa e interface |
| 4 | *Knobs* para potenciômetro | Ajuste ao eixo do seu modelo |
| ~ | Filamento PLA | Cerca de três peças, 3 h 10 min de impressão |

Custo estimado dos componentes eletrônicos: **R$ 71,73** (valores de referência, variam conforme a loja).


## 1. Montagem do circuito

O circuito é simples e dispensa resistores externos, pois os botões usam o *pull-up* interno do microcontrolador.

### Botões

Cada botão tem um terminal ligado ao **GND** e o outro ao pino digital indicado:

| Índice | Pino | Função |
|:---:|:---:|---|
| 0 | 2 | Dó |
| 1 | 3 | Ré |
| 2 | 4 | Mi |
| 3 | 5 | Fá |
| 4 | 6 | Sol |
| 5 | 7 | Lá |
| 6 | 8 | Si |
| 7 | 9 | Oitava |

### Potenciômetros

Os terminais externos vão a **VCC (5 V)** e **GND**, e o terminal central (cursor) vai à entrada analógica:

| Potenciômetro | Pino | CC |
|:---:|:---:|:---:|
| 1 | A0 | 20 |
| 2 | A1 | 21 |
| 3 | A2 | 22 |
| 4 | A3 | 23 |

O arquivo `Esquema Fritzing/MIDI Livre.fzz` traz o esquema de conexões em *protoboard*. Recomenda-se testar tudo na *protoboard* antes de soldar.

### Dica de montagem

Fixe botões e potenciômetros no painel superior da caixa **antes** de soldar. Assim, a tampa inferior pode ser removida sem deslocar a fiação.

## 2. Impressão 3D

Modelo desenvolvido no Tinkercad e exportado em STL. São três peças que se encaixam sem parafusos ou cola:

| Peça | Arquivo | Tempo aprox. |
|---|---|:---:|
| Caixa principal (150 × 95 × 36 mm) | `stl/caixa.stl` | 1 h 55 min |
| Tampa inferior deslizante | `stl/tampa_deslizante.stl` | 20 min |
| Suporte do Arduino Pro Micro | `stl/suporte_arduino.stl` | 55 min |

### Parâmetros utilizados

Impressora: Bambu Lab A1. Fatiador: Bambu Studio. Filamento: PLA (*Speed Premium*, 3D LAB).

| Parâmetro | Valor |
|---|---|
| Bico | 220 °C |
| Mesa | 65 °C |
| Superfície superior | *Monotonic line* |
| Superfície inferior | *Monotonic* |
| Sólido interno | *Rectilinear* |
| Preenchimento esparso | *Grid* |

### Orientação e suportes

- **Caixa principal:** imprimir com a **face superior (painel) apoiada na mesa**, para obter acabamento liso na face visível.
- **Suportes tipo árvore (*tree support*):** habilitar na caixa, para sustentar os trilhos do suporte do Arduino, e no suporte do Arduino, para as abas de travamento.
- **Tampa e suporte do Arduino:** imprimir na orientação original de montagem.

### Tolerâncias

O projeto prevê folga de cerca de 0,3 mm nos furos e 0,25 mm na tampa. Se sua impressora tiver tolerância diferente, imprima primeiro um teste (ou só a tampa) e ajuste o modelo ou a calibração de fluxo.

### Montagem mecânica

1. Fixe os botões (furos de 16,3 mm, com faces planas que impedem a rotação) e os potenciômetros (furos de 7,4 mm, com pino anti-rotação alojado no orifício cego na face inferior do painel).
2. Solde o cabo aos terminais e ao Arduino (ou use fios, conforme o esquema).
3. Encaixe o Arduino no suporte e deslize o suporte pelos trilhos tipo cauda de andorinha da parede traseira. A abertura de 12 × 8 mm na parede traseira recebe o conector USB.
4. Deslize a tampa inferior pela lateral direita até sentir o clique da trava. Ela também prende o suporte na posição final.
5. Coloque os *knobs*.

## 3. Firmware

### Requisitos

- [Arduino IDE](https://www.arduino.cc/en/software/) (2.x recomendado)
- Biblioteca **MIDIUSB** (Gerenciador de Bibliotecas: *Sketch → Incluir Biblioteca → Gerenciar Bibliotecas → "MIDIUSB"*)

### Gravação

1. Abra `Firmware/MIDI Livre.ino`.
2. Em *Ferramentas → Placa*, selecione **Arduino Leonardo** (o Pro Micro de 5 V usa o mesmo ATmega32U4 e é compatível). Alternativamente, instale o pacote SparkFun AVR Boards e escolha **SparkFun Pro Micro** (5 V, 16 MHz).
3. Em *Ferramentas → Porta*, selecione a porta da placa.
4. Clique em **Carregar**.

Após a gravação, o computador passa a reconhecer a placa como um dispositivo MIDI USB.

### Principais parâmetros configuráveis

| Constante | Padrão | Descrição |
|---|:---:|---|
| `ccValores` | `{20, 21, 22, 23}` | Números de CC dos potenciômetros |
| `POT_THRESHOLD` | `2` | Variação mínima para enviar novo CC (aumente se houver ruído) |
| `DEBOUNCE_MS` | `15` | Tempo de estabilização dos botões (ms) |
| `CICLO_OITAVAS` | `{2, 3, 4, 5, 6}` | Oitavas percorridas pelo botão de transposição |
| `indiceOitava` | `2` | Posição inicial no ciclo (oitava 4) |
| `MIDI_CHANNEL` | `0` | Canal MIDI (0 = canal 1 nas DAWs) |
| `VELOCITY` | `100` | Velocidade fixa das notas |

## 4. Testando e usando na DAW

### Teste rápido

Conecte a controladora e abra um monitor MIDI:

- **Windows:** [MIDI-OX](http://www.midiox.com/) ou MIDIberry
- **macOS:** MIDI Monitor
- **Linux:** `aseqdump -l` (lista as portas) e `aseqdump -p <porta>`
- **Qualquer sistema:** testadores baseados em Web MIDI (navegadores Chromium)

Ao pressionar o botão **Dó** na oitava padrão, deve chegar um *Note On* canal 1, nota 60, velocidade 100. Ao soltar, um *Note Off* da mesma nota. Girar um potenciômetro deve gerar mensagens *Control Change* (CC 20 a 23, valores 0 a 127).

### Uso em DAW

1. Em qualquer DAW (Ableton Live, FL Studio, Reaper, Reason, Logic, etc.), habilite o dispositivo nas preferências de MIDI como entrada.
2. Arme uma pista de instrumento virtual para tocar com os botões.
3. Para os potenciômetros, use a função de **mapeamento MIDI** da DAW (*MIDI Learn*): ative o modo, clique no parâmetro desejado e gire o potenciômetro.

> **Nota sobre oitavas:** o firmware usa a convenção Dó central (C4) = nota 60. Algumas DAWs (como Ableton Live e FL Studio) exibem a nota 60 como **C3**. Isso é só uma diferença de rótulo.

## Personalização

- **Outros CCs:** altere `ccValores` (CCs 20 a 31 são indefinidos no padrão MIDI).
- **Outra escala:** altere o vetor `intervalos` (semitons a partir da tônica).
- **Outras oitavas:** edite `CICLO_OITAVAS`. Valores negativos são suportados.
- **Outro canal:** altere `MIDI_CHANNEL` (0 a 15).

## Limitações

- Velocidade fixa, pois os botões não detectam a força do toque.
- Apenas as sete notas naturais da escala de Dó maior, sem acidentes. Use o botão de oitava ou transponha na DAW.
- Sem polifonia limitada por hardware, mas o número de teclas simultâneas depende do arranjo físico dos botões.

## Solução de problemas

| Problema | Possível causa e solução |
|---|---|
| Placa não aparece na porta | Cabo USB só de carga. Use um cabo de dados. |
| Falha ao carregar o firmware | Dê dois toques rápidos no pino RST para GND para entrar no *bootloader* e tente novamente. |
| Computador não reconhece como MIDI | Confirme que o firmware foi gravado e que a biblioteca MIDIUSB está instalada. |
| Nota dispara várias vezes | Aumente `DEBOUNCE_MS`. Confira a solda do botão. |
| CCs "tremendo" sozinhos | Aumente `POT_THRESHOLD`. Verifique GND e 5 V dos potenciômetros. |
| Botão não responde | Confirme a ligação botão → pino digital e GND (lógica *pull-up*: pressionado = nível baixo). |
| Peças não encaixam | Ajuste a calibração de tolerância/fluxo da impressora ou a folga no modelo. |

## Autoria

Pedro Pinheiro Saad Batista — TADS, IFG – Câmpus Formosa. Orientador(a): Prof. Dr. João Ricardo Braga de Paiva.
