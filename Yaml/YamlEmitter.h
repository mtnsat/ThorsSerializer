
#ifndef THORSANVIL_YAML_YAML_EMITTER_H
#define THORSANVIL_YAML_YAML_EMITTER_H

#include "Parser/EmitterInterface.h"
#include <ostream>
#include <vector>
#include <yaml.h>

namespace ThorsAnvil
{
    namespace Yaml
    {

extern "C" int writeHandlerCB(void* ext, unsigned char* buffer, std::size_t size);

class YamlEmitter: public Parser::EmitterInterface
{
    //std::ostream&   stream;
    yaml_emitter_t  emitter;

    std::string getAttribute(Parser::Attributes const& attributes, std::string const& name, std::string const& d) const;
    template<typename T>
    bool    writeCompund(int (*)(yaml_event_t*, yaml_char_t*, yaml_char_t*, int, T), Parser::Attributes const& attributes);
    bool    writeScalar(Parser::ParserValue const& item);
    int     writeHandler(unsigned char* buffer, std::size_t size);

    friend int writeHandlerCB(void* ext, unsigned char* buffer, std::size_t size);

    public:
        YamlEmitter(std::ostream& stream);
        ~YamlEmitter();

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

