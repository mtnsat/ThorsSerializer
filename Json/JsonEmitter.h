
#ifndef THORSANVIL_JSON_JSON_EMITTER_H
#define THORSANVIL_JSON_JSON_EMITTER_H

#include "Parser/EmitterInterface.h"
#include "Parser/ParserInterface.h"
#include <ostream>
#include <vector>

namespace ThorsAnvil
{
    namespace Json
    {

class JsonEmitter: public Parser::EmitterInterface
{
    bool first;
    std::vector<std::pair<Parser::ParserObjectType, std::size_t>>   state;
    std::string const& seporator();
    public:
        JsonEmitter(std::ostream& stream);
        ~JsonEmitter();

        virtual void writeString(std::string const& value, Parser::Attributes const& attributes)override;
        virtual void writeNumber(std::string const& value, Parser::Attributes const& attributes)override;
        virtual void writeBool(std::string const& value, Parser::Attributes const& attributes)  override;
        virtual void writeNull(std::string const& value, Parser::Attributes const& attributes)  override;
        virtual void writeMapStart(Parser::Attributes const& attributes)                        override;
        virtual void writeMapEnd(Parser::Attributes const& attributes)                          override;
        virtual void writeArrayStart(Parser::Attributes const& attributes)                      override;
        virtual void writeArrayEnd(Parser::Attributes const& attributes)                        override;
        virtual void writeDocStart(Parser::Attributes const& attributes)                        override;
        virtual void writeDocEnd(Parser::Attributes const& attributes)                          override;

        virtual void writeNumber(long long value, Parser::Attributes const& attributes)         override;
        virtual void writeNumber(double value, Parser::Attributes const& attributes)            override;
        virtual void writeBool(bool value, Parser::Attributes const& attributes)                override;
};

    }
}

#endif

