#include <can_decoder/SignalDecoder.hpp>

double SignalDecoder::decode(const uint8_t* payload, const DbcSignal& signal) const
{
    uint64_t raw = signal.isBigEndian
        ? extractRawBigEndian(payload,signal) 
        : extractRawLittleEndian(payload, signal);

    if (signal.isSigned)
    {
        int64_t signedRaw = signExtend(raw, signal.length);
        return static_cast<double>(signedRaw) * signal.scale + signal.offset;
    }

    return static_cast<double>(raw) * signal.scale + signal.offset;
}

uint64_t SignalDecoder::extractRawBigEndian(const uint8_t *payload, const DbcSignal& signal) const
{
    uint64_t raw = 0;
    uint32_t bit = signal.startBit;

    for (uint32_t i = 0; i < signal.length; i++)
    {
        uint32_t byteIdx = bit / 8;
        uint32_t bitIdx = bit % 8;
        
        uint32_t valueBitPos = signal.length - 1 - i;

        if (payload[byteIdx] & (1 << bitIdx))
        {
            raw |= (1ULL << valueBitPos);
        }

        if (bit % 8 == 0)
        {
            bit += 15; 
        } else {
            bit -= 1;
        }
    }
    return raw; 
}

uint64_t SignalDecoder::extractRawLittleEndian(const uint8_t *payload, const DbcSignal& signal) const
{
    uint64_t raw = 0;
    
    for (int i = 0; i < 8; i++)
    {
        raw |= (static_cast<uint64_t>(payload[i]) << (8 * i));
    }

    uint64_t shifted = raw >> signal.startBit;
    uint64_t mask = (signal.length == 64) ? ~0ULL : ((1ULL << signal.length) - 1);

    return shifted & mask;
}

int64_t SignalDecoder::signExtend(uint64_t raw, uint32_t length) const
{
    // check if MSB of the signal is 1 (length-1 position) is set
    if (raw & (1ULL << (length - 1)))
    {
        // fill up all the bits with 1 above the length
        raw |= ~((1ULL << length) - 1);
    }
    return static_cast<int64_t>(raw);
}