#pragma once

/*
パケットの処理に必要な便利関数の実装
*/

#include <stdint.h>
#include <utility>
#include <string>
#include <vector>
#include <cstring>

bool _readBits(uint8_t value, uint8_t e);
uint8_t _readBits(uint8_t value, std::pair<uint8_t, uint8_t> e);

template<typename... Args>
auto readBits(const uint8_t *src, uint16_t &pos, Args... args) {
    auto result = std::tuple{_readBits(src[pos], args)...};
    pos++;
    return result;
}

uint8_t readUInt8(const uint8_t *src, uint16_t &pos);
uint16_t readUInt16(const uint8_t *src, uint16_t &pos);
uint32_t readUInt32(const uint8_t *src, uint16_t &pos);

void readString(const uint8_t *src, std::vector<std::string> &data, uint16_t &pos);

void readData(const uint8_t* src, uint8_t* dst, uint16_t len, uint16_t &pos);
void _writeBits(uint8_t *src, std::pair<bool, uint8_t> e);
void _writeBits(uint8_t *src, std::pair<uint8_t, std::pair<uint8_t, uint8_t>> e);

template<typename... Args>
void writeBits(uint8_t *src, uint16_t &pos, Args... args) {
    ((void) _writeBits(src + pos, args), ...);
    pos++;
}

void writeUInt8(uint8_t *dst, uint16_t &pos, uint8_t value);
void writeUInt16(uint8_t *dst, uint16_t &pos, uint16_t value);
void writeUInt32(uint8_t *dst, uint16_t &pos, uint32_t value);

void writeData(uint8_t *dst, uint16_t &pos, uint8_t *src, uint16_t len);

uint16_t writeStringSize(const std::vector<std::string> &data);
void writeString(uint8_t *dst, uint16_t &pos, std::vector<std::string> &data);

constexpr auto range = std::make_pair<uint8_t, uint8_t>;
constexpr auto bb = std::make_pair<bool, uint8_t>;
constexpr auto i8 = std::make_pair<uint8_t, std::pair<uint8_t, uint8_t>>;

std::string join(const std::vector<std::string> data, const std::string dlim);
std::vector<std::string> split(const std::string data, const std::string dlim);