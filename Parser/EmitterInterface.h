
#ifndef THORSANVIL_PARSER_EMITTER_INTERFACE
#define THORSANVIL_PARSER_EMITTER_INTERFACE

#include "ParserCommon.h"

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

                virtual void writeString(std::string const& value, Parser::Attributes const& attributes)    = 0;

                virtual void writeNumber(std::string const& value, Parser::Attributes const& attributes)    = 0;
                virtual void writeNumber(long long value, Parser::Attributes const& attributes)             {this->writeNumber(std::to_string(value), attributes);}
                virtual void writeNumber(double value, Parser::Attributes const& attributes)                {this->writeNumber(std::to_string(value), attributes);}

                virtual void writeBool(std::string const& value, Parser::Attributes const& attributes)      = 0;
                virtual void writeBool(bool value, Parser::Attributes const& attributes)                    {this->writeBool(value?"true":"false", attributes);}

                virtual void writeNull(std::string const& value, Parser::Attributes const& attributes)      = 0;
                virtual void writeNull(Parser::Attributes const& attributes)                                {this->writeNull("null", attributes);}

                virtual void writeMapStart(Parser::Attributes const& attributes)                            = 0;
                virtual void writeMapEnd(Parser::Attributes const& attributes)                              = 0;
                virtual void writeArrayStart(Parser::Attributes const& attributes)                          = 0;
                virtual void writeArrayEnd(Parser::Attributes const& attributes)                            = 0;
                virtual void writeDocStart(Parser::Attributes const& attributes)                            = 0;
                virtual void writeDocEnd(Parser::Attributes const& attributes)                              = 0;
        };
    }
}

#endif

