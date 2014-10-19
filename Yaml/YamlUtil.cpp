
#include "YamlUtil.h"

yaml_char_t* ThorsAnvil::Yaml::convertStringToYamlCharPtr(std::string const& value, EmptyStringAction action)
{
    if (action == EmptyStringIsNull && value == "")
    {   return nullptr;
    }
    return reinterpret_cast<yaml_char_t*>(const_cast<char*>(value.c_str()));
}

std::string ThorsAnvil::Yaml::convertYamlCharPtrToString(yaml_char_t const* value)
{
    if (value == nullptr)
    {   return "";
    }
    return std::string(reinterpret_cast<char const*>(value));
}

