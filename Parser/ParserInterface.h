
#ifndef THORSANVIL_PARSER_PARSER_PARSER_INTERFACE_H
#define THORSANVIL_PARSER_PARSER_PARSER_INTERFACE_H

#include "ParserDom.h"
#include "ParserException.h"
#include "LexerParser.h"

#include <string>
#include <iostream>


namespace ThorsAnvil
{
    namespace Parser
    {

    class LexerParser;
    struct ParserInterface;

/*
 * Exceptions:
 *      class InvalidConversion
 *      class ParsingError
 *
 * Interface for Parsing
 *      struct ParserInterface
 *      struct ParserLogInterface   (generated debug output)
 *      struct ParserCleanInterface
 *      struct ParserDomInterface
 *
 */

/*
 * The interface injected into the parser that will do the work.
 */
struct ParserInterface
{
    ~ParserInterface()  {}
    virtual void            doneMap(ParserMap* map)                             = 0;
    virtual void            doneAray(ParserArray* array)                        = 0;
    virtual void            mapOpen()                                           = 0;
    virtual void            mapClose()                                          = 0;
    virtual ParserMap*      mapCreate()                                         = 0;
    virtual ParserMap*      mapCreate(ParserMapValue* val)                      = 0;
    virtual ParserMap*      mapAppend(ParserMap* map, ParserMapValue* val)      = 0;
    virtual ParserMapValue* mapCreateElement(std::string* k,ParserValue* val)   = 0;
    virtual std::string*    mapKeyNote(std::string* k)                          = 0;
    virtual void            arrayOpen()                                         = 0;
    virtual void            arrayClose()                                        = 0;
    virtual ParserArray*    arrayCreate()                                       = 0;
    virtual ParserArray*    arrayCreate(ParserValue* val)                       = 0;
    virtual ParserArray*    arrayAppend(ParserArray* arr, ParserValue* val)     = 0;
    virtual ParserArray*    arrayNote(ParserArray* arr)                         = 0;
    virtual ParserValue*    arrayCreateElement(ParserValue* val)                = 0;
    virtual ParserValue*    valueParseMap(ParserMap* map)                       = 0;
    virtual ParserValue*    valueParseArray(ParserArray* arr)                   = 0;
    virtual ParserValue*    valueParseString(std::string* str)                  = 0;
    virtual ParserValue*    valueParseNumber(std::string* num)                  = 0;
    virtual ParserValue*    valueParseBool(bool value)                          = 0;
    virtual ParserValue*    valueParseNULL()                                    = 0;
    virtual int             lexResult(int val)                                  { return val;}
};

struct ParseLogInterface: ParserInterface
{
    virtual void            doneMap(ParserMap*)                                 {std::cout << "ParserObject: ParserMap\n";}
    virtual void            doneAray(ParserArray*)                              {std::cout << "ParserObject: ParserArray\n";}
    virtual void            mapOpen()                                           {}
    virtual void            mapClose()                                          {std::cout << "ParserMap: { ParserMapValueListOpt }\n";}
    virtual ParserMap*      mapCreate()                                         {std::cout << "ParserMapValueListOpt: EMPTY\n";                               return NULL;}
    virtual ParserMap*      mapCreate(ParserMapValue*)                          {std::cout << "ParserMapValueList: ParserMapValue\n";                           return NULL;}
    virtual ParserMap*      mapAppend(ParserMap*, ParserMapValue*)              {std::cout << "ParserMapValueList: ParserMapValueList , ParserMapValue\n";        return NULL;}
    virtual ParserMapValue* mapCreateElement(std::string*,ParserValue*)         {std::cout << "ParserMapValue: PARSER_STRING : ParserValue\n";                  return NULL;}
    virtual std::string*    mapKeyNote(std::string*)                            {                                                                           return NULL;}
    virtual void            arrayOpen()                                         {}
    virtual void            arrayClose()                                        {std::cout << "ParserArray: [ ParserArrayValueListOpt ]\n";}
    virtual ParserArray*    arrayCreate()                                       {std::cout << "ParserArrayValueListOpt: EMPTY\n";                             return NULL;}
    virtual ParserArray*    arrayCreate(ParserValue*)                           {std::cout << "ParserArrayValueList: ParserArrayValue\n";                       return NULL;}
    virtual ParserArray*    arrayAppend(ParserArray*, ParserValue*)             {std::cout << "ParserArrayValueList: ParserArrayListItem ParserArrayValue\n";     return NULL;}
    virtual ParserArray*    arrayNote(ParserArray*)                             {std::cout << "ParserArrayListItem: ParserArrayValueList ','";                  return NULL;}
    virtual ParserValue*    arrayCreateElement(ParserValue*)                    {std::cout << "ParserArrayValue: ParserValue\n";                                return NULL;}
    virtual ParserValue*    valueParseMap(ParserMap*)                           {std::cout << "ParserValue: ParserMap\n";                                       return NULL;}
    virtual ParserValue*    valueParseArray(ParserArray*)                       {std::cout << "ParserValue: ParserArray\n";                                     return NULL;}
    virtual ParserValue*    valueParseString(std::string*)                      {std::cout << "ParserValue: ParserString\n";                                    return NULL;}
    virtual ParserValue*    valueParseNumber(std::string*)                      {std::cout << "ParserValue: ParserNumber\n";                                    return NULL;}
    virtual ParserValue*    valueParseBool(bool)                                {std::cout << "ParserValue: ParserTrue\n";                                      return NULL;}
    virtual ParserValue*    valueParseNULL()                                    {std::cout << "ParserValue: ParserFalse\n";                                     return NULL;}
    virtual int             lexResult(int val)                                  {std::cout << "LEX(" << val << ")\n";                                       return val;}
};

struct ParserCleanInterface: ParserInterface
{
    virtual void            doneMap(ParserMap* map)                             { delete map;}
    virtual void            doneAray(ParserArray* array)                        { delete array;}
    virtual void            mapOpen()                                           {}
    virtual void            mapClose()                                          {}
    virtual ParserMap*      mapCreate()                                         { return NULL;}
    virtual ParserMap*      mapCreate(ParserMapValue* val)                        { delete val; return NULL;}
    virtual ParserMap*      mapAppend(ParserMap* map, ParserMapValue* val)          { std::unique_ptr<ParserMapValue> aval(val); delete map; return NULL;}
    virtual ParserMapValue* mapCreateElement(std::string* k,ParserValue* val)     { std::unique_ptr<ParserValue> aval(val); delete k;   return NULL;}
    virtual std::string*    mapKeyNote(std::string* k)                          { delete k; return NULL;}
    virtual void            arrayOpen()                                         {}
    virtual void            arrayClose()                                        {}
    virtual ParserArray*    arrayCreate()                                       { return NULL;}
    virtual ParserArray*    arrayCreate(ParserValue* val)                       { delete val; return NULL;}
    virtual ParserArray*    arrayAppend(ParserArray* arr, ParserValue* val)     { std::unique_ptr<ParserArray> aarr(arr); delete val; return NULL;}
    virtual ParserArray*    arrayNote(ParserArray* arr)                         { delete arr; return NULL;}
    virtual ParserValue*    arrayCreateElement(ParserValue* val)                { delete val; return NULL;}
    virtual ParserValue*    valueParseMap(ParserMap* map)                       { delete map; return NULL;}
    virtual ParserValue*    valueParseArray(ParserArray* arr)                   { delete arr; return NULL;}
    virtual ParserValue*    valueParseString(std::string* str)                  { delete str; return NULL;}
    virtual ParserValue*    valueParseNumber(std::string* num)                  { delete num; return NULL;}
    virtual ParserValue*    valueParseBool(bool)                                { return NULL;}
    virtual ParserValue*    valueParseNULL()                                    { return NULL;}
};

struct ParserDomInterface: ParserCleanInterface
{
    virtual ParserMap*      mapCreate()                                         { return new ParserMap();}
    virtual ParserMap*      mapCreate(ParserMapValue* val)                      { std::unique_ptr<ParserMapValue>aval(val);std::unique_ptr<ParserMap> amap(new ParserMap());amap->insert(*(aval->first), aval->second.release());return amap.release();}
    virtual ParserMap*      mapAppend(ParserMap* map, ParserMapValue* val)      { std::unique_ptr<ParserMapValue>aval(val);std::unique_ptr<ParserMap> amap(map);          amap->insert(*(aval->first), aval->second.release());return amap.release();}
    virtual ParserMapValue* mapCreateElement(std::string* k,ParserValue* val)   { std::unique_ptr<ParserValue>  aval(val);std::unique_ptr<std::string> ak(k);
                                                                                  std::unique_ptr<ParserMapValue> result(new ParserMapValue);result->first = std::move(ak);result->second = std::move(aval);
                                                                                  return result.release();
                                                                                }
    virtual std::string*    mapKeyNote(std::string* k)                          { return k;}
    virtual ParserArray*    arrayCreate()                                       { return new ParserArray();}
    virtual ParserArray*    arrayCreate(ParserValue* val)                       { std::unique_ptr<ParserValue>  aval(val);std::unique_ptr<ParserArray>   aarr(new ParserArray());aarr->push_back(aval.release()); return aarr.release();}
    virtual ParserArray*    arrayAppend(ParserArray* arr, ParserValue* val)     { std::unique_ptr<ParserValue>  aval(val);std::unique_ptr<ParserArray>   aarr(arr);            aarr->push_back(aval.release()); return aarr.release();}
    virtual ParserArray*    arrayNote(ParserArray* arr)                         { return arr;}
    virtual ParserValue*    arrayCreateElement(ParserValue* val)                { std::unique_ptr<ParserValue>  aval(val); return aval.release();}

