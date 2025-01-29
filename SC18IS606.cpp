// SC18IS606.cpp
#include "SC18IS606.h"



SC18IS606::SC18IS606(uint8_t address) : 
    _address(address),
    _gpioState(0x00),
    _pinConfig(0xFF) // All pins set to Open-Drain as default
{
}

//configuration of SPI mode and speed
void SC18IS606::configSPI(SPI_Mode mode, SPI_Speed speed) {
    uint8_t configVal = ((mode & 0x03) << 2) | (speed & 0x03);
    Wire.beginTransmission(_address);
    Wire.write(CONFIG_SPI_CMD);
    Wire.write(configVal);
    Wire.endTransmission();
}

//sending data via SPI
void SC18IS606::spiTransfer(uint8_t cs, const uint8_t *dataToSend, uint8_t len, uint8_t *dataReceived) {

    Wire.beginTransmission(_address);
    Wire.write(cs);
    for (uint8_t i = 0; i < len; i++) {
        Wire.write(dataToSend[i]);
    }
    Wire.endTransmission();

    delay(5);

    Wire.requestFrom((int)_address, (int)len);
    for (uint8_t i = 0; i < len; i++) {
        dataReceived[i] = (Wire.available()) ? Wire.read() : 0xFF;
    }


    uint8_t dummyOut[1] = {0xFF};
    uint8_t dummyIn[1];

    for (uint8_t tries = 0; tries < 10; tries++) {
        Wire.beginTransmission(_address);
        Wire.write(cs);
        Wire.write(dummyOut[0]);
        Wire.endTransmission();

        delay(5);

        Wire.requestFrom((int)_address, 1);
        dummyIn[0] = (Wire.available()) ? Wire.read() : 0xFF;

        if (dummyIn[0] == 0xFF) {
            break;
        }
    }
}

//selecting pin
void SC18IS606::selectPinMode(uint8_t pin, GPIOPinMode mode) {
    if (pin > 3) return;

    _pinConfig &= ~(0b11 << (pin * 2));    // Reset bits for a given pin
    _pinConfig |= (mode << (pin * 2));      // Set new mode

    configureGPIOPins(_pinConfig);

    // Enable pin (set to HIGH for default off state)
    _gpioState |= (1 << pin);
    writeGPIO(_gpioState);
}


// testing method
void SC18IS606::configPins() {
    enableGPIO(0x0F);
    configureGPIOPins(_pinConfig);
}

//enabling pin
void SC18IS606::enableGPIO(uint8_t mask) {
    Wire.beginTransmission(_address);
    Wire.write(GPIO_ENABLE_CMD);
    Wire.write(mask);
    Wire.endTransmission();
}

//configurating pins
void SC18IS606::configureGPIOPins(uint8_t config) {
    Wire.beginTransmission(_address);
    Wire.write(GPIO_CONFIGURATION_CMD);
    Wire.write(config);
    Wire.endTransmission();
}

//send bytes
void SC18IS606::writeGPIO(uint8_t value) {
    Wire.beginTransmission(_address);
    Wire.write(GPIO_WRITE_CMD);
    Wire.write(value);
    Wire.endTransmission();
}

//set to idle mode SPI bridge
void SC18IS606::setIdleMode() {
    Wire.beginTransmission(_address);
    Wire.write(IDLE_CMD);
    Wire.endTransmission();
    delay(10);
}
