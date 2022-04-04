#include "question.h"
#include "utils.h"

DNSQuestion::DNSQuestion(const uint8_t *src, uint16_t &pos) {
    readString(src, qname, pos);
    qtype = static_cast<QTYPE>(readUInt16(src, pos));
    qclass = static_cast<QCLASS>(readUInt16(src, pos));
}

std::string DNSQuestion::summarize() {
    return "-\n"
    "qname\t: " + join(qname, ".") + "\n"
    "qtype\t: " + std::to_string(static_cast<uint16_t>(qtype)) + "\n"
    "qclass\t: " + std::to_string(static_cast<uint16_t>(qclass)) + "\n"
    "\n";
}