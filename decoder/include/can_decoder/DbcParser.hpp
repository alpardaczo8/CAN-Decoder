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
    uint32_t startBit;
    uint32_t length;
    bool isBigEndian;
    bool isSigned;
    std::unordered_map<int32_t, std::string> valueTable; // to be handled
};

struct DbcMessage {
    std::string messageName;
    std::string sender;
    uint32_t messageId;
    uint8_t dlc;
    std::vector<DbcSignal> signals;
};

class DbcParser 
{
public:
    explicit DbcParser (const std::string& filePath);
    bool parse();
    const DbcMessage* findMessage(uint32_t id) const;
    void displayMessages() const;
private:
    std::string m_filePath;
    std::unordered_map<uint32_t, DbcMessage> m_messages;
};