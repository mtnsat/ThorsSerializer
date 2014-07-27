
#include "ParserDom.h"
#include "ParserInterface.h"

using namespace ThorsAnvil::Parser;

void ParserMap::insert(ParserInterface& pi, std::unique_ptr<ParserValue>&& key, std::unique_ptr<ParserValue>&& value)
{
    std::string     keyString(pi.mapValueToKey(*key));
    keyData.insert(keyString, key.release());
    mapData.insert(keyString, value.release());
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

