#include "Interface/Oled.hpp"
#include <Wire.h>
#include <esp32-hal-log.h>
using namespace Izi::Oled;

// Construtor
OledHUD::OledHUD(uint8_t sdaPin, uint8_t sclPin )
    : u8g2(U8G2_R0, U8X8_PIN_NONE), 
      velocity(25.0), 
      drivingMode("Eco"), 
      kilometers(0) 
      /*bleStatus(bleStatus)*/ {
    Wire.begin(sdaPin, sclPin);
}

// Destrutor
OledHUD::~OledHUD() {}

// Método principal de gerenciamento (pode ser expandido futuramente)
void OledHUD::Manager() {}

// Inicialização do OLED
bool OledHUD::begin() {
    log_d("Iniciando configurações do HUD");
    if (!u8g2.begin()) {
        log_w("Não foi possível iniciar o display OLED...");
        return false;
    }

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
    log_d("Atualizando HUD");

    // Limpa o buffer e redesenha o display
    u8g2.clearBuffer();
    this->drawPanel();
    this->displayData();
    u8g2.sendBuffer();
}

// Define os quilômetros percorridos
void OledHUD::setKilometers(int km) {
    this->kilometers = km;
}

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
    // Bordas e divisões do display
    u8g2.drawFrame(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);  // Borda externa
    // u8g2.drawLine(64, 0, 64, IMAGE_HEIGHT);          // Linha vertical
    // u8g2.drawLine(0, 32, IMAGE_WIDTH, 32);           // Linha horizontal

    // Títulos e status BLE
    u8g2.setCursor(5, 12);
    u8g2.print("Izi Mobile");
    u8g2.setCursor(72, 12);
    u8g2.print(bleStatus);
}

// Método para exibir dados dinâmicos
void OledHUD::displayData() {
u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(90, 10, "BAT:");
  u8g2.drawBox(110, 3, 15, 7); // Ícone da bateria
  u8g2.drawBox(111, 4, 13, 5); // Nível da bateria
  u8g2.drawStr(115, 20, "85%"); // Porcentagem

  // Velocidade
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setCursor(10, 53);
  u8g2.print("25"); // Velocidade
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(50, 52, "km/h"); // Unidade

  // Modo
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(100, 35, "FOC"); // Modo atual

  // Distância
  u8g2.drawStr(100, 58, "12.5 km"); // Distância percorrida

  u8g2.sendBuffer(); // Atualiza o display
}
