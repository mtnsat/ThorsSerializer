
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
    ParserObjectType                type;
    std::unique_ptr<ParserValue>    result;

    void validate(ParserObjectType valueType)
    {
        if (result.get() == nullptr)
        {   throw std::runtime_error("Invalid Type: No result set");
        }
        if (type != valueType)
        {   throw std::runtime_error("Invalid Type: Different result set by parser");
        }
    }

    public:
    template<typename Parser>
    ParserObjectType parse(std::istream& stream);
    ParserMap&     getMap()       { validate(ParserMapObject);  return *(static_cast<ParserMapItem&>(*result).value);}
    ParserArray&   getArray()     { validate(ParserArrayObject);return *(static_cast<ParserArrayItem&>(*result).value);}
    ParserValue&   getValue()     { validate(ParserValueObject);return *result;}
};

class ScannerDomInterface: public ParserDomInterface
{
    ParserObjectType&               type;
    std::unique_ptr<ParserValue>&   resultRef;
    public:
    ScannerDomInterface(ParserObjectType& type, std::unique_ptr<ParserValue>&  result)
        : type(type)
        , resultRef(result)
    {}

    virtual void            done(ParserObjectType valueType, ParserValue* result)        { type = valueType; resultRef.reset(result);}
};

template<typename Parser>
ParserObjectType ScannerDom::parse(std::istream& stream)
{
    result.release();

    ScannerDomInterface     scanner(type, result);
    Parser                  parser(stream, scanner);

    // If this fails it throws.
    parser.parse();

    return type;
}

    }
}

#endif

