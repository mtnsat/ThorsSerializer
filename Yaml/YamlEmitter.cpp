
#include "YamlEmitter.h"
#include "YamlUtil.h"
#include <iostream>

using namespace ThorsAnvil::Yaml;

extern "C" int writeHandlerCB(void* ext, unsigned char* buffer, std::size_t size)
{
    //std::cerr << std::string(buffer, buffer+size);
    return reinterpret_cast<YamlEmitter*>(ext)->writeHandler(buffer, size);
}

YamlEmitter::YamlEmitter(std::ostream& stream)
    : EmitterInterface(stream)
{
    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output(&emitter, writeHandlerCB, this);

    yaml_event_t    event;
    if (!yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING) || !yaml_emitter_emit(&emitter, &event))
    {
        yaml_emitter_delete(&emitter);
        throw std::runtime_error("YamlEmitter::YamlEmitter: Failed stream_start_event_initialize");
    }
}
YamlEmitter::~YamlEmitter()
{
    yaml_event_t    event;
    if (!yaml_stream_end_event_initialize(&event) || !yaml_emitter_emit(&emitter, &event))
    {   /* Error Do nothing in Destructor */
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

std::string YamlEmitter::getAttribute(Parser::Attributes const& attributes, std::string const& name, std::string const& d) const
{
    auto find = attributes.find(name);
    if (find == attributes.end())
    {   return d;
    }
    return find->second;
}

bool YamlEmitter::writeScalar(Parser::ParserValue const& item)
{
    std::string         const&  value       = item.value;
    Parser::Attributes  const&  attributes  = item.getAttributes();

    yaml_char_t*  data            = convertStringToYamlCharPtr(value, UseEmptyString);
    yaml_char_t*  anchor          = convertStringToYamlCharPtr(getAttribute(attributes, "anchor", ""));
    yaml_char_t*  tag             = convertStringToYamlCharPtr(getAttribute(attributes, "tag", ""));
    int           plain_implicit  = std::stoi(getAttribute(attributes, "plain_implicit",  "0"));
    int           quoted_implicit = std::stoi(getAttribute(attributes, "quoted_implicit", "0"));
    int           style           = std::stoi(getAttribute(attributes, "style",           "0"));

    yaml_event_t    event;
    return (yaml_scalar_event_initialize(&event, anchor, tag, data, value.size(), plain_implicit,  quoted_implicit, static_cast<yaml_scalar_style_t>(style)) != 0)
        && (yaml_emitter_emit(&emitter, &event) != 0);
}


void YamlEmitter::writeString(Parser::ParserStringItem const& stringItem)
{
    if (!writeScalar(stringItem))
    {
        throw std::runtime_error("YamlEmitter::writeString: Failed scalar_event_initialize");
    }
}

void YamlEmitter::writeNumber(Parser::ParserNumberItem const& numberItem)
{
    if (!writeScalar(numberItem))
    {
        throw std::runtime_error("YamlEmitter::writeNumber: Failed scalar_event_initialize");
    }
}

void YamlEmitter::writeBool(Parser::ParserBoolItem const& boolItem)
{
    if (!writeScalar(boolItem))
    {
        throw std::runtime_error("YamlEmitter::writeBool: Failed scalar_event_initialize");
    }
}

void YamlEmitter::writeNull(Parser::ParserNULLItem const& nullItem)
{
    if (!writeScalar(nullItem))
    {
        throw std::runtime_error("YamlEmitter::writeNull: Failed scalar_event_initialize");
    }
}

#if 0
    YAML_ANY_MAPPING_STYLE  Let the emitter choose the style.
    YAML_BLOCK_MAPPING_STYLE    The block mapping style.
    YAML_FLOW_MAPPING_STYLE     The flow mapping style.
#endif


template<typename T>
bool YamlEmitter::writeCompund(int (*CompoundEventInit)(yaml_event_t*, yaml_char_t*, yaml_char_t*, int, T), Parser::Attributes const& attributes)
{
    yaml_char_t*    anchor          = convertStringToYamlCharPtr(getAttribute(attributes, "anchor", ""));
    yaml_char_t*    tag             = convertStringToYamlCharPtr(getAttribute(attributes, "tag",    ""));
    int             implicit        = std::stoi(getAttribute(attributes, "implicit", "0"));
    int             style           = std::stoi(getAttribute(attributes, "style",    "0"));

    yaml_event_t    event;
    return (CompoundEventInit(&event, anchor, tag, implicit, static_cast<T>(style)) != 0)
        && (yaml_emitter_emit(&emitter, &event) != 0);
}

void YamlEmitter::writeMapStart(Parser::Attributes const& attributes)
{
    /* Note:
       Either Tag or implicit must be set
    */
    if (!writeCompund(yaml_mapping_start_event_initialize, attributes))
    {
        throw std::runtime_error("YamlEmitter::writeMapStart: Failed scalar_event_initialize");
    }
}


void YamlEmitter::writeMapEnd(Parser::Attributes const&)
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

void YamlEmitter::writeArrayStart(Parser::Attributes const& attributes)
{
    /* Note:
       Either Tag or implicit must be set
    */
    if (!writeCompund(yaml_sequence_start_event_initialize, attributes))
    {
        throw std::runtime_error("YamlEmitter::writeArrayStart: Failed sequence_start_event_initialize");
    }
}

void YamlEmitter::writeArrayEnd(Parser::Attributes const&)
{
    yaml_event_t    event;
    if (!yaml_sequence_end_event_initialize(&event) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeArrayEnd: Failed sequence_end_event_initialize");
    }
}

void YamlEmitter::writeDocStart(Parser::Attributes const& attributes)
{
    /*
    yaml_version_directive_t *      version_directive,
    yaml_tag_directive_t *      tag_directives_start,
    yaml_tag_directive_t *      tag_directives_end,
    int     implicit
    */

    std::string const&                  version     = getAttribute(attributes, "version", "-1.0");
    std::string const&                  implicit    = getAttribute(attributes, "implicitHead", "1");
    std::vector<yaml_tag_directive_t>   directives;

    for(auto loop = attributes.begin(); loop != attributes.end(); ++loop)
    {
        std::string const& handle   = loop->first;
        std::string const& key      = loop->second;
        if (handle.substr(0,4) == "dir:")
        {
            yaml_tag_directive_t   dir;
            dir.handle = convertStringToYamlCharPtr(handle) + 4;
            dir.prefix = convertStringToYamlCharPtr(key);
            directives.push_back(dir);
        }
    }

    bool                        hasDirective = false;
    yaml_version_directive_t    vDirective;
    if (version != "-1.0")
    {
        hasDirective    = true;
        vDirective.major = 1;
        vDirective.minor = 2;
    }

    yaml_tag_directive_t const*   start   = nullptr;
    yaml_tag_directive_t const*   end     = nullptr;
    if (directives.size() != 0)
    {
        start   = &directives[0];
        end     = &directives[0] + directives.size() + 1;
    }

    int implicitVal     = std:: stoi(implicit);

    yaml_event_t    event;
    if (!yaml_document_start_event_initialize(&event,
                                                hasDirective?&vDirective:nullptr,
                                                const_cast<yaml_tag_directive_t*>(start),
                                                const_cast<yaml_tag_directive_t*>(end),
                                                implicitVal) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeDocStart: Failed document_start_event_initialize");
    }
}
void YamlEmitter::writeDocEnd(Parser::Attributes const& attributes)
{
    int implicitVal     = std:: stoi(getAttribute(attributes, "implicitTail", "1"));
    yaml_event_t    event;
    if (!yaml_document_end_event_initialize(&event, implicitVal) || !yaml_emitter_emit(&emitter, &event))
    {
        throw std::runtime_error("YamlEmitter::writeDocEnd: Failed yaml_document_end_event_initialize");
    }
}

