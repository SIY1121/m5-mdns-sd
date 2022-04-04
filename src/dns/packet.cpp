#include "packet.h"

DNSPacket::DNSPacket(std::unique_ptr<uint8_t[]> raw) {
    uint16_t pos = 0;
    header = DNSHeader(raw.get(), pos);
    for(uint16_t i = 0; i < header.qdcount; i++) {
        questions.push_back(DNSQuestion(raw.get(), pos));
    }
    for(uint16_t i = 0; i < header.ancount; i++) {
        answers.push_back(DNSResource(raw.get(), pos));
    }
    for(uint16_t i = 0; i < header.nscount; i++) {
        authorities.push_back(DNSResource(raw.get(), pos));
    }
    for(uint16_t i = 0; i < header.arcount; i++) {
        additionals.push_back(DNSResource(raw.get(), pos));
    }
}

DNSPacket DNSPacket::answerFor(const DNSPacket &p) {
    auto newPacket = DNSPacket();
    newPacket.header.id = p.header.id;
    newPacket.header.qr = QR::RESPONSE;
    newPacket.header.opcode = p.header.opcode;
    return newPacket;
}

std::string DNSPacket::summarize() {
    updateHeaderCount();
    std::string result = "----------------------------------------------\n";
    result += "header\n";
    result += header.summarize();

    if(questions.size() > 0) {
        result += "questions\n";
        for(auto &q: questions)
            result += q.summarize();
    }
    if(answers.size() > 0) {
        result += "answers\n";
        for(auto &a: answers)
            result += a.summarize();
    }
    if(authorities.size() > 0) {
        result += "authorities\n";
        for(auto &a: authorities)
            result += a.summarize();
    }
    if(additionals.size() > 0) {
        result += "additionals\n";
        for(auto &a: additionals)
            result += a.summarize();
    }

    return result + "\n\n";
}

uint16_t DNSPacket::size() {
    updateHeaderCount();
    
    uint16_t size = 0;
    size += header.size();
    for(auto &a: answers)
        size += a.size();
    for(auto &a: authorities)
        size += a.size();
    for(auto &a: additionals)
        size += a.size();
    
    return size;
}

void DNSPacket::write(uint8_t *dst) {
    updateHeaderCount();
    uint16_t pos = 0;
    header.write(dst, pos);
    for(auto &a: answers)
        a.write(dst, pos);
    for(auto &a: authorities)
        a.write(dst, pos);
    for(auto &a: additionals)
        a.write(dst, pos);
}

void DNSPacket::updateHeaderCount() {
    header.qdcount = questions.size();
    header.ancount = answers.size();
    header.nscount = authorities.size();
    header.arcount = additionals.size();
}