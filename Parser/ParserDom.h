
#ifndef THORSANVIL_PARSER_PARSER_DOM_H
#define THORSANVIL_PARSER_PARSER_DOM_H

#include "ParserException.h"
#include "ParserCommon.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace ThorsAnvil
{
    namespace Parser
    {

/* Functions:
 *
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
struct ParserValueVisitor;
struct ParserValueConstVisitor;

class ParserMap
{
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
        void    accept(ParserValueConstVisitor& visitor) const;
        void    accept(ParserValueVisitor& visitor);
};


class ParserArray: public boost::ptr_vector<ParserValue>
{
    public:
        void    accept(ParserValueConstVisitor& visitor) const;
        void    accept(ParserValueVisitor& visitor);
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

template<> struct ParseTrait<long long>         { typedef long         GetType;};
template<> struct ParseTrait<long>              { typedef long         GetType;};
template<> struct ParseTrait<int>               { typedef long         GetType;};
template<> struct ParseTrait<short>             { typedef long         GetType;};
template<> struct ParseTrait<signed char>       { typedef long         GetType;};

template<> struct ParseTrait<unsigned long long>{ typedef long         GetType;};
template<> struct ParseTrait<unsigned long>     { typedef long         GetType;};
template<> struct ParseTrait<unsigned int>      { typedef long         GetType;};
template<> struct ParseTrait<unsigned short>    { typedef long         GetType;};
template<> struct ParseTrait<unsigned char>     { typedef long         GetType;};

template<> struct ParseTrait<bool>              { typedef bool         GetType;};

template<> struct ParseTrait<std::string>       { typedef std::string  GetType;};


/*
 * The base type of all values extracted from Parser
 */
struct ParserValue
{
    virtual ~ParserValue();

    template<typename T>
    T getValue() const
    {
        typename ParseTrait<T>::GetType     value;
        this->setValue(value);
        return value;
    }
    virtual std::string keyValue()                              const   {throw std::runtime_error("Invalid Parser");}
    virtual void accept(ParserValueConstVisitor& /*visitor*/)   const   {throw std::runtime_error("Invalid Parser");}
    virtual void accept(ParserValueVisitor& /*visitor*/)                {throw std::runtime_error("Invalid Parser");}

    private:
    virtual void setValue(long&)                                const   {throw InvalidConversion();}
    virtual void setValue(double&)                              const   {throw InvalidConversion();}
    virtual void setValue(bool&)                                const   {throw InvalidConversion();}
    virtual void setValue(std::string&)                         const   {throw InvalidConversion();}
};

struct ParserStringItem: ParserValue
{
    std::unique_ptr<std::string>    value;
    ParserStringItem(std::unique_ptr<std::string>& data): value(std::move(data)) {}

    virtual std::string keyValue()                              const   {return *value;}
    virtual void accept(ParserValueConstVisitor& visitor)       const;
    virtual void accept(ParserValueVisitor& visitor);
    private:
        virtual void setValue(std::string&       dst)           const   {dst = *value;}
};
struct ParserNumberItem: ParserValue
{
    int                             base;
    int                             offset;
    std::unique_ptr<std::string>    value;
    ParserNumberItem(std::unique_ptr<std::string>& data);
    ParserNumberItem(int base, int offset, std::unique_ptr<std::string>& data);

    virtual std::string keyValue()                              const   {return *value;}
    virtual void accept(ParserValueConstVisitor& visitor)       const;
    virtual void accept(ParserValueVisitor& visitor);
    private:
        virtual void setValue(long&         dst)                const;
        virtual void setValue(double&       dst)                const;
};
struct ParserBoolItem: ParserValue
{
    bool                            value;
    ParserBoolItem(bool data): value(data)                              {}

    virtual std::string keyValue()                              const   {return value ? "true" : "false";}
    virtual void accept(ParserValueConstVisitor& visitor)       const;
    virtual void accept(ParserValueVisitor& visitor);
    private:
        virtual void setValue(bool&         dst)                const   {dst = value;}
};
struct ParserNULLItem: ParserValue
{
    virtual std::string keyValue()                              const   { if (!okForKey) {throw std::runtime_error("Using NULL as KEY");} return "null";}
    virtual void accept(ParserValueConstVisitor& visitor)       const;
    virtual void accept(ParserValueVisitor& visitor);

    // JSON it is not OK thus the default false.
    // YAML allows NULL keys in the map.
    ParserNULLItem(bool okForKey = false): okForKey(okForKey)           {}
    private:
        bool        okForKey;
        virtual void setValue(long&         dst)                const   {dst= 0;}
        virtual void setValue(double&       dst)                const   {dst= 0.0;}
        virtual void setValue(bool&         dst)                const   {dst= false;}
        virtual void setValue(std::string&  dst)                const   {dst.clear();}
};
struct ParserMapItem: ParserValue
{
    std::unique_ptr<ParserMap>      value;
    ParserMapItem(std::unique_ptr<ParserMap>& data): value(std::move(data))    {}

    virtual std::string keyValue()                              const;
    virtual void accept(ParserValueConstVisitor& visitor)       const;
    virtual void accept(ParserValueVisitor& visitor);
    private:
};
struct ParserArrayItem: ParserValue
{
    std::unique_ptr<ParserArray>        value;
    ParserArrayItem(std::unique_ptr<ParserArray>& data): value(std::move(data))    {}

    virtual std::string keyValue()                              const;
    virtual void accept(ParserValueConstVisitor& visitor)       const;
    virtual void accept(ParserValueVisitor& visitor);
    private:
};

    }
}

#endif

