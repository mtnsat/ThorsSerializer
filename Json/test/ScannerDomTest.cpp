

#include "gtest/gtest.h"
#include "JsonParser.h"
#include "ParserShiftReduce.h"
#include "ParserRecursive.h"
#include "JsonScanner.h"
#include <sstream>

using ThorsAnvil::Json::JsonParser;
using ThorsAnvil::Json::JsonParserAlt;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Json::JsonScannerDom;
using ThorsAnvil::Json::JsonScannerAltDom;

TEST(ScannerDom, ShiftReduceScanMapEmpty)
{
    JsonScannerDom                  scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserValue&  map    = scanner.getValue();
}

TEST(ScannerDom, ShiftReduceScanArrayEmpty)
{
    JsonScannerDom                  scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserValue& array = scanner.getValue();
}

TEST(ScannerDom, RecursiveScanMapEmpty)
{
    JsonScannerAltDom               scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserValue&  map    = scanner.getValue();
}

TEST(ScannerDom, RecursiveScanArrayEmpty)
{
    JsonScannerAltDom               scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserValue& array = scanner.getValue();
}


TEST(ScannerDom, ShiftReduceScanMap)
{
    JsonScannerDom                  scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserValue&  map    = scanner.getValue();
}

TEST(ScannerDom, ShiftReduceScanArray)
{
    JsonScannerDom                  scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserValue& array = scanner.getValue();
}

TEST(ScannerDom, RecursiveScanMap)
{
    JsonScannerAltDom               scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserValue&  map    = scanner.getValue();
}

TEST(ScannerDom, RecursiveScanArray)
{
    JsonScannerAltDom               scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserValue& array = scanner.getValue();
}


