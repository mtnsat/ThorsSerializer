
#include "DomSerialize.h"


using namespace ThorsAnvil::Parser;

namespace details
{
    class DocWriter
    {
        bool                        started;
        bool&                       docStart;
        ParserValue const&          item;
        EmitterInterface&           emitter;

      public:
        DocWriter(bool& docStart, ParserValue const& item, EmitterInterface& emitter)
            : started(docStart)
            , docStart(docStart)
            , item(item)
            , emitter(emitter)
        {
            if (!started)
            {
                docStart    = true;
                emitter.writeDocStart(item.getAttributes());
            }
        }
        ~DocWriter()
        {
            if (!started)
            {
                emitter.writeDocEnd(item.getAttributes());
                docStart    = false;
            }
        }
    };
}

DomSerializeVisitor::DomSerializeVisitor(EmitterInterface& emitter)
    : emitter(emitter)
    , docStart(false)
{}

void DomSerializeVisitor::visit(ParserStringItem const& item)
{
    details::DocWriter   writer(docStart, item, emitter);
    emitter.writeString(item);
}
void DomSerializeVisitor::visit(ParserNumberItem const& item)
{
    details::DocWriter   writer(docStart, item, emitter);
    emitter.writeNumber(item);
}
void DomSerializeVisitor::visit(ParserBoolItem const& item)
{
    details::DocWriter   writer(docStart, item, emitter);
    emitter.writeBool(item);
}
void DomSerializeVisitor::visit(ParserNULLItem const& item)
{
    details::DocWriter   writer(docStart, item, emitter);
    emitter.writeNull(item);
}
void DomSerializeVisitor::visit(ParserMapItem const& item)
{
    details::DocWriter   writer(docStart, item, emitter);
    emitter.writeMapStart(item.getAttributes());
    item.value->accept(*this);
    emitter.writeMapEnd(item.getAttributes());
}
void DomSerializeVisitor::visit(ParserArrayItem const& item)
{
    details::DocWriter   writer(docStart, item, emitter);
    emitter.writeArrayStart(item.getAttributes());
    item.value->accept(*this);
    emitter.writeArrayEnd(item.getAttributes());
}
void DomSerializeVisitor::visit(ParserArray const& node)
{
    for(auto loop = node.begin(); loop != node.end(); ++loop)
    {   loop->accept(*this);
    }
}
void DomSerializeVisitor::visit(ParserMap const& /*node*/, Storage const& mapData, Storage const& keyData)
{
    ParserMap::const_iterator loopK = keyData.begin();
    ParserMap::const_iterator loopV = mapData.begin();

    for(; loopV != mapData.end(); ++loopK, ++loopV)
    {
        loopK->second->accept(*this);
        loopV->second->accept(*this);
    }
}

