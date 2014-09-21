
#ifndef THORSANVIL_SERIALIZER_SERIALIZER_H
#define THORSANVIL_SERIALIZER_SERIALIZER_H

#include <boost/mpl/vector.hpp>
#include <istream>

#define THOR_BUILD_SERIALIZE(parent, local, ...)        \
namespace ThorsAnvil                                    \
{                                                       \
    namespace Serialzie                                 \
    {                                                   \
        namespace Json                                  \
        {                                               \
            template<>                                  \
            class JsonSerializeTraits<local>            \
            {};                                         \
        }                                               \
    }                                                   \
}

#define JsonSerializeTraits_MAKE1(parent,local, ...)    THOR_BUILD_SERIALIZE(parent, local, __VA_ARGS__)
#define JsonSerializeTraits_MAKE(parent,local, ...)
#define THORSANVIL_SERIALIZE_JsonAttribute(x)           typedef int x


namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {
            enum JsonSerializeType {Invalid, Array, Map};

            template<typename T>
            class JsonSerializeTraits
            {};
        }

template<typename T, typename Serializer>
class Importer
{
    public:
        Importer(T&){}

    friend std::istream& operator>>(std::istream& stream, Importer<T, Serializer> const&)
    {
        Serializer      serializer;
        serializer.parse(stream);
        return stream;
    }
};
template<typename T, typename Printer>
class Exporter
{
    public:
        Exporter(T const&){}

    friend std::ostream& operator<<(std::ostream& stream, Exporter<T, Printer> const&) {return stream;}
};

    }
}

#endif

