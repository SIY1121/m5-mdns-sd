#include "const.h"
#include <utility>
#include <memory>
#include <string>

struct DNSHeader {
    uint16_t id = 0;
    QR qr;
    OPCODE opcode;
    bool aa = false;
    bool tc = false;
    bool rd = false;
    bool ra = false;
    // uint8_t Z;
    RCODE rcode = RCODE::NO_ERROR;
    uint16_t qdcount = 0;
    uint16_t ancount = 0;
    uint16_t nscount = 0;
    uint16_t arcount = 0;
    
    static const uint16_t rawSize = 12;
    DNSHeader(const uint8_t *raw, uint16_t &pos);
    DNSHeader() = default;
    std::string summarize();

    uint16_t size();
    void write(uint8_t *dst, uint16_t &pos);
};