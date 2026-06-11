#include <iostream>

#include "can_decoder/DbcParser.hpp"

int main()
{
    DbcParser parser("files/sample.dbc");
    parser.parse();
    parser.displayMessages();
    return 0;
}