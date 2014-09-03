

#include "gtest/gtest.h"
#include "YamlParser.h"
#include "Parser/ScannerDom.h"
#include <sstream>

using ThorsAnvil::Yaml::YamlParser;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;

TEST(ScannerDom, ScanMapEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{}");

    ASSERT_TRUE(scanner.parse<YamlParser>(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, ScanArrayEmpty)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[]");

    ASSERT_TRUE(scanner.parse<YamlParser>(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}

TEST(ScannerDom, ScanMap)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("{ \"I1\": 1, \"I2\": \"S\", \"I3\": true, \"I4\": false, \"I6\": null, \"I7\": {}, \"I8\": [] }");

    ASSERT_TRUE(scanner.parse<YamlParser>(json) == ThorsAnvil::Parser::ParserMapObject);

    ParserMap&  map    = scanner.getMap();
}

TEST(ScannerDom, ScanArray)
{
    ThorsAnvil::Parser::ScannerDom    scanner;
    std::stringstream               json("[ 1, \"S\", true, false, null, {}, [] ]");

    ASSERT_TRUE(scanner.parse<YamlParser>(json) == ThorsAnvil::Parser::ParserArrayObject);

    ParserArray& array = scanner.getArray();
}


