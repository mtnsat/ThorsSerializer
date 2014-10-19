
#ifndef THORS_ANVIL_PARSER_DOM_SERIALIZER_H
#define THORS_ANVIL_PARSER_DOM_SERIALIZER_H

#include "ParserDom.h"
#include "ParserDomVisit.h"
#include "EmitterInterface.h"

namespace ThorsAnvil
{
    namespace Parser
    {

template<typename E, typename T>
struct DomSerializer
{
    T const& value;

    DomSerializer(T const& value)
        : value(value)
    {}
};

template<typename E>
inline DomSerializer<E, ParserValue>   DomSerialize(ParserValue const& value)     {return DomSerializer<E, ParserValue>(value);}
template<typename E>
inline DomSerializer<E, ParserMap>     DomSerialize(ParserMap   const& value)     {return DomSerializer<E, ParserMap>(value);}
template<typename E>
inline DomSerializer<E, ParserArray>   DomSerialize(ParserArray const& value)     {return DomSerializer<E, ParserArray>(value);}


struct DomSerializeVisitor: public ParserValueConstVisitor
{
    EmitterInterface&    emitter;
    bool                 docStart;

    DomSerializeVisitor(EmitterInterface& emitter);

    virtual void visit(ParserStringItem const& item);
    virtual void visit(ParserNumberItem const& item);
    virtual void visit(ParserBoolItem const& item);
    virtual void visit(ParserNULLItem const& item);
    virtual void visit(ParserMapItem const& item);
    virtual void visit(ParserArrayItem const& item);
    virtual void visit(ParserArray const& node);
    virtual void visit(ParserMap const& /*node*/, Storage const& mapData, Storage const& keyData);
};

template<typename E, typename T>
// The only valid values of T are [ParserValue, ParserMap, ParserArray]
std::ostream& operator<<(std::ostream& stream, DomSerializer<E, T> const& data)
{
    E                       emitter(stream);
    DomSerializeVisitor     serializer(emitter);
    data.value.accept(serializer);
    return stream;
}


    }
}

#endif

