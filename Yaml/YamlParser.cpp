
#include "YamlParser.h"
#include "Parser/ParserException.h"
#include <boost/lexical_cast.hpp>

using namespace ThorsAnvil::Yaml;
using ThorsAnvil::Parser::ParserValue;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;
using ThorsAnvil::Parser::ParserValue;
using ThorsAnvil::Parser::ParserMapValue;
using ThorsAnvil::Parser::ParserMapItem;
using ThorsAnvil::Parser::ParserArrayItem;

extern "C"
{
    int thorsanvilYamlStreamReader(void *data, unsigned char *buffer, size_t size, size_t *size_read);
}

int thorsanvilYamlStreamReader(void *data, unsigned char *buffer, size_t size, size_t *size_read)
{
    YamlParser*     owner = reinterpret_cast<YamlParser*>(data);
    return owner->yamlStreamReader(reinterpret_cast<char*>(buffer), size, *size_read);
}

YamlParser::YamlParser(std::istream& str, Parser::ParserInterface& pi)
    : str(str)
    , pi(pi)
{
    yaml_parser_initialize(&parser);
    yaml_parser_set_input(&parser, thorsanvilYamlStreamReader, this);
}

YamlParser::~YamlParser()
{
    yaml_parser_delete(&parser);
}

int YamlParser::yamlStreamReader(char* buffer, size_t size, size_t& size_read)
{
    bool    result  = false;
    size_read       = 0;
    str.read(buffer, size);
    size_read   = str.gcount();
    result      = (size_read != size_t(-1));

    return result;
}

struct YamlEventCleaner
{
    yaml_event_t&   event;
    YamlEventCleaner(yaml_event_t& event)
        : event(event)
    {}
    ~YamlEventCleaner()
    {
        yaml_event_delete(&event);
    }
};

int YamlParser::parse()
{
    done    = false;

    while(!done)
    {
        yaml_event_t        event;
        if (!yaml_parser_parse(&parser, &event))
        {   throw Parser::ParsingError("Yaml Parse Failed");
        }
        YamlEventCleaner    cleaner(event);

        switch(event.type)
        {
            case YAML_NO_EVENT:                                      break;

            case YAML_STREAM_START_EVENT:   start_stream(event);     break;
            case YAML_STREAM_END_EVENT:     stop_stream(event);      break;

            case YAML_DOCUMENT_START_EVENT: start_doc(event);        break;
            case YAML_DOCUMENT_END_EVENT:   stop_doc(event);         break;

            case YAML_ALIAS_EVENT:                                   break;
            case YAML_SCALAR_EVENT:         push_scalarValue(event); break;

            case YAML_SEQUENCE_START_EVENT: push_array(event);       break;
            case YAML_SEQUENCE_END_EVENT:   pop_array(event);        break;

            case YAML_MAPPING_START_EVENT:  push_map(event);         break;
            case YAML_MAPPING_END_EVENT:    pop_map(event);          break;
            //default:                        break;
        }
    }
    return 0;
}

