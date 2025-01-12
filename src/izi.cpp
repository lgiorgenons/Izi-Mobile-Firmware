#include "izi.hpp"

#include "esp_gatts_api.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

using namespace Izi;
using namespace Izi::Oled;

SemaphoreHandle_t positionMutex;
BLEScan* pBLEScan;
BLEAdvertisedDevice* myDevice;
OledHUD oledHUD(Oled::SDA_PIN, Oled::SCL_PIN);
BLEClient* pClient;
bool deviceConnected = false;
const char* deviceName = "VESC BLE UART";  // Substitua pelo nome do dispositivo nRF51/Flipsky

mc_values decodeMcValues(const std::string& rawData) {
    mc_values values;

    // Certifique-se de que o tamanho dos dados seja suficiente
    if (rawData.size() < sizeof(mc_values)) {
        Serial.println("Dados recebidos são menores do que esperado.");
        return values;
    }

    // Copie os dados binários para a struct
    memcpy(&values, rawData.data(), sizeof(mc_values));

    return values;
}


void readVescData() {
    if (!deviceConnected) {
        Serial.println("Dispositivo não conectado. Não é possível ler dados.");
        return;
    }

    // UUIDs específicos do serviço e característica BLE da VESC
    const char* serviceUUID = "0000xxxx-0000-1000-8000-00805f9b34fb"; // Substitua pelos UUIDs corretos
    const char* characteristicUUID = "0000yyyy-0000-1000-8000-00805f9b34fb";

    // Obtenha o serviço da VESC
    BLERemoteService* pService = pClient->getService(serviceUUID);
    if (pService == nullptr) {
        Serial.println("Serviço BLE da VESC não encontrado.");
        return;
    }

    // Obtenha a característica para leitura dos dados
    BLERemoteCharacteristic* pCharacteristic = pService->getCharacteristic(characteristicUUID);
    if (pCharacteristic == nullptr) {
        Serial.println("Característica BLE da VESC não encontrada.");
        return;
    }

    // Leia os dados da característica
    if (pCharacteristic->canRead()) {
        std::string rawData = pCharacteristic->readValue();

        // Decodificar os dados para a struct mc_values
        mc_values values = decodeMcValues(rawData);

        // Exibir os valores lidos
        Serial.printf("Tensão: %.2f V, Corrente: %.2f A, RPM: %.2f\n", 
                      values.v_in, values.current_motor, values.rpm);
    } else {
        Serial.println("Não é possível ler a característica.");
    }
}
 
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("Dispositivo encontrado: ");
    Serial.println(advertisedDevice.toString().c_str());

    // Verifique se o nome do dispositivo é o que você está procurando
    if (advertisedDevice.haveName()) {
      Serial.print("Nome do dispositivo: ");
      Serial.println(advertisedDevice.getName().c_str());
    } else {
      Serial.println("Nome do dispositivo não disponível.");
    }

    // Verifique se o nome do dispositivo é o que você está procurando
    if (advertisedDevice.haveName() && advertisedDevice.getName() == deviceName) {
      Serial.println("Dispositivo VESC encontrado!");
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      pBLEScan->stop();  // Parar a varredura
    } else if (!advertisedDevice.haveName()) {
      Serial.println("Dispositivo sem nome encontrado.");
    }
  }
};

void vTaskControll(void *pvParameters) {
    Encoder myencoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_MODE);
    Bluetooth ble;
    // OledHUD oledHUD(Oled::SDA_PIN, Oled::SCL_PIN);
    int32_t position;

    myencoder.begin();
    VescUart vesc(100); // Timeout de 100ms
    vesc.setSerialPort(&Serial1); // Configura a porta serial
    vesc.setCurrent(1.0); // Define a corrente do motor para 5A

    while (true) {
      
        uint8_t bat = 47;
        // Protege o acesso à variável position
        xSemaphoreTake(positionMutex, portMAX_DELAY);
        position = myencoder.getKM();
        xSemaphoreGive(positionMutex);

        oledHUD.setDisplay(position, bat, "PAS");
        oledHUD.updateHUD();
        Serial.println(vesc.data.rpm);
        Serial.println(vesc.data.inpVoltage);
        Serial.println(vesc.data.ampHours);
        Serial.println(vesc.data.tachometerAbs);
        vTaskDelay(200);
    }
}

void Izi::Controll::begin() {
    Encoder myencoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_MODE);
    Bluetooth ble;

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial2.begin(115200 | SERIAL_8N1, 16, 17);
    ble.begin("esp32IZi");
    log_d("Iniciando Bluetooth: izi_mobile");

    if (!oledHUD.begin()) {
        log_w("Nao foi possivel iniciar o display...");
    }

Serial.println("Iniciando teste BLE");

  BLEDevice::init("Esp32 Device");  // Inicializa o BLE
  pBLEScan = BLEDevice::getScan();  // Obtenha o scanner BLE
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->start(0);  // Comece a varredura BLE

  Serial.println("Aguardando dispositivo...");
    
// Espera até que um dispositivo válido seja encontrado
  if (myDevice && !deviceConnected) {
    Serial.println("Tentando conectar ao VESC...");

    // Tente conectar ao dispositivo encontrado
    pClient = BLEDevice::createClient();
    Serial.println("Conectando...");
    if (pClient->connect(myDevice)) {
      deviceConnected = true;
      Serial.println("Conexão bem-sucedida!");
    } else {
      Serial.println("Falha ao conectar.");
    }
  }

  if (deviceConnected) {
    // Aqui você pode começar a interagir com o VESC enquanto a conexão estiver ativa
    Serial.println("Interagindo com o VESC...");
    delay(1000);  // Atraso para simular um loop de interação
  }
    myencoder.begin();

    // Cria o mutex para proteger o acesso à posição do encoder
    positionMutex = xSemaphoreCreateMutex();

   positionMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore
    (
      vTaskControll,
     "controllTask",
      8192, 
      NULL, 
      1, 
      NULL, 
      1
      );
}


