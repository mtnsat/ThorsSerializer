
#ifndef THORSANVIL_YAML_YAML_EMITTER_H
#define THORSANVIL_YAML_YAML_EMITTER_H

#include <ostream>
#include <vector>
#include <yaml.h>

namespace ThorsAnvil
{
    namespace Yaml
    {

extern "C" int writeHandlerCB(void* ext, unsigned char* buffer, std::size_t size);
class YamlEmitter
{
    std::ostream&   stream;
    yaml_emitter_t  emitter;

    int writeHandler(unsigned char* buffer, std::size_t size);
    friend int writeHandlerCB(void* ext, unsigned char* buffer, std::size_t size);

    public:
        YamlEmitter(std::ostream& stream);
        ~YamlEmitter();

        void writeString(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style);
        void writeNumber(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style);
        void writeBool(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style);
        void writeNull(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style);
        void writeMapStart(std::string const& anchor, std::string const& tag, int implicit, int style);
        void writeMapEnd();
        void writeArrayStart(std::string const& anchor, std::string const& tag, int implicit, int style);
        void writeArrayEnd();
        void writeDocStart(std::string const& version, std::vector<yaml_tag_directive_t> const& directives, std::string const& implicit);
        void writeDocEnd(std::string const& implicit);
};

    }
}

#endif