void YamlParser::start_stream(yaml_event_t const&)
{
}
void YamlParser::stop_stream(yaml_event_t const&)
{
}
void YamlParser::start_doc(yaml_event_t const&)
{
}
void YamlParser::stop_doc(yaml_event_t const&)
{
    done    = true;
}
std::unique_ptr<ParserValue> YamlParser::getScalar(yaml_event_t const& event)
{
    std::string eventValue  = std::string(reinterpret_cast<char const*>(event.data.scalar.value), event.data.scalar.length);
    #if 0
    std::cout << "getScalar()\n";
    char const* anchor = event.data.scalar.anchor ? (char const*)event.data.scalar.anchor : "NULL";
    char const* tag    = event.data.scalar.tag    ? (char const*)event.data.scalar.tag    : "NULL";
    char const* value  = event.data.scalar.value  ? (char const*)event.data.scalar.value  : "NULL";

    std::cout << "Scalar: Anchor(" << anchor << ") tag(" << tag << ") value(" << value << ") Length(" << event.data.scalar.length << ") plain_implicit(" << event.data.scalar.plain_implicit << ") quoted_implicit(" << event.data.scalar.quoted_implicit << ") style(";
    switch(event.data.scalar.style)
    {
        case YAML_ANY_SCALAR_STYLE:             std::cout << "YAML_ANY_SCALAR_STYLE";break;
        case YAML_PLAIN_SCALAR_STYLE:           std::cout << "YAML_PLAIN_SCALAR_STYLE";break;
        case YAML_SINGLE_QUOTED_SCALAR_STYLE:   std::cout << "YAML_SINGLE_QUOTED_SCALAR_STYLE";break;
        case YAML_DOUBLE_QUOTED_SCALAR_STYLE:   std::cout << "YAML_DOUBLE_QUOTED_SCALAR_STYLE";break;
        case YAML_LITERAL_SCALAR_STYLE:         std::cout << "YAML_LITERAL_SCALAR_STYLE";break;
        case YAML_FOLDED_SCALAR_STYLE:          std::cout << "YAML_FOLDED_SCALAR_STYLE";break;
    }
    std::cout << ")\n";
    #endif
    if (event.data.scalar.quoted_implicit)
    {   return std::unique_ptr<ParserValue>(pi.valueParseString(new std::string(eventValue)));
    }
    if (event.data.scalar.tag)
    {
        //std::cout << "TAG: " << event.data.scalar.tag << "\n";
        std::string type    = reinterpret_cast<char const*>(event.data.scalar.tag);
        if (type == "tag:yaml.org,2002:str")    {return std::unique_ptr<ParserValue>(pi.valueParseString(new std::string(eventValue)));}
        if (type == "tag:yaml.org,2002:null")   {return std::unique_ptr<ParserValue>(pi.valueParseNULL(new std::string(eventValue)));}
        if (type == "tag:yaml.org,2002:bool")   {return std::unique_ptr<ParserValue>(pi.valueParseBool(new std::string(eventValue), eventValue == "true"));}
        if (type == "tag:yaml.org,2002:int")    {return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));}
        if (type == "tag:yaml.org,2002:float")  {return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));}
    }

    // std::cout << "Scalar: ->" << eventValue << "<-\n";
    if (eventValue == "null" || eventValue == "Null" || eventValue == "NULL" || eventValue == "~" || eventValue == "")
    {   return std::unique_ptr<ParserValue>(pi.valueParseNULL(new std::string(eventValue)));
    }
    if (eventValue == "true" || eventValue == "True" || eventValue == "TRUE")
    {   return std::unique_ptr<ParserValue>(pi.valueParseBool(new std::string(eventValue), true));
    }
    if (eventValue == "false" || eventValue == "False" || eventValue == "FALSE")
    {   return std::unique_ptr<ParserValue>(pi.valueParseBool(new std::string(eventValue), false));
    }

    auto end = eventValue.end();
    auto ptr = eventValue.begin();
    bool potentialNumber = false;

    // Check for Dec Integer
    if (ptr != end && (*ptr == '-' || *ptr == '+'))
    {   ++ptr;
    }
    if (ptr != end && *ptr >= '1' && *ptr <= '9')
    {
        potentialNumber = true;
        while(ptr != end && *ptr >= '0' && *ptr <= '9')
        {   ++ptr;
        }
    }
    if (potentialNumber && ptr == end)
    {   
        return std::unique_ptr<ParserValue>(pi.valueParseNumber(10, 0, new std::string(eventValue)));
    }

    // Check for Oct Integer
    ptr             = eventValue.begin();
    potentialNumber = false;
    if (ptr != end && *ptr == '0')
    {   ++ptr;
        if (ptr != end && *ptr == 'o')
        {   ++ptr;
            while(ptr != end && *ptr >= '0' && *ptr <= '7')
            {
                ++ptr;
                potentialNumber = true;
            }
        }
    }
    if (potentialNumber && ptr == end)
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(8, 2, new std::string(eventValue)));
    }

    // Check for Oct Integer
    ptr             = eventValue.begin();
    potentialNumber = false;
    if (ptr != end && *ptr == '0')
    {   ++ptr;
        if (ptr != end && *ptr == 'x')
        {   ++ptr;
            while(ptr != end && ((*ptr >= '0' && *ptr <= '9') || (*ptr >= 'a' && *ptr <= 'f') || (*ptr >= 'A' && *ptr <= 'F')))
            {
                ++ptr;
                potentialNumber = true;
            }
        }
    }
    if (potentialNumber && ptr == end)
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(16, 2, new std::string(eventValue)));
    }

    // Check for Float
    ptr             = eventValue.begin();
    potentialNumber = false;
    if (ptr != end && (*ptr == '-' || *ptr == '+'))
    {   ++ptr;
    }
    if (ptr != end && *ptr == '.')
    {
        ++ptr;
        while(ptr != end && *ptr >= '0' && *ptr <= '9')
        {
            ++ptr;
            potentialNumber = true;
        }
    }
    else
    {
        if (ptr != end && *ptr >= '0' && *ptr <= '9')
        {
            potentialNumber = true;
            while(ptr != end && *ptr >= '0' && *ptr <= '9')
            {   ++ptr;
            }
        }
   
        if (ptr != end && *ptr == '.')
        {
            ++ptr;
            while(ptr != end && *ptr >= '0' && *ptr <= '9')
            {   ++ptr;
            }
        }
    }
    if (ptr != end && (*ptr == 'e' || *ptr == 'E'))
    {
        ++ptr;
        if (ptr != end && (*ptr == '+' || *ptr == '-'))
        {   ++ptr;
        }
        potentialNumber = false;
        while(ptr != end && *ptr >= '0' && *ptr <= '9')
        {
            potentialNumber = true;
            ++ptr;
        }
    }
    if (potentialNumber && ptr == end)
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));
    }

    // Check for Float out of Range.
    if (eventValue == ".inf" || eventValue == ".Inf" || eventValue == ".INF")
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));
    }
    if (eventValue == "-.inf" || eventValue == "-.Inf" || eventValue == "-.INF")
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));
    }
    if (eventValue == "+.inf" || eventValue == "+.Inf" || eventValue == "+.INF")
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));
    }
    if (eventValue == ".nan" || eventValue == ".NaN" || eventValue == ".NAN")
    {   return std::unique_ptr<ParserValue>(pi.valueParseNumber(new std::string(eventValue)));
    }

    // Otherwise a String
    return std::unique_ptr<ParserValue>(pi.valueParseString(new std::string(eventValue)));
}
void YamlParser::push_scalarValue(yaml_event_t const& event)
{
    // std::cout << "push_scalarValue()\n";
    std::unique_ptr<ParserValue>    scalar(getScalar(event));
    if (scalar.get())
    {
        if (event.data.scalar.anchor)
        {   scalar->setAttribute("anchor",          reinterpret_cast<char*>(event.data.scalar.anchor));
        }
        if (event.data.scalar.tag)
        {   scalar->setAttribute("tag",             reinterpret_cast<char*>(event.data.scalar.tag));
        }
        scalar->setAttribute("plain_implicit",  std::to_string(event.data.scalar.plain_implicit));
        scalar->setAttribute("quoted_implicit", std::to_string(event.data.scalar.quoted_implicit));
        scalar->setAttribute("style",           std::to_string(event.data.scalar.style));
    }

    addToken(Parser::ParserValueObject, std::move(scalar));
}

