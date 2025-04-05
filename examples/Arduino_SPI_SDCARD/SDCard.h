// SDCard.h
#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include "SC18IS606.h"


class SDCard {
public:
    // The constructor accepts a reference to the SPI interface and the CS pin number.
    SDCard(SC18IS606& spi, uint8_t csPin);

    // Public methods for SD card 
    uint8_t sendCMD0(); //idle mode
    uint8_t sendCMD8(); //check type of sd card
    uint8_t sendCMD55(); //other type
    uint8_t sendACMD41(uint32_t arg); //left idle state
    void sendDummyClocks();

private:
    SC18IS606& _spi;  // SPI interface reference
    uint8_t _csPin;   // Number of CS pins
    
    // Private helper method for waiting for a response
    uint8_t waitForResponse();

    // Constants for commands
    #define CMD0  0x40
    #define CMD8  0x48
    #define CMD55 0x77
    #define ACMD41  0x69
};

#endif
