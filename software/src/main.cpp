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
  // uint8_t buf[1] = {0x01};
  // WriteCommand(0xF0, buf, sizeof(buf));
  // sx1280_1.WriteCommand(0x80, buf, 1);

  // init procedure
  Serial.println("SET STANDBY");
  sx1280_1.SetStandby();

  // set packet type
  Serial.println("SET PACKET TYPE");
  sx1280_1.SetPacketType(PACKET_TYPE_GFSK);
  Serial.println(sx1280_1.GetPacketType());

  Serial.println("SET RF FREQUENCY");
  std::vector<uint8_t> freq = {0xb8, 0x9D, 0x89}; // sequence for 2.4Ghz
  sx1280_1.SetRfFrequency(freq.data(), freq.size());

  Serial.println("SET BUFFER ADDRESSES");
  sx1280_1.SetBufferBaseAddress(0, 0);

  Serial.println("GET RX BUF STATUS");
  std::vector<uint8_t> buf = {0, 0};
  sx1280_1.getRXBufferStatus(buf.data(), buf.size());
  Serial.printf("payload length: %d, rxStartpointer: %d", buf[0], buf[1]);

  /*
  thoughts on usability
  - use already existing spi or create new one in class
  - if using multiple devices use existing one and specify different SS pin
  */

}

void loop() 
{

}
