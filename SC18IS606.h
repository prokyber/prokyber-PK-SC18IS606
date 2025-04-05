// SC18IS606.h
#ifndef SC18IS606_H
#define SC18IS606_H

#include <Arduino.h>
#include <Wire.h>

class SC18IS606 {
public:
    // SPI modes
    enum SPI_Mode {
        SPIMODE_0 = 0b00,
        SPIMODE_1 = 0b01,
        SPIMODE_2 = 0b10,
        SPIMODE_3 = 0b11
    };

    // SPI speeds
    enum SPI_Speed {
        SPICLK_1875_kHz = 0b00,
        SPICLK_455_kHz  = 0b01,
        SPICLK_115_kHz  = 0b10,
        SPICLK_58_kHz   = 0b11
    };

    // GPIO modes
    enum GPIOPinMode {
        GPIO_MODE_QUASI_BIDIRECTIONAL = 0b00,
        GPIO_MODE_PUSH_PULL          = 0b01,
        GPIO_MODE_INPUT_ONLY         = 0b10,
        GPIO_MODE_OPEN_DRAIN         = 0b11
    };

    SC18IS606(uint8_t address);

    void configSPI(SPI_Mode mode, SPI_Speed speed);
    void spiTransfer(uint8_t cs, const uint8_t *dataToSend, uint8_t len, uint8_t *dataReceived);
    void selectPinMode(uint8_t pin, GPIOPinMode mode);
    void configPins();
    void setIdleMode();
    void enableGPIO(uint8_t mask);
    void configureGPIOPins(uint8_t config);
    void writeGPIO(uint8_t value);

private:
    //Command bytes
    #define CONFIG_SPI_CMD  0xF0
    #define IDLE_CMD  0xF2
    #define GPIO_WRITE_CMD  0xF4
    #define GPIO_ENABLE_CMD  0xF6
    #define GPIO_CONFIGURATION_CMD  0xF7

    // Private variables
    uint8_t _address;
    uint8_t _gpioState;
    uint8_t _pinConfig;

    
};

#endif
