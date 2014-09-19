
#include "DomSerialize.h"
#include "Parser/ParserDomVisit.h"

namespace ThorsAnvil
{
    namespace Json
    {

struct JsonSerializeVisitor: public Parser::ParserValueConstVisitor
{
    std::ostream&   stream;

    JsonSerializeVisitor(std::ostream& stream)
        : stream(stream)
    {}

    virtual void visit(Parser::ParserStringItem const& item)
    {
        stream << '"';
        std::for_each(std::begin(item.value), std::end(item.value), [this](char next)
        {
            switch(next)
            {
                case '"':   stream   << "\\\"";          break;
                case '\\':  stream   << "\\\\";          break;
                default:    stream   << next;            break;
           }
        });
        stream << '"';
    }
    virtual void visit(Parser::ParserNumberItem const& item)        {   stream << item.value; }
    virtual void visit(Parser::ParserBoolItem const& item)          {   stream << std::boolalpha << item.boolValue; }
    virtual void visit(Parser::ParserNULLItem const&)               {   stream << "null"; }
    virtual void visit(Parser::ParserMapItem const& item)           {   item.value->accept(*this);}
    virtual void visit(Parser::ParserArrayItem const& item)         {   item.value->accept(*this);}
    virtual void visit(Parser::ParserArray const& node)
    {
        stream << "[";
        if (!node.empty())
        {
            Parser::ParserArray::const_iterator loop = node.begin();
            loop->accept(*this);

            for(++loop; loop != node.end(); ++loop)
            {
                stream << ", ";
                loop->accept(*this);
            }
        }
        stream << "]";
    }
    virtual void visit(Parser::ParserMap const& node, Parser::Storage const& mapData, Parser::Storage const& keyData)
    {
        stream << '{';
        if (!node.empty())
        {
            Parser::ParserMap::const_iterator loopK = keyData.begin();
            Parser::ParserMap::const_iterator loopV = mapData.begin();

            loopK->second->accept(*this);
            stream << ": ";
            loopV->second->accept(*this);

            for(++loopK, ++loopV; loopV != mapData.end(); ++loopK, ++loopV)
            {
                stream << ", ";
                loopK->second->accept(*this);
                stream << ": ";
                loopV->second->accept(*this);
            }
        }
        stream << "}";
    }
};


template<typename T>
std::ostream& operator<<(std::ostream& stream, DomSerializer<T> const& data)
{
    JsonSerializeVisitor    serializer(stream);
    data.value.accept(serializer);
    return stream;
}

template std::ostream& operator<<(std::ostream& stream, DomSerializer<Parser::ParserValue> const& data);
template std::ostream& operator<<(std::ostream& stream, DomSerializer<Parser::ParserMap> const& data);
template std::ostream& operator<<(std::ostream& stream, DomSerializer<Parser::ParserArray> const& data);

    }
}

