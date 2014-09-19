
#ifndef THORS_ANVIL_JSON_SCANNER_JSON_H
#define THORS_ANVIL_JSON_SCANNER_JSON_H

#include "Parser/ParserInterface.h"
#include "Parser/ScannerDom.h"
#include "Parser/ScannerSax.h"

namespace ThorsAnvil
{
    namespace Json
    {


struct JsonKeyGen: public Parser::KeyGenVisitor
{
    std::string     key;
    virtual std::string     getKey()                            { return key;}
    virtual void            visit(Parser::ParserStringItem const& item) { key = item.value;}

    virtual void visit(Parser::ParserNumberItem const&)        {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserNumberItem");}
    virtual void visit(Parser::ParserBoolItem const&)          {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserBoolItem");}
    virtual void visit(Parser::ParserNULLItem const&)          {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserNULLItem");}
    virtual void visit(Parser::ParserMapItem const&)           {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserMapItem");}
    virtual void visit(Parser::ParserArrayItem const&)         {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserArrayItem");}
    virtual void visit(Parser::ParserMap const&, Parser::Storage const&, Parser::Storage const&) {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserMap");}
    virtual void visit(Parser::ParserArray const&)             {throw std::runtime_error("JsonKeyGen: Invalid Key: ParserArray");}
};

class JsonScannerDom: public ThorsAnvil::Parser::ScannerDom<JsonKeyGen>
{
    public:
        Parser::ParserObjectType parse(std::istream& stream);
};
class JsonScannerSax: public ThorsAnvil::Parser::ScannerSax<JsonKeyGen>
{
    public:
        void parse(std::istream& stream);
};

class JsonScannerAltDom: public ThorsAnvil::Parser::ScannerDom<JsonKeyGen>
{
    public:
        Parser::ParserObjectType parse(std::istream& stream);
};
class JsonScannerAltSax: public ThorsAnvil::Parser::ScannerSax<JsonKeyGen>
{
    public:
        void parse(std::istream& stream);
};

    }
}

#endif

