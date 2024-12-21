#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include <Arduino.h>
#include <buffer.h>
#include <cstring>
#include <datatypes.h>

// Comandos do protocolo VESC
#define COMM_SET_FOC 6          // Configuração para modo FOC
#define COMM_SET_ADC 7          // Configuração para modo ADC
#define COMM_CUSTOM_CONFIG 9    // Comando para configuração personalizada

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <string>

class Bluetooth {
private:
    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    bool connected;
    BLEAdvertisedDevice* myDevice;

public:
    Bluetooth(void);
    bool begin(const std::string& deviceName);
    bool connectToDevice(const std::string& vescDeviceName);
    void disconnect(void);
    bool isConnected(void);
    int sendCommand(uint8_t* command, int len);
    int receiveData(uint8_t* buffer, int bufferSize);
    bool processReceivedData(uint8_t* payload, int len);

    // Métodos de configuração
    bool setMotorConfiguration(uint8_t mode);
    bool setFOCConfiguration(void);
    bool setAdcPasMode(void);
    bool setCustomConfiguration(uint8_t* config, int configLen);
};

#endif // BLUETOOTH_HPP
