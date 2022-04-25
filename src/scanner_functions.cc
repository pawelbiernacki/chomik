#include "chomik.h"

std::vector<std::pair<const char*, unsigned>> chomik_vector_of_buffer_data;

extern "C" void chomik_push_buffer_data(const char * const filename, unsigned line_number)
{
    chomik_vector_of_buffer_data.push_back(std::pair(filename, line_number));
}

extern "C" void chomik_pop_buffer_data(const char ** filename_ptr, unsigned * line_number_ptr)
{
    if (chomik_vector_of_buffer_data.size()>0)
    {
        *filename_ptr = chomik_vector_of_buffer_data[chomik_vector_of_buffer_data.size()-1].first;
        *line_number_ptr = chomik_vector_of_buffer_data[chomik_vector_of_buffer_data.size()-1].second;
    }
}
 
