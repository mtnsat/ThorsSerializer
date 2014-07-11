
#ifndef  THORSANVIL_JSON_PARSER_SHIFT_REDUCE_H
#define  THORSANVIL_JSON_PARSER_SHIFT_REDUCE_H

#include "Parser/ParserInterface.h"
#include "LexerJson.h"

using ThorsAnvil::Parser::ParserInterface;
using ThorsAnvil::Parser::ParserValue;

using ThorsAnvil::Parser::ParserMap;
using ThorsAnvil::Parser::ParserArray;
using ThorsAnvil::Parser::ParserMapValue;

using ThorsAnvil::Json::LexerJson;

#if (__GNUG__ > 4) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic push
#endif

#pragma GCC diagnostic ignored "-Wparentheses"


#include "ParserShiftReduce.tab.hpp"


#if (__GNUG__ > 4) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#else
#pragma GCC diagnostic error "-Wformat"
#endif

#endif

