
#ifndef THORSANVIL_PARSER_PARSER_SCANNER_DOM_H
#define THORSANVIL_PARSER_PARSER_SCANNER_DOM_H

#include "ParserInterface.h"
#include "ParserDom.h"


namespace ThorsAnvil
{
    namespace Parser
    {

class ScannerDom
{
    std::unique_ptr<ParserObject>   result;

    public:
    template<typename Parser>
    ParserObjectType parse(std::istream& stream);
    ParserMap&     getMap()       { return *result->data.map;}
    ParserArray&   getArray()     { return *result->data.array;}
    ParserValue&   getValue()     { return *result->data.value;}
};

class ScannerDomInterface: public ParserDomInterface
{
    std::unique_ptr<ParserObject>&  resultRef;
    public:
    ScannerDomInterface(std::unique_ptr<ParserObject>&  result)
        : resultRef(result)
    {}

    virtual void            done(ParserObject* result)        { resultRef.reset(result);}
};

template<typename Parser>
ParserObjectType ScannerDom::parse(std::istream& stream)
{
    result.release();

    ScannerDomInterface     scanner(result);
    Parser                  parser(stream, scanner);

    // If this fails it throws.
    parser.parse();

    return result->type;
}

    }
}

#endif

