

#include "ScannerSax.h"
#include "LexerParser.h"

#include <sstream>
#include <list>

using namespace ThorsAnvil::Parser;

ScannerBaseSax::ScannerBaseSax()
    : mapActions(1, ScannerMapActionMap())
    , arrActions(1, ScannerArrActionMap())
    , mapAction(mapActions.begin())
    , arrAction(arrActions.begin())
{
    mapActions.push_front(ScannerMapActionMap());
    arrActions.push_front(ScannerArrActionMap());
}

ScannerBaseSax::~ScannerBaseSax()
{
}

ActionRefNote ScannerBaseSax::registerActionOnAllMapItems(std::unique_ptr<SaxAction> action)
{
    // \xFF is an invalid UTF-8 character
    // The parser will never generate mapItem of this string
    return registerAction("\xFF", std::move(action));
}
ActionRefNote ScannerBaseSax::registerAction(std::string const& mapItem, std::unique_ptr<SaxAction> action)
{
    SaxAction*&  location    = mapActions.front()[mapItem];
    std::unique_ptr<SaxAction>  oldLocation(location);
    location    = action.release();
    return &location;
}

ActionRefNote ScannerBaseSax::registerActionOnAllArrItems(std::unique_ptr<SaxAction> action)
{
    SaxAction*&  location    = arrActions.front().map[-1];
    std::unique_ptr<SaxAction>  oldLocation(location);
    location    = action.release();
    return &location;
}
ActionRefNote ScannerBaseSax::registerActionNext(std::unique_ptr<SaxAction> action)
{
    int          index       = arrActions.front().max++;
    SaxAction*&  location    = arrActions.front().map[index];
    std::unique_ptr<SaxAction>  oldLocation(location);
    location    = action.release();
    return &location;
}

void ScannerBaseSax::replaceAction(ActionRefNote oldActionRef, std::unique_ptr<SaxAction> action)
{
    SaxAction*& location    = *reinterpret_cast<SaxAction**>(oldActionRef);
    std::unique_ptr<SaxAction>  oldLocation(location);
    location                = action.release();
}

SaxAction* ScannerBaseSax::getAction(std::string const& mapItem)
{
    SaxAction*                          action  = NULL;
    ScannerMapActionMap::const_iterator find1;
    ScannerMapActionMap::const_iterator find2;
    if ((find1 = mapAction->find(mapItem)) != mapAction->end())
    {
        action  = find1->second;
    }
    else if ((find2 = mapAction->find("\xFF")) != mapAction->end())
    {
        action  = find2->second;
    }
    return action;
}
SaxAction* ScannerBaseSax::getAction(int index)
{
    SaxAction*                          action  = NULL;
    ScannerArrActionMap::const_iterator find;
    if ((find = arrAction->find(index)) != arrAction->end())
    {
        action  = find->second;
    }
    else if ((find = arrAction->find(-1)) != arrAction ->end())
    {
        action  = find->second;
    }
    return action;
}

void ScannerBaseSax::preActivate(ParserValue const& mapItem)
{
    std::string key = mapItem.keyValue();
    SaxAction*  action = getAction(key);
    if (action != NULL)
    {
        action->doPreAction(*this, Key(key));
    }
}
void ScannerBaseSax::preActivate(int index)
{
    SaxAction*  action = getAction(index);
    if (action != NULL)
    {
        action->doPreAction(*this, Key(index));
    }
}

void ScannerBaseSax::activate(ParserValue const& mapItem, ParserValue const& value)
{
    std::string key = mapItem.keyValue();
    SaxAction*  action = getAction(key);
    if (action != NULL)
    {
        action->doAction(*this, Key(key), value);
    }
}
void ScannerBaseSax::activate(int index, ParserValue const& value)
{
    SaxAction*  action = getAction(index);
    if (action != NULL)
    {
        action->doAction(*this, Key(index), value);
    }
}



