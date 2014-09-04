
#ifndef THORS_ANVIL_PARSER_PARSER_COMMONG_H
#define THORS_ANVIL_PARSER_PARSER_COMMONG_H

#include <boost/ptr_container/ptr_map.hpp>

namespace ThorsAnvil
{
    namespace Parser
    {

struct ParserValue;
typedef boost::ptr_map<std::string, ParserValue> Storage; 


    }
}

#endif

