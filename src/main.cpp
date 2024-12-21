#include "izi.hpp"
using namespace Izi;


void setup() {
Izi::Controll controll;

Serial.begin(115200);
log_w("Inicializando Controll...");
Serial.printf("Version: %s\n", VERSION);

controll.begin();


}

void loop(){}