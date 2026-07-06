#include "can_decoder/LogParser.hpp"
#include "can_decoder/Compatibilty.hpp"
#include "can_decoder/DbcParser.hpp"

#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <charconv>


LogParser::LogParser(std::string logPath, std::string outputPath) :
    m_logPath(std::move(logPath)), m_outputPath(std::move(outputPath)) { }

bool LogParser::parse(const DbcParser& parser) 
{
    std::ifstream inputFile(m_logPath);
    std::ofstream outputFile(m_outputPath);

    if (!inputFile.is_open())
    {
        std::cout << "Couldn't open " << m_logPath << " file!\n";
        return false;
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        CanFrame canFrame;
        std::istringstream sstreamLine(line);

        std::string timeStamp;
        sstreamLine >> timeStamp;
        std::string_view timeStampView(timeStamp.data() + 1, timeStamp.size() - 2);
        double ts;
        #ifdef HAS_FROM_CHARS_DOUBLE
            auto [ptr, ec] = std::from_chars(
                timeStampView.data(),
                timeStampView.data() + timeStampView.size(),
                ts
            );

            if (ec == std::errc{})
            {
                canFrame.timeStamp = ts;
            } else {
                std::cout << "Error parsing timestamp!\n";
                return false; 
            }
        #else
            try {
                canFrame.timeStamp = std::stod(std::string(timeStampView));
                std::cout << "Timestamp: " << canFrame.timeStamp << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error parsing timestamp: " << e.what() << "\n";
                return false;
            }
        #endif
        std::string iface, frameStr;
        sstreamLine >> iface >> frameStr;

        std::istringstream frameSstream(frameStr);
        std::string CanIdString, payloadString;
        std::getline(frameSstream, CanIdString, '#');
        frameSstream >> payloadString;
        canFrame.canID = static_cast<uint32_t>(std::stoul(CanIdString, nullptr, 16));
        auto message = parser.findMessage(canFrame.canID);
        if (message != nullptr)
        {
            // Store the payload in a uint8 array
            #ifdef HAS_FROM_CHARS_DOUBLE
            canFrame.dlc = payloadString.size() / 2;
            for (int i = 0; i < canFrame.dlc; i++)
            {
                const char* start = payloadString.data() + 2 * i;
                uint8_t value = 0;
                std::from_chars(start, start + 2, value, 16);
                canFrame.payload[i] = value;
            }
            #else
            canFrame.dlc = payloadString.size() / 2;
            for (int i = 0; i < canFrame.dlc; i++)
            {
                canFrame.payload[i] = static_cast<uint8_t>(std::stoul(payloadString.substr(i * 2, 2), nullptr, 16));
            }
            #endif
            // Decode the signal values and print into the output file
            for (const DbcSignal& signal : message->signals)
            {
                double signalValue = m_decoder.decode(canFrame.payload, signal);
                outputFile << canFrame.timeStamp << ","
                            << message->messageName << ","
                            << signal.signalName << ","
                            << signalValue << ","
                            << signal.unit << "\n";
            }
        } else {
            std::cout << "Signal not found!\n";
        }
    }
    return true;
}