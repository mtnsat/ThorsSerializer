
#include "JsonEmitter.h"
#include <iostream>

using namespace ThorsAnvil::Json;

JsonEmitter::JsonEmitter(std::ostream& stream)
    : EmitterInterface(stream)
    , first(true)
{}
JsonEmitter::~JsonEmitter()
{}

std::string const& JsonEmitter::seporator()
{
    static std::string const   empty    = "";
    static std::string const   keyValue = ": ";
    static std::string const   element  = ", ";
    static std::string const   doc      = "\n";

    if (state.size() == 0 && first)
    {
        first   = false;
        return empty;
    }
    if (state.size() == 0)
    {   return doc;
    }
    ++state.back().second;
    if (state.back().second == 1)
    {   return empty;
    }
    if (state.back().first == Parser::ParserMapObject && state.back().second % 2 == 0)
    {   return keyValue;
    }
    return element;
}

void JsonEmitter::writeString(std::string const& stringValue, Parser::Attributes const&)
{
    stream << seporator() << '"';
    std::for_each(std::begin(stringValue), std::end(stringValue), [this](char next)
    {
        switch(next)
        {
            case '"':   stream   << "\\\"";          break;
            case '\\':  stream   << "\\\\";          break;
            default:    stream   << next;            break;
        }
    });
    stream << '"';
}

void JsonEmitter::writeNumber(std::string const& stringValue, double, Parser::Attributes const&)
{
    stream << seporator() << stringValue;
}

void JsonEmitter::writeNumber(std::string const& stringValue, long, Parser::Attributes const&)
{
    stream << seporator() << stringValue;
}

void JsonEmitter::writeBool(std::string const&, bool value, Parser::Attributes const&)
{
    stream << seporator() << std::boolalpha << value;
}

void JsonEmitter::writeNull(std::string const&, Parser::Attributes const&)
{
    stream << seporator() << "null";
}

void JsonEmitter::writeMapStart(Parser::Attributes const&)
{
    stream << seporator() << '{';
    state.push_back(std::make_pair(Parser::ParserMapObject, 0));
}


void JsonEmitter::writeMapEnd(Parser::Attributes const&)
{
    stream << '}';
    state.pop_back();
}

void JsonEmitter::writeArrayStart(Parser::Attributes const&)
{
    stream << seporator() << '[';
    state.push_back(std::make_pair(Parser::ParserArrayObject, 0));
}

void JsonEmitter::writeArrayEnd(Parser::Attributes const&)
{
    stream << ']';
    state.pop_back();
}

void JsonEmitter::writeDocStart(Parser::Attributes const&)
{}
void JsonEmitter::writeDocEnd(Parser::Attributes const&)
{}

