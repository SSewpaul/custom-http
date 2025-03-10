#define MAXDATASIZE 1024

class Frame {
    public:
        unsigned int length;
        unsigned int type;
        unsigned int flags;
        unsigned int stream_id;
        unsigned int *payload;
        
        Frame() = default;
        
        Frame (char (&buffer)[MAXDATASIZE]) {
            length = (unsigned int) buffer[0] << 16 | buffer[1] << 8 | buffer[2];
        }
};