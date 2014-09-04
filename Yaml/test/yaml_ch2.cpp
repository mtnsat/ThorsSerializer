
#include "gtest/gtest.h"
#include "YamlParser.h"
#include "YamlScanner.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserDomVisit.h"
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
using ThorsAnvil::Parser::ParserValueJsonPrinter;

std::string getExpected(std::string const& file)
{
    std::ifstream       expectedFile(file);
    std::stringstream   expectedStream;
    expectedStream << expectedFile.rdbuf();

    return expectedStream.str();
}

void CheckResults(YamlParser& parser, ParserDomInterface& domBuilder, ParserObjectType type, std::string const& expectedResult)
{
    ASSERT_EQ(0, parser.parse());
    ASSERT_EQ(domBuilder.type, type);

    std::stringstream result;
    ParserValueJsonPrinter  printer(result);
    domBuilder.result->accept(printer);

    ASSERT_EQ(expectedResult, result.str());
}

void runTest(ParserObjectType type, std::istream& input, std::string const& expectedResult)
{
    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, type, expectedResult);
}


TEST(Yaml_Ch2, SequenceOfScalars)
{
    std::stringstream input{    "- Mark McGwire\n"
                                "- Sammy Sosa\n"
                                "- Ken Griffey\n"
                           };
    std::string       output{   "[\"Mark McGwire\", \"Sammy Sosa\", \"Ken Griffey\"]"};

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch2, MappingScalarsToScalars)
{
    std::stringstream input{    "hr:  65    # Home runs\n"
                                "avg: 0.278 # Batting average\n"
                                "rbi: 147   # Runs Batted In\n"
                           };
    std::string       output{   "{\"avg\": 0.278, \"hr\": 65, \"rbi\": 147}"};

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2,  MappingScalarsToSequences)
{
    std::stringstream input{    "american:\n"
                                "  - Boston Red Sox\n"
                                "  - Detroit Tigers\n"
                                "  - New York Yankees\n"
                                "national:\n"
                                "  - New York Mets\n"
                                "  - Chicago Cubs\n"
                                "  - Atlanta Braves\n"
                           };
    std::string       output{   "{\"american\": [\"Boston Red Sox\", \"Detroit Tigers\", \"New York Yankees\"], \"national\": [\"New York Mets\", \"Chicago Cubs\", \"Atlanta Braves\"]}"};

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2,  SequenceOfMappings)
{
    std::stringstream input{    "-\n"
                                "  name: Mark McGwire\n"
                                "  hr:   65\n"
                                "  avg:  0.278\n"
                                "-\n"
                                "  name: Sammy Sosa\n"
                                "  hr:   63\n"
                                "  avg:  0.288\n"
                           };
    std::string       output{   "[{\"avg\": 0.278, \"hr\": 65, \"name\": \"Mark McGwire\"}, {\"avg\": 0.288, \"hr\": 63, \"name\": \"Sammy Sosa\"}]"};

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch2,  SequenceOfSequences)
{
    std::stringstream input{    "- [name        , hr, avg  ]\n"
                                "- [Mark McGwire, 65, 0.278]\n"
                                "- [Sammy Sosa  , 63, 0.288]\n"
                           };
    std::string       output{ "[[\"name\", \"hr\", \"avg\"], [\"Mark McGwire\", 65, 0.278], [\"Sammy Sosa\", 63, 0.288]]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch2,  MappingOfMappings)
{
    std::stringstream input{    "Mark McGwire: {hr: 65, avg: 0.278}\n"
                                "Sammy Sosa: {\n"
                                "    hr: 63,\n"
                                "    avg: 0.288\n"
                                "  }\n"
                           };
    std::string       output{ "{\"Mark McGwire\": {\"avg\": 0.278, \"hr\": 65}, \"Sammy Sosa\": {\"avg\": 0.288, \"hr\": 63}}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2,   TwoDocumentsInAStream)
{
    std::stringstream input{    "# Ranking of 1998 home runs\n"
                                "---\n"
                                "- Mark McGwire\n"
                                "- Sammy Sosa\n"
                                "- Ken Griffey\n"
                                "\n"
                                "# Team ranking\n"
                                "---\n"
                                "- Chicago Cubs\n"
                                "- St Louis Cardinals\n"
                           };
    std::string       output1{ "[\"Mark McGwire\", \"Sammy Sosa\", \"Ken Griffey\"]" };
    std::string       output2{ "[\"Chicago Cubs\", \"St Louis Cardinals\"]" };

    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserArrayObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserArrayObject, output2);
}

TEST(Yaml_Ch2,  PlayByPlayFeed)
{
    std::stringstream input{    "---\n"
                                "time: 20:03:20\n"
                                "player: Sammy Sosa\n"
                                "action: strike (miss)\n"
                                "...\n"
                                "---\n"
                                "time: 20:03:47\n"
                                "player: Sammy Sosa\n"
                                "action: grand slam\n"
                                "...\n"
                           };
    std::string       output1{ "{\"action\": \"strike (miss)\", \"player\": \"Sammy Sosa\", \"time\": \"20:03:20\"}" };
    std::string       output2{ "{\"action\": \"grand slam\", \"player\": \"Sammy Sosa\", \"time\": \"20:03:47\"}" };

    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);

    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserMapObject, output1);
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserMapObject, output2);
}

