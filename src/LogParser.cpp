#include "can_decoder/LogParser.hpp"
#include "can_decoder/Compatibilty.hpp"

#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <charconv>


LogParser::LogParser(std::string logPath, std::string outputPath) :
    m_logPath(std::move(logPath)), m_outputPath(std::move(m_outputPath)) { }

bool LogParser::parse() 
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
        std::string CanId, payload;
        std::getline(frameSstream, CanId, '#');
        frameSstream >> payload;
        std::cout << "CanID = " << CanId << "\n";
        std::cout << "Payload: " << payload << "\n";
    }
    return true;
}