
#include "gtest/gtest.h"
#include "YamlParser.h"
#include "Parser/ScannerSax.h"
#include <sstream>

using ThorsAnvil::Yaml::YamlParser;

template<typename T>
struct TestAction: ThorsAnvil::Parser::SaxAction
{
    TestAction(int& c, bool& pa, bool& a, T& v) : count(c), preAction(pa), action(a), value(v)
    {
        count       = 0;
        preAction   = false;
        action      = false;
    }
    int&    count;
    bool&   preAction;
    bool&   action;
    T&      value;

    virtual void doPreAction(ThorsAnvil::Parser::ScannerSax&, ThorsAnvil::Parser::Key const&)
    {
        preAction   = true;
        ++count;
    }
    virtual void doAction(ThorsAnvil::Parser::ScannerSax&, ThorsAnvil::Parser::Key const&, ThorsAnvil::Parser::ParserValue const& input)
    {
        action      = true;
        value       = input.getValue<T>();
    }
};


TEST(ScannerSax, YamlScanMapEmpty)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    int                                             value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<int>(count, preAction, action, value));

    std::stringstream                               json("{}");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerAction("I1", std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 0);
    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
}

TEST(ScannerSax, YamlScanMapMiss)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    int                                             value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<int>(count, preAction, action, value));

    std::stringstream                               json("{ \"I2\" : 1}");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerAction("I1", std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 0);
    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
}


TEST(ScannerSax, YamlScanMapHit)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    int                                             value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<int>(count, preAction, action, value));

    std::stringstream                               json("{ \"I1\" : 123}");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerAction("I1", std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(action == true);
    ASSERT_TRUE(value == 123);
}

TEST(ScannerSax, YamlScanArrayEmpty)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    bool                                            value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<bool>(count, preAction, action, value));

    std::stringstream                               json("[]");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerAction("I1", std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 0);
    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
}

TEST(ScannerSax, YamlScanArrayMiss)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    bool                                            value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      emptyAction;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<bool>(count, preAction, action, value));

    std::stringstream                               json("[ true ]");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerActionNext(std::move(emptyAction));
    scanner.registerActionNext(std::move(emptyAction));
    scanner.registerActionNext(std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
}

TEST(ScannerSax, YamlScanArrayHit)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    bool                                            value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      emptyAction;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<bool>(count, preAction, action, value));

    std::stringstream                               json("[ 512, true ]");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerActionNext(std::move(emptyAction));
    scanner.registerActionNext(std::move(saxAction));

    scanner.parse<YamlParser>(json);

    ASSERT_EQ(true, action);
    ASSERT_EQ(1, count);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(value == true);
}

TEST(ScannerSax, YamlAltScanMapMiss)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    std::string                                     value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<std::string>(count, preAction, action, value));

    std::stringstream                               json("{ \"I2\" : \"S1\"}");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerAction("I1", std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 0);
    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
}

TEST(ScannerSax, YamlAltScanMapHit)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    std::string                                     value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<std::string>(count, preAction, action, value));

    std::stringstream                               json("{ \"I1\" : \"123SS\"}");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerAction("I1", std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(action == true);
    ASSERT_TRUE(value == "123SS");
}

TEST(ScannerSax, YamlAltScanArrayMiss)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    bool                                            value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      emptyAction;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<bool>(count, preAction, action, value));

    std::stringstream                               json("[ false ]");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerActionNext(std::move(emptyAction));
    scanner.registerActionNext(std::move(emptyAction));
    scanner.registerActionNext(std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 0);
    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
}

TEST(ScannerSax, YamlAltScanArrayHit)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    bool                                            value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      emptyAction;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<bool>(count, preAction, action, value));

    std::stringstream                               json("[ 512, false ]");
    ThorsAnvil::Parser::ScannerSax                    scanner;
    scanner.registerActionNext(std::move(emptyAction));
    scanner.registerActionNext(std::move(saxAction));
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 1);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(action == true);
    ASSERT_TRUE(value == false);
}

TEST(ScannerSax, ReplaceAction)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    int                                             value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<int>(count, preAction, action, value));

    int                                             count2;
    bool                                            preAction2;
    bool                                            action2;
    int                                             value2;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction2(new TestAction<int>(count2, preAction2, action2, value2));


    ThorsAnvil::Parser::ScannerSax                    scanner;

    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      emptyAction;
    scanner.registerActionNext(std::move(emptyAction));

    ThorsAnvil::Parser::ActionRefNote actNote = scanner.registerActionNext(std::move(saxAction));
    scanner.replaceAction(actNote, std::move(saxAction2));

    std::stringstream                               json("[ 512, 567 ]");
    scanner.parse<YamlParser>(json);

    ASSERT_TRUE(count == 0);
    ASSERT_TRUE(preAction == false);
    ASSERT_TRUE(action == false);
    ASSERT_TRUE(count2 == 1);
    ASSERT_TRUE(preAction2 == true);
    ASSERT_TRUE(action2 == true);
    ASSERT_TRUE(value2 == 567);
}

TEST(ScannerSax, DefaultArrayAction)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    int                                             value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<int>(count, preAction, action, value));


    ThorsAnvil::Parser::ScannerSax                    scanner;

    ThorsAnvil::Parser::ActionRefNote actNote = scanner.registerActionOnAllArrItems(std::move(saxAction));

    std::stringstream                               json("[ 512, 567 ]");
    scanner.parse<YamlParser>(json);

    ASSERT_EQ(2, count);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(action == true);
    ASSERT_EQ(567, value);
}

TEST(ScannerSax, DefaultMapAction)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    int                                             value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<int>(count, preAction, action, value));


    ThorsAnvil::Parser::ScannerSax                    scanner;

    ThorsAnvil::Parser::ActionRefNote actNote = scanner.registerAction("\xFF", std::move(saxAction));

    std::stringstream                               json("{ \"I1\": 512, \"I2\": 567 }");
    scanner.parse<YamlParser>(json);

    ASSERT_EQ(2, count);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(action == true);
    ASSERT_EQ(567, value);
}

TEST(ScannerSax, GetNullValue)
{
    int                                             count;
    bool                                            preAction;
    bool                                            action;
    bool                                            value;
    std::unique_ptr<ThorsAnvil::Parser::SaxAction>      saxAction(new TestAction<bool>(count, preAction, action, value));


    ThorsAnvil::Parser::ScannerSax                    scanner;

    ThorsAnvil::Parser::ActionRefNote actNote = scanner.registerAction("I1", std::move(saxAction));

    std::stringstream                               json("{ \"I1\": null}");
    scanner.parse<YamlParser>(json);

    ASSERT_EQ(1, count);
    ASSERT_TRUE(preAction == true);
    ASSERT_TRUE(action == true);
    ASSERT_TRUE(false == value);
}

#if 0
#endif

