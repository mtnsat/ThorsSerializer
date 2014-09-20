
#include "DomSerialize.h"
#include "YamlEmitter.h"
#include "YamlUtil.h"
#include "Parser/ParserDomVisit.h"


namespace ThorsAnvil
{
    namespace Yaml
    {

struct YamlSerializeVisitor: public Parser::ParserValueConstVisitor
{
    YamlEmitter     emitter;
    bool            docStart;

    YamlSerializeVisitor(std::ostream& stream)
        : emitter(stream)
        , docStart(false)
    {}

    void writeDocStart(Parser::ParserValue const& item)
    {
        docStart    = true;
        std::vector<yaml_tag_directive_t>      directives;

        for(auto loop = item.attrBegin(); loop != item.attrEnd(); ++loop)
        {
            std::string const& handle   = loop->first;
            std::string const& key      = loop->second;
            if (handle.substr(0,4) == "dir:")
            {
                yaml_tag_directive_t   dir;
                dir.handle = convertStringToYamlCharPtr(handle) + 4;
                dir.prefix = convertStringToYamlCharPtr(key);
                directives.push_back(dir);
            }
        }
        emitter.writeDocStart(item.getAttribute("version", "-1.0"), directives, item.getAttribute("implicitHead", "1"));
    }
    void writeDocEnd(Parser::ParserValue const& item)
    {
        emitter.writeDocEnd(item.getAttribute("implicitTail", "1"));
        docStart    = false;
    }

    void doWrite(std::function<void(std::string const&, std::string const&, int, int, int)> action, Parser::ParserValue const& item)
    {
        bool    start   = docStart;
        if (!start)
        {   writeDocStart(item);
        }
        std::string const&  anchor          = item.getAttribute("anchor");
        std::string const&  tag             = item.getAttribute("tag");
        int                 plain_implicit  = std::stoi(item.getAttribute("plain_implicit"));
        int                 quoted_implicit = std::stoi(item.getAttribute("quoted_implicit"));
        int                 style           = std::stoi(item.getAttribute("style"));

        action(anchor, tag, plain_implicit, quoted_implicit, style);
        if (!start)
        {   writeDocEnd(item);
        }
    }

    void doWriteContainer(std::function<void(std::string const& anchor, std::string const& tag, int i, int s)> action, Parser::ParserValue const& item)
    {
        bool    start   = docStart;
        if (!start)
        {   writeDocStart(item);
        }
        std::string const&  anchor          = item.getAttribute("anchor");
        std::string const&  tag             = item.getAttribute("tag");
        int                 implicit        = std::stoi(item.getAttribute("implicit"));
        int                 style           = std::stoi(item.getAttribute("style"));

        action(anchor, tag, implicit, style);
    }

    virtual void visit(Parser::ParserStringItem const& item)
    {
        doWrite([&item,this](std::string const& anchor, std::string const& tag, int pi, int qi, int s){this->emitter.writeString(item.value, anchor, tag, pi, qi, s);}, item);
    }
    virtual void visit(Parser::ParserNumberItem const& item)
    {
        doWrite([&item,this](std::string const& anchor, std::string const& tag, int pi, int qi, int s){this->emitter.writeNumber(item.value, anchor, tag, pi, qi, s);}, item);
    }
    virtual void visit(Parser::ParserBoolItem const& item)
    {
        doWrite([&item,this](std::string const& anchor, std::string const& tag, int pi, int qi, int s){this->emitter.writeBool(item.value, anchor, tag, pi, qi, s);}, item);
    }
    virtual void visit(Parser::ParserNULLItem const& item)
    {
        doWrite([&item,this](std::string const& anchor, std::string const& tag, int pi, int qi, int s){this->emitter.writeNull(item.value, anchor, tag, pi, qi, s);}, item);
    }
    virtual void visit(Parser::ParserMapItem const& item)
    {
        bool    start   = docStart;
        doWriteContainer([this](std::string const& anchor, std::string const& tag, int i, int s){this->emitter.writeMapStart(anchor, tag, i, s);}, item);
        item.value->accept(*this);
        emitter.writeMapEnd();
        if (!start)
        {   writeDocEnd(item);
        }
    }
    virtual void visit(Parser::ParserArrayItem const& item)
    {
        bool    start   = docStart;
        doWriteContainer([this](std::string const& anchor, std::string const& tag, int i, int s){this->emitter.writeArrayStart(anchor, tag, i, s);}, item);
        item.value->accept(*this);
        emitter.writeArrayEnd();
        if (!start)
        {   writeDocEnd(item);
        }
    }
    virtual void visit(Parser::ParserArray const& node)
    {
        for(auto loop = node.begin(); loop != node.end(); ++loop)
        {   loop->accept(*this);
        }
    }
    virtual void visit(Parser::ParserMap const& /*node*/, Parser::Storage const& mapData, Parser::Storage const& keyData)
    {
        Parser::ParserMap::const_iterator loopK = keyData.begin();
        Parser::ParserMap::const_iterator loopV = mapData.begin();

        for(; loopV != mapData.end(); ++loopK, ++loopV)
        {
            loopK->second->accept(*this);
            loopV->second->accept(*this);
        }
    }
};


template<typename T>
std::ostream& operator<<(std::ostream& stream, DomSerializer<T> const& data)
{
    YamlSerializeVisitor    serializer(stream);
    data.value.accept(serializer);
    return stream;
}

template std::ostream& operator<<(std::ostream& stream, DomSerializer<ThorsAnvil::Parser::ParserValue> const& data);
template std::ostream& operator<<(std::ostream& stream, DomSerializer<ThorsAnvil::Parser::ParserMap> const& data);
template std::ostream& operator<<(std::ostream& stream, DomSerializer<ThorsAnvil::Parser::ParserArray> const& data);

    }
}

