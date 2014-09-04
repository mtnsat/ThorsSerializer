#include "gtest/gtest.h"
#include "YamlParser.h"
#include "YamlScanner.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserInterface.h"
#include <sstream>
#include <fstream>
#include <string>

using ThorsAnvil::Yaml::YamlParser;
using ThorsAnvil::Yaml::YamlKeyGen;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;
using ThorsAnvil::Parser::ParserDomInterface;
using ThorsAnvil::Parser::ParserObjectType;

extern std::string getExpected(std::string const& file);
extern void CheckResults(YamlParser& parser, ParserDomInterface& domBuilder, ParserObjectType type, std::string const& expectedResult);
extern void runTest(ParserObjectType type, std::istream& input, std::string const& expectedResult);



TEST(Yaml_Ch9, DocumentPrefix)
{
    std::stringstream input{    "# Comment\n"
                                "# lines\n"
                                "Document\n"
                           };
    std::string       output{  "\"Document\"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch9, DISABLED_DocumentMarkers)
{
    std::stringstream input{    "%YAML 1.2\n"
                                "---\n"
                                "Document\n"
                                "    # Suffix\n"
                           };
    std::string       output{  "\"Document\"" };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch9, DISABLED_BareDocuments)
{
    std::stringstream input{    "Bare\n"
                                "document\n"
                                "...\n"
                                "# No document\n"
                                "...\n"
                                "|\n"
                                "%!PS-Adobe-2.0 # Not the first line\n"
                           };
    std::string       output1{  "\"Bare document\"" };
    std::string       output2{  "\"%!PS-Adobe-2.0\n\"" };

    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output2);
}

TEST(Yaml_Ch9, DISABLED_ExplicitDocuments)
{
    std::stringstream input{    "---\n"
                                "{ matches\n"
                                "% : 20 }\n"
                                "...\n"
                                "---\n"
                                "# Empty\n"
                                "...\n"
                           };
    std::string       output1{  "{\"matches %\": 20}" };
    std::string       output2{  "\"\"" };

    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserMapObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output2);
}

TEST(Yaml_Ch9, DISABLED_DirectivesDocuments)
{
    std::stringstream input{    "%YAML 1.2\n"
                                "--- |\n"
                                "%!PS-Adobe-2.0\n"
                                "...\n"
                                "%YAML1.2\n"
                                "---\n"
                                "# Empty\n"
                                "...\n"
                           };
    std::string       output1{  "\"%!PS-Adobe-2.0\n\"" };
    std::string       output2{  "\"\"" };

    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output2);
}

TEST(Yaml_Ch9, DISABLED_Stream)
{
    std::stringstream input{    "Document\n"
                                "---\n"
                                "# Empty\n"
                                "...\n"
                                "%YAML 1.2\n"
                                "---\n"
                                "matches %: 20\n"
                           };
    std::string       output1{  "\"Document\"" };
    std::string       output2{  "null" };
    std::string       output3{  "{\"matches %\": 20}" };

    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output2);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserValueObject, output3);
}
#if 0

TEST(Yaml_Ch9, XX)
{
    std::stringstream input{
                           };
    std::string       output{   };

    runTest(ThorsAnvil::Parser::ParserXXObject, input, output);
}
#endif

