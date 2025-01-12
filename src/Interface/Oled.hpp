#ifndef OLED_HUD_HPP
#define OLED_HUD_HPP

#include <U8g2lib.h>
#include <string>

namespace Izi {
    namespace Oled {
        static const uint8_t SDA_PIN = 21;
        static const uint8_t SCL_PIN = 22;
        class OledHUD {
        public:
            // Construtor e destrutor
            OledHUD(uint8_t sdaPin, uint8_t sclPin );
            ~OledHUD();

            // Métodos públicos
            bool begin();
            void Manager();
            void updateHUD();
            void setKilometers(int km);
            void setDrivingMode(const std::string& mode);
            void setVelocity(float speed);
            void setBLEStatus(const char* status);
            
            bool setDisplay(uint8_t velocidade, uint8_t bateria, const char* modo);
            void drawPanel();

            U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;


        private:
            uint8_t velocity;

            int8_t km_h;
            uint8_t bat;
            const char* mode;

            // Métodos privados
            
            void displayData();

            // Atributos privados            
            String drivingMode;
            String bleStatus; 

            // Constantes para dimensões do display
            static const int IMAGE_WIDTH = 128;
            static const int IMAGE_HEIGHT = 64;

            const char* kmBuffer;
        };

    }   //namespace Oled
} //namespace Izi

#endif // OLED_HUD_HPP
