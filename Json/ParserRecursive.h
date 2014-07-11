
#ifndef THORSANVIL_jSON_PARSER_PARSE_RECURSIVE_H
#define THORSANVIL_jSON_PARSER_PARSE_RECURSIVE_H

#include "LexerJson.h"
#include "Parser/ParserInterface.h"


namespace ThorsAnvil
{
    namespace Json
    {

class LexerJson;

class ParserRecursive
{
        LexerJson&                  lexer;
        Parser::ParserInterface&    pi;

        int JsonValueParse(int val, std::unique_ptr<Parser::ParserValue>& value);
        int JsonMapValueListParse(int val, std::unique_ptr<Parser::ParserMap>& ma);
        int JsonArrayValueListParse(int val, std::unique_ptr<Parser::ParserArray>& array);
        int JsonMapParse(int val, std::unique_ptr<Parser::ParserMap>& map);
        int JsonArrayParse(int val, std::unique_ptr<Parser::ParserArray>& array);
        int parseJosnObject(int val);

        int yylex() { return lexer.yylex(pi);}
        int error(int val, std::string const& msg);

    public:
        ParserRecursive(LexerJson& l, Parser::ParserInterface& p)
            : lexer(l)
            , pi(p)
        {}
        int parse();
};
    }
}

#endif