TEST(Yaml_Ch2,  SingleDocumentWithTwoComments )
{
    std::stringstream input{    "---\n"
                                "hr: # 1998 hr ranking\n"
                                "  - Mark McGwire\n"
                                "  - Sammy Sosa\n"
                                "rbi:\n"
                                "  # 1998 rbi ranking\n"
                                "  - Sammy Sosa\n"
                                "  - Ken Griffey\n"
                           };
    std::string       output{ "{\"hr\": [\"Mark McGwire\", \"Sammy Sosa\"], \"rbi\": [\"Sammy Sosa\", \"Ken Griffey\"]}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2,  DISABLED_NodeForSammySosaAppearsTwiceInThisDocument )
{
    std::stringstream input{    "---\n"
                                "hr:\n"
                                "  - Mark McGwire\n"
                                "  # Following node labeled SS\n"
                                "  - &SS Sammy Sosa\n"
                                "rbi:\n"
                                // Currently ThorsAnvil YAML parser does not support
                                // anchor and tags yet. This is disabled until it does.
                                "  - *SS # Subsequent occurrence\n"
                                "  - Ken Griffey\n"
                           };
    std::string       output{ "{\"hr\": [\"Mark McGwire\", \"Sammy Sosa\"], \"rbi\": [\"Sammy Sosa\", \"Ken Griffey\"]}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, MappingBetweenSequenc)
{
    std::stringstream input{    "? - Detroit Tigers\n"
                                "  - Chicago cubs\n"
                                ":\n"
                                "  - 2001-07-23\n"
                                "\n"
                                "? [ New York Yankees,\n"
                                "    Atlanta Braves ]\n"
                                ": [ 2001-07-02, 2001-08-12,\n"
                                "    2001-08-14 ]\n"
                           };
    std::string       output{ "{[\"Detroit Tigers\", \"Chicago cubs\"]: [\"2001-07-23\"], [\"New York Yankees\", \"Atlanta Braves\"]: [\"2001-07-02\", \"2001-08-12\", \"2001-08-14\"]}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2,  CompactNestedMapping)
{
    std::stringstream input{    "---\n"
                                "# Products purchased\n"
                                "- item    : Super Hoop\n"
                                "  quantity: 1\n"
                                "- item    : Basketball\n"
                                "  quantity: 4\n"
                                "- item    : Big Shoes\n"
                                "  quantity: 1\n"
                           };
    std::string       output{ "[{\"item\": \"Super Hoop\", \"quantity\": 1}, {\"item\": \"Basketball\", \"quantity\": 4}, {\"item\": \"Big Shoes\", \"quantity\": 1}]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch2,  InLiteralsNewlinesArePreserved )
{
    std::ifstream       input("test/data/InLiteralsNewlinesArePreserved.yaml", std::ios::binary);
    std::string         output  = getExpected("test/data/InLiteralsNewlinesArePreserved.json");

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch2, InTheFoldedScalarsNewlinesBecomeSpaces )
{
    // Note: New lines foleded to space (except last line as it ends the text!)
    //      Leading space is dropped again.
    std::stringstream input{    "--- >\n"
                                "  Mark McGwire's\n"
                                "  year was crippled\n"
                                "  by a knee injury.\n"
                           };
    std::string       output{ "\"Mark McGwire's year was crippled by a knee injury.\n\""};

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch2, FoldedNewlinesArePreservedForMoreIndentedAndBlankLines )
{
    std::stringstream input{    ">\n"
                                "  Sammy Sosa completed another\n"
                                "  fine season with great stats.\n"
                                "\n"
                                "    63 Home Runs\n"
                                "    0.288 Batting Average\n"
                                "\n"
                                "  What a year!\n"
                           };
    std::string       output{   "\""
                                "Sammy Sosa completed another fine season with great stats.\n"
                                "\n"
                                "  63 Home Runs\n"
                                "  0.288 Batting Average\n"
                                "\n"
                                "What a year!\n"
                                "\""  };

    runTest(ThorsAnvil::Parser::ParserValueObject, input, output);
}

TEST(Yaml_Ch2,  IndentationDeterminesScope)
{
    std::stringstream input{    "name: Mark McGwire\n"
                                "accomplishment: >\n"
                                "  Mark set a major league\n"
                                "  home run record in 1998.\n"
                                "stats: |\n"
                                "  65 Home Runs\n"
                                "  0.278 Batting Average\n"
                           };
    std::string       output{ "{\"accomplishment\": \"Mark set a major league home run record in 1998.\n\", \"name\": \"Mark McGwire\", \"stats\": \"65 Home Runs\n0.278 Batting Average\n\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, QuotedScalars)
{
    std::ifstream input("test/data/QuotedScalars.yaml");
    std::string   output    = getExpected("test/data/QuotedScalars.json");

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, MultiLineFlowScalars)
{
    std::stringstream input{    "plain:\n"
                                "  This unquoted scalar\n"
                                "  spans many lines.\n"
                                "\n"
                                "quoted: \"So does this\n"
                                "  quoted scalar.\n\"\n"
                           };
    std::string       output{ "{\"plain\": \"This unquoted scalar spans many lines.\", \"quoted\": \"So does this quoted scalar. \"}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, Integers)
{
    std::stringstream input{    "canonical: 12345\n"
                                "decimal: +12345\n"
                                "octal: 0o14\n"
                                "hexadecimal: 0xC\n"
                           };
    std::string       output{ "{\"canonical\": 12345, \"decimal\": +12345, \"hexadecimal\": 0xC, \"octal\": 0o14}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, FloatingPoint)
{
    std::stringstream input{    "canonical: 1.23015e+3\n"
                                "exponential: 12.3015e+02\n"
                                "fixed: 1230.15\n"
                                "negative infinity: -.inf\n"
                                "not a number: .NaN\n"
                           };
    std::string       output{  "{\"canonical\": 1.23015e+3, \"exponential\": 12.3015e+02, \"fixed\": 1230.15, \"negative infinity\": -.inf, \"not a number\": .NaN}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, Miscellaneous)
{
    std::stringstream input{
                                "null:\n"
                                "booleans: [ true, false ]\n"
                                "string: '012345'\n"
                           };
    std::string       output{ "{\"booleans\": [true, false], null: null, \"string\": \"012345\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, ISABLED_Timestamps)
{
    std::stringstream input{    "canonical: 2001-12-15T02:59:43.1Z\n"
                                "iso8601: 2001-12-14t21:59:43.10-05:00\n"
                                "spaced: 2001-12-14 21:59:43.10 -5\n"
                                "date: 2002-12-14\n"
                           };
    std::string       output{ "{\"canonical\": \"2001-12-15T02:59:43.1Z\", \"date\": \"2002-12-14\", \"iso8601\": \"2001-12-14t21:59:43.10-05:00\", \"spaced\": \"2001-12-14 21:59:43.10 -5\"}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, VariousExplicitTags)
{
    std::stringstream input{    "---\n"
                                "not-date: !!str 2002-04-28\n"
                                "\n"
                                "picture: !!binary |\n"
                                " R0lGODlhDAAMAIQAAP//9/X\n"
                                " 17unp5WZmZgAAAOfn515eXv\n"
                                " Pz7Y6OjuDg4J+fn5OTk6enp\n"
                                " 56enmleECcgggoBADs=\n"
                                "\n"
                                "application specific tag: !something |\n"
                                " The semantics of the tag\n"
                                " above may be different for\n"
                                " different documents.\n"
                           };
    std::string       output{ "{\"application specific tag\": \"The semantics of the tag\nabove may be different for\ndifferent documents.\n\", \"not-date\": \"2002-04-28\", \"picture\": \"R0lGODlhDAAMAIQAAP//9/X\n17unp5WZmZgAAAOfn515eXv\nPz7Y6OjuDg4J+fn5OTk6enp\n56enmleECcgggoBADs=\n\"}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, DISABLED_GlobalTags)
{
    // ThorsAnvil Yaml parser does not currently support anchor and tags.
    std::stringstream input{    "%TAG ! tag:clarkevans.com,2002:\n"
                                "--- !shape\n"
                                "  # Use the ! handle for presenting\n"
                                "  # tag:clarkevans.com,2002:circle\n"
                                "- !circle\n"
                                "  center: &ORIGIN {x: 73, y: 129}\n"
                                "  radius: 7\n"
                                "- !line\n"
                                "  start: *ORIGIN\n"
                                "  finish: { x: 89, y: 102 }\n"
                                "- !label\n"
                                "  start: *ORIGIN\n"
                                "  color: 0xFFEEBB\n"
                                "  text: Pretty vector drawing.\n"
                           };
    std::string       output{ "[{\"center\": {\"x\": 73, \"y\": 129}, \"radius\": 7}, {\"finish\": {\"x\": 89, \"y\": 102}, \"start\": {\"x\": 73, \"y\": 129}}, {\"color\": 65518, \"start\": {\"x\": 73, \"y\": 129}, \"text\": \"Pretty vector drawing.\"}]"  };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch2, UnorderedSets)
{
    std::stringstream input{    "# Sets are represented as a\n"
                                "# Mapping where each key is\n"
                                "# associated with a null value\n"
                                "--- !!set\n"
                                "? Mark McGwire\n"
                                "? Sammy Sosa\n"
                                "? Ken Griff\n"
                           };
    std::string       output{ "{\"Ken Griff\": null, \"Mark McGwire\": null, \"Sammy Sosa\": null}"  };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, OrderedMappings)
{
    std::stringstream input{    "# Ordered maps are represented as\n"
                                "# A sequence of mappings, with\n"
                                "# each mapping having one key\n"
                                "--- !!omap\n"
                                "- Mark McGwire: 65\n"
                                "- Sammy Sosa: 63\n"
                                "- Ken Griffy: 58\n"
                           };
    std::string       output{  "[{\"Mark McGwire\": 65}, {\"Sammy Sosa\": 63}, {\"Ken Griffy\": 58}]" };

    runTest(ThorsAnvil::Parser::ParserArrayObject, input, output);
}

TEST(Yaml_Ch2, DISABLED_Invoice)
{
    // ThorsAnvil YAML parser currently does not support Anchor and tags
    std::stringstream input{    "--- !<tag:clarkevans.com,2002:invoice>\n"
                                "invoice: 34843\n"
                                "date   : 2001-01-23\n"
                                "bill-to: &id001\n"
                                "    given  : Chris\n"
                                "    family : Dumars\n"
                                "    address:\n"
                                "        lines: |\n"
                                "            458 Walkman Dr.\n"
                                "            Suite #292\n"
                                "        city    : Royal Oak\n"
                                "        state   : MI\n"
                                "        postal  : 48046\n"
                                "ship-to: *id001\n"
                                "product:\n"
                                "    - sku         : BL394D\n"
                                "      quantity    : 4\n"
                                "      description : Basketball\n"
                                "      price       : 450.00\n"
                                "    - sku         : BL4438H\n"
                                "      quantity    : 1\n"
                                "      description : Super Hoop\n"
                                "      price       : 2392.00\n"
                                "tax  : 251.42\n"
                                "total: 4443.52\n"
                                "comments:\n"
                                "    Late afternoon is best.\n"
                                "    Backup contact is Nancy\n"
                                "    Billsmer @ 338-4338.\n"
                           };
    std::string       output{ "{"
                                    "\"bill-to\": "
                                        "{"
                                            "\"address\": "
                                                "{\"city\": \"Royal Oak\", \"lines\": \"458 Walkman Dr.\nSuite #292\n\", \"postal\": 48046, \"state\": \"MI\"}, "
                                            "\"family\": \"Dumars\", "
                                            "\"given\": \"Chris\""
                                        "}, "
                                    "\"comments\": \"Late afternoon is best. Backup contact is Nancy Billsmer @ 338-4338.\", "
                                    "\"date\": \"2001-01-23\", "
                                    "\"invoice\": 34843, "
                                    "\"product\": ["
                                        "{"
                                            "\"description\": \"Basketball\", "
                                            "\"price\": 450.00, "
                                            "\"quantity\": 4, "
                                            "\"sku\": \"BL394D\""
                                        "}, "
                                        "{"
                                            "\"description\": \"Super Hoop\", "
                                            "\"price\": 2392.00, "
                                            "\"quantity\": 1, "
                                            "\"sku\": \"BL4438H\""
                                        "}"
                                    "], "
                                    "\"ship-to\": "
                                        "{"
                                            "\"address\": "
                                                "{\"city\": \"Royal Oak\", \"lines\": \"458 Walkman Dr.\nSuite #292\n\", \"postal\": 48046, \"state\": \"MI\"}, "
                                            "\"family\": \"Dumars\", "
                                            "\"given\": \"Chris\""
                                        "}, "
                                    "\"tax\": 251.42, "
                                    "\"total\": 4443.52"
                                "}" };

    runTest(ThorsAnvil::Parser::ParserMapObject, input, output);
}

TEST(Yaml_Ch2, LogFile)
{
    std::ifstream input("test/data/LogFile.yaml");
    std::ifstream jsonoutput("test/data/LogFile.json");

    std::string         output;
    YamlKeyGen          keyGenerator;
    ParserDomInterface  domBuilder(keyGenerator);
    YamlParser          parser(input, domBuilder);


    output  = getExpected("test/data/LogFile.1.json");
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserMapObject, output);

    output  = getExpected("test/data/LogFile.2.json");
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserMapObject, output);

    output  = getExpected("test/data/LogFile.3.json");
    CheckResults(parser, domBuilder, ThorsAnvil::Parser::ParserMapObject, output);
}


