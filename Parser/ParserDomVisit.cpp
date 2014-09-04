
#include "ParserDomVisit.h"
#include "ParserDom.h"

using namespace ThorsAnvil::Parser;

ParserValueJsonPrinter::ParserValueJsonPrinter(std::ostream& stream)
    : stream(stream)
{}

void ParserValueJsonPrinter::visit(ParserStringItem const& item) 
{
    std::string escapedString;
    std::for_each(std::begin(*item.value), std::end(*item.value), [&escapedString](char next)
    {
        switch(next)
        {
            case '"':   escapedString   += "\\\"";          break;
            case '\\':  escapedString   += "\\\\";          break;
            default:    escapedString   += next;            break;
       }
    });
    stream << '"' << escapedString << '"';
}

void ParserValueJsonPrinter::visit(ParserNumberItem const& item)
{
    stream << *item.value;
}

void ParserValueJsonPrinter::visit(ParserBoolItem const& item)
{
    stream << std::boolalpha << item.value;
}

void ParserValueJsonPrinter::visit(ParserNULLItem const&)
{
    stream << "null";
}

void ParserValueJsonPrinter::visit(ParserMapItem const& item)
{
    item.value->accept(*this);
}

void ParserValueJsonPrinter::visit(ParserArrayItem const& item)
{
    item.value->accept(*this);
}

void ParserValueJsonPrinter::visit(ParserMap const& node, Storage const& mapData, Storage const& keyData)
{
    stream << "{";
    if (!node.empty())
    {
        ParserMap::const_iterator loopK = keyData.begin();
        ParserMap::const_iterator loopV = mapData.begin();

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

void ParserValueJsonPrinter::visit(ParserArray const& node)      
{
    stream << "[";
    if (!node.empty())
    {
        ParserArray::const_iterator loop = node.begin();
        loop->accept(*this);

        for(++loop; loop != node.end(); ++loop)
        {
            stream << ", ";
            loop->accept(*this);
        }
    }
    stream << "]";
}

