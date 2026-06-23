#pragma once

#include <can_decoder/DbcParser.hpp>

#include <cstdint>

class SignalDecoder 
{
public:
    double decode(const uint8_t *payload, const DbcSignal& signal) const;
private:
    uint64_t extractRawBigEndian(const uint8_t *payload, const DbcSignal& signal) const;
    uint64_t extractRawLittleEndian(const uint8_t *payload, const DbcSignal& signal) const;
    int64_t signExtend(uint64_t raw, uint32_t length) const;
};