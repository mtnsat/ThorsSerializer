
#include "LexerJson.h"
#include "Parser/ParserInterface.h"

using namespace ThorsAnvil::Json;

LexerJson::LexerJson(std::istream& input)
    : JsonBaseFlexLexer(&input, &std::cerr)
{}

int LexerJson::yylex(Parser::ParserInterface& pi)
{
    return pi.lexResult(JsonBaseFlexLexer::yylex());
}

void LexerJson::LexerError(const char* msg)
{
    throw Parser::ParsingError(msg);
}

std::string ThorsAnvil::Json::LexerJson::getString() const
{
    char const*     begin   = YYText();
    char const*     end     = begin + YYLeng();

    std::string     result;
    // Use begin+1 and end-1 to remove the quotes
    std::copy(begin+1, end-1, Parser::make_TransformParserStringIter(result));

    return result;
}
std::string ThorsAnvil::Json::LexerJson::getNumber() const
{
    return std::string(YYText(), YYLeng());
}


