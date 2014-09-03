#include "gtest/gtest.h"
#include "YamlParser.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserInterface.h"
#include <sstream>
#include <fstream>
#include <string>

using ThorsAnvil::Yaml::YamlParser;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;
using ThorsAnvil::Parser::ParserDomInterface;
using ThorsAnvil::Parser::ParserObjectType;

extern std::string getExpected(std::string const& file);
extern void CheckResults(YamlParser& parser, ParserDomInterface& domBuilder, ParserObjectType type, std::string const& expectedResult);
extern void runTest(ParserObjectType type, std::istream& input, std::string const& expectedResult);


TEST(Yaml_Ch5, BlockStructureIndicators)
{
    std::stringstream input{    "sequence:\n"
                                "- one\n"
                                "- two\n"
                                "mapping:\n"
                                "  ? sky\n"
                                "  : blue\n"
                                "  sea : green\n"
                           };
    std::string       output{ "{\"mapping\": {\"sea\": \"green\", \"sky\": \"blue\"}, \"sequence\": [\"one\", \"two\"]}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch5, FlowCollectionIndicators)
{
    std::stringstream input{    "sequence: [ one, two, ]\n"
                                "mapping: { sky: blue, sea: green }\n"
                           };
    std::string       output{ "{\"mapping\": {\"sea\": \"green\", \"sky\": \"blue\"}, \"sequence\": [\"one\", \"two\"]}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch5, DISABLED_NodePropertyIndicators)
{
    // ThorsAnvil YAML currently does not support anchor or tags
    std::stringstream input{    "anchored: !local &anchor value\n"
                                "alias: *anchor\n"
                           };
    std::string       output{ "{\"alias\": \"value\", \"anchored\": \"value\"}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch5, BlockScalarIndicators)
{
    std::stringstream input{    "literal: |\n"
                                "  some\n"
                                "  text\n"
                                "folded: >\n"
                                "  some\n"
                                "  text\n"
                           };
    std::string       output{ "{\"folded\": \"some text\n\", \"literal\": \"some\ntext\n\"}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch5, QuotedScalarIndicators)
{
    std::stringstream input{    "single: 'text'\n"
                                "double: \"text\"\n"
                           };
    std::string       output{ "{\"double\": \"text\", \"single\": \"text\"}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch5, DISABLED_DirectiveIndicator)
{
    std::stringstream input{    "%YAML 1.2\n"
                                "---\n"
                                "text\n"
                           };
    std::string       output{ "\"text\"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch5, LineBreakCharacters)
{
    std::stringstream input{    "|\n"
                                "  Line break (no glyph)\n"
                                "  Line break (glyphed)\n"
                           };
    std::string       output{ "\"Line break (no glyph)\nLine break (glyphed)\n\""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch5, TabsAndSpaces)
{
    std::ifstream input("test/data/TabsAndSpaces.yaml");
    std::string   output    = getExpected("test/data/TabsAndSpaces.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch5, EscapedCharacters)
{
    std::ifstream input("test/data/EscapedCharacters.yaml");
    std::string   output    = getExpected("test/data/EscapedCharacters.json");

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}
#if 0

TEST(Yaml_Ch5, XX)
{
    std::stringstream input{
                           };
    std::string       output{   };

    runTest(ThorsAnvil::Parser::ParserXXObject, input, output);
}
#endif

