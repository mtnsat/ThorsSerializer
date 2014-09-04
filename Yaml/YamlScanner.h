
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
    virtual void            visit(Parser::ParserStringItem const& item) { key = *item.value;}

    virtual void visit(Parser::ParserNumberItem const&)        {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserNumberItem");}
    virtual void visit(Parser::ParserBoolItem const&)          {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserBoolItem");}
    virtual void visit(Parser::ParserNULLItem const&)          {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserNULLItem");}
    virtual void visit(Parser::ParserMapItem const&)           {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserMapItem");}
    virtual void visit(Parser::ParserArrayItem const&)         {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserArrayItem");}
    virtual void visit(Parser::ParserMap const&, Parser::Storage const&, Parser::Storage const&) {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserMap");}
    virtual void visit(Parser::ParserArray const&)             {throw std::runtime_error("YamlKeyGen: Invalid Key: ParserArray");}
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

