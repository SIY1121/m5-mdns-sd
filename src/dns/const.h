#pragma once
#include <stdint.h>

enum class QR: bool {
    QUERY, RESPONSE
};
enum class OPCODE: uint8_t {
    QUERY, IQUERY, STATUS
};
enum class RCODE {
    NO_ERROR, FORMAT_ERROR, SERVER_FAILURE, NAME_ERROR, NOT_IMPLEMENTED, REFUSED
};
enum class TYPE: uint8_t {
    A = 1, NS, MD, MF, CNAME, SOA, MB, MBG, MR_, NULL_, WKS, PTR, HINFO, MINFO, MX, TXT, SRV = 33
};
enum class QTYPE: uint8_t {
    A = 1, NS, MD, MF, CNAME, SOA, MB, MBG, MR_, NULL_, WKS, PTR, HINFO, MINFO, MX, TXT, SRV = 33,
    AXFR = 252, MAILB, MAILA, ALL
};
enum class CLASS: uint8_t {
    IN = 1, CS, CH, HS
};
enum class QCLASS: uint8_t {
    IN = 1, CS, CH, HS, ALL = 255,
};
