#ifndef SX1280_H
#define SX1280_H

#include <SPI.h>
#include <Arduino.h>
#include <sx1280_utils.h>

class SX1280
{
    private:
        SPIClass &m_spi;
        uint8_t m_ss;
        uint8_t m_nrst;
        uint8_t m_busy;

    public:
        SX1280(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss, uint8_t nrst, uint8_t busy);
        SX1280(SPIClass &spi, uint8_t ss, uint8_t nrst, uint8_t busy);
        void WriteCommand(uint8_t command, uint8_t *buf, uint16_t size);
        void ReadCommand(uint8_t command, uint8_t *buf, uint16_t size);
        void WriteReg(uint8_t addr, uint8_t *buf, uint16_t size);
        void WaitOnBusy();
        void Reset();
        void WakeUp();
};

#endif