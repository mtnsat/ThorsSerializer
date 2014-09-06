
#ifndef THORSANVIL_PARSER_PARSER_DOM_VALUE_H
#define THORSANVIL_PARSER_PARSER_DOM_VALUE_H

#include "ParserCommon.h"

namespace ThorsAnvil
{
    namespace Parser
    {

/*
 * Classes that are used to build a DOM of a Parser-Object
 */
struct ParserStringItem;
struct ParserNumberItem;
struct ParserBoolItem;
struct ParserNULLItem;
struct ParserMapItem;
struct ParserArrayItem;
class  ParserMap;
class  ParserArray;
struct ParserValueVisitor
{
    virtual void visit(ParserStringItem& item)              = 0;
    virtual void visit(ParserNumberItem& item)              = 0;
    virtual void visit(ParserBoolItem& item)                = 0;
    virtual void visit(ParserNULLItem& item)                = 0;
    virtual void visit(ParserMapItem& item)                 = 0;
    virtual void visit(ParserArrayItem& item)               = 0;
    virtual void visit(ParserMap& item, Storage& mapData, Storage& keyData) = 0;
    virtual void visit(ParserArray& item)                   = 0;
};
struct ParserValueConstVisitor
{
    virtual void visit(ParserStringItem const& item)        = 0;
    virtual void visit(ParserNumberItem const& item)        = 0;
    virtual void visit(ParserBoolItem const& item)          = 0;
    virtual void visit(ParserNULLItem const& item)          = 0;
    virtual void visit(ParserMapItem const& item)           = 0;
    virtual void visit(ParserArrayItem const& item)         = 0;
    virtual void visit(ParserMap const& item, Storage const& mapData, Storage const& keyData) = 0;
    virtual void visit(ParserArray const& item)             = 0;
};

    }
}

#endif

