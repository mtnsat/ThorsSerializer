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



TEST(Yaml_Ch8, BlockMappings)
{
    std::stringstream input{    "block mapping:\n"
                                " key: value\n"
                           };
    std::string       output{  "{\"block mapping\": {\"key\": \"value\"}}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch8, ExplicitBlockMappingEntries)
{
    std::stringstream input{    "? explicit key # Empty value\n"
                                "? |\n"
                                "  block key\n"
                                ": - one # Explicit compact\n"
                                "  - two # block value↓\n"
                           };
    std::string       output{  "{\"block key\n\": [\"one\", \"two\"], \"explicit key\": null}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch8, DISABLED_ImplicitBlockMappingEntries)
{
    std::stringstream input{    "plain key: in-line value\n"
                                ": # Both empty\n"
                                "\"quoted key\":\n"
                                "- entry\n"
                           };
    std::string       output{  "{\"plain key\": \"in-line value\", \"\": \"\", \"quoted key\": [\"entry\"]}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch8, CompactBlockMappings)
{
    std::stringstream input{    "- sun: yellow\n"
                                "- ? earth: blue\n"
                                "  : moon: white\n"
                           };
    std::string       output{  "[{\"sun\": \"yellow\"}, {{\"earth\": \"blue\"}: {\"moon\": \"white\"}}]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch8, BlockNodeTypes)
{
    std::stringstream input{    "-\n"
                                "  \"flow in block\"\n"
                                "- >\n"
                                " Block scalar\n"
                                "- !!map # Block collection\n"
                                "  foo : bar\n"
                           };
    std::string       output{  "[\"flow in block\", \"Block scalar\n\", {\"foo\": \"bar\"}]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch8, DISABLED_BlockScalarNodes)
{
    std::stringstream input{    "literal: |2\n"
                                "  value\n"
                                "folded:\n"
                                "   !foo\n"
                                "  >1\n"
                                " value\n"
                           };
    std::string       output{  "{\"folded\": \"value\", \"literal\": \"value\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch8, BlockCollectionNodes)
{
    std::stringstream input{    "sequence: !!seq\n"
                                "- entry\n"
                                "- !!seq\n"
                                " - nested\n"
                                "mapping: !!map\n"
                                " foo: bar\n"
                           };
    std::string       output{  "{\"mapping\": {\"foo\": \"bar\"}, \"sequence\": [\"entry\", [\"nested\"]]}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

#if 0

TEST(Yaml_Ch8, XX)
{
    std::stringstream input{
                           };
    std::string       output{   };

    runTest(ThorsAnvil::Parser::ParserXXObject, input, output);
}
#endif

