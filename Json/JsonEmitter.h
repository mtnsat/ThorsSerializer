
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

        virtual void writeString(Parser::ParserStringItem const& stringItem)    override;
        virtual void writeNumber(Parser::ParserNumberItem const& numberItem)    override;
        virtual void writeBool(Parser::ParserBoolItem const& boolItem)          override;
        virtual void writeNull(Parser::ParserNULLItem const& nullItem)          override;
        virtual void writeMapStart(Parser::Attributes const& attributes)        override;
        virtual void writeMapEnd(Parser::Attributes const& attributes)          override;
        virtual void writeArrayStart(Parser::Attributes const& attributes)      override;
        virtual void writeArrayEnd(Parser::Attributes const& attributes)        override;
        virtual void writeDocStart(Parser::Attributes const& attributes)        override;
        virtual void writeDocEnd(Parser::Attributes const& attributes)          override;
};

    }
}

#endif

