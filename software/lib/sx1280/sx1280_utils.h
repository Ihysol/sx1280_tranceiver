#ifndef SX1280_UTILS_H
#define SX1280_UTILS_H

#include <stdio.h>

struct ErrorCodeMap
{
    int code;
    const char *message;
};

extern struct ErrorCodeMap modeMap[];
extern struct ErrorCodeMap errorMap[];

const char* getErrorMessage(int errorCode, struct ErrorCodeMap *errorcodeMap, size_t mapSize);
const char* decodeStatusCode(char errorCode, char* buf, size_t buf_size);

#endif