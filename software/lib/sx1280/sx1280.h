#ifndef SX1280_H
#define SX1280_H

#include <SPI.h>
#include <Arduino.h>
#include <sx1280_utils.h>
#include <vector>

enum sx1280_packetType_e
{
    PACKET_TYPE_GFSK = 0x00,
    PACKET_TYPE_LORA = 0x01,
    PACKET_TYPE_RANGING = 0x02,
    PACKET_TYPE_FLRC = 0x03,
    PACKET_TYPE_BLE = 0x04,
};

enum sx1280_preambleLength_e
{
    PREAMBLE_LENGTH_04_BITS = 0x00,
    PREAMBLE_LENGTH_08_BITS = 0x10,
    PREAMBLE_LENGTH_12_BITS = 0x20,
    PREAMBLE_LENGTH_16_BITS = 0x30,
    PREAMBLE_LENGTH_20_BITS = 0x40,
    PREAMBLE_LENGTH_24_BITS = 0x50,
    PREAMBLE_LENGTH_28_BITS = 0x60,
    PREAMBLE_LENGTH_32_BITS = 0x70,
};

enum sx1280_syncwordLength_e
{
    SYNC_WORD_LEN_1_B = 0x00,
    SYNC_WORD_LEN_2_B = 0x02,
    SYNC_WORD_LEN_3_B = 0x04,
    SYNC_WORD_LEN_4_B = 0x06,
    SYNC_WORD_LEN_5_B = 0x08,
};

enum sx1280_syncwordCorrelator_e
{
    RADIO_SELECT_SYNCWORD_OFF = 0x00,
    RADIO_SELECT_SYNCWORD_1 = 0x10,
    RADIO_SELECT_SYNCWORD_2 = 0x20,
    RADIO_SELECT_SYNCWORD_1_2 = 0x30,
    RADIO_SELECT_SYNCWORD_3 = 0x40,
    RADIO_SELECT_SYNCWORD_1_3 = 0x50,
    RADIO_SELECT_SYNCWORD_2_3 = 0x60,
    RADIO_SELECT_SYNCWORD_1_2_3 = 0x70,
};

enum sx1280_headerType_e
{
    RADIO_PACKET_FIXED_LENGTH = 0x00,
    RADIO_PACKET_VARIABLE_LENGTH = 0x20,
};

enum sx1280_crcLength_e
{
    RADIO_CRC_OFF = 0x00,
    RADIO_CRC_1_BYTES = 0x10,
    RADIO_CRC_2_BYTES = 0x20,
};

enum sx1280_whitening_e
{
    WHITENING_ENABLE = 0x00,
    WHITENING_DISABLE = 0x08,
};

enum sx1280_rampTime_e
{
    RADIO_RAMP_02_US = 0x00,
    RADIO_RAMP_04_US = 0x20,
    RADIO_RAMP_06_US = 0x40,
    RADIO_RAMP_08_US = 0x60,
    RADIO_RAMP_10_US = 0x80,
    RADIO_RAMP_12_US = 0xA0,
    RADIO_RAMP_16_US = 0xC0,
    RADIO_RAMP_20_US = 0xE0,
};

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
        void WriteReg(uint16_t addr, uint8_t *buf, uint16_t size);
        void WaitOnBusy();
        void Reset();
        void WakeUp();

        void SetPacketType(uint8_t type);
        uint8_t GetPacketType();
        void SetModulationParams(uint8_t modParamSF, uint8_t modParamBW, uint8_t modParamCR);
        void SetPacketParams(uint8_t preambleLength, uint8_t syncWordLength, uint8_t syncCorrelatorLength, uint8_t headerType, uint8_t payloadLength, uint8_t crcLength, uint8_t whitening);        
        void SetRfFrequency(uint8_t * buf, uint8_t buf_size);
        void SetTxParams(uint8_t txPower, uint8_t rampTime);
        void SetBufferBaseAddress(uint8_t txBaseAddress, uint8_t rxBaseAddress);

        void getRXBufferStatus(uint8_t * buf, uint8_t size);

        void SetSyncWordReg(uint8_t * buf, uint8_t buf_size);

        void SetStandby(void);

        void WriteBuffer(uint8_t * buf, uint8_t buf_size);

};

#endif