#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>

#include <sx1280.h>

#define BUSY 21
#define NRESET 16

#define NSS 17

// uint8_t buf[6] = {0};

void Init(void)
{
  SPI.begin(18, 19, 23, 5);
  // SPI.begin();    
  Serial.begin(115200);

  pinMode(NRESET, OUTPUT);
  pinMode(BUSY, INPUT);
  
  pinMode(SPI.pinSS(), OUTPUT);

  digitalWrite(NRESET, HIGH);

}

void WaitOnBusy()
{
  while(digitalRead(BUSY) == 1) {
    Serial.println("waiting for busy");
  }
}

// void Reset(void)
// {
//   delay(20);
//   digitalWrite(NRESET, LOW);
//   delay(50);
//   digitalWrite(NRESET, HIGH);
//   delay(20);
// }

// void WakeUp(void)
// {
//   digitalWrite(SPI.pinSS(), LOW);
//   SPI.transfer(0xC0);
//   SPI.transfer(0x00);
//   digitalWrite(SPI.pinSS(), HIGH);

//   WaitOnBusy();
// }

void WriteCommand(uint8_t command, uint8_t * buf, uint8_t size)
{
  WaitOnBusy();
  
  digitalWrite(SPI.pinSS(), LOW);
  SPI.transfer( (uint8_t) command);
  
  // SPI.transfer(buf, (uint8_t) sizeof(buf)); // NOTE: expects 4 bytes and sends, hance it does not work as intended
  for(uint16_t i = 0; i < size; i++ )
  {
    SPI.transfer(buf[i]);
  }

  digitalWrite(SPI.pinSS(), HIGH);
}

void setup() 
{

  // using already existing spi
  SPI.begin();
  SX1280 test1(SPI, SS, NRESET, BUSY);

  // creating new spi
  SX1280 test2(18, 19, 23, 5, NRESET, BUSY);

  uint8_t buf[3] = {0xff, 0xf0, 0x0f};
  // WriteCommand(0xF0, buf, sizeof(buf));
  test1.WriteCommand(0xF0, buf, sizeof(buf));
  test2.WriteCommand(0xF0, buf, sizeof(buf));

  /*
  thoughts on usability
  - use already existing spi or create new one in class
  - if using multiple devices use existing one and specify different SS pin
  */

}

void loop() 
{

}
