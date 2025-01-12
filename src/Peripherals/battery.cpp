#include "battery.hpp"

Battery::Battery(uint8_t analogPin, uint16_t adcMinValue, uint16_t adcMaxValue)
    : pin(analogPin), adcMin(adcMinValue), adcMax(adcMaxValue), sampleIndex(0) {
    pinMode(pin, INPUT); // Configure the analog pin as input

    // Initialize the sample buffer with zeros
    for (uint8_t i = 0; i < numSamples; i++) {
        samples[i] = 0;
    }

    log_i("Battery initialized on pin %d with ADC range [%d, %d]", pin, adcMin, adcMax);
}

uint16_t Battery::readADC() {
    uint16_t adcValue = analogRead(pin);  // Read the ADC value
    samples[sampleIndex] = adcValue;     // Store the reading in the buffer
    sampleIndex = (sampleIndex + 1) % numSamples; // Increment index cyclically

    log_d("ADC reading: %d", adcValue); // Debug log for ADC value

    return avgSamples(); // Return the average of the samples
}

uint16_t Battery::avgSamples() {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < numSamples; i++) {
        sum += samples[i];
    }
    uint16_t avg = sum / numSamples;

    log_d("Average ADC: %d", avg); // Debug log for average calculation
    return avg;
}

uint8_t Battery::getLevel() {
    uint16_t adcValue = readADC();
    // Calculate battery level as a percentage (0-100%)
    uint32_t level = ((adcValue - adcMin) * 100) / (adcMax - adcMin);
    uint8_t constrainedLevel = constrain(level, 0, 100);

    log_i("Battery level: %d%%", constrainedLevel); // Info log for battery level
    return constrainedLevel;
}

bool Battery::isCritical() {
    uint8_t level = getLevel();
    if (level <= 10) {
        log_w("Battery critical: %d%%", level); // Warning log for critical battery
        return true;
    }
    return false;
}
