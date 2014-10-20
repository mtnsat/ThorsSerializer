#ifndef THORSANVIL_PARSER_PARSER_SCANNER_DOM_H
#define THORSANVIL_PARSER_PARSER_SCANNER_DOM_H

#include "ParserInterface.h"
#include "ParserDom.h"


namespace ThorsAnvil
{
    namespace Parser
    {

template<typename KeyGenerator>
class ScannerDom
{
    ParserObjectType                type;
    std::unique_ptr<ParserValue>    result;


    public:
    template<typename Parser>
    ParserObjectType parse(std::istream& stream);
    ParserValue&   getValue()     { return *result;}
};

class ScannerDomInterface: public ParserDomInterface
{
    ParserObjectType&               type;
    std::unique_ptr<ParserValue>&   resultRef;
    public:
    ScannerDomInterface(KeyGenVisitor& keyGenVisitor, ParserObjectType& type, std::unique_ptr<ParserValue>&  result)
        : ParserDomInterface(keyGenVisitor)
        , type(type)
        , resultRef(result)
    {}

    virtual void            done(ParserObjectType valueType, ParserValue* result)        { type = valueType; resultRef.reset(result);}
};

template<typename KeyGenerator>
template<typename Parser>
ParserObjectType ScannerDom<KeyGenerator>::parse(std::istream& stream)
{
    result.release();

    KeyGenerator            keyGenerator;
    ScannerDomInterface     scanner(keyGenerator, type, result);
    Parser                  parser(stream, scanner);

    // If this fails it throws.
    parser.parse();

    return type;
}

    }
}

#endif

