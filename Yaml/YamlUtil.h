
#ifndef THORSANVIL_YAML_YAML_UTIL_H
#define THORSANVIL_YAML_YAML_UTIL_H

#include <string>
#include <yaml.h>

namespace ThorsAnvil
{
    namespace Yaml
    {

enum EmptyStringAction {EmptyStringIsNull, UseEmptyString};
yaml_char_t* convertStringToYamlCharPtr(std::string const& value, EmptyStringAction action = EmptyStringIsNull);
std::string  convertYamlCharPtrToString(yaml_char_t const* value);

    }
}

#endif

