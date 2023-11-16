#include <sx1280.h>

char err_buf[50];

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
    uint8_t err = 0;
    WaitOnBusy();

    digitalWrite(m_ss, LOW);
    err = m_spi.transfer((uint8_t)command);
    Serial.println(decodeStatusCode(err, err_buf, sizeof(err_buf)));

    for(uint16_t i = 0; i < size; i++)
    {
        err = m_spi.transfer(buf[i]);
        Serial.println(decodeStatusCode(err, err_buf, sizeof(err_buf)));
    }
    digitalWrite(m_ss, HIGH);
}

void SX1280::ReadCommand(uint8_t command, uint8_t * buf, uint16_t size)
{
    uint8_t err = 0;
    WaitOnBusy();

    digitalWrite(m_ss, LOW);
    err = m_spi.transfer((uint8_t) command);
    Serial.println(decodeStatusCode(err, err_buf, sizeof(err_buf)));

    err = m_spi.transfer(0x00);
    Serial.println(decodeStatusCode(err, err_buf, sizeof(err_buf)));

    for(uint16_t i=  0; i < size; i++)
    {
        buf[i] = m_spi.transfer(0x00);
    }
    digitalWrite(m_ss, HIGH);

    WaitOnBusy();
}

void SX1280::WriteReg(uint16_t addr, uint8_t *buf, uint16_t size)
{
    std::vector<uint8_t> addr_buf = {static_cast<uint8_t>((addr>>8) & 0xff), static_cast<uint8_t>(addr&0xff)};
    std::vector<uint8_t> mergedBuffer;
    mergedBuffer.reserve(addr_buf.size()+ size);
    mergedBuffer.insert(mergedBuffer.end(), addr_buf.begin(), addr_buf.end());
    mergedBuffer.insert(mergedBuffer.end(), buf, buf+size);

    WriteCommand(0x18, mergedBuffer.data(), mergedBuffer.size());
}

void SX1280::SetPacketType(uint8_t type)
{
    std::vector<uint8_t> buf = {type};
    WriteCommand(0x8A, buf.data(), buf.size());
}

uint8_t SX1280::GetPacketType()
{
    std::vector<uint8_t> buf = {0};
    ReadCommand(0x03, buf.data(), buf.size());
    return buf[0];
}

void SX1280::SetModulationParams(uint8_t modParamSF, uint8_t modParamBW, uint8_t modParamCR)
{
    std::vector<uint8_t> buf = {modParamSF, modParamSF, modParamCR};
    WriteCommand(0x8B, buf.data(), buf.size());
}

/***
 * NOTE: accepts up to 7 arguments. However in BLE mode only 4 are accepted and the remaining 3 MUST be set to 0!
*/
void SX1280::SetPacketParams(uint8_t preambleLength, uint8_t syncWordLength, uint8_t syncCorrelatorLength, uint8_t headerType, uint8_t payloadLength, uint8_t crcLength, uint8_t whitening)
{
    std::vector<uint8_t> buf = {preambleLength, syncWordLength, syncCorrelatorLength, headerType, payloadLength, crcLength, whitening};
    WriteCommand(0x8C, buf.data(), buf.size());
}

void SX1280::SetSyncWordReg(uint8_t * buf, uint8_t buf_size)
{
    WriteReg(0x09CE, buf, buf_size);
}

void SX1280::SetRfFrequency(uint8_t * buf , uint8_t buf_size)
{
    // TODO: check if shifting is correct
    // std::vector<uint8_t> buf = {static_cast<uint8_t>((rfFrequency>>16)&0xff), static_cast<uint8_t>((rfFrequency>>8)&0xff), static_cast<uint8_t>(rfFrequency&0xff)};
    WriteCommand(0x86, buf, buf_size);
}

/***
 * NOTE:
 * @param txPower is calculated in dB by the following formula: P_out[db] = -18+power.
 * P_out_min = -18dBm (power = 0)
 * P_out_max = 13dBm (power = 31)
 * @param rampTime is defined in Table 11-49 of datasheet
*/
void SX1280::SetTxParams(uint8_t txPower, uint8_t rampTime)
{
    std::vector<uint8_t> buf = {txPower, rampTime};
    WriteCommand(0x8E, buf.data(), buf.size());
}

/***
 * default initialization for both @param txBaseAddress and @param rxBaseAddress are 0x00 
 * to utilize full buffer size for both rx and tx operations.
*/
void SX1280::SetBufferBaseAddress(uint8_t txBaseAddress, uint8_t rxBaseAddress)
{
    std::vector<uint8_t> buf = {txBaseAddress, rxBaseAddress};
    WriteCommand(0x8F, buf.data(), buf.size());
}

void SX1280::getRXBufferStatus(uint8_t * buf, uint8_t size)
{
    ReadCommand(0x17, buf, size);
}

void SX1280::SetStandby(void)
{
    std::vector<uint8_t> buf = {0x00};
    WriteCommand(0x80, buf.data(), buf.size());
}

void SX1280::WriteBuffer(uint8_t * buf, uint8_t buf_size)
{
    WriteCommand(0x1A, buf, buf_size);
}

