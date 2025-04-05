#include <Wire.h>
#include "SC18IS606.h"
#include "SDCard.h"

//CS PINS
#define SC0_CS 0x01
#define SC1_CS 0x02
#define SC2_CS 0x04

//selecting CS PIN
uint8_t selectedCS = SC0_CS;

// Creating instance
SC18IS606 spi(0x28);
SDCard sdCard(spi, selectedCS);

//setup
void setup() {
  // depends on your board
  Wire.begin(6, 7);
  Serial.begin(115200);
  Serial.println("Beginning Initialization of SD card...");

  // reseting pins
  spi.setIdleMode();
  delay(100);

  //configurating SPI mode and speed
  spi.configSPI(SC18IS606::SPIMODE_0, SC18IS606::SPICLK_1875_kHz);

  //sets CS pin LOW
  spi.enableGPIO(selectedCS);
  spi.writeGPIO(selectedCS);

  // send dummy bytes
  sdCard.sendDummyClocks();
  //set other pins for open drain if want low state need pull up resistor
  spi.selectPinMode(2, SC18IS606::GPIO_MODE_OPEN_DRAIN);
  spi.selectPinMode(1, SC18IS606::GPIO_MODE_OPEN_DRAIN);

  //declaration of data variables
  uint8_t r1;
  uint8_t r8;
  //sending twice byte for changing state for SD Card to idle
  r1 = sdCard.sendCMD0();
  r1 = sdCard.sendCMD0();

  Serial.print("R1 after CMD0: 0x");
  Serial.println(r1, HEX);

  if (r1 == 0x01) {
    Serial.println("Card is in Idle mode.");
    r8 = sdCard.sendCMD8();
    Serial.print("R1 CMD8: 0x");
    Serial.println(r8, HEX);

    if (r8 == 0x01) {
      Serial.println("Card supports CMD8 (SDHC/SDXC).");
      while (true) {
        uint8_t r55 = sdCard.sendCMD55();
        if (r55 > 0x01) {
          Serial.println("Error CMD55");
          break;
        }
        uint8_t racmd41 = sdCard.sendACMD41(0x40000000);
        Serial.print("R1 after ACMD41: 0x");
        Serial.println(racmd41, HEX);
        if (racmd41 == 0x00) {
          Serial.println("Card left idle state (SDHC/SDXC)");
          break;
        }
        delay(100);
      }
    } else {
      Serial.println("CMD8 failed, possible SDSC type card.");
      while (true) {
        uint8_t r55 = sdCard.sendCMD55();
        if (r55 > 0x01) {
          Serial.println("Error CMD55");
          break;
        }
        uint8_t racmd41 = sdCard.sendACMD41(0x00000000);
        Serial.print("R1 after ACMD41 (SDSC): 0x");
        Serial.println(racmd41, HEX);
        if (racmd41 == 0x00) {
          Serial.println("SDSC is ready.");
          break;
        }
        delay(100);
      }
    }

  } else {
    Serial.println("Card didn't respond expecting value after sending CMD0.");
  }
}

void loop() {
}
