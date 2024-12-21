#include "Bluetooth.hpp"

// Construtor da classe Bluetooth
Bluetooth::Bluetooth(void) {
    this->connected = false;
}

// Inicializa o dispositivo BLE
bool Bluetooth::begin(const std::string& deviceName) {
    BLEDevice::init(deviceName.c_str());  // Inicializa o dispositivo BLE com nome
    pServer = BLEDevice::createServer();
    
    // Criando um serviço BLE
    BLEService *pService = pServer->createService(BLEUUID((uint16_t)0xFFE0));

    // Criando uma característica dentro do serviço
    pCharacteristic = pService->createCharacteristic(
        BLEUUID((uint16_t)0xFFE1),
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setValue("Hello BLE");

    // Inicia a propagação
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start(); // Inicia a propagação do nome do dispositivo BLE
    pService->start(); // Inicia o serviço BLE
    return true;
}

// Conecta-se a um dispositivo BLE pelo nome
bool Bluetooth::connectToDevice(const std::string& vescDeviceName) {
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true); // Ativa a varredura ativa
    pBLEScan->start(5); // Inicia a busca de dispositivos por 5 segundos
    
    // Verifica os dispositivos encontrados
    BLEAdvertisedDevice* foundDevice = nullptr;
    for (int i = 0; i < pBLEScan->getResults().getCount(); i++) {
        BLEAdvertisedDevice advertisedDevice = pBLEScan->getResults().getDevice(i);
        if (advertisedDevice.getName().compare(vescDeviceName) == 0) {
            foundDevice = &advertisedDevice;
            break;
        }
    }

    if (foundDevice != nullptr) {
        myDevice = foundDevice;
        this->connected = true;
        return true;
    } else {
        return false;
    }
}

// Desconecta o dispositivo BLE
void Bluetooth::disconnect(void) {
    if (this->connected && myDevice != nullptr) {
        BLEClient* client = BLEDevice::createClient(); // Certifique-se de ter uma instância do cliente
        client->disconnect();                         // Desconecta do dispositivo
        this->connected = false;
    }
}


// Verifica se está conectado
bool Bluetooth::isConnected(void) {
    return this->connected;
}

// Envia um comando via BLE
int Bluetooth::sendCommand(uint8_t* command, int len) {
    if (!this->connected) return -1;
    
    // Envia o comando via BLE
    pCharacteristic->setValue(command, len);
    pCharacteristic->notify();
    return len;
}

// Recebe dados via BLE (a lógica de leitura depende do protocolo específico)
int Bluetooth::receiveData(uint8_t* buffer, int bufferSize) {
    if (!this->connected) return -1;

    // Aqui você pode implementar a lógica de leitura de dados via BLE (dependendo do protocolo)
    // Para simplicidade, vou apenas retornar 0 como se não houvesse dados.
    return 0;
}

// Processa os dados recebidos e verifica o CRC, como no código anterior
bool Bluetooth::processReceivedData(uint8_t* payload, int len) {
    if (len < 5) return false; // Mensagem muito curta
    // Processa os dados recebidos e verifica o CRC, como no código anterior
    return true;
}

// Configura o motor com um comando
bool Bluetooth::setMotorConfiguration(uint8_t mode) {
    uint8_t command[2] = { 0x01, mode }; // Exemplo de comando
    return sendCommand(command, sizeof(command)) > 0;
}

// Configura o FOC
bool Bluetooth::setFOCConfiguration(void) {
    uint8_t command[1] = { 0x02 }; // Exemplo de comando
    return sendCommand(command, sizeof(command)) > 0;
}

// Configura o ADC Pas Mode
bool Bluetooth::setAdcPasMode(void) {
    uint8_t command[1] = { 0x03 }; // Exemplo de comando
    return sendCommand(command, sizeof(command)) > 0;
}

// Configuração personalizada
bool Bluetooth::setCustomConfiguration(uint8_t* config, int configLen) {
    uint8_t command[configLen + 1];
    command[0] = 0x04; // Exemplo de comando
    memcpy(&command[1], config, configLen);
    return sendCommand(command, configLen + 1) > 0;
}
