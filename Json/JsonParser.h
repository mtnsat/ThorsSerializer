
#ifndef THORSANVIL_JSON_JSON_PARSER_H
#define THORSANVIL_JSON_JSON_PARSER_H

#include "LexerJson.h"
#include "ParserShiftReduce.h"
#include "ParserRecursive.h"

namespace ThorsAnvil
{
    namespace Parser
    {
        struct ParserInterface;
    }
    namespace Json
    {

template<typename P>
struct JsonParserTemp
{
    std::istream&               str;
    Parser::ParserInterface&    pi;

    JsonParserTemp(std::istream& str, Parser::ParserInterface& pi)
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

struct JsonParser: public JsonParserTemp<yy::ParserShiftReduce>
{
    JsonParser(std::istream& str, Parser::ParserInterface& pi)
        : JsonParserTemp(str, pi)
    {}
};

struct JsonParserAlt: public JsonParserTemp<ParserRecursive>
{
    JsonParserAlt(std::istream& str, Parser::ParserInterface& pi)
        : JsonParserTemp(str, pi)
    {}
};

    }
}

#endif

