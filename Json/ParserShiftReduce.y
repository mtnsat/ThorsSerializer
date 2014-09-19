
%skeleton "lalr1.cc"
%require "2.1a"
%defines
%define "parser_class_name" "ParserShiftReduce"

%{

#include "Parser/ParserInterface.h"
#include "LexerJson.h"
#include <stdexcept>
#include <sstream>

using ThorsAnvil::Parser::ParserInterface;
using ThorsAnvil::Parser::ParserValue;

using ThorsAnvil::Parser::ParserStringItem;
using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;
using ThorsAnvil::Parser::ParserMapValue;
using ThorsAnvil::Parser::ParserMapItem;
using ThorsAnvil::Parser::ParserArrayItem;

using ThorsAnvil::Json::LexerJson;

namespace ThorsAnvil
{
    namespace Json
    {
int yylex(void*, LexerJson& lexer, ParserInterface& pi);
    }
}
%}

%parse-param {LexerJson&            lexer}
%parse-param {ParserInterface&      pi}
%lex-param   {LexerJson&            lexer}
%lex-param   {ParserInterface&      pi}

%union {
    ParserMap*                          jsonMap;
    ParserArray*                        jsonArray;
    ParserMapValue*                     jsonMapValue;
    ParserValue*                        jsonValue;
    std::string*                        jsonToken;
} 

%token  JSON_STRING
%token  JSON_NUMBER
%token  JSON_TRUE
%token  JSON_FALSE
%token  JSON_NULL

%type   <jsonValue>     JsonValue
%type   <jsonToken>     JsonString
%type   <jsonToken>     JsonNumber
%type   <jsonToken>     JsonTrue
%type   <jsonToken>     JsonFalse
%type   <jsonToken>     JsonNull

%type   <jsonMap>       JsonMap
%type   <jsonMap>       JsonMapValueListOpt
%type   <jsonMap>       JsonMapValueList
%type   <jsonMapValue>  JsonMapValue
%type   <jsonValue>     JsonMapKey
%type   <jsonValue>     JsonMapKeyValue

%type   <jsonArray>     JsonArray
%type   <jsonArray>     JsonArrayValueListOpt
%type   <jsonArray>     JsonArrayValueList
%type   <jsonArray>     JsonArrayListItem
%type   <jsonValue>     JsonArrayValue

%%

JsonData                :   JsonObject                              {return 0;}

JsonObject              :   JsonMap                                 {std::unique_ptr<ParserMap>     map($1);pi.done(ThorsAnvil::Parser::ParserMapObject,   new ParserMapItem(map));}
                        |   JsonArray                               {std::unique_ptr<ParserArray>   arr($1);pi.done(ThorsAnvil::Parser::ParserArrayObject, new ParserArrayItem(arr));}

JsonMap                 :   '{'                                     {pi.mapOpen();}
                            JsonMapValueListOpt
                            '}'                                     {pi.mapClose();$$=$3;}
JsonMapValueListOpt     :                                           {$$ = pi.mapCreate();}
                        |   JsonMapValueList                        {$$ = $1;}
JsonMapValueList        :   JsonMapValue                            {$$ = pi.mapCreate($1);}
                        |   JsonMapValueList ',' JsonMapValue       {$$ = pi.mapAppend($1, $3);}
JsonMapValue            :   JsonMapKey JsonValue                    {$$ = pi.mapCreateElement($1, $2);}
JsonMapKey              :   JsonMapKeyValue ':'                     {$$ = pi.mapKeyNote($1);}
JsonMapKeyValue         :   JsonString                              {std::unique_ptr<std::string> keyVal($1);$$ = new ParserStringItem(keyVal);}


JsonArray               :   '['                                     {pi.arrayOpen();}
                            JsonArrayValueListOpt
                            ']'                                     {pi.arrayClose();$$=$3;}
JsonArrayValueListOpt   :                                           {$$ = pi.arrayCreate();}
                        |   JsonArrayValueList                      {$$ = $1;}
JsonArrayValueList      :   JsonArrayValue                          {$$ = pi.arrayCreate($1);}
                        |   JsonArrayListItem JsonArrayValue        {$$ = pi.arrayAppend($1, $2);}
JsonArrayListItem       :   JsonArrayValueList ','                  {$$ = pi.arrayNote($1);}
JsonArrayValue          :   JsonValue                               {$$ = pi.arrayCreateElement($1);}

JsonValue               :   JsonMap                                 {$$ = pi.valueParseMap($1);}
                        |   JsonArray                               {$$ = pi.valueParseArray($1);}
                        |   JsonString                              {$$ = pi.valueParseString($1);}
                        |   JsonNumber                              {$$ = pi.valueParseNumber($1);}
                        |   JsonTrue                                {$$ = pi.valueParseBool($1, true);}
                        |   JsonFalse                               {$$ = pi.valueParseBool($1, false);}
                        |   JsonNull                                {$$ = pi.valueParseNULL($1);}

JsonString              :   JSON_STRING                             {$$ = new std::string(lexer.getToken());}
JsonNumber              :   JSON_NUMBER                             {$$ = new std::string(lexer.getToken());}
JsonTrue                :   JSON_TRUE                               {$$ = new std::string(lexer.getToken());}
JsonFalse               :   JSON_FALSE                              {$$ = new std::string(lexer.getToken());}
JsonNull                :   JSON_NULL                               {$$ = new std::string(lexer.getToken());}


%%

int ThorsAnvil::Json::yylex(void*, LexerJson& lexer, ParserInterface& pi)
{
    return lexer.yylex(pi);
}

void yy::ParserShiftReduce::error(yy::location const&, std::string const& msg)
{
    std::string         lastToken(lexer.YYText(), lexer.YYText() + lexer.YYLeng());
    std::stringstream  extended;
    extended << msg << " -> Last Token: " << lastToken << " At line: " << lexer.lineno();

    throw ThorsAnvil::Parser::ParsingError(extended.str());
}

