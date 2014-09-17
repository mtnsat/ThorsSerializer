
#include "YamlEmitter.h"
#include <iostream>

using namespace ThorsAnvil::Yaml;

extern "C" int writeHandlerCB(void* ext, unsigned char* buffer, std::size_t size)
{
    //std::cerr << std::string(buffer, buffer+size);
    return reinterpret_cast<YamlEmitter*>(ext)->writeHandler(buffer, size);
}

YamlEmitter::YamlEmitter(std::ostream& stream)
    : stream(stream)
{
    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output(&emitter, writeHandlerCB, this);

    yaml_event_t    event;
    if (!yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING) || !yaml_emitter_emit(&emitter, &event))
    {
        yaml_emitter_delete(&emitter);
        throw std::runtime_error("YamlEmitter::YamlEmitter: Failed stream_start_event_initialize");
    }
    if (!yaml_document_start_event_initialize(&event, nullptr, nullptr, nullptr, 1) || !yaml_emitter_emit(&emitter, &event))
    {
        yaml_emitter_delete(&emitter);
        throw std::runtime_error("YamlEmitter::YamlEmitter: Failed document_start_event_initialize");
    }
}
YamlEmitter::~YamlEmitter()
{
    yaml_event_t    event;
    if (!yaml_document_end_event_initialize(&event, 1) || !yaml_emitter_emit(&emitter, &event))
    {   /* Error Do nothing in Destructor */
    }
    else
    {
        if (!yaml_stream_end_event_initialize(&event) || !yaml_emitter_emit(&emitter, &event))
        {   /* Error Do nothing in Destructor */
        }
    }
    yaml_emitter_delete(&emitter);
}

int YamlEmitter::writeHandler(unsigned char* buffer, std::size_t size)
{
    stream.write(reinterpret_cast<char*>(buffer), size);
    return stream.good();
}

#if 0
   /** Let the emitter choose the style. */
   YAML_ANY_SCALAR_STYLE,

   /** The plain scalar style. */
   YAML_PLAIN_SCALAR_STYLE,

   /** The single-quoted scalar style. */
   YAML_SINGLE_QUOTED_SCALAR_STYLE,

   /** The double-quoted scalar style. */
   YAML_DOUBLE_QUOTED_SCALAR_STYLE,

   /** The literal scalar style. */
   YAML_LITERAL_SCALAR_STYLE,

   /** The folded scalar style. */
   YAML_FOLDED_SCALAR_STYLE
#endif

yaml_char_t* convertStringToYamlCharPtr(std::string const& value)
{
    if (value == "")
    {   return nullptr;
    }
    return reinterpret_cast<yaml_char_t*>(const_cast<char*>(value.c_str()));
}
void YamlEmitter::writeString(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style)
{
    yaml_event_t    event;
    unsigned char*  data    = reinterpret_cast<unsigned char*>(const_cast<char*>(value.c_str()));
    if (!yaml_scalar_event_initialize(&event, convertStringToYamlCharPtr(anchor), convertStringToYamlCharPtr(tag), data, value.size(), plain_implicit,  quoted_implicit, static_cast<yaml_scalar_style_t>(style)) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeString: Failed scalar_event_initialize");
    }
}

void YamlEmitter::writeNumber(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style)
{
    yaml_event_t    event;
    unsigned char*  data    = reinterpret_cast<unsigned char*>(const_cast<char*>(value.c_str()));
    if (!yaml_scalar_event_initialize(&event, convertStringToYamlCharPtr(anchor), convertStringToYamlCharPtr(tag), data, value.size(), plain_implicit,  quoted_implicit, static_cast<yaml_scalar_style_t>(style)) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeNumber: Failed scalar_event_initialize");
    }
}

void YamlEmitter::writeBool(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style)
{
    yaml_event_t    event;
    unsigned char*  data    = reinterpret_cast<unsigned char*>(const_cast<char*>(value.c_str()));
    if (!yaml_scalar_event_initialize(&event, convertStringToYamlCharPtr(anchor), convertStringToYamlCharPtr(tag), data, value.size(), plain_implicit,  quoted_implicit, static_cast<yaml_scalar_style_t>(style)) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeBool: Failed scalar_event_initialize");
    }
}

void YamlEmitter::writeNull(std::string const& value, std::string const& anchor, std::string const& tag, int plain_implicit, int quoted_implicit, int style)
{
    yaml_event_t    event;
    unsigned char*  data    = reinterpret_cast<unsigned char*>(const_cast<char*>(value.c_str()));
    if (!yaml_scalar_event_initialize(&event, convertStringToYamlCharPtr(anchor), convertStringToYamlCharPtr(tag), data, value.size(), plain_implicit,  quoted_implicit, static_cast<yaml_scalar_style_t>(style)) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeNull: Failed scalar_event_initialize");
    }
}

#if 0
    YAML_ANY_MAPPING_STYLE  Let the emitter choose the style.
    YAML_BLOCK_MAPPING_STYLE    The block mapping style.
    YAML_FLOW_MAPPING_STYLE     The flow mapping style.
#endif
void YamlEmitter::writeMapStart()
{
    /* Note:
       Either Tag or implicit must be set
    */
    yaml_event_t    event;
    if (!yaml_mapping_start_event_initialize(&event, /*anchor*/nullptr, /*tag*/nullptr, /*implicit*/1, YAML_ANY_MAPPING_STYLE)   || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeMapStart: Failed scalar_event_initialize");
    }
}


void YamlEmitter::writeMapEnd()
{
    yaml_event_t    event;
    if (!yaml_mapping_end_event_initialize(&event) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeMapEnd: Failed mapping_end_event_initialize");
    }
}

#if 0
  /** Let the emitter choose the style. */
  YAML_ANY_SEQUENCE_STYLE,

  /** The block sequence style. */
  YAML_BLOCK_SEQUENCE_STYLE,

  /** The flow sequence style. */
  YAML_FLOW_SEQUENCE_STYLE
#endif

void YamlEmitter::writeArrayStart()
{
    /* Note:
       Either Tag or implicit must be set
    */
    yaml_event_t    event;
    if (!yaml_sequence_start_event_initialize(&event, /*anchor*/nullptr, /*tag*/nullptr, /*implicit*/1, YAML_ANY_SEQUENCE_STYLE) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeArrayStart: Failed sequence_start_event_initialize");
    }
}

void YamlEmitter::writeArrayEnd()
{
    yaml_event_t    event;
    if (!yaml_sequence_end_event_initialize(&event) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeArrayEnd: Failed sequence_end_event_initialize");
    }
}


