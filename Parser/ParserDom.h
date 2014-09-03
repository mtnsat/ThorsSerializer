
#ifndef THORSANVIL_PARSER_PARSER_DOM
#define THORSANVIL_PARSER_PARSER_DOM

#include "ParserException.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <sstream>
#include <algorithm>

namespace ThorsAnvil
{
    namespace Parser
    {

/* Functions:
 *
 * ParserObject             // The result of a parse is held in one of these.
 *                          // Useful for passing around the result of parsing.
 *                          // You can distinguish between the two structure types.
 *                          // and the scalar type (ParserValue)
 *      ParserValue;
 *      ParserMap;
 *      ParserArray;
 *
 * Internal Objects
 *      ParserValue Traits for conversion on extraction
 *          struct ParseTrait
 *
 *      ParserValue             Base Class.
 *          ParserStringItem:   ParserValue
 *          ParserNumberItem:   ParserValue
 *          ParserBoolItem:     ParserValue
 *          ParserNULLItem:     ParserValue
 *          ParserMapItem:      ParserValue     Holds a ParserMap
 *          ParserArrayItem:    ParserValue     Holds a ParserArray
 *
 */

/*
 * Classes that are used to build a DOM of a Parser-Object
 */
struct ParserValue;
struct ParserInterface;
//typedef boost::ptr_map<std::string, ParserValue> ParserMap;
class ParserMap
{
    typedef boost::ptr_map<std::string, ParserValue> Storage; 
    Storage     mapData;
    Storage     keyData;

    public:
        typedef Storage::const_iterator     const_iterator;
        typedef Storage::iterator           iterator;

        bool            empty() const               {return mapData.empty();}
        std::size_t     size()  const               {return mapData.size();}
        iterator        find(std::string const& key){return mapData.find(key);}
        const_iterator  begin() const               {return mapData.begin();}
        const_iterator  end()   const               {return mapData.end();}
        iterator        begin()                     {return mapData.begin();}
        iterator        end()                       {return mapData.end();}

        void                          insert(ParserInterface& pi, std::unique_ptr<ParserValue>&& key, std::unique_ptr<ParserValue>&& value);
        std::unique_ptr<ParserValue>  release(iterator);

        void            transfer(iterator, ParserMap&);
        void            erase(iterator);
        ParserValue&        operator[](std::string const& key)          {return mapData[key];}
        friend std::ostream& operator<<(std::ostream& stream, ParserMap const& node);
};



typedef boost::ptr_vector<ParserValue>           ParserArray;
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
    virtual void print(std::ostream& stream) const
    {
        std::string escapedString;
        std::for_each(std::begin(*value), std::end(*value), [&escapedString](char next){
            switch(next)
            {
                case '"':   escapedString   += "\\\"";          break;
                case '\\':  escapedString   += "\\\\";          break;
                default:    escapedString   += next;            break;
           }
        });
        stream << '"' << escapedString << '"';
    }
    private:
        virtual void setValue(std::string&       dst)  const {dst = *value;}
};
struct ParserNumberItem: ParserValue
{
    int                              base;
    int                              offset;
    std::unique_ptr<std::string>     value;
    ParserNumberItem(std::unique_ptr<std::string>& data): base(0), offset(0), value(std::move(data))   {}
    ParserNumberItem(int base, int offset, std::unique_ptr<std::string>& data): base(base), offset(offset), value(std::move(data))   {}

    virtual std::string keyValue() const              {return *value;}
    virtual void print(std::ostream& stream) const    { stream << *value; }
    private:
        virtual void setValue(long&         dst)  const {dst = base == 0 ? std::atol(value->c_str()) : std::strtol(value->c_str() + offset, nullptr, base);}
        virtual void setValue(double&       dst)  const {dst = base == 0 ? std::atof(value->c_str()) : static_cast<double>(std::strtol(value->c_str() + offset, nullptr, base));}
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
    virtual std::string keyValue() const              { if (!okForKey) {throw std::runtime_error("Using NULL as KEY");} return "null";}
    virtual void print(std::ostream& stream) const    { stream << "null"; }

    // JSON it is not OK thus the default false.
    // YAML allows NULL keys in the map.
    ParserNULLItem(bool okForKey = false)
        : okForKey(okForKey)
    {}
    private:
        bool        okForKey;
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
        ParserMap::const_iterator loopK = node.keyData.begin();
        ParserMap::const_iterator loopV = node.mapData.begin();

        stream << (*loopK->second) << ": " << (*loopV->second);

        for(++loopK, ++loopV; loopV != node.mapData.end(); ++loopK, ++loopV)
        {
            stream << ", " << (*loopK->second) << ": " << (*loopV->second);
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

    }
}

#endif

