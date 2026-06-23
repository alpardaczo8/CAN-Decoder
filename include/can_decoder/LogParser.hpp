#pragma once

#include <cstdint>
#include <string>

#include <can_decoder/DbcParser.hpp>
#include <can_decoder/SignalDecoder.hpp>

struct CanFrame
{
    double timeStamp;
    uint32_t canID;
    uint8_t dlc;
    uint8_t payload[8];
};

class LogParser {
public:
    LogParser(std::string logPath, std::string outputPath);
    bool parse();
private:
    std::string m_logPath;
    std::string m_outputPath;
    SignalDecoder m_decoder;
};