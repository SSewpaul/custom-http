#include <cstdint>
#include <vector>

#define MAXDATASIZE 1024
#define HEADER_SIZE 9

class Frame
{
public:
    uint32_t length;
    uint8_t type;
    uint8_t flags;
    uint32_t streamId;
    std::vector<uint8_t> payload;

    Frame() = default;

    Frame(uint8_t type, uint8_t flags, uint32_t streamId, std::vector<uint8_t>& payload) : type(type), flags(flags), payload(payload) {
        length = static_cast<uint32_t>(payload.size());
        streamId &= 0x7FFFFFFF;
        this->streamId = streamId;
    }

    Frame(std::vector<uint8_t>& bytes) {
        if (bytes.size() < HEADER_SIZE) {
            throw std::runtime_error("Frame is too short");
        }

        length = bytes[0] << 16 | bytes[1] << 8 | bytes[2];
        
        if (length > bytes.size()) {
            throw std::runtime_error("Frame too short");
        }

        type = bytes[3];
        flags = bytes[4];

        streamId = ntohl(bytes[5] << 24 | bytes[6] << 16 | bytes[7] << 8 | bytes[8]) & 0x7FFFFFFF;

        if (bytes.size() > HEADER_SIZE) {
            payload.insert(payload.begin(), bytes.begin() + HEADER_SIZE, bytes.end());
        }
    }

    virtual std::vector<uint8_t> getPayload();

    std::vector<uint8_t> serialize() {
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

        // Append payload to vector
        if (payload.size() > 0) {
            bytes.insert(bytes.end(), payload.begin(), payload.end());
        }

        return bytes;
    }
};