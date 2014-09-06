
#ifndef THORSANVIL_YAML_YAML_EMITTER_H
#define THORSANVIL_YAML_YAML_EMITTER_H

#include <ostream>
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

        void writeString(std::string const& value);
        void writeNumber(std::string const& value);
        void writeBool(std::string const& value);
        void writeNull(std::string const& value);
        void writeMapStart();
        void writeMapEnd();
        void writeArrayStart();
        void writeArrayEnd();
};

    }
}

#endif

