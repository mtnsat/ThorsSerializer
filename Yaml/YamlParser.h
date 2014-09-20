
#ifndef THORSANIVL_YAML_YAML_PARSER_H
#define THORSANIVL_YAML_YAML_PARSER_H

#include "Parser/ParserInterface.h"
#include <istream>
#include <yaml.h>

namespace ThorsAnvil
{
    namespace Yaml
    {

struct YamlObject
{
    YamlObject()
        : type(Parser::NotSet)
    {}
    YamlObject(Parser::ParserMap* map)
        : type(Parser::ParserMapObject)
    {
        data.map    = map;
    }
    YamlObject(Parser::ParserArray* array)
        : type(Parser::ParserArrayObject)
    {
        data.array  = array;
    }
    YamlObject(Parser::ParserValue* value)
        : type(Parser::ParserValueObject)
    {
        data.value  = value;
    }
    ~YamlObject();
    YamlObject(YamlObject&& move);
    YamlObject& operator=(YamlObject&& move);
    YamlObject(YamlObject const&)            = delete;
    YamlObject& operator=(YamlObject const&) = delete;

    Parser::ParserObjectType  type;
    union
    {
        Parser::ParserMap*    map;
        Parser::ParserArray*  array;
        Parser::ParserValue*  value;
    } data;
};

class YamlParser: public Parser::LexerParser
{
    struct YamlHierarchyObject
    {
        YamlObject                            obj;
        bool                                    set;
        bool                                    first;
        std::unique_ptr<Parser::ParserValue>    key;
        bool                                    keySet;
        std::string                             anchor;
        std::string                             tag;
        int                                     implicit;
        int                                     style;
        YamlHierarchyObject(YamlHierarchyObject const&)             = delete;
        YamlHierarchyObject& operator=(YamlHierarchyObject const&)  = delete;


        public:
        YamlHierarchyObject(Parser::ParserValue* val)
            : obj(val)
            , set(val != nullptr)
            , first(true)
            , key(nullptr)
            , keySet(false)
        {}
        YamlHierarchyObject(Parser::ParserArray* val, yaml_event_t const& event)
            : obj(val)
            , set(val != nullptr)
            , first(true)
            , key(nullptr)
            , keySet(false)
            , anchor(event.data.sequence_start.anchor ? reinterpret_cast<char*>(event.data.sequence_start.anchor) : "")
            , tag(event.data.sequence_start.tag       ? reinterpret_cast<char*>(event.data.sequence_start.tag)    : "")
            , implicit(event.data.sequence_start.implicit)
            , style(event.data.sequence_start.style)
        {}
        YamlHierarchyObject(Parser::ParserMap* val, yaml_event_t const& event)
            : obj(val)
            , set(val != nullptr)
            , first(true)
            , key(nullptr)
            , keySet(false)
            , anchor(event.data.mapping_start.anchor ? reinterpret_cast<char*>(event.data.mapping_start.anchor) : "")
            , tag(event.data.mapping_start.tag       ? reinterpret_cast<char*>(event.data.mapping_start.tag)    : "")
            , implicit(event.data.mapping_start.implicit)
            , style(event.data.mapping_start.style)
        {}
        YamlHierarchyObject(YamlHierarchyObject&& move)
            : obj(std::move(move.obj))
            , set(std::move(move.set))
            , first(std::move(move.first))
            , key(std::move(move.key))
            , keySet(std::move(move.keySet))
        {}
    };

    typedef std::pair<std::string, std::string>     YamlDirective;
    typedef std::pair<int, int>                     Version;

    std::unique_ptr<Parser::ParserValue>resultValue;
    Parser::ParserObjectType            resultType;
    std::vector<YamlHierarchyObject>    hierarchy;
    std::vector<YamlDirective>          directives;
    Version                             version;
    int                                 implicitHead;
    int                                 implicitTail;
    std::istream&                       str;
    Parser::ParserInterface&            pi;
    yaml_parser_t                       parser;   
    bool                                done;

    virtual int yylex(Parser::ParserInterface& pi)   override;
    virtual std::string getToken() const     {return "";}

    void start_stream(yaml_event_t const& event);
    void stop_stream(yaml_event_t const& event);
    void start_doc(yaml_event_t const& event);
    void stop_doc(yaml_event_t const& event);
    void push_scalarValue(yaml_event_t const& event);
    void push_array(yaml_event_t const& event);
    void pop_array(yaml_event_t const& event);
    void push_map(yaml_event_t const& event);
    void pop_map(yaml_event_t const& event);

    std::unique_ptr<Parser::ParserValue>    getScalar(yaml_event_t const& event);
    void addToken(Parser::ParserObjectType type, std::unique_ptr<Parser::ParserValue>&& token);

    public:
        YamlParser(std::istream& str, Parser::ParserInterface& pi);
        ~YamlParser();
        int yamlStreamReader(char* buffer, size_t size, size_t& size_read);

        int parse();
};

    }
}

#endif

