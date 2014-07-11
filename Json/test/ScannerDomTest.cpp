

#include "gtest/gtest.h"
#include "JsonParser.h"
#include "ParserShiftReduce.h"
#include "ParserRecursive.h"
#include "Parser/ScannerDom.h"
#include <sstream>

using ThorsAnvil::Json::JsonParser;

TEST(ScannerDom, ShiftReduceScanMapEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<JsonParser<yy::ParserShiftReduce>>(json) == ThorsAnvil::Parser::ParserMapObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, ShiftReduceScanArrayEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<JsonParser<yy::ParserShiftReduce>>(json) == ThorsAnvil::Parser::ParserArrayObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}

TEST(ScannerDom, RecursiveScanMapEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<JsonParser<ThorsAnvil::Json::ParserRecursive>>(json) == ThorsAnvil::Parser::ParserMapObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, RecursiveScanArrayEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<JsonParser<ThorsAnvil::Json::ParserRecursive>>(json) == ThorsAnvil::Parser::ParserArrayObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}


TEST(ScannerDom, ShiftReduceScanMap)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<JsonParser<yy::ParserShiftReduce>>(json) == ThorsAnvil::Parser::ParserMapObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, ShiftReduceScanArray)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<JsonParser<yy::ParserShiftReduce>>(json) == ThorsAnvil::Parser::ParserArrayObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}

TEST(ScannerDom, RecursiveScanMap)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<JsonParser<ThorsAnvil::Json::ParserRecursive>>(json) == ThorsAnvil::Parser::ParserMapObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserMap>&  map    = scanner.getMap();
    ASSERT_TRUE(map.get() != NULL);
}

TEST(ScannerDom, RecursiveScanArray)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<JsonParser<ThorsAnvil::Json::ParserRecursive>>(json) == ThorsAnvil::Parser::ParserArrayObject);

    std::unique_ptr<ThorsAnvil::Parser::ParserArray>& array = scanner.getArray();
    ASSERT_TRUE(array.get() != NULL);
}


