#include "chomik.h"
#include <cstring>

std::vector<std::pair<std::string, unsigned>> chomik_vector_of_buffer_data;
extern char chomik_current_file[];

extern "C" void chomik_push_buffer_data(const char * const filename, unsigned line_number)
{
    chomik_vector_of_buffer_data.push_back(std::pair(std::string(filename), line_number));
    strncpy(chomik_current_file, filename, 1023);
}

extern "C" void chomik_pop_buffer_data(const char ** filename_ptr, unsigned * line_number_ptr)
{
    if (chomik_vector_of_buffer_data.size()>0)
    {
        *filename_ptr = chomik_current_file;
                        
        strncpy(chomik_current_file, chomik_vector_of_buffer_data[chomik_vector_of_buffer_data.size()-1].first.c_str(), 1023);
                
        *line_number_ptr = chomik_vector_of_buffer_data[chomik_vector_of_buffer_data.size()-1].second;
        chomik_vector_of_buffer_data.pop_back();
    }
}
 
