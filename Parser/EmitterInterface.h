
#ifndef THORSANVIL_PARSER_EMITTER_INTERFACE
#define THORSANVIL_PARSER_EMITTER_INTERFACE

#include "ParserCommon.h"
#include "ParserDom.h"

namespace ThorsAnvil
{
    namespace Parser
    {
        struct EmitterInterface
        {
            protected:
                std::ostream&   stream;

            public:
                EmitterInterface(std::ostream& stream)
                    : stream(stream)
                {}
                virtual ~EmitterInterface()
                {}

                template<typename T>
                EmitterInterface& operator<<(T const& val)    {stream << val;return *this;}

                virtual void writeString(ParserStringItem const& stringItem)        = 0;
                virtual void writeNumber(ParserNumberItem const& numberItem)        = 0;
                virtual void writeBool(ParserBoolItem const& boolItem)              = 0;
                virtual void writeNull(ParserNULLItem const& nullItem)              = 0;
                virtual void writeMapStart(Parser::Attributes const& attributes)    = 0;
                virtual void writeMapEnd(Parser::Attributes const& attributes)      = 0;
                virtual void writeArrayStart(Parser::Attributes const& attributes)  = 0;
                virtual void writeArrayEnd(Parser::Attributes const& attributes)    = 0;
                virtual void writeDocStart(Parser::Attributes const& attributes)    = 0;
                virtual void writeDocEnd(Parser::Attributes const& attributes)      = 0;
        };
    }
}

#endif

