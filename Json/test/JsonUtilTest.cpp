

#include "gtest/gtest.h"
#include "JsonParser.h"
#include "ParserShiftReduce.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserUtil.h"
#include "Parser/ScannerDom.h"

using namespace ThorsAnvil::Parser;
using namespace ThorsAnvil::Json;

TEST(JsonUtil, MergeArrays)
{
    ScannerDom      scanner;

    std::stringstream   data1("[1, 2, 3, 4]");
    scanner.parse<JsonParser>(data1);
    ParserArray&    array1 = scanner.getArray();

    std::stringstream   data2("[1, 2, 3, 4]");
    scanner.parse<JsonParser>(data2);
    ParserArray&    array2 = scanner.getArray();

    mergeParserDom(array1, array2, "Test");
    EXPECT_EQ(8, array1.size());
}


TEST(JsonUtil, MergeMaps)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 1, \"item2\": 2, \"item3\": 3, \"item4\": 4}");
    scanner.parse<JsonParser>(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item5\": 1, \"item6\": 2, \"item7\": 3, \"item8\": 4}");
    scanner.parse<JsonParser>(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(8, map1.size());
}


TEST(JsonUtil, OverwiteMapPODElement)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 4}");
    scanner.parse<JsonParser>(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": 5}");
    scanner.parse<JsonParser>(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());
    EXPECT_EQ(5, map1["item1"].getValue<int>());
}


TEST(JsonUtil, OverwiteMapElement)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"t1\": 9}}");
    scanner.parse<JsonParser>(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": {\"t2\": 15}}");
    scanner.parse<JsonParser>(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserMap&    subMap  = *static_cast<ParserMapItem&>(map1["item1"]).value;
    EXPECT_EQ(9, (subMap)["t1"].getValue<int>());
    EXPECT_EQ(15, (subMap)["t2"].getValue<int>());
}


TEST(JsonUtil, AddElementsToArrayInMap)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse<JsonParser>(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": {\"item5\": 34}}");
    scanner.parse<JsonParser>(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserArray&    subArray  = *static_cast<ParserArrayItem&>(map1["item1"]).value;
    EXPECT_EQ(4, subArray.size());
}


TEST(JsonUtil, AddArrayToArrayInMap)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse<JsonParser>(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": [ 4, 5, {\"plop\": 8}]}");
    scanner.parse<JsonParser>(data2);
    ParserMap&    map2 = scanner.getMap();

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserArray&    subArray  = *static_cast<ParserArrayItem&>(map1["item1"]).value;
    EXPECT_EQ(6, subArray.size());
}


TEST(JsonUtil, MergeNonMapIntoMap)
{
    ScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"plop\": 1}}");
    scanner.parse<JsonParser>(data1);
    ParserMap&    map1 = scanner.getMap();

    std::stringstream   data2("{ \"item1\": 12}");
    scanner.parse<JsonParser>(data2);
    ParserMap&    map2 = scanner.getMap();

    ASSERT_THROW(mergeParserDom(map1, map2, "Test"), std::runtime_error);
}



