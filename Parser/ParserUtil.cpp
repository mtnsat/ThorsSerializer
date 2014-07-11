
#include "ParserUtil.h"
#include <sstream>

using namespace ThorsAnvil::Parser;

/* Forward declaration of static functions */
static void mergeParserDom(ParserMap&   dst, ParserMap&   src, std::string const& errorMsg, std::string const& index);
static void mergeParserDom(ParserArray& dst, ParserArray& src, std::string const& errorMsg, std::string const& index);


/* Implementation */
static void mergeParserDom(ParserMap& dst, ParserMap& src, std::string const& errorMsg, std::string const& index)
{
    ParserMap::iterator next;
    for(ParserMap::iterator loop = src.begin(); loop != src.end(); loop = next)
    {
        next = loop;
        ++next;
        ParserMap::iterator find = dst.find(loop->first);
        if (find == dst.end())
        {
            dst.transfer(loop, src);
        }
        else
        {
            ParserNULLItem*   nil = dynamic_cast<ParserNULLItem*>(loop->second);
            if (nil != NULL)
            {
                // ParserNull objects cause the node in the destination to be deleted.
                dst.erase(find);
                continue;
            }

            ParserMapItem*    map = dynamic_cast<ParserMapItem*>(find->second);
            ParserArrayItem*  arr = dynamic_cast<ParserArrayItem*>(find->second);


            if ((map == NULL) && (arr == NULL))
            {
                // Normal Item (not map or array). 
                // The src item over-rides the destination item
                dst.erase(find);
                dst.transfer(loop, src);
            }
            else
            {
                // The item in the destination is a map or an array.
                // Check what the value we are merging into this object is
                ParserMapItem*    srcMapItem = dynamic_cast<ParserMapItem*>(loop->second);

                if ((map != NULL) && (srcMapItem == NULL))
                {
                    // Maps can only be combined with other maps.

                    char const* into = (map != NULL)        ? "Map" : "Array";
                    char const* from = (srcMapItem != NULL) ? "Map" : "Other";
                    std::stringstream msg;
                    msg << "Invalid Config Merge into " << into << " from " << from << " in file: `" << errorMsg << "` into `" << index << "`:" << find->first;
                    throw std::runtime_error(msg.str());
                }

                if (map != NULL) // If he destination is a map then the source must be a map
                {
                    // Recursively combine maps.
                    ParserMap&  dstMap    = *map->value;  
                    ParserMap&  srcMap    = *srcMapItem->value;

                    mergeParserDom(dstMap, srcMap, errorMsg, index + ":" + find->first);
                }
                else if (arr != NULL)
                {
                    // Combine stuff into an array.
                    ParserArrayItem*    srcArrayItem = dynamic_cast<ParserArrayItem*>(loop->second);

                    if (srcArrayItem != NULL)
                    {
                        // Two arrays are concatenate together.
                        ParserArray&  dstArray    = *arr->value;  
                        ParserArray&  srcArray    = *srcArrayItem->value;

                        mergeParserDom(dstArray, srcArray, errorMsg, index);
                    }
                    else
                    {
                        // Normal Items are just append onto the end of the array
                        ParserArray&  dstArray    = *arr->value;  
                        dstArray.push_back(src.release(loop).release());
                    }
                }
            }
        }
    }
}

static void mergeParserDom(ParserArray& dst, ParserArray& src, std::string const& /*errorMsg*/, std::string const& /*index*/)
{
    dst.transfer(dst.end(), src.begin(), src.end(), src);
}

void ThorsAnvil::Parser::mergeParserDom(ParserMap& dst, ParserMap& src, std::string const& errorMsg)
{
    ::mergeParserDom(dst, src, errorMsg, "root");
}

void ThorsAnvil::Parser::mergeParserDom(ParserArray& dst, ParserArray& src, std::string const& errorMsg)
{
    ::mergeParserDom(dst, src, errorMsg, "root");
}



