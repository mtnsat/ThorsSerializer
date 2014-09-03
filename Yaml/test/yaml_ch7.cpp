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


TEST(Yaml_Ch7, DISABLED_AliasNodes)
{
    // ThorsAnvil YAML currently does not support anchor and tags
    std::stringstream input{    "First occurrence: &anchor Foo\n"
                                "Second occurrence: *anchor\n"
                                "Override anchor: &anchor Bar\n"
                                "Reuse anchor: *anchor\n"
                           };
    std::string       output{   };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, EmptyContent)
{
    std::stringstream input{    "{\n"
                                "  foo : !!str ,\n"
                                "  !!str : bar,\n"
                                "}\n"
                           };
    std::string       output{  "{\"\": \"bar\", \"foo\": \"\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, DISABLED_CompletelyEmptyFlowNodes)
{
    std::stringstream input{    "{\n"
                                "  ? foo : ,\n"
                                "   : bar,\n"
                                "}\n"
                           };
    std::string       output{  "{\"\": \"bar\", \"foo\": \"\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, DoubleQuotedImplicitKeys)
{
    std::stringstream input{    "\"implicit block key\" : [\n"
                                "  \"implicit flow key\" : value,\n"
                                " ]\n"
                           };
    std::string       output{  "{\"implicit block key\": [{\"implicit flow key\": \"value\"}]}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, DoubleQuotedLineBreaks)
{
    std::stringstream input{    "\"folded \n"
                                "to a space,\t\n"
                                " \n"
                                "to a line feed, or \t\\\n"
                                " \\ \tnon-content\"\n"
                           };
    std::string       output{  "\"folded to a space,\nto a line feed, or \t \tnon-content\"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch7, DoubleQuotedLines)
{
    std::stringstream input{    "\" 1st non-empty\n"
                                "\n"
                                " 2nd non-empty \n"
                                "\t3rd non-empty \"\n"
                           };
    std::string       output{ "\" 1st non-empty\n2nd non-empty 3rd non-empty \"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch7, SingleQuotedCharacters)
{
    std::stringstream input{    "'here''s to \"quotes\"'" };
    std::string       output{ "\"here's to \\\"quotes\\\"\""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch7, SingleQuotedImplicitKeys)
{
    std::stringstream input{    "'implicit block key' : [\n"
                                "  'implicit flow key' : value,\n"
                                " ]\n"
                           };
    std::string       output{ "{\"implicit block key\": [{\"implicit flow key\": \"value\"}]}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, SingleQuotedLines)
{
    std::stringstream input{    "' 1st non-empty\n"
                                "\n"
                                " 2nd non-empty \n"
                                "\t3rd non-empty '\n"
                           };
    std::string       output{ "\" 1st non-empty\n2nd non-empty 3rd non-empty \""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch7, DISABLED_PlainCharacters)
{
    std::stringstream input{    "# Outside flow collection:\n"
                                "- ::vector\n"
                                "- \": - ()\"\n"
                                "- Up, up, and away!\n"
                                "- -123\n"
                                "- http://example.com/foo#bar\n"
                                "# Inside flow collection:\n"
                                "- [ ::vector,\n"
                                "  \": - ()\",\n"
                                "  \"Up, up and away!\",\n"
                                "  -123,\n"
                                "  http://example.com/foo#bar ]\n"
                           };
    std::string       output{  "[\"::vector\", \": - ()\", \"Up, up, and away!\", \"-123\", \"http://example.com/foo#bar\", [\"::vector\", \": - ()\", \"Up, up, and away!\", \"-123\", \"http://example.com/foo#bar\"]]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7, PlainImplicitKeys)
{
    std::stringstream input{    "implicit block key : [\n"
                                "  implicit flow key : value,\n"
                                " ]\n"
                           };
    std::string       output{ "{\"implicit block key\": [{\"implicit flow key\": \"value\"}]}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, PlainLines)
{
    std::stringstream input{    "1st non-empty\n"
                                "\n"
                                " 2nd non-empty \n"
                                "\t3rd non-empty\n"
                           };
    std::string       output{  "\"1st non-empty\n2nd non-empty 3rd non-empty\"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch7, FlowSequence)
{
    std::stringstream input{    "- [ one, two, ]\n"
                                "- [three ,four]\n"
                           };
    std::string       output{ "[[\"one\", \"two\"], [\"three\", \"four\"]]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7, FlowSequenceEntries)
{
    std::stringstream input{    "[\n"
                                "\"double\n"
                                " quoted\", 'single\n"
                                "           quoted',\n"
                                "plain\n"
                                " text, [ nested ],\n"
                                "single: pair,\n"
                                "]\n"
                           };
    std::string       output{ "[\"double quoted\", \"single quoted\", \"plain text\", [\"nested\"], {\"single\": \"pair\"}]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7, FlowMappings)
{
    std::stringstream input{    "- { one : two , three: four , }\n"
                                "- {five: six,seven : eight}\n"
                           };
    std::string       output{ "[{\"one\": \"two\", \"three\": \"four\"}, {\"five\": \"six\", \"seven\": \"eight\"}]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7, FlowMappingEntries)
{
    std::stringstream input{    "{\n"
                                "? explicit: entry,\n"
                                "implicit: entry,\n"
                                "?\n"
                                "}\n"
                           };
    std::string       output{ "{\"explicit\": \"entry\", \"implicit\": \"entry\", null: null}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, DISABLED_FlowMappingSeparateValues)
{
    std::stringstream input{    "{\n"
                                "unquoted : \"separate\",\n"
                                "http://foo.com,\n"
                                "omitted value:°,\n"
                                "°: omitted key,\n"
                                "}\n"
                           };
    std::string       output{  "{\"unquoted\": \"separate\", \"http://foo.com\": \"\", \"omitted value\": \"\", \"\": \"omitted key\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, FlowMappingAdjacentValues)
{
    std::stringstream input{    "{\n"
                                "\"adjacent\":value,\n"
                                "\"readable\": value,\n"
                                "\"empty\":\n"
                                "}\n"
                           };
    std::string       output{  "{\"adjacent\": \"value\", \"empty\": null, \"readable\": \"value\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch7, SinglePairFlowMappings)
{
    std::stringstream input{    "[\n"
                                "foo: bar\n"
                                "]\n"
                           };
    std::string       output{ "[{\"foo\": \"bar\"}]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7, SinglePairExplicitEntry)
{
    std::stringstream input{    "[\n"
                                "? foo\n"
                                " bar : baz\n"
                                "]\n"
                           };
    std::string       output{  "[{\"foo bar\": \"baz\"}]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7,  DISABLED_SinglePairImplicitEntries)
{
    std::stringstream input{    "- [ YAML: separate ]\n"
                                "- [ : empty key entry ]\n"
                                "- [ {JSON: like}:adjacent ]\n"
                           };
    std::string       output{ "[[{\"YAML\": \"separate\"}], [{\"\": \"empty key entry\"}], [{{\"JSON\": \"like\"}: \"adjacent\"}]]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7, FlowContent)
{
    std::stringstream input{    "- [ a, b ]\n"
                                "- { a: b }\n"
                                "- \"a\"\n"
                                "- 'b'\n"
                                "- c\n"
                           };
    std::string       output{  "[[\"a\", \"b\"], {\"a\": \"b\"}, \"a\", \"b\", \"c\"]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch7,  DISABLED_FlowNodes)
{
    // ThorsAnvil YAML currently does not support anchor and tags
    std::stringstream input{    "- !!str \"a\"\n"
                                "- 'b'\n"
                                "- &anchor \"c\"\n"
                                "- *anchor\n"
                                "- !!str\n"
                           };
    std::string       output{ "[\"a\", \"b\", \"c\", \"c\", \"\"]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}
#if 0

TEST(Yaml_Ch7, XX)
{
    std::stringstream input{
                           };
    std::string       output{   };

    runTest(ThorsAnvil::Parser::ParserXXObject, input, output);
}
#endif

