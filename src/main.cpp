#include "izi.hpp"
using namespace Izi;


void setup() {
Izi::Controll controll;

Serial.begin(115200);
log_w("Inicializando Controll...");
Serial.printf("Version: %s\n", IZI_FW_VERSION);

controll.begin();


log_w("Controll iniciado!");

}

void loop(){

}