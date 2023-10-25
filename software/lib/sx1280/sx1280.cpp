#include <sx1280.h>


SX1280::SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss, uint8_t nrst, uint8_t busy) : m_spi(SPI), m_ss(ss), m_nrst(nrst), m_busy(busy)
{
    m_spi.begin(sck, miso, mosi, ss);

    pinMode(m_nrst, OUTPUT);    // nreset
    pinMode(m_busy, INPUT);     // busy input
    
    pinMode(m_ss, OUTPUT);      // ss pin ouput (important)
    digitalWrite(m_nrst, HIGH); 

}

SX1280::SX1280(SPIClass &spi, uint8_t ss, uint8_t nrst, uint8_t busy) : m_spi(spi), m_ss(ss), m_nrst(nrst), m_busy(busy)
{
    pinMode(m_nrst, OUTPUT);    // nreset
    pinMode(m_busy, INPUT);     // busy input

    pinMode(m_ss, OUTPUT);      // ss pin ouput (important)
    digitalWrite(m_nrst, HIGH);
}


void SX1280::WaitOnBusy(void)
{
    while(digitalRead(m_busy) == 1);
}

void SX1280::Reset(void)
{
    delay(20);
    digitalWrite(m_nrst, LOW);
    delay(50);
    digitalWrite(m_nrst, HIGH);
    delay(20);
}

void SX1280::WakeUp(void)
{
    digitalWrite(m_ss, LOW);
    m_spi.transfer(0xC0);
    m_spi.transfer(0x00);
    digitalWrite(m_ss, HIGH);
    WaitOnBusy();
}

void SX1280::WriteCommand(uint8_t command, uint8_t * buf, uint16_t size)
{
    WaitOnBusy();

    digitalWrite(m_ss, LOW);
    m_spi.transfer((uint8_t)command);
    for(uint16_t i = 0; i < size; i++)
    {
        m_spi.transfer(buf[i]);
    }
    digitalWrite(m_ss, HIGH);
}

void SX1280::ReadCommand(uint8_t command, uint8_t * buf, uint16_t size)
{
    WaitOnBusy();

    digitalWrite(m_ss, LOW);
    m_spi.transfer((uint8_t) command);
    m_spi.transfer(0x00);
    for(uint16_t i=  0; i < size; i++)
    {
        buf[i] = m_spi.transfer(0x00);
    }
    digitalWrite(m_ss, HIGH);

    WaitOnBusy();
}

void SX1280::WriteReg(uint8_t addr, uint8_t *buf, uint16_t size)
{
    WaitOnBusy();
    digitalWrite(m_ss, LOW);
    
    digitalWrite(m_ss, HIGH);
}

