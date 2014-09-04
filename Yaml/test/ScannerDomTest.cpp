

#include "gtest/gtest.h"
#include "YamlParser.h"
#include "YamlScanner.h"
#include <sstream>

using ThorsAnvil::Yaml::YamlParser;
using ThorsAnvil::Yaml::YamlScannerDom;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;

TEST(ScannerDom, ScanMapEmpty)
{
    YamlScannerDom                  scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, ScanArrayEmpty)
{
    YamlScannerDom                  scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}

TEST(ScannerDom, ScanMap)
{
    YamlScannerDom                  scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, ScanArray)
{
    YamlScannerDom                  scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}


