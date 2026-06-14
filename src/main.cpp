#include <iostream>

#include "can_decoder/DbcParser.hpp"
#include "can_decoder/LogParser.hpp"

int main()
{
    // DbcParser parser("files/sample.dbc");
    // parser.parse();
    // parser.displayMessages();
    LogParser logParser("files/sample.log", "signals.out");
    logParser.parse();
    return 0;
}