
#ifndef THORS_ANVIL_YAML_SCANNER_YAML_H
#define THORS_ANVIL_YAML_SCANNER_YAML_H

#include "Parser/ParserInterface.h"
#include "Parser/ScannerDom.h"
#include "Parser/ScannerSax.h"

namespace ThorsAnvil
{
    namespace Yaml
    {


struct YamlKeyGen: public Parser::KeyGenVisitor
{
    std::string     key;
    virtual std::string     getKey()                            { return key;}
    virtual void            reset()                             { key.clear();}

    virtual void visit(Parser::ParserStringItem const& item)
    {
        std::for_each(std::begin(item.value), std::end(item.value), [this](char next)
        {
            switch(next)
            {
                case '"':   key   += "\\\"";          break;
                case '\\':  key   += "\\\\";          break;
                default:    key   += next;            break;
           }
        });
    }
    virtual void visit(Parser::ParserNumberItem const& item)    { key += item.value;}
    virtual void visit(Parser::ParserBoolItem const& item)      { key += item.value;}
    virtual void visit(Parser::ParserNULLItem const& item)      { key += item.value;}
    virtual void visit(Parser::ParserMapItem const& item)       { item.value->accept(*this);}
    virtual void visit(Parser::ParserArrayItem const& item)     { item.value->accept(*this);}
    virtual void visit(Parser::ParserArray const& node)
    {
        key += "[";
        if (!node.empty())
        {
            Parser::ParserArray::const_iterator loop = node.begin();
            loop->accept(*this);

            for(++loop; loop != node.end(); ++loop)
            {
                key += ", ";
                loop->accept(*this);
            }
        }
        key += "]";
    }
    virtual void visit(Parser::ParserMap const& node, Parser::Storage const& keyData, Parser::Storage const& mapData)
    {
        key += '{';
        if (!node.empty())
        {
            Parser::ParserMap::const_iterator loopK = keyData.begin();
            Parser::ParserMap::const_iterator loopV = mapData.begin();

            loopK->second->accept(*this);
            key += ": ";
            loopV->second->accept(*this);

            for(++loopK, ++loopV; loopV != mapData.end(); ++loopK, ++loopV)
            {
                key += ", ";
                loopK->second->accept(*this);
                key += ": ";
                loopV->second->accept(*this);
            }
        }
        key += "}";
    }
};

class YamlScannerDom: public ThorsAnvil::Parser::ScannerDom<YamlKeyGen>
{
    public:
        Parser::ParserObjectType parse(std::istream& stream);
};
class YamlScannerSax: public ThorsAnvil::Parser::ScannerSax<YamlKeyGen>
{
    public:
        void parse(std::istream& stream);
};

    }
}

#endif

