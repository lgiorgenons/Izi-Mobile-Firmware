#include "Encoder.hpp"

// Inicializa o ponteiro estático
Encoder* Encoder::instance = nullptr;

// Construtor
Encoder::Encoder(uint8_t pinA, uint8_t pinB, const String& mode)
    : pinA(pinA), pinB(pinB), mode(mode), encoder_counter(0), chA(0), chB(0), lut_index(0), dir(0), prev_position(0) {
    // Atribui a instância estática
    instance = this;
}

// Função para preencher as lookup tables
void Encoder::fillLookupTable() {
    // Lookup table para X2
    this->x2qei_lut[0]  = -2; x2qei_lut[1]  =  0; x2qei_lut[2]  =  0; x2qei_lut[3]  = -1;
    this->x2qei_lut[4]  =  0; x2qei_lut[5]  = +2; x2qei_lut[6]  = +1; x2qei_lut[7]  =  0;
    this->x2qei_lut[8]  =  0; x2qei_lut[9]  = +1; x2qei_lut[10] = +2; x2qei_lut[11] =  0;
    this->x2qei_lut[12] = -1; x2qei_lut[13] =  0; x2qei_lut[14] =  0; x2qei_lut[15] = -2;

    // Lookup table para X4
    // Direção = 0 (CW)
    x4qei_lut[0] = 0; x4qei_lut[1] = -1; x4qei_lut[2] = +1; x4qei_lut[3] = +2;
    x4qei_lut[4] = +1; x4qei_lut[5] = 0; x4qei_lut[6] = +2; x4qei_lut[7] = -1;
    x4qei_lut[8] = -1; x4qei_lut[9] = +2; x4qei_lut[10] = 0; x4qei_lut[11] = +1;
    x4qei_lut[12] = +2; x4qei_lut[13] = +1; x4qei_lut[14] = -1; x4qei_lut[15] = 0;

    // Direção = 1 (CCW)
    x4qei_lut[16] = 0; x4qei_lut[17] = -1; x4qei_lut[18] = +1; x4qei_lut[19] = -2;
    x4qei_lut[20] = +1; x4qei_lut[21] = 0; x4qei_lut[22] = -2; x4qei_lut[23] = -1;
    x4qei_lut[24] = -1; x4qei_lut[25] = -2; x4qei_lut[26] = 0; x4qei_lut[27] = +1;
    x4qei_lut[28] = -2; x4qei_lut[29] = +1; x4qei_lut[30] = -1; x4qei_lut[31] = 0;
}

// Inicializa os índices
void Encoder::initIndex() {
    this->lut_index |= digitalRead(pinA) << 3;
    this->lut_index |= digitalRead(pinB) << 2;
    this->lut_index &= 0b00001100;
}

// Configuração inicial
void Encoder::begin() {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    this->fillLookupTable();
    this->initIndex();

    if (mode == "x1" || mode == "X1") {
        attachInterrupt(digitalPinToInterrupt(pinA), isrReadEncoderX1, RISING);
    } else if (mode == "x2" || mode == "X2") {
        attachInterrupt(digitalPinToInterrupt(pinA), isrReadEncoderX2, CHANGE);
    } else if (mode == "x4" || mode == "X4") {
        attachInterrupt(digitalPinToInterrupt(pinA), isrReadEncoderX4, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pinB), isrReadEncoderX4, CHANGE);
    }
}


// Inicializa as interrupções com base no modo
void Encoder::initInterrupts() {
    if (mode == "x1" || mode == "X1") {
        attachInterrupt(digitalPinToInterrupt(pinA), Encoder::isrReadEncoderX1, RISING);
    } else if (mode == "x2" || mode == "X2") {
        attachInterrupt(digitalPinToInterrupt(pinA), Encoder::isrReadEncoderX2, CHANGE);
    } else if (mode == "x4" || mode == "X4") {
        attachInterrupt(digitalPinToInterrupt(pinA), Encoder::isrReadEncoderX4, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pinB), Encoder::isrReadEncoderX4, CHANGE);
    }
}

// Função ISR para modo X1
void Encoder::isrReadEncoderX1() {
    instance->isrReadEncoderX1Instance();
}

// Função ISR para modo X2
void Encoder::isrReadEncoderX2() {
    instance->isrReadEncoderX2Instance();
}

// Função ISR para modo X4
void Encoder::isrReadEncoderX4() {
    instance->isrReadEncoderX4Instance();
}

// Implementação dos ISRs de instância
void Encoder::isrReadEncoderX1Instance() {
    chA = (uint8_t)(REG_READ(GPIO_IN_REG) >> pinA) & 0x1;
    chB = (uint8_t)(REG_READ(GPIO_IN_REG) >> pinB) & 0x1;
    encoder_counter += (chA != chB) ? 1 : -1;
}

void Encoder::isrReadEncoderX2Instance() {
    chA = (uint8_t)(REG_READ(GPIO_IN_REG) >> pinA) & 0x1;
    chB = (uint8_t)(REG_READ(GPIO_IN_REG) >> pinB) & 0x1;
    lut_index |= (chA << 1) | chB;
    encoder_counter += x2qei_lut[lut_index];
    lut_index = ((lut_index << 2) & 0b00001100);
}

void Encoder::isrReadEncoderX4Instance() {
    chA = (uint8_t)(REG_READ(GPIO_IN_REG) >> pinA) & 0x1;
    chB = (uint8_t)(REG_READ(GPIO_IN_REG) >> pinB) & 0x1;
    dir = (chA != chB) ? 0b00000000 : 0b00010000;
    lut_index |= dir | (chA << 1) | chB;
    encoder_counter += x4qei_lut[lut_index];
    lut_index = ((lut_index << 2) & 0b00001100);
}

// Obtém a posição atual
int32_t Encoder::getPosition() {
    return encoder_counter;
}
