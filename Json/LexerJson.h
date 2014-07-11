

#ifndef THORS_ANVIL_JSON_PARSER_LEXER_JSON_H
#define THORS_ANVIL_JSON_PARSER_LEXER_JSON_H


#ifndef IN_LEXER
#ifdef  yyFlexLexer
#undef  yyFlexLexer
#endif
#define yyFlexLexer JsonBaseFlexLexer
#include <FlexLexer.h>
#endif

#include "Parser/LexerParser.h"

namespace ThorsAnvil
{
    namespace Parser
    {
        struct ParserInterface;
    }
    namespace Json
    {
        
class LexerJson: public JsonBaseFlexLexer, public Parser::LexerParser
{
    public:
        LexerJson(std::istream& input);
        virtual int yylex(Parser::ParserInterface& pi);

        virtual void LexerError(const char* msg);
        virtual std::string getString()  const;
        virtual std::string getNumber() const;
};

    }
}

#endif

