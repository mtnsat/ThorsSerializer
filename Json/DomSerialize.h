
#ifndef THORS_ANVIL_JSON_JSON_SERIALIZER_H
#define THORS_ANVIL_JSON_JSON_SERIALIZER_H

#include "Parser/ParserDom.h"

namespace ThorsAnvil
{
    namespace Json
    {


template<typename T>
struct DomSerializer
{
    T const& value;

    DomSerializer(T const& value)
        : value(value)
    {}
};

inline DomSerializer<Parser::ParserValue>   DomSerialize(Parser::ParserValue const& value)   {return DomSerializer<Parser::ParserValue>(value);}
inline DomSerializer<Parser::ParserMap>     DomSerialize(Parser::ParserMap   const& value)   {return DomSerializer<Parser::ParserMap>(value);}
inline DomSerializer<Parser::ParserArray>   DomSerialize(Parser::ParserArray const& value)   {return DomSerializer<Parser::ParserArray>(value);}

template<typename T>
// The only valid values of T are [Parser::ParserValue, Parser::ParserMap, Parser::ParserArray]
std::ostream& operator<<(std::ostream& stream, DomSerializer<T> const& data);

    }
}

#endif

