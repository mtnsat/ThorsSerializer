
#include "DomSerialize.h"
#include "YamlEmitter.h"
#include "Parser/ParserDomVisit.h"


namespace ThorsAnvil
{
    namespace Yaml
    {

struct YamlSerializeVisitor: public Parser::ParserValueConstVisitor
{
    YamlEmitter     emitter;

    YamlSerializeVisitor(std::ostream& stream)
        : emitter(stream)
    {}

    virtual void visit(Parser::ParserStringItem const& item)
    {
        emitter.writeString(*item.value);
    }
    virtual void visit(Parser::ParserNumberItem const& item)
    {
        emitter.writeNumber(*item.value);
    }
    virtual void visit(Parser::ParserBoolItem const& item)
    {
        emitter.writeBool(item.value?"true":"false");
    }
    virtual void visit(Parser::ParserNULLItem const&)
    {
        emitter.writeNull("null");
    }
    virtual void visit(Parser::ParserMapItem const& item)
    {
        item.value->accept(*this);
    }
    virtual void visit(Parser::ParserArrayItem const& item)
    {
        item.value->accept(*this);
    }
    virtual void visit(Parser::ParserArray const& node)
    {
        emitter.writeArrayStart();
        for(auto loop = node.begin(); loop != node.end(); ++loop)
        {   loop->accept(*this);
        }
        emitter.writeArrayEnd();
    }
    virtual void visit(Parser::ParserMap const& /*node*/, Parser::Storage const& mapData, Parser::Storage const& keyData)
    {
        emitter.writeMapStart();

        Parser::ParserMap::const_iterator loopK = keyData.begin();
        Parser::ParserMap::const_iterator loopV = mapData.begin();

        for(; loopV != mapData.end(); ++loopK, ++loopV)
        {
            loopK->second->accept(*this);
            loopV->second->accept(*this);
        }

        emitter.writeMapEnd();
    }
};


template<typename T>
std::ostream& operator<<(std::ostream& stream, DomSerializer<T> const& data)
{
    YamlSerializeVisitor    serializer(stream);
    data.value.accept(serializer);
    return stream;
}

template std::ostream& operator<<(std::ostream& stream, DomSerializer<ThorsAnvil::Parser::ParserValue> const& data);
template std::ostream& operator<<(std::ostream& stream, DomSerializer<ThorsAnvil::Parser::ParserMap> const& data);
template std::ostream& operator<<(std::ostream& stream, DomSerializer<ThorsAnvil::Parser::ParserArray> const& data);

    }
}

