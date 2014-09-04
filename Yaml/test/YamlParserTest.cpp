
#include "gtest/gtest.h"
#include "Parser/ParserInterface.h"
#include "YamlParser.h"
#include <sstream>

using ThorsAnvil::Parser::ParserCleanInterface;
using ThorsAnvil::Parser::ParserLogInterface;
using ThorsAnvil::Parser::KeyGenVisitor;
using ThorsAnvil::Yaml::YamlParser;

TEST(YamlParserTest, EmptyMap)
{
    std::stringstream           json("{}");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, EmptyArray)
{
    std::stringstream           json("[]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithNumber)
{
    std::stringstream           json("[ 1 ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithString)
{
    std::stringstream           json("[ \"String\" ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithBoolTrue)
{
    std::stringstream           json("[ true ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithBoolFalse)
{
    std::stringstream           json("[ false ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithNull)
{
    std::stringstream           json("[ null ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithArray)
{
    std::stringstream           json("[ [] ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, ArrayWithMap)
{
    std::stringstream           json("[ {} ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithNumber)
{
    std::stringstream           json("{ \"item\":1 }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithString)
{
    std::stringstream           json("{ \"item\":\"String\" }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithBoolTrue)
{
    std::stringstream           json("{ \"item\":true }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithBoolFalse)
{
    std::stringstream           json("{ \"item\":false }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithNull)
{
    std::stringstream           json("{ \"item\":null }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithArray)
{
    std::stringstream           json("{ \"item\":[] }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MapWithMap)
{
    std::stringstream           json("{ \"item\":{} }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MultiItemArray)
{
    std::stringstream           json("[ 12, \"String\", true, false, null, { \"Item\": 1, \"Plop\": 2}, [ 1, 2] ]");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, MultiItemMap)
{
    std::stringstream           json("{ \"I1\": 12, \"I2\": \"String\", \"I3\": true, \"I4\": false, \"I5\": null, \"I6\": { \"Item\": 1, \"Plop\": 2}, \"I7\": [ 1, 2] }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

TEST(YamlParserTest, BadMapValue)
{
    std::stringstream           json("{ \"I1\": ] }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Parser::ParsingError);
}

TEST(YamlParserTest, BadArrayValue)
{
    std::stringstream           json("{ ] }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Parser::ParsingError);
}

TEST(YamlParserTest, BadMapMissingComma)
{
    std::stringstream           json("{ \"I1\": 12 \"I2\": 13 }");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_THROW(parser.parse(), ThorsAnvil::Parser::ParsingError);
}

TEST(YamlParserTest, HighLevelObject)
{
    std::stringstream           json("12");
    KeyGenVisitor               keyGenerator;
    ParserCleanInterface        interface(keyGenerator);
    YamlParser                  parser(json, interface);

    ASSERT_TRUE(parser.parse() == 0);
}

