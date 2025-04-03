#ifndef BASEFRAME_H
#define BASEFRAME_H
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <arpa/inet.h>

#define MAXDATASIZE 1024
#define HEADER_SIZE 9

class BaseFrame
{
private:
    uint32_t length;
    uint8_t type;
    uint8_t flags;
    uint32_t streamId;

public:
    BaseFrame() = default;

    BaseFrame(uint32_t length, uint8_t type, uint8_t flags, uint32_t streamId);

    BaseFrame(std::vector<uint8_t>& bytes);

    virtual std::vector<uint8_t> serialize();
};

#endif