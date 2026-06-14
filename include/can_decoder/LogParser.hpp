#pragma once

#include <stdint.h>
#include <string>

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
};