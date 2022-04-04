#include "header.h"
#include "question.h"
#include "resource.h"
#include <vector>
#include <memory>
#include <string>

struct DNSPacket {
    DNSHeader header;
    std::vector<DNSQuestion> questions;
    std::vector<DNSResource> answers;
    std::vector<DNSResource> authorities;
    std::vector<DNSResource> additionals;

    DNSPacket(std::unique_ptr<uint8_t[]> raw);
    DNSPacket() = default;
    static DNSPacket answerFor(const DNSPacket &p);

    std::string summarize();
    uint16_t size();
    void write(uint8_t *dst);

private:
    void updateHeaderCount();
};