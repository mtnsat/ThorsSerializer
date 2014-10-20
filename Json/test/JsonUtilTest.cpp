

#include "gtest/gtest.h"
#include "JsonParser.h"
#include "ParserShiftReduce.h"
#include "Parser/ParserDom.h"
#include "Parser/ParserUtil.h"
#include "JsonScanner.h"

using namespace ThorsAnvil::Parser;
using namespace ThorsAnvil::Json;

TEST(JsonUtil, MergeArrays)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("[1, 2, 3, 4]");
    scanner.parse(data1);
    ParserValue&    a1     = scanner.getValue();
    ParserArray&    array1 = *static_cast<ParserArrayItem&>(a1).value;

    std::stringstream   data2("[1, 2, 3, 4]");
    scanner.parse(data2);
    ParserValue&    a2     = scanner.getValue();
    ParserArray&    array2 = *static_cast<ParserArrayItem&>(a2).value;

    mergeParserDom(array1, array2, "Test");
    EXPECT_EQ(8, array1.size());
}


TEST(JsonUtil, MergeMaps)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 1, \"item2\": 2, \"item3\": 3, \"item4\": 4}");
    scanner.parse(data1);
    ParserValue&  m1   = scanner.getValue();
    ParserMap&    map1 = *static_cast<ParserMapItem&>(m1).value;

    std::stringstream   data2("{ \"item5\": 1, \"item6\": 2, \"item7\": 3, \"item8\": 4}");
    scanner.parse(data2);
    ParserValue&  m2   = scanner.getValue();
    ParserMap&    map2 = *static_cast<ParserMapItem&>(m2).value;

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(8, map1.size());
}


TEST(JsonUtil, OverwiteMapPODElement)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": 4}");
    scanner.parse(data1);
    ParserValue&  m1   = scanner.getValue();
    ParserMap&    map1 = *static_cast<ParserMapItem&>(m1).value;

    std::stringstream   data2("{ \"item1\": 5}");
    scanner.parse(data2);
    ParserValue&  m2   = scanner.getValue();
    ParserMap&    map2 = *static_cast<ParserMapItem&>(m2).value;

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());
    EXPECT_EQ(5, map1["item1"].getValue<int>());
}


TEST(JsonUtil, OverwiteMapElement)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"t1\": 9}}");
    scanner.parse(data1);
    ParserValue&  m1   = scanner.getValue();
    ParserMap&    map1 = *static_cast<ParserMapItem&>(m1).value;

    std::stringstream   data2("{ \"item1\": {\"t2\": 15}}");
    scanner.parse(data2);
    ParserValue&  m2   = scanner.getValue();
    ParserMap&    map2 = *static_cast<ParserMapItem&>(m2).value;

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserMap&    subMap  = *static_cast<ParserMapItem&>(map1["item1"]).value;
    EXPECT_EQ(9, (subMap)["t1"].getValue<int>());
    EXPECT_EQ(15, (subMap)["t2"].getValue<int>());
}


TEST(JsonUtil, AddElementsToArrayInMap)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse(data1);
    ParserValue&  m1   = scanner.getValue();
    ParserMap&    map1 = *static_cast<ParserMapItem&>(m1).value;

    std::stringstream   data2("{ \"item1\": {\"item5\": 34}}");
    scanner.parse(data2);
    ParserValue&  m2   = scanner.getValue();
    ParserMap&    map2 = *static_cast<ParserMapItem&>(m2).value;

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserArray&    subArray  = *static_cast<ParserArrayItem&>(map1["item1"]).value;
    EXPECT_EQ(4, subArray.size());
}


TEST(JsonUtil, AddArrayToArrayInMap)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": [ 1, 2, 3 ]}");
    scanner.parse(data1);
    ParserValue&  m1   = scanner.getValue();
    ParserMap&    map1 = *static_cast<ParserMapItem&>(m1).value;

    std::stringstream   data2("{ \"item1\": [ 4, 5, {\"plop\": 8}]}");
    scanner.parse(data2);
    ParserValue&  m2   = scanner.getValue();
    ParserMap&    map2 = *static_cast<ParserMapItem&>(m2).value;

    mergeParserDom(map1, map2, "Test");
    EXPECT_EQ(1, map1.size());

    ParserArray&    subArray  = *static_cast<ParserArrayItem&>(map1["item1"]).value;
    EXPECT_EQ(6, subArray.size());
}


TEST(JsonUtil, MergeNonMapIntoMap)
{
    JsonScannerDom      scanner;

    std::stringstream   data1("{ \"item1\": {\"plop\": 1}}");
    scanner.parse(data1);
    ParserValue&  m1   = scanner.getValue();
    ParserMap&    map1 = *static_cast<ParserMapItem&>(m1).value;

    std::stringstream   data2("{ \"item1\": 12}");
    scanner.parse(data2);
    ParserValue&  m2   = scanner.getValue();
    ParserMap&    map2 = *static_cast<ParserMapItem&>(m2).value;

    ASSERT_THROW(mergeParserDom(map1, map2, "Test"), std::runtime_error);
}



