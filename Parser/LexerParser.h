
#ifndef THORSANVIL_PARSER_LEXER_PARSER_H
#define THORSANVIL_PARSER_LEXER_PARSER_H

namespace ThorsAnvil
{
    namespace Parser
    {

struct ParserInterface;
class LexerParser
{
    public:
        virtual ~LexerParser() {}
        virtual int yylex(ParserInterface& pi)   = 0;
        virtual std::string getToken() const     = 0;

};

    }
}

#endif

