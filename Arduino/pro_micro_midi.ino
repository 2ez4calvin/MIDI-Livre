/*
  Arduino Pro Micro - Controlador MIDI USB
  ------------------------------------------
  - 4 Potenciômetros em A0, A1, A2, A3 -> enviam Control Change (CC)
  - 7 Botões em pinos 2..8 -> notas da escala de Dó maior (C D E F G A B)
                              enviam Note On / Note Off
  - 1 Botão no pino 9      -> transposição de oitava (função exclusiva do firmware,
                              não envia mensagem MIDI). A cada acionamento avança
                              para a próxima oitava do ciclo definido em
                              CICLO_OITAVAS; ao chegar ao fim, volta ao início.

  Requer a biblioteca MIDIUSB (Gary Grewal).
  Instale em: Sketch > Incluir Biblioteca > Gerenciar Bibliotecas > "MIDIUSB"
*/

#include <MIDIUSB.h>

// Potenciometros
const uint8_t NUM_POTS = 4;
const uint8_t potPins[NUM_POTS] = {A0, A1, A2, A3};
const uint8_t ccValores[NUM_POTS] = {20, 21, 22, 23};
int potValoresAnt[NUM_POTS] = {-128, -128, -128, -128};
const uint8_t POT_THRESHOLD = 2;

// Botoes: 7 notas (C D E F G A B) + 1 botao de oitava
const uint8_t NUM_BOTOES = 8;
const uint8_t NUM_NOTAS  = 7;
const uint8_t BOTAO_OITAVA = 7;
const uint8_t botaoPins[NUM_BOTOES] = {2, 3, 4, 5, 6, 7, 8, 9};

const uint8_t intervalos[NUM_NOTAS] = {0, 2, 4, 5, 7, 9, 11}; // C D E F G A B


const int8_t CICLO_OITAVAS[] = {2, 3, 4, 5, 6};
const uint8_t NUM_OITAVAS = sizeof(CICLO_OITAVAS) / sizeof(CICLO_OITAVAS[0]);
uint8_t indiceOitava = 2;

uint8_t notaAtiva[NUM_NOTAS];

bool estadoAnt[NUM_BOTOES];
bool estadoAtual[NUM_BOTOES];
unsigned long ultimoDebounce[NUM_BOTOES];
const unsigned long DEBOUNCE_MS = 15;

const uint8_t MIDI_CHANNEL = 0;
const uint8_t VELOCITY = 100;

// ---------- FUNÇÕES MIDI ----------

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, (uint8_t)(0x90 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, (uint8_t)(0x80 | channel), pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, (uint8_t)(0xB0 | channel), control, value};
  MidiUSB.sendMIDI(event);
}

// ---------- OITAVA ----------

// Número MIDI da nota do botão i na oitava atual: Dó da oitava n = 12 * (n + 1)
uint8_t notaDoBotao(uint8_t i) {
  int nota = 12 * (CICLO_OITAVAS[indiceOitava] + 1) + intervalos[i];
  return (uint8_t)constrain(nota, 0, 127);
}

void avancarOitava() {
  indiceOitava = (indiceOitava + 1) % NUM_OITAVAS;
}

void lerBotoes() {
  for (uint8_t i = 0; i < NUM_BOTOES; i++) {
    bool leitura = digitalRead(botaoPins[i]);

    if (leitura != estadoAnt[i]) {
      ultimoDebounce[i] = millis();
    }

    if ((millis() - ultimoDebounce[i]) > DEBOUNCE_MS) {
      if (leitura != estadoAtual[i]) {
        estadoAtual[i] = leitura;
        bool pressionado = (estadoAtual[i] == LOW);

        if (i == BOTAO_OITAVA) {
          if (pressionado) {
            avancarOitava();
          }
        } else if (pressionado) {
          notaAtiva[i] = notaDoBotao(i);
          noteOn(MIDI_CHANNEL, notaAtiva[i], VELOCITY);
        } else {
          noteOff(MIDI_CHANNEL, notaAtiva[i], 0);
        }
      }
    }

    estadoAnt[i] = leitura;
  }
}

void lerPotenciometros() {
  for (uint8_t i = 0; i < NUM_POTS; i++) {
    int leituraAnalogica = analogRead(potPins[i]);
    int valorMidi = leituraAnalogica >> 3;               

    if (abs(valorMidi - potValoresAnt[i]) >= POT_THRESHOLD) {
      controlChange(MIDI_CHANNEL, ccValores[i], valorMidi);
      potValoresAnt[i] = valorMidi;
    }
  }
}


// ---------- SETUP ----------

void setup() {
  for (uint8_t i = 0; i < NUM_BOTOES; i++) {
    pinMode(botaoPins[i], INPUT_PULLUP); 
    estadoAnt[i] = HIGH;
    estadoAtual[i] = HIGH;
    ultimoDebounce[i] = 0;
  }
  for (uint8_t i = 0; i < NUM_NOTAS; i++) {
    notaAtiva[i] = 0;
  }
}

// ---------- LOOP ----------

void loop() {
  lerBotoes();
  lerPotenciometros();
  MidiUSB.flush();
}