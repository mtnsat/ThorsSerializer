
#ifndef THORSANVIL_PARSER_PARSER_UTIL_H
#define THORSANVIL_PARSER_PARSER_UTIL_H

#include "ParserDom.h"

namespace ThorsAnvil
{
    namespace Parser
    {

void mergeParserDom(ParserMap&   dst, ParserMap&   src, std::string const& errorMsg);
void mergeParserDom(ParserArray& dst, ParserArray& src, std::string const& errorMsg);

    }
}


#endif

