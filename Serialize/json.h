
#ifndef THORSANVIL_SERIALIZE_JSON_H
#define THORSANVIL_SERIALIZE_JSON_H

#include "serializer.h"
#include "Json/JsonScanner.h"

namespace ThorsAnvil
{
    namespace Serialize
    {

template<typename T>
Importer<T, ThorsAnvil::Json::JsonScannerSax> jsonImport(T& obj)
{
    return Importer<T, ThorsAnvil::Json::JsonScannerSax>(obj);
}
template<typename T>
Exporter<T, ThorsAnvil::Json::JsonPrinterSax> jsonExport(T const& obj)
{
    return Exporter<T, ThorsAnvil::Json::JsonPrinterSax>(obj);
}

// To force an object file to be created (so tests will be run correctly).
class F { public: F(); };

    }
}

#endif

