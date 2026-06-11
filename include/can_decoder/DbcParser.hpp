#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct DbcSignal {
    std::string signalName;
    std::string unit; //optional
    double scale;
    double offset;
    uint8_t startBit;
    uint8_t length;
    bool isBigEndian;
};

struct DbcMessage {
    std::string messageName;
    uint32_t messageId;
    std::vector<DbcSignal> signals;
};

class DbcParser 
{
public:
    DbcParser (std::string filePath);
    bool parse();
    const DbcMessage* findMessage(uint32_t id) const;
private:
    std::string m_filePath;
    std::unordered_map<uint32_t, DbcMessage> m_messages;
};