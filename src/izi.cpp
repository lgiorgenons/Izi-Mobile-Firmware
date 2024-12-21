#include "izi.hpp"

using namespace Izi;
using namespace Izi::Oled;

void Izi::Controll::begin(){
    Bluetooth ble;
    OledHUD oledHUD(Oled::SDA_PIN, Oled::SCL_PIN);
    
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
                                        
    if(!ble.begin("ESP_32_LIGA")) {
        uint8_t retry;
        Serial2.begin(9600 | SERIAL_8N1, 16, 17);
        log_w("Nao foi possivel iniciar o Bluetooth...");
        while (!ble.isConnected()) {
            log_w("Não possivel iniciar o Bluetooth... Tentando novamente");
            
            retry++;

            if (retry >= 5) {
                log_w("Nao foi possivel iniciar o Bluetooth");

                //Executar algum erro no display
            }

        }
    }

    log_d("Iniciando Bluetooth: izi_mobile");
    

    if (!oledHUD.begin()){
            
    }
}
