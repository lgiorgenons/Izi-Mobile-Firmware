#ifndef IZI_HPP
#define IZI_HPP

#define TAKE_TIMEOUT_200MS  200
#define TAKE_TIMEOUT_500MS  500
#define TAKE_TIMEOUT_1000MS 1000

// #define de Strings constantes do controle  
 
#define IZI_FW_VERSION "0.0.2"
#define IZI_TAG_STRING "Izi"
#define IZI_DEVICE_NAME "Izi Controll"

//LIBS PROPRIAS
#include <Comunicacao/Bluetooth.hpp>
#include <Interface/Oled.hpp>
#include <Interface/Encoder.hpp>

//NATIVOS ESP32
#include <soc/soc.h>       // Para WRITE_PERI_REG e READ_PERI_REG
#include <soc/rtc_cntl_reg.h>  // Para RTC_CNTL_BROWN_OUT_REG
#include <esp32-hal-log.h>

namespace Izi {

    static const char* FW_Version = IZI_FW_VERSION;    

    static const char*  TAG =   "Izi";
    static const char*  devicename = "Izi Controll";
    static uint8_t vesc_fw_version = 51;

    typedef enum {
        INOP_SYS,
        PAS_INTERNAL,
        EXTERNAL_PAS
    } izi_FSM;

    typedef enum {
        IDLE,
        LOCKED,
        UNLOCKED,   
        FAULT,
        FAULT_LOCKED
    } izi_State;

    class Controll{
        private:

        public:
            void begin();
            bool getStatus();
            bool setStatus();

        public:
    };

}

#endif