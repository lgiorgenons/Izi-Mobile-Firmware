#ifndef BATTERY_HPP
#define BATTERY_HPP

#include <Arduino.h>

class Battery {
private:
    uint8_t pin;          // Analog pin used
    uint16_t adcMin;      // Minimum ADC value for voltage
    uint16_t adcMax;      // Maximum ADC value for voltage

    static const uint8_t numSamples = 10; // Number of samples for moving average
    uint16_t samples[numSamples];         // Buffer for ADC samples
    uint8_t sampleIndex;                  // Current index in the buffer

    uint16_t readADC();      // Reads the ADC value with filtering
    uint16_t avgSamples();   // Computes the average of the samples

public:
    Battery(uint8_t analogPin, uint16_t adcMinValue, uint16_t adcMaxValue);

    uint8_t getLevel();   // Returns the battery level (0-100%)
    bool isCritical();    // Returns true if the battery is in a critical state
};

#endif
