#ifndef IZI_HPP
#define IZI_HPP

#define VERSION "0.0.2"

#define TAKE_TIMEOUT_200MS 200;
#define TAKE_TIMEOUT_500MS 500;
#define TAKE_TIMEOUT_1000MS 1000;


#include <Comunicacao/Bluetooth.hpp>
#include <Interface/Oled.hpp>
#include <soc/soc.h>       // Para WRITE_PERI_REG e READ_PERI_REG
#include <soc/rtc_cntl_reg.h>  // Para RTC_CNTL_BROWN_OUT_REG
#include <esp32-hal-log.h>


namespace Izi {

    static const char* TAG = "Izi";
    static const char* devicename = "Izi Controll";

    class Controll{
        private:

        public:
        void begin();
    };

    class Status {
        bool locked;
    };
}

#endif