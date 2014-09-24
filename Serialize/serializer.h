
#ifndef THORSANVIL_SERIALIZER_SERIALIZER_H
#define THORSANVIL_SERIALIZER_SERIALIZER_H

#include "Parser/ScannerSax.h"
#include <boost/mpl/vector.hpp>
#include <istream>
#include <tuple>

#define THOR_BUILD_SERIALIZE_COUNT_ARGS(...)             THOR_BUILD_SERIALIZE_COUNT_ARGS_ACT( __VA_ARGS__, 20,19, 18, 17, 16, 15, 14,13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 )
#define THOR_BUILD_SERIALIZE_COUNT_ARGS_ACT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,N,...)   N

#define THORSANVIL_SERIALIZE_JsonAttribute(x)                     typedef int x
#define THOR_BUILD_Attribute_Type(Type, ...)                      THOR_BUILD_Attribute_TypeC1(THOR_BUILD_SERIALIZE_COUNT_ARGS(__VA_ARGS__), Type, __VA_ARGS__)
#define THOR_BUILD_Attribute_TypeC1(Count, Type, ...)             THOR_BUILD_Attribute_TypeC2(Count, Type, __VA_ARGS__)
#define THOR_BUILD_Attribute_TypeC2(Count, Type, ...)             THOR_BUILD_Attribute_Type ## Count(Type, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type20(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type19(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type19(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type18(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type18(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type17(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type17(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type16(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type16(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type15(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type15(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type14(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type14(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type13(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type13(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type12(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type12(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type11(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type11(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type10(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type10(Type, local, member, ...)     MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type9(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type9(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type8(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type8(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type7(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type7(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type6(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type6(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type5(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type5(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type4(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type4(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type3(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type3(Type, local, member, ...)      MemberAccess<local, decltype(&local::member), Type>, THOR_BUILD_Attribute_Type2(Type, local, __VA_ARGS__)
#define THOR_BUILD_Attribute_Type2(Type, local, member)           MemberAccess<local, decltype(&local::member), Type>
#define THOR_BUILD_Attribute_Type1(Type, local)

#define THOR_BUILD_Initialize_Val(Sc, Obj, ...)                  THOR_BUILD_Initialize_ValC1(THOR_BUILD_SERIALIZE_COUNT_ARGS(__VA_ARGS__), Sc, Obj, __VA_ARGS__)
#define THOR_BUILD_Initialize_ValC1(Count, Sc, Obj, ...)         THOR_BUILD_Initialize_ValC2(Count, Sc, Obj, __VA_ARGS__)
#define THOR_BUILD_Initialize_ValC2(Count, Sc, Obj, ...)         THOR_BUILD_Initialize_Val ## Count(Sc, Obj, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val20(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val19(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val19(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val18(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val18(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val17(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val17(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val16(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val16(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val15(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val15(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val14(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val14(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val13(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val13(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val12(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val12(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val11(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val11(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val10(Sc, Obj, local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val10(Sc, Obj, local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val9(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val9(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val8(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val8(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val7(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val7(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val6(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val6(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val5(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val5(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val4(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val4(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val3(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val3(Sc, Obj,  local, member, ...) { Sc, Obj, #member, &local::member }, THOR_BUILD_Initialize_Val2(Sc, Obj,  local, __VA_ARGS__)
#define THOR_BUILD_Initialize_Val2(Sc, Obj,  local, member)      { Sc, Obj, #member, &local::member }
#define THOR_BUILD_Initialize_Val1(Sc, Obj,  local)

#define THOR_BUILD_GetLocal(...)                THOR_BUILD_GetLocal_C1(THOR_BUILD_SERIALIZE_COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)
#define THOR_BUILD_GetLocal_C1(Count, ...)      THOR_BUILD_GetLocal_C2(Count, __VA_ARGS__)
#define THOR_BUILD_GetLocal_C2(Count, ...)      THOR_BUILD_GetLocal ## Count(__VA_ARGS__)
#define THOR_BUILD_GetLocal20(local, ...)       local
#define THOR_BUILD_GetLocal19(local, ...)       local
#define THOR_BUILD_GetLocal18(local, ...)       local
#define THOR_BUILD_GetLocal17(local, ...)       local
#define THOR_BUILD_GetLocal16(local, ...)       local
#define THOR_BUILD_GetLocal15(local, ...)       local
#define THOR_BUILD_GetLocal14(local, ...)       local
#define THOR_BUILD_GetLocal13(local, ...)       local
#define THOR_BUILD_GetLocal12(local, ...)       local
#define THOR_BUILD_GetLocal11(local, ...)       local
#define THOR_BUILD_GetLocal10(local, ...)       local
#define THOR_BUILD_GetLocal9(local, ...)        local
#define THOR_BUILD_GetLocal8(local, ...)        local
#define THOR_BUILD_GetLocal7(local, ...)        local
#define THOR_BUILD_GetLocal6(local, ...)        local
#define THOR_BUILD_GetLocal5(local, ...)        local
#define THOR_BUILD_GetLocal4(local, ...)        local
#define THOR_BUILD_GetLocal3(local, ...)        local
#define THOR_BUILD_GetLocal2(local, ...)        local
#define THOR_BUILD_GetLocal1(local)             local

