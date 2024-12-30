// SDCard.cpp
#include "SDCard.h"

//selects pin
SDCard::SDCard(SC18IS606& spi, uint8_t csPin) : 
    _spi(spi),
    _csPin(csPin) {
}

//Waiting for response
uint8_t SDCard::waitForResponse() {
    uint8_t r;
    for (uint8_t tries = 0; tries < 10; tries++) {
        uint8_t dummyOut[1] = {0xFF};
        uint8_t dummyIn[1];
        _spi.spiTransfer(_csPin, dummyOut, 1, dummyIn);
        r = dummyIn[0];
        if (r != 0xFF) return r;
    }
    return 0xFF;
}

//sending bytes for switching to idle state SD card
uint8_t SDCard::sendCMD0() {
    uint8_t cmd0[6] = {CMD0, 0x00, 0x00, 0x00, 0x00, 0x95};
    uint8_t resp[6];
    _spi.spiTransfer(_csPin, cmd0, 6, resp);

    for (uint8_t i = 0; i < 6; i++) {
        if (resp[i] != 0xFF) return resp[i];
    }

    return waitForResponse();
}

//checks type of SD card
uint8_t SDCard::sendCMD8() {
    uint8_t cmd8[6] = {CMD8, 0x00, 0x00, 0x01, 0xAA, 0x87};
    uint8_t resp[6];
    _spi.spiTransfer(_csPin, cmd8, 6, resp);

    for (uint8_t i = 0; i < 6; i++) {
        if (resp[i] != 0xFF) return resp[i];
    }

    return waitForResponse();
}

//leaving idle state to working mode
uint8_t SDCard::sendCMD55() {
    uint8_t cmd55[6] = {CMD55, 0x00, 0x00, 0x00, 0x00, 0xFF};
    uint8_t resp[6];
    _spi.spiTransfer(_csPin, cmd55, 6, resp);

    for (uint8_t i = 0; i < 6; i++) {
        if (resp[i] != 0xFF) return resp[i];
    }
    return waitForResponse();
}

//other type of SD card
uint8_t SDCard::sendACMD41(uint32_t arg) {
    uint8_t cmd41[6];
    cmd41[0] = ACMD41;
    cmd41[1] = (arg >> 24) & 0xFF;
    cmd41[2] = (arg >> 16) & 0xFF;
    cmd41[3] = (arg >> 8) & 0xFF;
    cmd41[4] = arg & 0xFF;
    cmd41[5] = 0xFF;

    uint8_t resp[6];
    _spi.spiTransfer(_csPin, cmd41, 6, resp);

    for (uint8_t i = 0; i < 6; i++) {
        if (resp[i] != 0xFF) return resp[i];
    }
    return waitForResponse();
}

//send dummy bytes
void SDCard::sendDummyClocks() {
    uint8_t dummy[10];
    for (uint8_t i = 0; i < 10; i++) dummy[i] = 0xFF;
    uint8_t resp[10];
    _spi.spiTransfer(_csPin, dummy, 10, resp);
}