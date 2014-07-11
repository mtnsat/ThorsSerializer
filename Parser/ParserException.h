
#ifndef THORSANVIL_PARSER_PARSER_EXCEPTION_H
#define THORSANVIL_PARSER_PARSER_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace ThorsAnvil
{
    namespace Parser
    {

/*
 * Exceptions
 */
class ParserException: public std::runtime_error
{   public: ParserException(std::string const& msg) : std::runtime_error(msg) {}
};

// Throw when trying to get an integer from a string or string from an integer etc.
class InvalidConversion: public ParserException
{   public: InvalidConversion(): ParserException("Invalid Conversion"){}};
// Thrown when there is something wrong with the input stream
class ParsingError: public ParserException
{   public: ParsingError(std::string const& msg): ParserException(msg) {}};


    }
}

#endif