/*
#
# Arguments.
#   parent:     Type of the parent.
#   local:      Type of the class we want to serialize.
#   members*:   Name of 0 or more members that need to be serialized.
*/
#define THOR_BUILD_SERIALIZE(Type, parent, ...)                                         \
namespace ThorsAnvil                                                                    \
{                                                                                       \
    namespace Serialize                                                                 \
    {                                                                                   \
        namespace Json                                                                  \
        {                                                                               \
            template<>                                                                  \
            class JsonSerializeTraits<THOR_BUILD_GetLocal(__VA_ARGS__)>                 \
            {                                                                           \
                typedef std::tuple<                                                     \
                    THOR_BUILD_Attribute_Type(Type, __VA_ARGS__)                        \
                          >   Pointers;                                                 \
                Pointers    pointers;                                                   \
                public:                                                                 \
                enum { type = Type };                                                   \
                using   LocalType = THOR_BUILD_GetLocal(__VA_ARGS__);                   \
                JsonSerializeTraits(Parser::ScannerBaseSax& scanner, LocalType& object) \
                    : pointers{                                                         \
                    THOR_BUILD_Initialize_Val(scanner, object, __VA_ARGS__)             \
                      }                                                                 \
                {}                                                                      \
            };                                                                          \
        }                                                                               \
    }                                                                                   \
}

#define JsonSerializeTraits_MAKE(parent, ...)    THOR_BUILD_SERIALIZE(ThorsAnvil::Serialize::Json::Map, parent, __VA_ARGS__)


namespace ThorsAnvil
{
    namespace Serialize
    {
        namespace Json
        {
            enum JsonSerializeType {Invalid, Array, Map};

            template<typename T>
            class JsonSerializeTraits
            {
                public:
                    enum { type = Invalid };
                    JsonSerializeTraits(Parser::ScannerBaseSax&,T&){}
            };
        }

template<typename ValueType, int TypeSpecialization>
struct DoActionForMember;
template<typename ValueType, int TypeSpecialization>
struct DoPreActionForMember;

template<typename T, typename M, int TypeSpecialization>
class MemberAccess
{
    using SelfPtr       =  T*;
    using ValueTypeRef  =  decltype((SelfPtr(nullptr))->*(M(nullptr)));
    using ValueType     =  typename std::remove_reference<ValueTypeRef>::type;

    T&                  object;
    M                   dst;

    class MemberAccessAction: public Parser::SaxAction
    {
        MemberAccess&   member;
        virtual void doAction(Parser::ScannerBaseSax& scanner, Parser::Key const& key, Parser::ParserValue const& value)
        {
            T&      object   = this->member.object;
            M&      dst      = this->member.dst;

            DoActionForMember<ValueType, TypeSpecialization>    action;
            action(scanner, key, value, (object.*dst));
        }
        virtual void doPreAction(Parser::ScannerBaseSax& scanner, Parser::Key const& key)
        {
            T&      object   = this->member.object;
            M&      dst      = this->member.dst;

            DoPreActionForMember<ValueType, TypeSpecialization>    action;
            action(scanner, key, (object.*dst));
        }
        public:
            MemberAccessAction(MemberAccess& member)
                : member(member)
            {}
    };

    friend class MemberAccessAction;
    public:
        MemberAccess(Parser::ScannerBaseSax& serializer, T& object, std::string const& name, M dst)
            : object(object)
            , dst(dst)
        {
            // 87         ActionRefNote   registerAction(std::string const& mapItem, std::unique_ptr<SaxAction> action);
            // 88         ActionRefNote   registerActionNext(std::unique_ptr<SaxAction> action);
            // 89         ActionRefNote   registerActionOnAllMapItems(std::unique_ptr<SaxAction> action);
            // 90         ActionRefNote   registerActionOnAllArrItems(std::unique_ptr<SaxAction> action);
            std::unique_ptr<Parser::SaxAction>  action(new MemberAccessAction(*this));
            serializer.registerAction(name, std::move(action));
        }
};

template<typename ValueType>
struct DoActionForMember<ValueType, ThorsAnvil::Serialize::Json::Invalid>
{
    void operator()(Parser::ScannerBaseSax&, Parser::Key const&, Parser::ParserValue const& value, ValueType& dest)
    {
        dest    = value.getValue<ValueType>();
    }
};
template<typename ValueType>
struct DoActionForMember<ValueType, ThorsAnvil::Serialize::Json::Map>
{
    void operator()(Parser::ScannerBaseSax&, Parser::Key const&, Parser::ParserValue const&, ValueType&)
    {}
};
/*
template<typename ValueType>
struct DoActionForMember<ValueType, ThorsAnvil::Serialize::Json::Array>
{
    void operator()(Parser::ScannerBaseSax&, Parser::Key const&, Parser::ParserValue const&, ValueType&)
    {
    }
};
*/
template<typename ValueType>
struct DoPreActionForMember<ValueType, ThorsAnvil::Serialize::Json::Invalid>
{
    void operator()(Parser::ScannerBaseSax&, Parser::Key const&, ValueType&)
    {}
};
template<typename ValueType>
struct DoPreActionForMember<ValueType, ThorsAnvil::Serialize::Json::Map>
{
    void operator()(Parser::ScannerBaseSax&, Parser::Key const&, ValueType&)
    {
        /*
        typedef ThorsAnvil::Serialize::Json::JsonSerializeTraits<ValueType>    Traits;

        Traits          traits(serializer, object);
        serializer.parse(stream);
        */
    }
};
/*
template<typename ValueType>
struct DoPreActionForMember<ValueType, ThorsAnvil::Serialize::Json::Array>
{
    void operator()(Parser::ScannerBaseSax&, Parser::Key const&, ValueType&)
    {
    }
};
*/

template<typename T, typename Serializer>
class Importer
{
    T&  object;
    public:
        Importer(T& object)
            : object(object)
        {}

    friend std::istream& operator>>(std::istream& stream, Importer<T, Serializer> const& data)
    {
        typedef ThorsAnvil::Serialize::Json::JsonSerializeTraits<T>    Traits;

        Serializer      serializer;
        Traits          traits(serializer, data.object);

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

