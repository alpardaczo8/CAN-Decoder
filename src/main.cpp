#include <iostream>

#include "can_decoder/DbcParser.hpp"

int main()
{
    DbcParser parser("files/sample.dbc");
    parser.parse();
    parser.displayMessages();
    auto message = parser.findMessage(36);
    std::cout << (message != nullptr ? "Not nullptr" : "nullptr");
    return 0;
}