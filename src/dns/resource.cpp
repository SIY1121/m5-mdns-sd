#include "resource.h"
#include "utils.h"
#include <cstring>

DNSResource::DNSResource(const uint8_t* src, uint16_t &pos) {
    readString(src, name, pos);
    type = static_cast<TYPE>(readUInt16(src, pos));
    clazz = static_cast<CLASS>(readUInt16(src, pos));
    ttl = readUInt32(src, pos);
    rdLength = readUInt16(src, pos);
    rdata = std::make_unique<uint8_t[]>(rdLength);
    readData(src, rdata.get(), rdLength, pos);
}

std::string DNSResource::summarize() {
    return "-\n"
    "name\t: " + join(name, ".") + "\n"
    "type\t: " + std::to_string(static_cast<uint16_t>(type)) + "\n"
    "class\t: " + std::to_string(static_cast<uint16_t>(clazz)) + "\n"
    "ttl\t: " + std::to_string(ttl) + "\n"
    "rdLength\t: " + std::to_string(rdLength) + "\n"
    // "rdata\t: " + std::string((char*)rdata.get(), rdLength) + "\n"
    "\n";
}

uint16_t DNSResource::size() {
    return writeStringSize(name) + 2 + 2 + 4 + 2 + rdLength;
}

void DNSResource::write(uint8_t *dst, uint16_t &pos) {
    writeString(dst, pos, name);
    writeUInt16(dst, pos, static_cast<uint16_t>(type));
    writeUInt16(dst, pos, static_cast<uint16_t>(clazz));
    writeUInt32(dst, pos, ttl);
    writeUInt16(dst, pos, rdLength);

    writeData(dst, pos, rdata.get(), rdLength);
}

DNSResource DNSResource::createARecord(std::string name, IPAddress addr) {
    auto r = DNSResource();
    r.name = split(name, ".");
    r.type = TYPE::A;
    r.clazz = CLASS::IN;
    r.ttl = 60;
    r.rdLength = 4;
    auto a =  (uint32_t)addr;
    r.rdata = std::make_unique<uint8_t[]>(4);
    r.rdata[0] = a ;
    r.rdata[1] = a >> 8;
    r.rdata[2] = a >> 16;
    r.rdata[3] = a >> 24;

    return r;
}

DNSResource DNSResource::createTXTRecord(std::string name, std::string data) {
    auto r = DNSResource();
    r.name = split(name, ".");
    r.type = TYPE::TXT;
    r.clazz = CLASS::IN;
    r.ttl = 60;
    r.rdLength = data.length();
    r.rdata = std::make_unique<uint8_t[]>(r.rdLength);
    data.copy((char*)r.rdata.get(), r.rdLength);
    return r;
}

DNSResource DNSResource::createPTRRecord(std::string name, std::string data) {
    auto r = DNSResource();
    r.name = split(name, ".");
    r.type = TYPE::PTR;
    r.clazz = CLASS::IN;
    r.ttl = 60;

    auto _data = split(data, ".");
    r.rdLength = writeStringSize(_data);
    r.rdata = std::make_unique<uint8_t[]>(r.rdLength);
    uint16_t pos = 0;
    writeString(r.rdata.get(), pos, _data);
    return r;
}

DNSResource DNSResource::createSRVRecord(std::string service, std::string protocol, std::string name, std::string target) {
    auto r = DNSResource();
    r.name = std::vector{service, protocol, name};
    r.type = TYPE::SRV;
    r.clazz = CLASS::IN;
    r.ttl = 60;
    
    uint16_t pos = 0;

    auto _target = split(target, ".");
    r.rdLength = 2 + 2 + 2 + writeStringSize(_target);
    r.rdata = std::make_unique<uint8_t[]>(r.rdLength);

    uint16_t priority = 0;
    uint16_t weight = 0;
    uint16_t port = 80;

    writeUInt16(r.rdata.get(), pos, priority);
    writeUInt16(r.rdata.get(), pos, weight);
    writeUInt16(r.rdata.get(), pos, port);
    writeString(r.rdata.get(), pos, _target);

    return r;
}