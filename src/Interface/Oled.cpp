#include "Interface/Oled.hpp"
#include <Wire.h>
#include <esp32-hal-log.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>



using namespace Izi::Oled;

void vTaskOled(){

}

// Construtor
OledHUD::OledHUD(uint8_t sdaPin, uint8_t sclPin )
    : u8g2(U8G2_R0, U8X8_PIN_NONE)
      /*bleStatus(bleStatus)*/ {
    Wire.begin(sdaPin, sclPin);
}


// Destrutor
OledHUD::~OledHUD() {}

// Método principal de gerenciamento (pode ser expandido futuramente)
void OledHUD::Manager() {

}

// Inicialização do OLED
bool OledHUD::begin() {
    log_d("Iniciando configurações do HUD");
    if (!u8g2.begin()) {
        log_w("Não foi possível iniciar o display OLED...");
        return false;
    }

    this->km_h = 0;
    this->bat = 0;
    this->mode = "eco";

    // Configuração inicial do display
    u8g2.setFont(u8g2_font_ncenB08_tr);

    // Exibe o painel inicial
    u8g2.firstPage();
    do {
        drawPanel();
    } while (u8g2.nextPage());

    updateHUD();

    return true;
}

// Atualiza os dados do HUD
void OledHUD::updateHUD() {
    // Limpa o buffer e redesenha o display
    u8g2.clearBuffer();
    OledHUD::displayData();
    u8g2.updateDisplay();
}

// Define os quilômetros percorridos

// Define a velocidade
void OledHUD::setVelocity(float speed) {
    this->velocity = speed;
}

// Define o status BLE
void OledHUD::setBLEStatus(const char* status) {
    this->bleStatus = status;
}

// Método para desenhar o painel estático
void OledHUD::drawPanel() {
    u8g2.drawFrame(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);  // Borda externa
    // Títulos e status BLE
    u8g2.setCursor(72, 12);
    u8g2.print(bleStatus);
}

// Método para exibir dados dinâmicos
void OledHUD::displayData() {
    uint8_t bateria = this->bat;
    uint8_t velocidade = this->velocity;
    const char *modo = this->mode;

    u8g2.clearBuffer();
    OledHUD::drawPanel();
    this->mode = "ECO";
    
    // Limpeza da área onde a bateria está exibida
    u8g2.setDrawColor(0);  // Defina a cor para "limpar" a área (cor de fundo)
    u8g2.drawBox(90, 0, 50, 10);  // Apaga a área onde está o texto "BAT:"
    u8g2.setDrawColor(1);  // Defina a cor de desenho (cor do texto)
    
    // Bateria
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setCursor(5, 12);
    u8g2.print("Izi Mobile");
    u8g2.drawStr(90, 10, "BAT:");
    u8g2.drawBox(110, 3, 15, 7); // Ícone da bateria
    u8g2.drawBox(111, 4, 13, 5); // Nível da bateria
    u8g2.drawStr(115, 20, String(bateria).c_str()); // Porcentagem

    // Limpeza da área onde a velocidade está exibida
    u8g2.setDrawColor(0);
    u8g2.drawBox(10, 40, 50, 20);  // Apaga a área onde a velocidade aparece
    u8g2.setDrawColor(1);
    
    // Velocidade
    u8g2.setFont(u8g2_font_logisoso24_tr);
    u8g2.setCursor(10, 53);
    u8g2.drawStr(10, 53, String(velocidade).c_str()); // Velocidade
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(50, 52, "km/h"); // Unidade

    // Modo
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(100, 35, this->mode); // Modo atual

    // Distância
    u8g2.drawStr(100, 58, "OK"); // Distância percorrida
    
    u8g2.sendBuffer();
}
bool OledHUD::setDisplay(uint8_t Velocidade, uint8_t bateria, const char* modo) {

    this->velocity = Velocidade;
    this->bat = bateria;
    this->mode = modo;

    return true;
}

