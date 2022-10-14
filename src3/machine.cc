#include "http_chomik.h"


void http_chomik::machine::create_predefined_streams()
{
    // we do not call the original implementation here!
    // we want our streams to replace the original ones!
        
    add_stream(std::make_unique<chomik::generic_stream_stringstream>());
    add_stream(std::make_unique<chomik::generic_stream_stringstream>());
    add_stream(std::make_unique<chomik::generic_stream_stringstream>());        
}
 
