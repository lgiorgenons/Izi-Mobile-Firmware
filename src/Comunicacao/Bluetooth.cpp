#include "Comunicacao/Bluetooth.hpp"     
#include "izi.hpp"          
BluetoothSerial SerialBT;      // Objeto para comunicação Bluetooth

Bluetooth::Bluetooth() : connected(false), connectedDeviceName("") {
    // Construtor da classe Bluetooth
}

Bluetooth::~Bluetooth() {
    // Destruidor da classe Bluetooth (limpeza, se necessário)
}

void Bluetooth::begin(const char* deviceName) {
    SerialBT.begin(deviceName);  
    delay(100);  
    Serial.println("Bluetooth Iniciado");
}

int Bluetooth::send_payload(uint8_t* payload, int lenPay) {
    uint16_t crcPayload = crc16(payload, lenPay);
    int count = 0;
    uint8_t messageSend[256];

    if (lenPay <= 256) {
        messageSend[count++] = 2;
        messageSend[count++] = lenPay;
    } else {
        messageSend[count++] = 3;
        messageSend[count++] = (uint8_t)(lenPay >> 8);
        messageSend[count++] = (uint8_t)(lenPay & 0xFF);
    }
    memcpy(&messageSend[count], payload, lenPay);
    count += lenPay;
    messageSend[count++] = (uint8_t)(crcPayload >> 8);
    messageSend[count++] = (uint8_t)(crcPayload & 0xFF);
    messageSend[count++] = 3;

    SerialBT.write(messageSend, count);

    // Aguarda resposta após envio
    delay(100);  // Pequena espera para resposta
    if (SerialBT.available()) {
        Serial.println("Resposta do VESC:");
        while (SerialBT.available()) {
            uint8_t byte = SerialBT.read();
            Serial.print(byte, HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("Nenhuma resposta do VESC");
    }

    return count;
}

bool Bluetooth::connectToDevice(const char* deviceName) {
    Serial.println("Tentando conectar...");
    if (SerialBT.connect("VESC BLE UART")) {
        connected = true;
        connectedDeviceName = deviceName;
        Serial.print("Conectado a: ");
        Serial.println(deviceName);
        return true;
    }
    Serial.println("Falha ao conectar");
    return false;
}

void Bluetooth::sendCommand(const String& command) {
    if (connected) {
        SerialBT.println(command);  
        Serial.println("Comando enviado: " + command);
    } else {
        Serial.println("Não conectado ao VESC");
    }
}

void Bluetooth::receiveData() {
    if (SerialBT.available()) {
        String data = SerialBT.readString();
        Serial.print("Dados recebidos: ");
        Serial.println(data);

        // Se quiser ver os dados em formato hexadecimal
        Serial.print("Dados em HEX: ");
        for (int i = 0; i < data.length(); i++) {
            Serial.print(data[i], HEX);  // Exibe cada byte em hexadecimal
            Serial.print(" ");
        }
        Serial.println();
    }
}

bool Bluetooth::isConnected() {
    return connected;
}

void Bluetooth::stop() {
    if (connected) {
        SerialBT.disconnect();
        connected = false;
        Serial.println("Desconectado do VESC");
    }
}

int Bluetooth::check_crc(uint8_t *messageReceived, int message_len) {
    int i;
    uint8_t payload[256];
    uint16_t crcMessage = 0;
    uint16_t crcPayload = 0;
    int payload_len;
    
    crcMessage = 0;
    crcPayload = 0;

    payload_len = messageReceived[1];
    crcMessage = messageReceived[message_len - 3] << 8;
    crcMessage &= 0xFF00;
    crcMessage += messageReceived[message_len - 2];      

    for(i=0; i<payload_len; i++) payload[i] = messageReceived[i+2];
    crcPayload = crc16(payload, payload_len);

    return (crcMessage == crcPayload) ? 1 : 0;
}

int Bluetooth::get_vesc_values_51() {
    uint8_t command[1] = { COMM_GET_VALUES };
    uint8_t messageReceived[256];
    int counter = 0;
    int32_t ind;
    int packet_id;
    int crc_status = 0;
    long start;
    int timeout;
         
    send_payload(command, 1);

    start = millis();
    timeout = 200;
    while (millis() - start < timeout) {
        if (SerialBT.available()) messageReceived[counter] = SerialBT.read();
        else continue;
        counter++;
        if (counter == 1) {
           if (messageReceived[0] != 2) counter = 0; 
        }
        else if (counter == 2) {             
           if (messageReceived[1] != 73) counter = 0; 
        }
        if (counter >= 256) counter = 0;  
    }

    if (counter == 78 && messageReceived[77] == 3) crc_status = check_crc(messageReceived, counter);
    else return(0);

    if (crc_status) {
        ind = 2; 
        packet_id = (int)(messageReceived[ind++]);
        vescMeasuredValues.temp_mos = buffer_get_float16(messageReceived, 1e1, &ind);
        vescMeasuredValues.temp_motor = buffer_get_float16(messageReceived, 1e1, &ind);
        vescMeasuredValues.current_motor = buffer_get_float32(messageReceived, 1e2, &ind);
        vescMeasuredValues.current_in = buffer_get_float32(messageReceived, 1e2, &ind);
        vescMeasuredValues.id = buffer_get_float32(messageReceived, 1e2, &ind);
        vescMeasuredValues.iq = buffer_get_float32(messageReceived, 1e2, &ind);
        vescMeasuredValues.duty_now = buffer_get_float16(messageReceived, 1e3, &ind);
        vescMeasuredValues.rpm = buffer_get_float32(messageReceived, 1e0, &ind);
        vescMeasuredValues.v_in = buffer_get_float16(messageReceived, 1e1, &ind);
        vescMeasuredValues.amp_hours = buffer_get_float32(messageReceived, 1e4, &ind);
        vescMeasuredValues.amp_hours_charged = buffer_get_float32(messageReceived, 1e4, &ind);
        vescMeasuredValues.watt_hours = buffer_get_float32(messageReceived, 1e4, &ind);
        vescMeasuredValues.watt_hours_charged = buffer_get_float32(messageReceived, 1e4, &ind);
        vescMeasuredValues.tachometer = buffer_get_int32(messageReceived, &ind);
        vescMeasuredValues.tachometer_abs = buffer_get_int32(messageReceived, &ind);
        vescMeasuredValues.fault_code = (mc_fault_code)(messageReceived[ind++]);
        vescMeasuredValues.position = buffer_get_float32(messageReceived, 1e6, &ind);
        vescMeasuredValues.vesc_id = messageReceived[ind++];
        vescMeasuredValues.temp_mos_1 = buffer_get_float16(messageReceived, 1e1, &ind);
        vescMeasuredValues.temp_mos_2 = buffer_get_float16(messageReceived, 1e1, &ind);
        vescMeasuredValues.temp_mos_3 = buffer_get_float16(messageReceived, 1e1, &ind);
        vescMeasuredValues.vd = buffer_get_float32(messageReceived, 1e3, &ind);
        vescMeasuredValues.vq = buffer_get_float32(messageReceived, 1e3, &ind);     
        return(1);
    } else {
        return(0);
    }
}

#define COMM_SET_CURRENT 10  // Substitua pelo valor correto do comando do VESC

// Função para enviar o target de corrente para o VESC
void Bluetooth::setTargetCurrent(float targetCurrent) {
    // Construir o comando para configurar a corrente
    uint8_t command[5]; // O comando pode precisar de mais ou menos bytes dependendo da configuração
    command[0] = COMM_SET_CURRENT;  // Defina o comando que configura a corrente
    uint8_t *targetCurrentPtr = (uint8_t*)&targetCurrent;  // Converte o float para bytes
    
    // Envia o valor da corrente como dois floats (se necessário)
    for (int i = 0; i < 4; i++) {
        command[i + 1] = targetCurrentPtr[i];  // Preenche os 4 bytes do float
    }

    // Log para exibir a corrente que está sendo configurada
    Serial.print("Enviando corrente alvo: ");
    Serial.println(targetCurrent, 4);  // Imprime o valor da corrente com 4 casas decimais

    // Log para exibir o comando em formato hexadecimal
    Serial.print("Comando enviado: ");
    for (int i = 0; i < sizeof(command); i++) {
        Serial.print(command[i], HEX);  // Mostra o valor em hexadecimal
        Serial.print(" ");
    }
    Serial.println();

    // Envia o comando via Bluetooth
    send_payload(command, sizeof(command));
}

int Bluetooth::get_vesc_data() {
    int status;

    if (Izi::vesc_fw_version == 51) {
        status = this->get_vesc_values_51();
        this->voltage = vescMeasuredValues.v_in;
        this->current = vescMeasuredValues.current_in;
        this->rpm = vescMeasuredValues.rpm;

        log_w("Voltage: %.2f V", this->voltage);
        log_w("Current: %.2f A", this->current);
        log_w("RPM: %.2f", this->rpm);
    }
    return status;
}


void Bluetooth::setVescMode(uint8_t mode) {
    uint8_t command[2] = { COMM_SET_MODE, mode }; // Comando para configurar o modo
    send_payload(command, sizeof(command));
    Serial.printf("Modo VESC configurado: %d\n", mode);
}

void Bluetooth::setTargetRPM(float targetRPM) {
    uint8_t command[5];
    command[0] = COMM_SET_RPM; // Comando para configurar RPM
    memcpy(&command[1], &targetRPM, sizeof(targetRPM)); // Converte o float para bytes

    send_payload(command, sizeof(command));
    Serial.printf("RPM alvo configurado: %.2f\n", targetRPM);
}

void Bluetooth::setDutyCycle(float dutyCycle) {
    uint8_t command[5];
    command[0] = COMM_SET_DUTY; // Comando para configurar Duty Cycle
    memcpy(&command[1], &dutyCycle, sizeof(dutyCycle)); // Converte o float para bytes

    send_payload(command, sizeof(command));
    Serial.printf("Duty Cycle configurado: %.2f\n", dutyCycle);
}

bool Bluetooth::reconnect() {
    if (!connected) {
        Serial.println("Tentando reconectar...");
        connected = SerialBT.connect(connectedDeviceName.c_str());
        if (connected) {
            Serial.println("Reconexão bem-sucedida!");
        } else {
            Serial.println("Falha na reconexão.");
        }
    }
    return connected;
}

void Bluetooth::parse_vesc_data(uint8_t* data, int len) {
    int32_t ind = 2; // Índice inicial após cabeçalho
    vescMeasuredValues.temp_mos = buffer_get_float16(data, 1e1, &ind);
    vescMeasuredValues.temp_motor = buffer_get_float16(data, 1e1, &ind);
    vescMeasuredValues.current_motor = buffer_get_float32(data, 1e2, &ind);
    vescMeasuredValues.current_in = buffer_get_float32(data, 1e2, &ind);
    vescMeasuredValues.rpm = buffer_get_float32(data, 1e0, &ind);
    vescMeasuredValues.v_in = buffer_get_float16(data, 1e1, &ind);

    // Log dos dados principais
    Serial.printf("Tensão: %.2f V\n", vescMeasuredValues.v_in);
    Serial.printf("Corrente: %.2f A\n", vescMeasuredValues.current_in);
    Serial.printf("RPM: %.2f\n", vescMeasuredValues.rpm);
}

int Bluetooth::get_vesc_values_51() {
    uint8_t command[1] = { COMM_GET_VALUES };
    uint8_t messageReceived[256];
    int counter = 0;
    int crc_status = 0;
    long start = millis();
    int timeout = 200;

    send_payload(command, sizeof(command));

    start = millis();
    while (millis() - start < timeout) {
        if (SerialBT.available()) messageReceived[counter++] = SerialBT.read();
        if (counter >= sizeof(messageReceived)) break;
    }

    if (counter > 0 && messageReceived[counter - 1] == 3) {
        crc_status = check_crc(messageReceived, counter);
    }

    if (crc_status) {
        parse_vesc_data(messageReceived, counter);
        return 1; // Sucesso
    }
    return 0; // Falha
}
