
#ifndef THORSANVIL_PARSER_PARSER_SCANNER_SAX_H
#define THORSANVIL_PARSER_PARSER_SCANNER_SAX_H

#include "ParserInterface.h"
#include <string>
#include <istream>
#include <list>
#include <map>

namespace  ThorsAnvil
{
    namespace Parser
    {

struct Key
{
    std::string const   mapKey;
    int         const   arrKey;
    bool        const   isMapKey;

    Key(std::string const& key): mapKey(key), arrKey(0),   isMapKey(true) {}
    Key(int key):                mapKey(""),  arrKey(key), isMapKey(false){}
};

class ScannerSax;
struct SaxAction
{
    virtual ~SaxAction() {}
    virtual void doAction(ScannerSax&, Key const& key, ParserValue const&) = 0;
    virtual void doPreAction(ScannerSax&,Key const& key)                 = 0;
};


class ScannerSaxInterface;
typedef void*  ActionRefNote;
class ScannerSax
{
    template<typename Key>
    class PtrMap: public std::map<Key, SaxAction*>
    {
        void clean()
        {
            for(typename std::map<Key, SaxAction*>::const_iterator loop = this->begin(); loop != this->end(); ++loop)
            {
                delete loop->second;
            }
        }
        public:
        // Note: Copy constructor only works for empty map.
        // The copy constructor is only used to push these object into a list when they are empty
        // So that works OK
        ~PtrMap()
        {
            clean();
        }
        void clear()
        {
            clean();
            std::map<Key, SaxAction*>::clear();
        }
    };
    typedef PtrMap<std::string>                 ScannerMapActionMap;
    typedef std::list<ScannerMapActionMap>      ScannerMapActionList;
    struct ScannerArrActionMap
    {
        typedef PtrMap<int>::const_iterator     const_iterator;

        ScannerArrActionMap() : max(0)          {}
        void            clear()                 { max = 0; map.clear();}
        const_iterator  find(int index) const   {return map.find(index);}
        const_iterator  end()           const   {return map.end();}
        int                                 max;
        PtrMap<int>                         map;
    };
    typedef std::list<ScannerArrActionMap>      ScannerArrActionList;

    ScannerMapActionList                    mapActions;
    ScannerArrActionList                    arrActions;

    ScannerMapActionList::const_iterator    mapAction;
    ScannerArrActionList::const_iterator    arrAction;

    public:
        ~ScannerSax();
         ScannerSax();
        template<typename Parser>
        void            parse(std::istream& src);
        ActionRefNote   registerAction(std::string const& mapItem, std::unique_ptr<SaxAction> action);
        ActionRefNote   registerActionNext(std::unique_ptr<SaxAction> action);
        ActionRefNote   registerActionOnAllMapItems(std::unique_ptr<SaxAction> action);
        ActionRefNote   registerActionOnAllArrItems(std::unique_ptr<SaxAction> action);
        void            replaceAction(ActionRefNote oldActionRef, std::unique_ptr<SaxAction> action);

    private:
        friend class ScannerSaxInterface;
        void    preActivate(std::string const& mapItem);
        void    preActivate(int index);
        void    activate(std::string const& mapItem, ParserValue const& value);
        void    activate(int index, ParserValue const& value);

        SaxAction* getAction(std::string const& item);
        SaxAction* getAction(int index);

        void    push_mapAction()    {mapActions.push_front(ScannerMapActionMap());--mapAction;}
        void    push_arrAction()    {arrActions.push_front(ScannerArrActionMap());--arrAction;}
        void    pop_mapAction()     {mapActions.pop_front();mapActions.front().clear();++mapAction;}
        void    pop_arrAction()     {arrActions.pop_front();arrActions.front().clear();++arrAction;}
};

class ScannerSaxInterface: public ParserCleanInterface
{
    std::list<int> currentArrayIndex;
    ScannerSax&     parent;
    public:
    ScannerSaxInterface(ScannerSax& p): parent(p)   {}
    virtual void            mapOpen()                                           { parent.push_mapAction();}
    virtual void            mapClose()                                          { parent.pop_mapAction();}
    virtual std::string*    mapKeyNote(std::string* k)                          { std::unique_ptr<std::string> ak(k);                                         parent.preActivate(*ak);     return ak.release();}
    virtual ParserMapValue* mapCreateElement(std::string* k,ParserValue* val)   { std::unique_ptr<std::string> ak(k);   std::unique_ptr<ParserValue> aval(val); parent.activate(*ak, *aval); return NULL;}
    virtual void            arrayOpen()                                         { currentArrayIndex.push_front(0); parent.push_arrAction(); parent.preActivate(currentArrayIndex.front());}
    virtual void            arrayClose()                                        { currentArrayIndex.pop_front();   parent.pop_arrAction();}
    virtual ParserArray*    arrayNote(ParserArray* arr)                         { std::unique_ptr<ParserArray>   aarr(arr); parent.preActivate(currentArrayIndex.front());return NULL;}
    virtual ParserValue*    arrayCreateElement(ParserValue* val)                { std::unique_ptr<ParserValue>   aval(val); parent.activate(currentArrayIndex.front()++, *aval);return NULL;}
    virtual ParserValue*    valueParseString(std::string* str)                  { std::unique_ptr<std::string> astr(str); return new ParserStringItem(astr);}
    virtual ParserValue*    valueParseNumber(std::string* num)                  { std::unique_ptr<std::string> anum(num); return new ParserNumberItem(anum);}
    virtual ParserValue*    valueParseBool(bool val)                            {                                         return new ParserBoolItem(val);}
    virtual ParserValue*    valueParseNULL()                                    {                                         return new ParserNULLItem();}
};
template<typename Parser>
void ScannerSax::parse(std::istream& src)
{
    ScannerSaxInterface     scanner(*this);
    Parser                  parser(src, scanner);

    parser.parse();
}


    }
}

#endif

