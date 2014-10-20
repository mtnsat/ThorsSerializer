
#include "gtest/gtest.h"
#include "JsonScanner.h"
#include "JsonEmitter.h"
#include "Parser/DomSerialize.h"

using ThorsAnvil::Json::JsonScannerDom;
using ThorsAnvil::Json::JsonEmitter;
using ThorsAnvil::Parser::DomSerialize;

TEST(JsonSerialize, EmptyMap)
{
    std::stringstream   json("{}");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserMapObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("{}", result.str());
}

TEST(JsonSerialize, EmptyArray)
{
    std::stringstream   json("[]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[]", result.str());
}

TEST(JsonSerialize, ArrayEmptyString)
{
    std::stringstream   json("[\"\"]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[\"\"]", result.str());
}

TEST(JsonSerialize, ArrayFullString)
{
    std::stringstream   json("[\"Hi there\"]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[\"Hi there\"]", result.str());
}

TEST(JsonSerialize, ArrayNumber)
{
    std::stringstream   json("[56]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[56]", result.str());
}

TEST(JsonSerialize, ArrayNull)
{
    std::stringstream   json("[null]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[null]", result.str());
}

TEST(JsonSerialize, EmptyBool)
{
    std::stringstream   json("[true]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[true]", result.str());
}

TEST(JsonSerialize, ArrayWithEverything)
{
    std::stringstream   json("[\"A string\", 45.67, null, false]");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserArrayObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("[\"A string\", 45.67, null, false]", result.str());
}
TEST(JsonSerialize, MapWithEverything)
{
    std::stringstream   json("{\"String\": \"A string\", \"Number\": 45.67, \"Null\": null, \"Bool\": false}");
    JsonScannerDom      scanner;

    ASSERT_EQ(ThorsAnvil::Parser::ParserMapObject, scanner.parse(json));

    std::stringstream   result;
    result  << DomSerialize<JsonEmitter>(scanner.getValue());
    ASSERT_EQ("{\"Bool\": false, \"Null\": null, \"Number\": 45.67, \"String\": \"A string\"}", result.str());
}


