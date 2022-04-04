#include "const.h"
#include <string>
#include <vector>

struct DNSQuestion {
    std::vector<std::string> qname;
    QTYPE qtype;
    QCLASS qclass;

    uint16_t rawSize;
    DNSQuestion(const uint8_t *raw, uint16_t &pos);
    std::string summarize();
};