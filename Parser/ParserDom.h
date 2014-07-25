
#ifndef THORSANVIL_PARSER_PARSER_DOM
#define THORSANVIL_PARSER_PARSER_DOM

#include "ParserException.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <sstream>

namespace ThorsAnvil
{
    namespace Parser
    {

/* Functions:
 * Parser-Dom Object
 *      ParserValue;
 *      ParserMap;
 *      ParserArray;
 *
 * Internal Objects
 *      ParserValue Traits for conversion on extraction
 *          struct ParseTrait
 *      ParserValue derived type for different internal objects:
 *          struct ParserValue
 *          struct ParserString
 *          struct ParserNumber
 *          struct ParserBool
 *          struct ParserNULL
 *          struct ParserMap
 *          struct ParserArray
 *
 */

/*
 * Classes that are used to build a DOM of a Parser-Object
 */
struct ParserValue;
typedef boost::ptr_map<std::string, ParserValue> ParserMap;
typedef boost::ptr_vector<ParserValue>           ParserArray;
enum ParserObjectType { ParserMapObject, ParserArrayObject, ParserValueObject };
struct ParserObject
{
    ParserObjectType  type;
    ParserObject(ParserMap* map)
        : type(ParserMapObject)
    {
        data.map    = map;
    }
    ParserObject(ParserArray* array)
        : type(ParserArrayObject)
    {
        data.array  = array;
    }
    ParserObject(ParserValue* value)
        : type(ParserValueObject)
    {
        data.value  = value;
    }
    ~ParserObject();
    ParserObject(ParserObject&& move);
    ParserObject& operator=(ParserObject&& move);
    ParserObject(ParserObject const&)            = delete;
    ParserObject& operator=(ParserObject const&) = delete;

    union
    {
        ParserMap*    map;
        ParserArray*  array;
        ParserValue*  value;
    } data;
};
typedef std::pair<std::unique_ptr<ParserValue>,std::unique_ptr<ParserValue> >       ParserMapValue;
typedef std::pair<std::string,std::unique_ptr<ParserValue> >                        ParserMapPair;

/*
 * Parser only supports three types:
 *      String
 *      double
 *      bool
 *
 * The ParseTraits class provides a mapping from all other POD types to these basic types.
 * It is used in ParserValue::getValue() to retrieve a value of appropriate type from the
 * ParserValue object which is then converted to the type you want.
 *
 * e.g.
 *      int x = pv->getValue<int>(); Will get the ParserValue() internal state as a long
 *                                   truncating the value towards zero. Normal long to int
 *                                   conversion will be done before returning the value
 *                                   which may overflow the value.
 */
template<typename T> struct ParseTrait {};

template<> struct ParseTrait<long double>       { typedef double       GetType;};
template<> struct ParseTrait<double>            { typedef double       GetType;};
template<> struct ParseTrait<float>             { typedef double       GetType;};

//template<> struct ParseTrait<long long>         { typedef long         GetType;};
template<> struct ParseTrait<long>              { typedef long         GetType;};
template<> struct ParseTrait<int>               { typedef long         GetType;};
template<> struct ParseTrait<short>             { typedef long         GetType;};
template<> struct ParseTrait<signed char>       { typedef long         GetType;};

//template<> struct ParseTrait<unsigned long long>{ typedef long         GetType;};
template<> struct ParseTrait<unsigned long>     { typedef long         GetType;};
template<> struct ParseTrait<unsigned int>      { typedef long         GetType;};
template<> struct ParseTrait<unsigned short>    { typedef long         GetType;};
template<> struct ParseTrait<unsigned char>     { typedef long         GetType;};

template<> struct ParseTrait<bool>              { typedef bool         GetType;};

template<> struct ParseTrait<std::string>       { typedef std::string  GetType;};


/*
 * The base type of all values extracted from Parser
 */
std::ostream& operator<<(std::ostream& stream, ParserValue const& node);
std::ostream& operator<<(std::ostream& stream, ParserMap   const& node);
std::ostream& operator<<(std::ostream& stream, ParserArray const& node);

struct ParserValue
{
    virtual ~ParserValue()       {}

    template<typename T>
    T getValue() const
    {
        typename ParseTrait<T>::GetType     value;
        this->setValue(value);
        return value;
    }
    virtual std::string keyValue()      const { throw std::runtime_error("Invalid Parser");}
    virtual void print(std::ostream& /*stream*/) const {throw std::runtime_error("Invalid Parser");}

