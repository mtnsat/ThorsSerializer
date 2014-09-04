
#include "YamlScanner.h"
#include "YamlParser.h"

using namespace ThorsAnvil::Yaml;

ThorsAnvil::Parser::ParserObjectType YamlScannerDom::parse(std::istream& stream)
{
    return ThorsAnvil::Parser::ScannerDom<YamlKeyGen>::parse<YamlParser>(stream);
}

void YamlScannerSax::parse(std::istream& stream)
{
    ThorsAnvil::Parser::ScannerSax<YamlKeyGen>::parse<YamlParser>(stream);
}

