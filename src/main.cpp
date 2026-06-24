#include <iostream>

#include "can_decoder/DbcParser.hpp"
#include "can_decoder/LogParser.hpp"

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: can_decoder <dbc_file> <log_file> <output_csv>\n";
        return 1;
    }

    DbcParser parser(argv[1]);
    if (!parser.parse())
    {
        std::cout << "Failed to parse dbc file!\n";
        return 1;
    }

    LogParser logParser(argv[2], argv[3]);
    if (!logParser.parse(parser))
    {
        std::cout << "Failed to parse the log file!\n";
        return 1;
    }
    
    std::cout << "Done. Output written to " << argv[3];
    return 0;
}