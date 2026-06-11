#include "DbcParser.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>

DbcParser::DbcParser(const std::string& filePath) : m_filePath(filePath) {}

bool DbcParser::parse() 
{
    std::string line;
    std::ifstream DbcFile(m_filePath);
    std::optional<DbcMessage> currentMessage;

    while (std::getline(DbcFile, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;    
        
        if (token == "BO_")
        {
            uint32_t rawId = 0;
            std::string messageName;
            uint8_t length;
            std::string receiver;

            // check if we already read a DbcMessage before:
            // if yes, then store that message
            if (currentMessage.has_value())
            {
                m_messages[currentMessage->messageId] = *currentMessage;
            }
            // creating a new message
            currentMessage = DbcMessage{};

            ss >> rawId >> messageName >> length >> receiver;
            // removing ':' from name
            messageName.pop_back();
            // masking to the real CAN ID
            uint32_t canId = rawId & 0x7FFFFFFF;

            // setting the message name and id
            currentMessage->messageId = canId;
            currentMessage->messageName = messageName;
        }
        else if (token == "SG_")
        {
            DbcSignal signal{};
            std::string bitInfo, colon;

            ss >> signal.signalName >> colon;

            // extracting startBit, length and endianess
            ss >> bitInfo;
            std::istringstream bitSs(bitInfo); // bitInfo = "startBit|length@Endiannes(+/-)"
            std::string startBit, length;
            size_t stringSize = bitInfo.size();

            std::getline(bitSs, startBit, '|'); // extracts startBit, stops at '|'
            std::getline(bitSs, length, '@');  // extracts length, stops at '@'
            
            signal.startBit = std::stoi(startBit);
            signal.length = std::stoul(length);
            signal.isBigEndian = bitInfo[stringSize - 2] == '0';
            signal.isSigned = bitInfo[stringSize - 1] == '-';
            bitSs.clear();
            // extracting scale and offset
            ss >> bitInfo;
            bitSs = std::istringstream(bitInfo.substr(1, bitInfo.size()-1));
            std::string scale, offset;
            std::getline(bitSs, scale, ',');
            std::getline(bitSs, offset, ')');
            signal.scale = std::stod(scale);
            signal.offset = std::stod(offset);
            // extracting the unit
            ss >> bitInfo >> bitInfo;
            if (bitInfo != "") 
            {
                bitInfo.pop_back();
                signal.unit = bitInfo.substr(1);
            }
            // adding to the message
            currentMessage->signals.push_back(signal);
        }
        
    }
    if (currentMessage.has_value())
    {
        m_messages[currentMessage->messageId] = *currentMessage;
    }
    return true;
}

const DbcMessage* DbcParser::findMessage(uint32_t id) const
{
    auto it = m_messages.find(id);
    if (it == m_messages.end())
    {
        return nullptr;
    }
    return &it->second;
}

void DbcParser::displayMessages() 
{
    
    for (const auto&[id, message] : m_messages)
    {
        std::cout << "========== DBC MESSAGE ============\n";
        std::cout << "Message ID: " << message.messageId << "\n";
        std::cout << "Message Name: " << message.messageName << "\n";

        std::cout << "Signals:\n";
        for (const auto& signal : message.signals)
        {
            std::cout << signal.signalName << " : " << static_cast<int>(signal.startBit) 
                    << "|" << static_cast<int>(signal.length) << "@" 
                    << (signal.isBigEndian == true ? 0 : 1)
                    << (signal.isSigned == true ? '-' : '+' )
                    << " (" << signal.scale <<  ","
                    << signal.offset << ") \""
                    << signal.unit << "\"\n";
        }
    }
}