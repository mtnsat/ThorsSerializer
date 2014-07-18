

#include "gtest/gtest.h"
#include "JsonParser.h"
#include "ParserShiftReduce.h"
#include "ParserRecursive.h"
#include "Parser/ScannerDom.h"
#include <sstream>

using ThorsAnvil::Json::JsonParser;
using ThorsAnvil::Json::JsonParserAlt;
using ThorsAnvil::Parser::ParserMap;

TEST(ScannerDom, ShiftReduceScanMapEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<JsonParser>(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, ShiftReduceScanArrayEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<JsonParser>(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}

TEST(ScannerDom, RecursiveScanMapEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<JsonParserAlt>(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, RecursiveScanArrayEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<JsonParserAlt>(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}


TEST(ScannerDom, ShiftReduceScanMap)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<JsonParser>(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, ShiftReduceScanArray)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<JsonParser>(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}

TEST(ScannerDom, RecursiveScanMap)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<JsonParserAlt>(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, RecursiveScanArray)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<JsonParserAlt>(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}