    virtual ParserValue*    valueParseMap(ParserMap* map)                       { std::unique_ptr<ParserMap>     amap(map); return new ParserMapItem(amap);}
    virtual ParserValue*    valueParseArray(ParserArray* arr)                   { std::unique_ptr<ParserArray>   aarr(arr); return new ParserArrayItem(aarr);}
    virtual ParserValue*    valueParseString(std::string* str)                  { std::unique_ptr<std::string> astr(str); return new ParserStringItem(astr);}
    virtual ParserValue*    valueParseNumber(std::string* num)                  { std::unique_ptr<std::string> anum(num); return new ParserNumberItem(anum);}
    virtual ParserValue*    valueParseBool(bool value)                          { return new ParserBoolItem(value);}
    virtual ParserValue*    valueParseNULL()                                    { return new ParserNULLItem();}
};


template<typename C>
struct TransformParserStringIter: std::iterator<std::input_iterator_tag, char, ptrdiff_t, char*,char&>
{
    C&       cont;
    bool     lastWasSlash;
    int      unicodeCount;
    uint32_t unicodeValue;
    TransformParserStringIter(C& c)
        : cont(c)
        , lastWasSlash(false)
        , unicodeCount(0)
    {}
    TransformParserStringIter& operator++()       {return *this;}
    TransformParserStringIter& operator*()        {return *this;}
    void                     operator=(char x)
    {
        if (unicodeCount)
        {
            if (unicodeCount == 6)
            {
                if (x != '\\')  { throw ThorsAnvil::Parser::ParsingError("Surrogate pair(No Slash): \\uD8xx Must be followed by \\uDCxx");}
                --unicodeCount;
            }
            else if (unicodeCount == 5)
            {
                if (x != 'u')   { throw ThorsAnvil::Parser::ParsingError("Surrogate pair(No u): \\uD8xx Must be followed by \\uDCxx");}
                --unicodeCount;
            }
            else
            {
                unicodeValue <<= 4;
                unicodeValue += ('0' <= x && x <= '9') ? (x - '0') : 10 + (('A' <= x && x <= 'F') ? (x - 'A') : (x - 'a'));
                --unicodeCount;
                if (unicodeCount == 0)
                {
                    if (unicodeValue <= 0x7F)
                    {
                        // Encode as single UTF-8 character
                        cont.push_back(unicodeValue);
                    }
                    else if (unicodeValue <= 0x7FF)
                    {
                        // Encode as two UTF-8 characters
                        cont.push_back(0xC0 |((unicodeValue >>  6)));
                        cont.push_back(0x80 |((unicodeValue >>  0) & 0x3F));
                    }
                    else if (unicodeValue <= 0xFFFF)
                    {
                        if ((unicodeValue & 0xFC00) != 0xD800)
                        {
                            // Encode as three UTF-8 characters
                            cont.push_back(0xE0 |((unicodeValue >> 12)));
                            cont.push_back(0x80 |((unicodeValue >>  6) & 0x3F));
                            cont.push_back(0x80 |((unicodeValue >>  0) & 0x3F));
                        }
                        else
                        {
                            // We have a found first part of surrogate pair
                            unicodeCount    = 6;
                        }
                    }
                    else
                    {
                        // Surrogate pair
                        if ((unicodeValue & 0xFC00FC00) != 0xD800DC00){ throw ThorsAnvil::Parser::ParsingError("Surrogate pair(No DC): \\uD8xx Must be followed by \\uDCxx");}

                        // Decode surrogate pair
                        unicodeValue    = 0x00010000 | ((unicodeValue & 0x03FF0000) >> 6) | (unicodeValue & 0x000003FF);

                        // Encode as 4 UTF-8 characters
                        cont.push_back(0xF0 |((unicodeValue >> 18)));
                        cont.push_back(0x80 |((unicodeValue >> 12) & 0x3F));
                        cont.push_back(0x80 |((unicodeValue >>  6) & 0x3F));
                        cont.push_back(0x80 |((unicodeValue >>  0) & 0x3F));
                    }
                }
            }
        }
        else if (lastWasSlash)
        {
            switch(x)
            {
                case '"':   cont.push_back('"');    break;
                case '\\':  cont.push_back('\\');   break;
                case '/':   cont.push_back('/');    break;
                case 'b':   cont.push_back('\b');   break;
                case 'f':   cont.push_back('\f');   break;
                case 'n':   cont.push_back('\n');   break;
                case 'r':   cont.push_back('\r');   break;
                case 't':   cont.push_back('\t');   break;
                case 'u':   unicodeCount = 4; unicodeValue = 0; break;
            }
            lastWasSlash    = false;
        }
        else
        {
            if (x == '\\')
            {   lastWasSlash    = true;
            }
            else
            {   cont.push_back(x);
            }
        }
    }
};
template<typename C> TransformParserStringIter<C> make_TransformParserStringIter(C& cont)   {return TransformParserStringIter<C>(cont);}

    }
}

#endif

