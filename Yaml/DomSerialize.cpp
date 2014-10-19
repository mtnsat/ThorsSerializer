
#include "DomSerialize.h"
#include "YamlEmitter.h"
#include "YamlUtil.h"
#include "Parser/ParserDomVisit.h"


namespace ThorsAnvil
{
    namespace Yaml
    {

struct YamlSerializeVisitor: public Parser::ParserValueConstVisitor
{
    YamlEmitter     emitter;
    bool            docStart;

    YamlSerializeVisitor(std::ostream& stream)
        : emitter(stream)
        , docStart(false)
    {}

    class DocWriter
    {
        bool                        started;
        bool&                       docStart;
        Parser::ParserValue const&  item;
        YamlEmitter&                emitter;

      public:
        DocWriter(bool& docStart, Parser::ParserValue const& item, YamlEmitter& emitter)
            : started(docStart)
            , docStart(docStart)
            , item(item)
            , emitter(emitter)
        {
            if (!started)
            {
                docStart    = true;
                emitter.writeDocStart(item.getAttributes());
            }
        }
        ~DocWriter()
        {
            if (!started)
            {
                emitter.writeDocEnd(item.getAttributes());
                docStart    = false;
            }
        }
    };

    virtual void visit(Parser::ParserStringItem const& item)
    {
        DocWriter   writer(docStart, item, emitter);
        emitter.writeString(item.value, item.getAttributes());
    }
    virtual void visit(Parser::ParserNumberItem const& item)
    {
        DocWriter   writer(docStart, item, emitter);
        emitter.writeNumber(item.value, item.getAttributes());
    }
    virtual void visit(Parser::ParserBoolItem const& item)
    {
        DocWriter   writer(docStart, item, emitter);
        emitter.writeBool(item.value, item.getAttributes());
    }
    virtual void visit(Parser::ParserNULLItem const& item)
    {
        DocWriter   writer(docStart, item, emitter);
        emitter.writeNull(item.value, item.getAttributes());
    }
    virtual void visit(Parser::ParserMapItem const& item)
    {
        DocWriter   writer(docStart, item, emitter);
        emitter.writeMapStart(item.getAttributes());
        item.value->accept(*this);
        emitter.writeMapEnd(item.getAttributes());
    }
    virtual void visit(Parser::ParserArrayItem const& item)
    {
        DocWriter   writer(docStart, item, emitter);
        emitter.writeArrayStart(item.getAttributes());
        item.value->accept(*this);
        emitter.writeArrayEnd(item.getAttributes());
    }
    virtual void visit(Parser::ParserArray const& node)
    {
        for(auto loop = node.begin(); loop != node.end(); ++loop)
        {   loop->accept(*this);
        }
    }
    virtual void visit(Parser::ParserMap const& /*node*/, Parser::Storage const& mapData, Parser::Storage const& keyData)
    {
        Parser::ParserMap::const_iterator loopK = keyData.begin();
        Parser::ParserMap::const_iterator loopV = mapData.begin();

        for(; loopV != mapData.end(); ++loopK, ++loopV)
        {
            loopK->second->accept(*this);
            loopV->second->accept(*this);
        }
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

