#include <can_decoder/SignalDecoder.hpp>

double SignalDecoder::decode(const uint8_t* payload, const DbcParser& dbcParser)
{

}

uint64_t SignalDecoder::extractRawBigEndian(const uint8_t *payload, const DbcSignal& signal) const
{

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
