
#include "JsonScanner.h"
#include "JsonParser.h"

using namespace ThorsAnvil::Json;

ThorsAnvil::Parser::ParserObjectType JsonScannerDom::parse(std::istream& stream)
{
    return ThorsAnvil::Parser::ScannerDom<JsonKeyGen>::parse<JsonParser>(stream);
}

void JsonScannerSax::parse(std::istream& stream)
{
    ThorsAnvil::Parser::ScannerSax<JsonKeyGen>::parse<JsonParser>(stream);
}

ThorsAnvil::Parser::ParserObjectType JsonScannerAltDom::parse(std::istream& stream)
{
    return ThorsAnvil::Parser::ScannerDom<JsonKeyGen>::parse<JsonParserAlt>(stream);
}

void JsonScannerAltSax::parse(std::istream& stream)
{
    ThorsAnvil::Parser::ScannerSax<JsonKeyGen>::parse<JsonParserAlt>(stream);
}


