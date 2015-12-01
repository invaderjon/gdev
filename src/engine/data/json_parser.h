// json_parser.h
#ifndef INCLUDED_JSON_PARSER
#define INCLUDED_JSON_PARSER
#include "json_entity.h"

namespace StevensDev
{

namespace sgdd
{

struct JsonParser
{
    static JsonEntity* fromString(
        const std::string& rawJson,
        sgdm::IAllocator<JsonEntity>* allocator = nullptr );
      // Constructs a json entity hierarchy from a given json string.
      //
      // Throws runtime_error if:
      // raw json string is invalid

    static JsonEntity* fromFile(
        const std::string& path,
        sgdm::IAllocator<JsonEntity>* allocator = nullptr );
      // Constructs a json entity hierarchy from a file located at the given
      // path.
      //
      // Throws runtime_error if:
      // raw json data is invalid
};

} // End nspc sgdd

} // End nspc StevensDev

#endif
