#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include <Arduino.h>
#include <VescUart/src/VescUart.h>
#include <VescUart/src/datatypes.h>
#include <VescUart/src/buffer.h>
#include <VescUart/src/crc.h>
#include <cstring>
#include <BluetoothSerial.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

class Bluetooth {
private:
mc_values vescMeasuredValues; // Substitui VescMeasuredValues_51_teste

    bool connected;                    // Estado da conexão Bluetooth
    String connectedDeviceName;        // Nome do dispositivo conectado
    float voltage;                     // Tensão atual medida pela VESC
    float current;                     // Corrente atual medida pela VESC
    float rpm;                         // RPM atual medido pela VESC

    int check_crc(uint8_t* messageReceived, int message_len);  // Verifica CRC
    void parse_vesc_data(uint8_t* data, int len);              // Interpreta dados do VESC

public:
    Bluetooth();
    ~Bluetooth();

    void begin(const char* deviceName);                       // Inicializa o Bluetooth
    bool connectToDevice(const char* deviceName);             // Conecta a um dispositivo BLE
    bool reconnect();                                         // Reconexão automática

    int send_payload(uint8_t* payload, int lenPay);           // Envia dados para o VESC
    void sendCommand(const String& command);                  // Envia comandos genéricos
    void receiveData();                                       // Recebe dados do VESC
    void stop();                                              // Desconecta do dispositivo

    bool isConnected();                                       // Retorna o estado da conexão

    // Funções para controle do motor
    void setTargetCurrent(float targetCurrent);               // Configura corrente alvo
    void setTargetRPM(float targetRPM);                       // Configura RPM alvo
    void setDutyCycle(float dutyCycle);                       // Configura Duty Cycle
    void setVescMode(uint8_t mode);                           // Configura modos no VESC

    // Funções para coleta de dados
    int get_vesc_values_51();                                 // Obtém valores do VESC (modelo 51)
    int get_vesc_data();                                      // Captura dados do VESC

    // Getters para dados coletados
    float getVoltage() const { return voltage; }              // Retorna a tensão
    float getCurrent() const { return current; }              // Retorna a corrente
    float getRPM() const { return rpm; }                      // Retorna o RPM
};

#endif

