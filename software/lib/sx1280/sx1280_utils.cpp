#include <sx1280_utils.h>

struct ErrorCodeMap modeMap[] = 
{
    {0x00, "reserved"},
    {0x01, "reserved"},
    {0x02, "STDBY_RC"},
    {0x03, "STDBY_XOSC"},
    {0x04, "FS"},
    {0x05, "Rx"},
    {0x06, "Tx"},
};

struct ErrorCodeMap errorMap[] =
{
    {0x00, "Reserved"},
    {0x01, "cmd successful"},
    {0x02, "data available"},
    {0x03, "command timeout"},
    {0x04, "cmd processing error"},
    {0x05, "cmd execution failed"},
    {0x06, "cmd Tx done"},
};

const char* getErrorMessage(int errorCode, struct ErrorCodeMap *errorcodeMap, size_t mapSize)
{
    for (size_t i = 0; i <mapSize; ++i)
    {
        if(errorcodeMap[i].code == errorCode)
        {
            return errorcodeMap[i].message;
        }
    }
    return "unknown err code";
}

const char* decodeStatusCode(char errorCode, char* buf, size_t buf_size)
{
    snprintf(buf, buf_size, "mode: %s, stat: %s", getErrorMessage((errorCode>>5)&0x7, modeMap, sizeof(modeMap)), getErrorMessage((errorCode>>2)&0x7, errorMap, sizeof(errorMap)));
    return buf;
}