

#include "gtest/gtest.h"
#include "YamlParser.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserUtil.h"
#include "YamlScanner.h"

using namespace ThorsAnvil::Parser;
using namespace ThorsAnvil::Yaml;

TEST(YamlUtil, MergeArrays)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("[1, 2, 3, 4]");
    scanner.parse(data1);
    ParserArray&    array1 = scanner.getArray();

    std::stringstream   data2("[1, 2, 3, 4]");
    scanner.parse(data2);
    ParserArray&    array2 = scanner.getArray();

    mergeParserDom(array1, array2, "Test");
    EXPECT_EQ(8, array1.size());
}


TEST(YamlUtil, MergeMaps)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 1, \"item2\": 2, \"item3\": 3, \"item4\": 4}");
    scanner.parse(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item5\": 1, \"item6\": 2, \"item7\": 3, \"item8\": 4}");
    scanner.parse(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(8, map1.size());
}


TEST(YamlUtil, OverwiteMapPODElement)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 4}");
    scanner.parse(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": 5}");
    scanner.parse(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());
    EXPECT_EQ(5, map1["item1"].getValue<int>());
}


TEST(YamlUtil, OverwiteMapElement)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"t1\": 9}}");
    scanner.parse(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": {\"t2\": 15}}");
    scanner.parse(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserMap&    subMap  = *static_cast<ParserMapItem&>(map1["item1"]).value;
    EXPECT_EQ(9, (subMap)["t1"].getValue<int>());
    EXPECT_EQ(15, (subMap)["t2"].getValue<int>());
}


TEST(YamlUtil, AddElementsToArrayInMap)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": {\"item5\": 34}}");
    scanner.parse(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserArray&    subArray  = *static_cast<ParserArrayItem&>(map1["item1"]).value;
    EXPECT_EQ(4, subArray.size());
}


TEST(YamlUtil, AddArrayToArrayInMap)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": [ 4, 5, {\"plop\": 8}]}");
    scanner.parse(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserArray&    subArray  = *static_cast<ParserArrayItem&>(map1["item1"]).value;
    EXPECT_EQ(6, subArray.size());
}


TEST(YamlUtil, MergeNonMapIntoMap)
{
    YamlScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"plop\": 1}}");
    scanner.parse(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": 12}");
    scanner.parse(data2);
    ParserMap&    map2 = scanner.getMap();

    ASSERT_THROW(mergeParserDom(map1, map2, "Test"), std::runtime_error);
}



