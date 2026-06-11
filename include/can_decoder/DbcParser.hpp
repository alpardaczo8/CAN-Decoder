#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>

struct DbcSignal {
    std::string signalName;
    std::string unit; //optional
    double scale;
    double offset;
    uint8_t startBit;
    uint8_t length;
    bool isBigEndian;
    bool isSigned;
};

struct DbcMessage {
    std::string messageName;
    uint32_t messageId;
    std::vector<DbcSignal> signals;
};

class DbcParser 
{
public:
    explicit DbcParser (const std::string& filePath);
    bool parse();
    const DbcMessage* findMessage(uint32_t id) const;
    void displayMessages();
private:
    std::string m_filePath;
    std::unordered_map<uint32_t, DbcMessage> m_messages;
};