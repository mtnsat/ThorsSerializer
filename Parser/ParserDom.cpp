
#include "ParserDom.h"
#include "ParserDomVisit.h"
#include "ParserInterface.h"

using namespace ThorsAnvil::Parser;

ParserValue::~ParserValue()
{}

ParserNumberItem::ParserNumberItem(std::unique_ptr<std::string>& data)
    : ParserValue(std::move(*data))
    , base(0)
    , offset(0)
{}

ParserNumberItem::ParserNumberItem(int base, int offset, std::unique_ptr<std::string>& data)
    : ParserValue(std::move(*data))
    , base(base)
    , offset(offset)
{}

void ParserNumberItem::setValue(long& dst)  const
{
    dst = base == 0
            ? std::atol(value.c_str())
            : std::strtol(value.c_str() + offset, nullptr, base);
}

void ParserNumberItem::setValue(double& dst)  const
{
    dst = base == 0
        ? std::atof(value.c_str())
        : static_cast<double>(std::strtol(value.c_str() + offset, nullptr, base));
}

void ParserNumberItem::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserNumberItem::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

void ParserBoolItem::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserBoolItem::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

void ParserNULLItem::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserNULLItem::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

void ParserMapItem::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserMapItem::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

void ParserArrayItem::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserArrayItem::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

void ParserMap::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this, mapData, keyData);
}

void ParserMap::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this, mapData, keyData);
}

void ParserArray::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserArray::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

void ParserMap::insert(ParserInterface& pi, std::unique_ptr<ParserValue>&& key, std::unique_ptr<ParserValue>&& value)
{
    std::string     keyString(pi.mapValueToKey(*key));
    keyData.insert(keyString, key.release());
    mapData.insert(keyString, value.release());
}

void ParserStringItem::accept(ParserValueConstVisitor& visitor) const
{
    visitor.visit(*this);
}

void ParserStringItem::accept(ParserValueVisitor& visitor)
{
    visitor.visit(*this);
}

std::unique_ptr<ParserValue> ParserMap::release(iterator iter)
{
    auto key    = keyData.release(keyData.find(iter->first));
    auto val    = mapData.release(mapData.find(iter->first));

    return std::unique_ptr<ParserValue>(val.release());
}

void ParserMap::transfer(iterator iter, ParserMap& src)
{
    keyData.transfer(src.keyData.find(iter->first), src.keyData);
    mapData.transfer(src.mapData.find(iter->first), src.mapData);
}

void ParserMap::erase(iterator iter)
{
    keyData.erase(iter->first);
    mapData.erase(iter->first);
}

