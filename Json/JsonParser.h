
#ifndef THORSANVIL_JSON_JSON_PARSER_H
#define THORSANVIL_JSON_JSON_PARSER_H

#include "LexerJson.h"

namespace ThorsAnvil
{
    namespace Parser
    {
        struct ParserInterface;
    }
    namespace Json
    {

template<typename P>
struct JsonParser
{
    std::istream&               str;
    Parser::ParserInterface&    pi;

    JsonParser(std::istream& str, Parser::ParserInterface& pi)
        : str(str)
        , pi(pi)
    {}
    int parse()
    {
        LexerJson       lexer(str);
        P               parser(lexer, pi);

        return parser.parse();
    }
};

    }
}

#endif

