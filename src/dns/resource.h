#include "const.h"
#include <string>
#include <vector>
#include <memory>
#include <IPAddress.h>

struct DNSResource {
    std::vector<std::string> name;
    TYPE type;
    CLASS clazz;
    uint32_t ttl;
    uint16_t rdLength = 0;
    std::unique_ptr<uint8_t[]> rdata;
    DNSResource(const uint8_t *raw, uint16_t &pos);
    DNSResource() = default;
    std::string summarize();

    uint16_t size();
    void write(uint8_t *dst, uint16_t &pos);

    static DNSResource createARecord(std::string name, IPAddress addr);
    static DNSResource createTXTRecord(std::string name, std::string data);
    static DNSResource createSRVRecord(std::string service, std::string protocol, std::string name, std::string target);
    static DNSResource createPTRRecord(std::string name, std::string data);
};