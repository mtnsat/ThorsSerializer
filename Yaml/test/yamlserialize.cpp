
#include "gtest/gtest.h"
#include "YamlParser.h"
#include "YamlScanner.h"
#include "DomSerialize.h"
#include "Json/DomSerialize.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserDomVisit.h"
#include "Parser/ParserInterface.h"
#include <sstream>
#include <fstream>
#include <string>

using ThorsAnvil::Yaml::YamlParser;
using ThorsAnvil::Yaml::YamlKeyGen;
using ThorsAnvil::Yaml::DomSerialize;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;
using ThorsAnvil::Parser::ParserDomInterface;
using ThorsAnvil::Parser::ParserObjectType;

void CheckResults1(YamlParser& parser, ParserDomInterface& domBuilder, ParserObjectType type, std::string const& expectedResult)
{
    ASSERT_EQ(0, parser.parse());
    ASSERT_EQ(domBuilder.type, type);

    std::stringstream result;

    result << DomSerialize(*domBuilder.result);

    ASSERT_EQ(expectedResult, result.str());
}

void runTest1(ParserObjectType type, std::istream& input, std::string const& expectedResult)
{
    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults1(parser, domBuilder, type, expectedResult);
}


TEST(YamlSerialize, SequenceOfScalars)
{
    std::stringstream input{    "- Mark McGwire\n"
                                "- Sammy Sosa\n"
                                "- Ken Griffey\n"
                           };
    std::string       output{   "- Mark McGwire\n- Sammy Sosa\n- Ken Griffey\n" };

    runTest1(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(YamlSerialize, MappingScalarsToScalars)
{
    std::stringstream input{    "hr:  65    # Home runs\n"
                                "avg: 0.278 # Batting average\n"
                                "rbi: 147   # Runs Batted In\n"
                           };
    std::string       output{   "avg: 0.278\nhr: 65\nrbi: 147\n" };

    runTest1(ThorsAnvil::Parser::ParserMapObject, input, output);
}


TEST(YamlSerialize, Integers)
{
    std::stringstream input{    "canonical: 12345\n"
                                "decimal: +12345\n"
                                "octal: 0o14\n"
                                "hexadecimal: 0xC\n"
                           };
    std::string       output{ "canonical: 12345\ndecimal: +12345\nhexadecimal: 0xC\noctal: 0o14\n" };

    runTest1(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(YamlSerialize, FloatingPoint)
{
    std::stringstream input{    "canonical: 1.23015e+3\n"
                                "exponential: 12.3015e+02\n"
                                "fixed: 1230.15\n"
                                "negative infinity: -.inf\n"
                                "not a number: .NaN\n"
                           };
    std::string       output{  "canonical: 1.23015e+3\nexponential: 12.3015e+02\nfixed: 1230.15\nnegative infinity: -.inf\nnot a number: .NaN\n" };

    runTest1(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(YamlSerialize, Miscellaneous)
{
    std::stringstream input{
                                "null:\n"
                                "booleans: [ true, false ]\n"
                                "string: '012345'\n"
                           };
    std::string       output{ "booleans: [true, false]\nnull: \nstring: '012345'\n" };

    runTest1(ThorsAnvil::Parser::ParserMapObject, input, output);
}

