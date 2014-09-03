
#include "LexerJson.h"
#include "Parser/ParserInterface.h"

using namespace ThorsAnvil::Json;

LexerJson::LexerJson(std::istream& input)
    : JsonBaseFlexLexer(&input, &std::cerr)
{}

int LexerJson::yylex(Parser::ParserInterface&)
{
    return JsonBaseFlexLexer::yylex();
}

void LexerJson::LexerError(const char* msg)
{
    throw Parser::ParsingError(msg);
}

std::string ThorsAnvil::Json::LexerJson::getToken() const
{
    char const*     begin   = YYText();
    char const*     end     = begin + YYLeng();

    std::string     result;
    if ((end - begin >= 2) && (*begin == '"'))
    {
        ++begin;
        --end;
    }
    // Use begin+1 and end-1 to remove the quotes
    std::copy(begin, end, Parser::make_TransformParserStringIter(result));

    return result;
}


