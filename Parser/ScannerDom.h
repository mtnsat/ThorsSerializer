
#ifndef THORSANVIL_PARSER_PARSER_SCANNER_DOM_H
#define THORSANVIL_PARSER_PARSER_SCANNER_DOM_H

#include "ParserInterface.h"


namespace ThorsAnvil
{
    namespace Parser
    {

class ScannerDom
{
    std::unique_ptr<ParserMap>      map;
    std::unique_ptr<ParserArray>    array;
    ParserObjectType                objectType;

    public:
    template<typename Parser>
    ParserObjectType parse(std::istream& stream);
    std::unique_ptr<ParserMap>&     getMap()       { return map;}
    std::unique_ptr<ParserArray>&   getArray()     { return array;}
};

class ScannerDomInterface: public ParserDomInterface
{
    ParserObjectType&               objectTypeRef;
    std::unique_ptr<ParserMap>&     mapRef;
    std::unique_ptr<ParserArray>&   arrayRef;
    public:
    ScannerDomInterface(ParserObjectType& objectType, std::unique_ptr<ParserMap>&  map, std::unique_ptr<ParserArray>& array)
        : objectTypeRef(objectType)
        , mapRef(map)
        , arrayRef(array)
    {}

    virtual void            doneMap(ParserMap* map)           { mapRef.reset(map);    objectTypeRef = ParserMapObject; }
    virtual void            doneAray(ParserArray* array)      { arrayRef.reset(array);objectTypeRef = ParserArrayObject; }
};

template<typename Parser>
ParserObjectType ScannerDom::parse(std::istream& stream)
{
    map.release();
    array.release();

    ScannerDomInterface     scanner(objectType, map, array);
    Parser                  parser(stream, scanner);

    // If this fails it throws.
    parser.parse();

    return objectType;
}

    }
}

#endif

