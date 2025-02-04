#include <string>
#include <ctime>

#define ENTITY_HEADER_FIELDS ["Allow", "Content-Encoding", "Content-Length", "Content-Type", "Expires", "Last-Modified"]

class EntityHeader {
    public:
        std::string allow;
        std::string content_encoding;
        uint content_length;
        std::string content_type;
        time_t expires;
        time_t last_modified;

        EntityHeader() = default;
};