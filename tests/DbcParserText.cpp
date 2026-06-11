#include <gtest/gtest.h>
#include <can_decoder/DbcParser.hpp>

TEST(DbcParserTest, FindDbcMessageByIdSuccessful) {
    DbcParser parser(std::string(TEST_FILES_DIR) + "/sample.dbc");
    parser.parse();

    const DbcMessage* message;
    message = parser.findMessage(36);
    ASSERT_NE(message, nullptr);
}

TEST(DbcParserTest, FindDbcMessageByIdFailed) {
    DbcParser parser("files/sample.dbc");
    parser.parse();

    const DbcMessage* message;
    message = parser.findMessage(32U);
    ASSERT_EQ(message, nullptr);
}