void YamlParser::push_array(yaml_event_t const& event)
{
    pi.arrayOpen();
    hierarchy.emplace_back(static_cast<Parser::ParserArray*>(nullptr), event);
}
void YamlParser::pop_array(yaml_event_t const&)
{
    YamlHierarchyObject&    back = hierarchy.back();
    if (!back.set)
    {
        back.obj.data.array    = pi.arrayCreate();
    }

    pi.arrayClose();

    std::unique_ptr<ParserValue>  array{pi.valueParseArray(back.obj.data.array)};
    if (array.get())
    {
        array->setAttribute("anchor",      back.anchor);
        array->setAttribute("tag",         back.tag);
        array->setAttribute("implicit",    std::to_string(back.implicit));
        array->setAttribute("style",       std::to_string(back.style));
    }
    back.obj.data.array                 = nullptr;
    hierarchy.pop_back();
    addToken(Parser::ParserArrayObject, std::move(array));
}
void YamlParser::push_map(yaml_event_t const& event)
{
    pi.mapOpen();
    hierarchy.emplace_back(static_cast<Parser::ParserMap*>(nullptr), event);
}
void YamlParser::pop_map(yaml_event_t const&)
{
    YamlHierarchyObject&    back = hierarchy.back();
    if (!back.set)
    {
        back.obj.data.map    = pi.mapCreate();
    }

    pi.mapClose();

    std::unique_ptr<ParserValue>  map{pi.valueParseMap(back.obj.data.map)};
    if (map.get())
    {
        map->setAttribute("anchor",      back.anchor);
        map->setAttribute("tag",         back.tag);
        map->setAttribute("implicit",    std::to_string(back.implicit));
        map->setAttribute("style",       std::to_string(back.style));
    }
    back.obj.data.map               = nullptr;
    hierarchy.pop_back();
    addToken(Parser::ParserMapObject, std::move(map));
}

