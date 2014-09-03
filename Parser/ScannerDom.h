
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

    void validate(ParserObjectType type)
    {
        if (result.get() == nullptr)
        {   throw std::runtime_error("Invalid Type: No result set");
        }
        if (result->type != type)
        {   throw std::runtime_error("Invalid Type: Different result set by parser");
        }
    }

    public:
    template<typename Parser>
    ParserObjectType parse(std::istream& stream);
    ParserMap&     getMap()       { validate(ParserMapObject);  return *result->data.map;}
    ParserArray&   getArray()     { validate(ParserArrayObject);return *result->data.array;}
    ParserValue&   getValue()     { validate(ParserValueObject);return *result->data.value;}
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

