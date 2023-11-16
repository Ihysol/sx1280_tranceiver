#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>

#include <sx1280.h>

#define BUSY 21
#define NRESET 16
// #define NSS 17

void setup() 
{
  Serial.begin(115200);
  // using already existing spi
  SPI.begin();
  SX1280 sx1280_1(SPI, SS, NRESET, BUSY);

  // creating new spi

  uint8_t buf[3] = {0xff, 0xf0, 0x0f};
  // WriteCommand(0xF0, buf, sizeof(buf));
  sx1280_1.WriteCommand(0xF0, buf, sizeof(buf));

  /*
  thoughts on usability
  - use already existing spi or create new one in class
  - if using multiple devices use existing one and specify different SS pin
  */

}

void loop() 
{

}
