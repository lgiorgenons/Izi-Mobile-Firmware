#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <Arduino.h>

#define ENCODER_CLK_PIN     25
#define ENCODER_DT_PIN      26
#define ENCODER_SW_PIN      27
#define ENCODER_MODE        "x4"

#define MAX_SET_POINT_ENCODER 100
#define MIN_SET_POINT_ENCODER 0

    static const uint8_t encoder_clk_pin = ENCODER_CLK_PIN;
    static const uint8_t encoder_dt_pin = ENCODER_DT_PIN;
    static const uint8_t encoder_sw_pin = ENCODER_SW_PIN;
    static const String encoder_mode = ENCODER_MODE;

class Encoder {

private:
    // Pinos do encoder
    uint8_t pinA, pinB;
    // Modo de operação: "x1", "x2", "x4"
    String mode;
    // Lookup tables
    int8_t x2qei_lut[16], x4qei_lut[32];
    // Variáveis globais
    volatile int32_t encoder_counter;
    volatile uint8_t chA, chB, lut_index, dir;
    int32_t prev_position;
    // Método privado para inicializar as interrupções
    void initInterrupts();

    // Ponteiro estático para a instância
    static Encoder* instance;

    // Métodos estáticos para interrupções
    static void isrReadEncoderX1();
    static void isrReadEncoderX2();
    static void isrReadEncoderX4();

           void getReadEncoder();

public:
    // Construtor
    Encoder(uint8_t pinA, uint8_t pinB, const String& mode = "x1");
    uint32_t getKM();
    // Configuração inicial
    void begin();

    // Funções ISR (interrupções)
    void isrReadEncoderX1Instance();
    void isrReadEncoderX2Instance();
    void isrReadEncoderX4Instance();

    void initIndex();

    void fillLookupTable();
    // Leitura da posição atual
    int32_t getPosition();
};


#endif // ENCODER_HPP