void YamlParser::addToken(Parser::ParserObjectType type, std::unique_ptr<ParserValue>&& token)
{
    // std::cout << "addToken()\n";
    if (hierarchy.size() == 0)
    {
        switch(type)
        {
            case Parser::ParserMapObject:
            {
                std::unique_ptr<ParserMapItem>  mapItem(dynamic_cast<ParserMapItem*>(token.release()));
                if (mapItem.get())
                {
                    pi.done(Parser::ParserMapObject, mapItem.release());
                }
                break;
            }
            case Parser::ParserArrayObject:
            {
                std::unique_ptr<ParserArrayItem>  arrayItem(dynamic_cast<ParserArrayItem*>(token.release()));
                if (arrayItem.get())
                {
                    pi.done(Parser::ParserArrayObject, arrayItem.release());
                }
                break;
            }
            case Parser::ParserValueObject:
            {
                pi.done(Parser::ParserValueObject, token.release());
                break;
            }
            default:    throw std::runtime_error("Invalid Object type");
        }
        return;
    }
    // std::cout << "Size: " << hierarchy.size() << "\n";
    YamlHierarchyObject&    back = hierarchy.back();
    // std::cout << "Back: " << &back << "\n";

    if ((back.obj.type == Parser::ParserMapObject) && (!back.keySet))
    {
        // std::cout << "Setting up Key\n";
        ParserValue* k1 = token.release();
        // std::cout << "k1: " << k1 << "\n";

        ParserValue* k2 = pi.mapKeyNote(k1);
        // std::cout << "k2: " << k2 << "\n";

        back.key.reset(k2);
        back.keySet = true;
        return;
    }

    if (back.obj.type == Parser::ParserMapObject)
    {
        // std::cout << "Setting Up Key/Value\n";
        ParserValue* k2 = back.key.release();
        // std::cout << "k2: " << k2 << "\n";

        ParserValue* t1 = token.release();
        // std::cout << "t1: " << t1 << "\n";

        std::unique_ptr<ParserMapValue> kv{pi.mapCreateElement(k2, t1)};
        back.obj.data.map       = (!back.set)
                                    ?  pi.mapCreate(kv.release())
                                    : pi.mapAppend(back.obj.data.map, kv.release());
        back.set    = true;
        back.keySet = false;
    }
    if (back.obj.type == Parser::ParserArrayObject)
    {
        if (!back.set)
        {
            // std::cout << "Array1: NULL\n";
            // std::cout << "Token: " << token.get() << "\n";
            ParserValue* el = pi.arrayCreateElement(token.release());
            // std::cout << "Element: " << el << "\n";
            back.obj.data.array = pi.arrayCreate(el);
            // std::cout << "Array1: " << back.obj.data.array << "\n";
            back.set    = true;
        }
        else
        {
            // std::cout << "Array2a: " << back.obj.data.array << "\n";
            back.obj.data.array = pi.arrayNote(back.obj.data.array);
            // std::cout << "Array2b: " << back.obj.data.array << "\n";
            back.obj.data.array = pi.arrayAppend(back.obj.data.array, pi.arrayCreateElement(token.release()));
            // std::cout << "Array2c: " << back.obj.data.array << "\n";
        }
    }
}