    private:
    virtual void setValue(long&)        const { throw InvalidConversion();}
    virtual void setValue(double&)      const { throw InvalidConversion();}
    virtual void setValue(bool&)        const { throw InvalidConversion();}
    virtual void setValue(std::string&) const { throw InvalidConversion();}
};
struct ParserStringItem: ParserValue
{
    std::unique_ptr<std::string>     value;
    ParserStringItem(std::unique_ptr<std::string>& data): value(std::move(data)) {}

    virtual std::string keyValue() const              {return *value;}
    virtual void print(std::ostream& stream) const    { stream << '"' << *value << '"'; }
    private:
        virtual void setValue(std::string&       dst)  const {dst = *value;}
};
struct ParserNumberItem: ParserValue
{
    std::unique_ptr<std::string>     value;
    ParserNumberItem(std::unique_ptr<std::string>& data): value(std::move(data))   {}

    virtual std::string keyValue() const              {return *value;}
    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
        virtual void setValue(long&         dst)  const {dst = std::atol(value->c_str());}
        virtual void setValue(double&       dst)  const {dst = std::atof(value->c_str());}
};
struct ParserBoolItem: ParserValue
{
    bool                            value;
    ParserBoolItem(bool data): value(data)       {}

    virtual std::string keyValue() const              {return value ? "true" : "false";}
    virtual void print(std::ostream& stream) const    { stream << std::boolalpha << value; }
    private:
        virtual void setValue(bool&         dst)  const {dst = value;}
};
struct ParserNULLItem: ParserValue
{
    virtual std::string keyValue() const              { throw std::runtime_error("Using NULL as KEY");}
    virtual void print(std::ostream& stream) const    { stream << "null"; }
    private:
        virtual void setValue(long&         dst)  const {dst= 0;}
        virtual void setValue(double&       dst)  const {dst= 0.0;}
        virtual void setValue(bool&         dst)  const {dst= false;}
        virtual void setValue(std::string&  dst)  const {dst.clear();}
};
struct ParserMapItem: ParserValue
{
    std::unique_ptr<ParserMap>          value;
    ParserMapItem(std::unique_ptr<ParserMap>& data): value(std::move(data))    {}

    virtual std::string keyValue() const              { std::stringstream result;result << (*this);return result.str();}
    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
};
struct ParserArrayItem: ParserValue
{
    std::unique_ptr<ParserArray>        value;
    ParserArrayItem(std::unique_ptr<ParserArray>& data): value(std::move(data))    {}

    virtual std::string keyValue() const              { std::stringstream result;result << (*this);return result.str();}
    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
};

inline std::ostream& operator<<(std::ostream& stream, ParserValue const& node)
{
    node.print(stream);
    return stream;
}
inline std::ostream& operator<<(std::ostream& stream, ParserMap const& node)
{
    stream << "{";
    if (!node.empty())
    {
        ParserMap::const_iterator loop = node.begin();
        stream << "\"" << loop->first << "\": " << (*loop->second);

        for(++loop; loop != node.end(); ++loop)
        {
            stream << ", \"" << loop->first << "\": " << (*loop->second);
        }
    }
    stream << "}";

    return stream;
}
inline std::ostream& operator<<(std::ostream& stream, ParserArray const& node)
{
    stream << "[";
    if (!node.empty())
    {
        ParserArray::const_iterator loop = node.begin();
        stream << (*loop);

        for(++loop; loop != node.end(); ++loop)
        {
            stream << ", " << (*loop);
        }
    }
    stream << "]";

    return stream;
}

inline ParserObject::~ParserObject()
{
    switch(type)
    {
        case ParserMapObject:       delete data.map;    break;
        case ParserArrayObject:     delete data.array;  break;
        case ParserValueObject:     delete data.value;  break;
    }
}
inline ParserObject::ParserObject(ParserObject&& move)
    : type(move.type)
{
    switch(type)
    {
        case ParserMapObject:       data.map    = nullptr;  break;
        case ParserArrayObject:     data.array  = nullptr;  break;
        case ParserValueObject:     data.value  = nullptr;  break;
    }
    (*this) = std::move(move);
}
inline ParserObject& ParserObject::operator=(ParserObject&& move)
{
    if (this != &move)
    {
        switch(type)
        {
            case ParserMapObject:       delete data.map;    break;
            case ParserArrayObject:     delete data.array;  break;
            case ParserValueObject:     delete data.value;  break;
        }
        type = move.type;
        switch(type)
        {
            case ParserMapObject:       std::swap(data.map,   move.data.map);   break;
            case ParserArrayObject:     std::swap(data.array, move.data.array); break;
            case ParserValueObject:     std::swap(data.value, move.data.value); break;
        }
    }
    return *this;
}
    }
}

#endif

