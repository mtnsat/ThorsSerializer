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
using ThorsAnvil::Parser::KeyGenVisitor;

extern std::string getExpected(std::string const& file);
extern void CheckResults(YamlParser& parser, ParserDomInterface& domBuilder, ParserObjectType type, std::string const& expectedResult);
extern void runTest(ParserObjectType type, std::istream& input, std::string const& expectedResult);

TEST(Yaml_Ch6, IndentationSpaces)
{
    std::ifstream input("test/data/IndentationSpaces.yaml");
    std::string   output    = getExpected("test/data/IndentationSpaces.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6,  IndentationIndicators)
{
    std::ifstream input("test/data/IndentationIndicators.yaml");
    std::string   output = getExpected("test/data/IndentationIndicators.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, DISABLED_SeparationSpaces)
{
    std::ifstream input("test/data/SeparationSpaces.yaml");
    std::string   output    = getExpected("test/data/SeparationSpaces.json");

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch6, LinePrefixes)
{
    std::ifstream input("test/data/LinePrefixes.yaml");
    std::string   output    = getExpected("test/data/LinePrefixes.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, EmptyLines)
{
    std::ifstream input("test/data/EmptyLines.yaml");
    std::string   output    = getExpected("test/data/EmptyLines.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, LineFolding)
{
    std::ifstream input("test/data/LineFolding.yaml");
    std::string   output    = getExpected("test/data/LineFolding.json");

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, BlockFolding)
{
    std::ifstream input("test/data/BlockFolding.yaml");
    std::string   output    = getExpected("test/data/BlockFolding.json");

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, FlowFolding)
{
    std::ifstream input("test/data/FlowFolding.yaml");
    std::string   output    = getExpected("test/data/FlowFolding.json");

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, SeparatedComment)
{
    std::ifstream input("test/data/SeparatedComment.yaml");
    std::string   output    = getExpected("test/data/SeparatedComment.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, MultiLineComments)
{
    std::ifstream input("test/data/MultiLineComments.yaml");
    std::string   output    = getExpected("test/data/MultiLineComments.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, SeparationSpaces2)
{
    std::ifstream input("test/data/SeparationSpaces2.yaml");
    std::string   output    = getExpected("test/data/SeparationSpaces2.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, DISABLED_ReservedDirectives)
{
    std::stringstream input{    "\%FOO  bar baz # Should be ignored\n"
                                "               # with a warning.\n"
                                "--- \"foo\"\n"
                           };
    std::string       output{ "\"foo\""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, TagDirective)
{
    std::stringstream input{    "\%TAG !yaml! tag:yaml.org,2002:\n"
                                "---\n"
                                "!yaml!str \"foo\"\n"
                           };
    std::string       output{ "\"foo\""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, PrimaryTagHandle)
{
    std::stringstream input{    "# Private\n"
                                "!foo \"bar\"\n"
                                "...\n"
                                "# Global\n"
                                "%TAG ! tag:example.com,2000:app/\n"
                                "---\n"
                                "!foo \"bar\"\n"
                           };
    std::string       output1{ "\"bar\""  };
    std::string       output2{ "\"bar\""  };

    KeyGenVisitor       keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output2);
}

TEST(Yaml_Ch6, SecondaryTagHandle)
{
    std::stringstream input{    "%TAG !! tag:example.com,2000:app/\n"
                                "---\n"
                                "!!int 1 - 3 # Interval, not integer\n"
                           };
    std::string       output{ "\"1 - 3\""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, TagHandles)
{
    std::stringstream input{    "%TAG !e! tag:example.com,2000:app/\n"
                                "---\n"
                                "!e!foo \"bar\"\n"
                           };
    std::string       output{  "\"bar\"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch6, LocalTagPrefix)
{
    std::stringstream input{    "%TAG !m! !my-\n"
                                "--- # Bulb here\n"
                                "!m!light fluorescent\n"
                                "...\n"
                                "%TAG !m! !my-\n"
                                "--- # Color here\n"
                                "!m!light green\n"
                           };
    std::string       output1{ "\"fluorescent\""  };
    std::string       output2{ "\"green\""  };

    KeyGenVisitor       keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output2);
}

TEST(Yaml_Ch6, GlobalTagPrefix)
{
    std::stringstream input{    "%TAG !e! tag:example.com,2000:app/\n"
                                "---\n"
                                "- !e!foo \"bar\"\n"
                           };
    std::string       output{ "[\"bar\"]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch6, DISABLED_NodeProperties)
{
    // ThorsAnvil YAML currently does not support anchor and tags
    std::stringstream input{    "!!str &a1 \"foo\":\n"
                                "  !!str bar\n"
                                "&a2 baz : *a1\n"
                           };
    std::string       output{  "{\"foo\": \"bar\", \"baz\": {}}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, VerbatimTags)
{
    std::stringstream input{    "!<tag:yaml.org,2002:str> foo :\n"
                                "  !<!bar> baz\n"
                           };
    std::string       output{ "{\"foo\": \"baz\"}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch6, TagShorthands)
{
    std::stringstream input{    "%TAG !e! tag:example.com,2000:app/\n"
                                "---\n"
                                "- !local foo\n"
                                "- !!str bar\n"
                                "- !e!tag%21 baz\n"
                           };
    std::string       output{ "[\"foo\", \"bar\", \"baz\"]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch6, NonSpecificTags)
{
    std::stringstream input{    "# Assuming conventional resolution:\n"
                                "- \"12\"\n"
                                "- 12\n"
                                "- ! 12\n"
                           };
    std::string       output{ "[\"12\", 12, 12]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch6, DISABLED_NodeAnchors)
{
    // ThorsAnvil YAML currently does not support anchor and tags
    std::stringstream input{    "First occurrence: &anchor Value\n"
                                "Second occurrence: *anchor\n"
                           };
    std::string       output{  "{\"First occurrence\": \"Value\", \"Second occurrence\": \"Value\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}
#if 0

TEST(Yaml_Ch6, XX)
{
    std::stringstream input{
                           };
    std::string       output{   };

    runTest(ThorsAnvil::Parser::ParserXXObject, input, output);
}
#endif