int YamlParser::yylex(Parser::ParserInterface& /*pi*/)
{
    return 1;
}

YamlObject::~YamlObject()
{
    switch(type)
    {
        case Parser::ParserMapObject:       delete data.map;    break;
        case Parser::ParserArrayObject:     delete data.array;  break;
        case Parser::ParserValueObject:     delete data.value;  break;
        default:                                                break;
    }
}
YamlObject::YamlObject(YamlObject&& move)
    : type(move.type)
{
    switch(type)
    {
        case Parser::ParserMapObject:       data.map    = nullptr;  break;
        case Parser::ParserArrayObject:     data.array  = nullptr;  break;
        case Parser::ParserValueObject:     data.value  = nullptr;  break;
        default:                                                    break;
    }
    (*this) = std::move(move);
}
YamlObject& YamlObject::operator=(YamlObject&& move)
{
    if (this != &move)
    {
        if (type != move.type)
        {
            switch(type)
            {
                case Parser::ParserMapObject:       delete data.map;    break;
                case Parser::ParserArrayObject:     delete data.array;  break;
                case Parser::ParserValueObject:     delete data.value;  break;
                default:                                                break;
            }
            type = move.type;
            switch(type)
            {
                case Parser::ParserMapObject:       data.map    = nullptr;  break;
                case Parser::ParserArrayObject:     data.array  = nullptr;  break;
                case Parser::ParserValueObject:     data.value  = nullptr;  break;
                default:                                                    break;
            }
        }
        switch(type)
        {
            case Parser::ParserMapObject:       std::swap(data.map,   move.data.map);   break;
            case Parser::ParserArrayObject:     std::swap(data.array, move.data.array); break;
            case Parser::ParserValueObject:     std::swap(data.value, move.data.value); break;
            default:                                                                    break;
        }
    }
    return *this;
}
/*
struct ParserInterface
{
    ~ParserInterface()  {}
    virtual void            done(YamlObject* result)                          = 0;
    virtual void            mapOpen()                                           = 0; *
    virtual void            mapClose()                                          = 0; *
    virtual ParserMap*      mapCreate()                                         = 0; *
    virtual ParserMap*      mapCreate(ParserMapValue* val)                      = 0; *
    virtual ParserMap*      mapAppend(ParserMap* map, ParserMapValue* val)      = 0; *
    virtual ParserMapValue* mapCreateElement(ParserValue* k,ParserValue* val)   = 0; *
    virtual ParserValue*    mapKeyNote(ParserValue* k)                          = 0; *
    virtual void            arrayOpen()                                         = 0; *
    virtual void            arrayClose()                                        = 0; *
    virtual ParserArray*    arrayCreate()                                       = 0; *
    virtual ParserArray*    arrayCreate(ParserValue* val)                       = 0; *
    virtual ParserArray*    arrayAppend(ParserArray* arr, ParserValue* val)     = 0; *
    virtual ParserArray*    arrayNote(ParserArray* arr)                         = 0; *
    virtual ParserValue*    arrayCreateElement(ParserValue* val)                = 0; *

    virtual ParserValue*    valueParseMap(ParserMap* map)                       = 0; *
    virtual ParserValue*    valueParseArray(ParserArray* arr)                   = 0; *
    virtual ParserValue*    valueParseString(std::string* str)                  = 0; *
    virtual ParserValue*    valueParseNumber(std::string* num)                  = 0; *
    virtual ParserValue*    valueParseBool(bool value)                          = 0; *
    virtual ParserValue*    valueParseNULL()                                    = 0; *
};
*/

