#include "utils.h"

bool _readBits(const uint8_t value, uint8_t e) {
    return value & (1 << (7 - e));
}

uint8_t _readBits(const uint8_t value, std::pair<uint8_t, uint8_t> e){
    auto [start, end] = e;
    return (uint8_t)(value << start) >> (7 - (end - start));
}

uint8_t readUInt8(const uint8_t *src, uint16_t &pos) {
    return src[pos++];
}

uint16_t readUInt16(const uint8_t *src, uint16_t &pos) {
    return src[pos++] << 8 | src[pos++];
}

uint32_t readUInt32(const uint8_t *src, uint16_t &pos) {
    return src[pos++] << 24 | src[pos++] << 16 | src[pos++] << 8 | src[pos++];
}

void readString(const uint8_t *src, std::vector<std::string> &data,uint16_t &pos) {
    while(true) {
        auto len = src[pos++];
        if(len == '\0')
            break; // 終端文字なら終了
        else if(len >= 0b11000000) {
            // 圧縮ポインタの場合、指定された箇所を読みに行って終了
            uint16_t next = ((len & 0b00111111) << 8) | src[pos++];
            readString(src, data, next);
            break;
        } else {
            // それ以外は指定された文字数を読み取る
            std::string result = "";
            for(uint8_t i = 0; i < len; i++) {
                result += (char)src[pos++];
            }
            data.push_back(result);
        }
    }
}

void readData(const uint8_t* src, uint8_t* dst, uint16_t len, uint16_t &pos) {
    std::memcpy(dst, src, len);
    pos += len;
}

void _writeBits(uint8_t *src, std::pair<bool, uint8_t> e) {
    auto [value, offset] = e;
    *src |= value << (7 -  offset);
}
void _writeBits(uint8_t *src, std::pair<uint8_t, std::pair<uint8_t, uint8_t>> e) {
    auto [value, range] = e;
    auto [start, end] = range;
    *src |= (value << (7 - (end - start))) >> start;
}

void writeUInt8(uint8_t *dst, uint16_t &pos, uint8_t value) {
    dst[pos++] = value;
}
void writeUInt16(uint8_t *dst, uint16_t &pos, uint16_t value) {
    dst[pos++] = value >> 8;
    dst[pos++] = value;
}
void writeUInt32(uint8_t *dst, uint16_t &pos, uint32_t value) {
    dst[pos++] = value >> 24;
    dst[pos++] = value >> 16;
    dst[pos++] = value >> 8;
    dst[pos++] = value;
}

void writeData(uint8_t *dst, uint16_t &pos, uint8_t *src, uint16_t len) {
    std:memcpy(dst + pos, src, len);
    pos += len;
}

uint16_t writeStringSize(const std::vector<std::string> &data) {
    uint16_t res = 0;
    for(auto &d: data)
        res += d.length();
    return res + data.size() + 1;
}

void writeString(uint8_t *dst, uint16_t &pos, std::vector<std::string> &data) {
    for(auto &d: data) {
        dst[pos++] = d.length();
        d.copy((char*)(dst + pos), d.length());
        pos += d.length();
    }
    dst[pos++] = '\0';
}

std::string join(const std::vector<std::string> data, const std::string dlim) {
    if(data.empty()) return "";
    std::string res = "";
    for(auto &d: data)
        res += d + dlim;
    return res.erase(res.size() - dlim.size());
}

std::vector<std::string> split(const std::string _data, const std::string dlim) {
    std::vector<std::string> res;
    if(_data.empty()) return res;
    std::string data(_data);
    size_t pos = 0;
    while ((pos = data.find(dlim)) != std::string::npos) {
        res.push_back(data.substr(0, pos));
        data.erase(0, pos + dlim.length());
    }
    res.push_back(data);
    return res;
}