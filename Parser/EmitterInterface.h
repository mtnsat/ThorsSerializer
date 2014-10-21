
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

                virtual void writeString(std::string const& stringValue, Attributes const& attributes)                  = 0;
                virtual void writeNumber(std::string const& stringValue, double value, Attributes const& attributes)    = 0;
                virtual void writeNumber(std::string const& stringValue, long value, Attributes const& attributes)      = 0;
                virtual void writeBool(std::string const& stringValue, bool value, Attributes const& attributes)        = 0;
                virtual void writeNull(std::string const& stringValue, Attributes const& attributes)                    = 0;

                virtual void writeMapStart(Attributes const& attributes)    = 0;
                virtual void writeMapEnd(Attributes const& attributes)      = 0;
                virtual void writeArrayStart(Attributes const& attributes)  = 0;
                virtual void writeArrayEnd(Attributes const& attributes)    = 0;
                virtual void writeDocStart(Attributes const& attributes)    = 0;
                virtual void writeDocEnd(Attributes const& attributes)      = 0;
        };
    }
}

#endif

