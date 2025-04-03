#include "baseFrame.hpp"

BaseFrame::BaseFrame(uint32_t length, uint8_t type, uint8_t flags, uint32_t streamId) : length(length), type(type), flags(flags)
{
    streamId &= 0x7FFFFFFF;
    this->streamId = streamId;
}

BaseFrame::BaseFrame(std::vector<uint8_t> &bytes)
{
    if (bytes.size() < HEADER_SIZE)
    {
        throw std::runtime_error("Frame is too short");
    }

    length = bytes[0] << 16 | bytes[1] << 8 | bytes[2];

    if (length > bytes.size())
    {
        throw std::runtime_error("Frame too short");
    }

    type = bytes[3];
    flags = bytes[4];

    streamId = ntohl(bytes[5] << 24 | bytes[6] << 16 | bytes[7] << 8 | bytes[8]) & 0x7FFFFFFF;
}

std::vector<uint8_t> BaseFrame::serialize()
{
    std::vector<uint8_t> bytes;

    // Length is 3 bytes long, hence shifting is needed to preserve value
    bytes.push_back(length >> 16 & 0xFF);
    bytes.push_back(length >> 8 & 0xFF);
    bytes.push_back(length & 0xFF);

    bytes.push_back(type);
    bytes.push_back(flags);

    // Convert stream id to network readable format
    uint32_t networkStreamId = htonl(streamId & 0x7FFFFFFF); // stream id is only 31 bit. MSB is reserved field and is always 0
    bytes.push_back(networkStreamId >> 24 & 0xFF);
    bytes.push_back(networkStreamId >> 16 & 0xFF);
    bytes.push_back(networkStreamId >> 8 & 0xFF);
    bytes.push_back(networkStreamId & 0xFF);

    return bytes;
}