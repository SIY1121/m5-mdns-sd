#include "header.h"
#include "utils.h"

DNSHeader::DNSHeader(const uint8_t *raw, uint16_t &pos) {
    id = readUInt16(raw, pos);

    auto [_qr, _opcode, _aa, _tc, _rd] = readBits(raw, pos, 0, range(1, 4), 5, 6, 7);
    qr = static_cast<QR>(_qr);
    opcode = static_cast<OPCODE>(_opcode);
    aa = _aa;
    tc = _tc;
    rd = _rd;

    auto [_ra, _rcode] = readBits(raw, pos, 0, range(4, 7));
    ra = _ra;
    rcode = static_cast<RCODE>(_ra);

    qdcount = readUInt16(raw, pos);
    ancount = readUInt16(raw, pos);
    nscount = readUInt16(raw, pos);
    arcount = readUInt16(raw, pos);
}

std::string DNSHeader::summarize() {
    return
    "id\t: " + std::to_string(id) + "\n"
    "qr\t: " + std::to_string(static_cast<bool>(qr)) + "\n"
    "opcode\t: " + std::to_string(static_cast<uint8_t>(opcode)) + "\n"
    "aa\t: " + std::to_string(aa) + "\n"
    "tc\t: " + std::to_string(tc) + "\n"
    "rd\t: " + std::to_string(rd) + "\n"
    "ra\t: " + std::to_string(ra) + "\n"
    "rcode\t: " + std::to_string(static_cast<uint8_t>(rcode)) + "\n"
    "qdcount\t: " + std::to_string(qdcount) + "\n"
    "ancount\t: " + std::to_string(ancount) + "\n"
    "nscount\t: " + std::to_string(nscount) + "\n"
    "arcount\t: " + std::to_string(arcount) + "\n"
    "\n\n";
}

uint16_t DNSHeader::size() {
    return 12;
}

void DNSHeader::write(uint8_t *dst, uint16_t &pos) {
    writeUInt16(dst, pos, id);
    writeBits(
        dst, pos,
        bb(static_cast<bool>(qr), 0),
        i8(static_cast<uint8_t>(opcode), range(1, 4)),
        bb(static_cast<bool>(aa), 5),
        bb(static_cast<bool>(tc), 6),
        bb(static_cast<bool>(rd), 7)
    );

    writeBits(
        dst, pos,
        bb(static_cast<bool>(ra), 0),
        i8(static_cast<uint8_t>(rcode), range(4, 7))
    );

    writeUInt16(dst, pos, qdcount);
    writeUInt16(dst, pos, ancount);
    writeUInt16(dst, pos, nscount);
    writeUInt16(dst, pos, arcount);
}