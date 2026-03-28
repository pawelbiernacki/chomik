#include "chomik.h"
#include <cstring>

namespace chomik
{
	namespace scanner_internal
	{
		class chomik_code_info
		{
		public:
			std::string filename;
			unsigned line_number;
			bool it_is_a_file;

			chomik_code_info(std::string new_filename, unsigned new_line_number):
				filename{new_filename},
				line_number{new_line_number},
				it_is_a_file{true} {}

			chomik_code_info():
				it_is_a_file{false} {}
		};
	}
}

std::vector<chomik::scanner_internal::chomik_code_info> chomik_vector_of_buffer_data;
extern char chomik_current_file[];
extern bool chomik_it_is_a_file;

extern "C" void chomik_push_buffer_data_file(const char * const filename, unsigned line_number)
{
	if (filename == nullptr)
	{
		*chomik::parser::current_error_stream << "chomik_push_buffer_data failed at line " << line_number << "\n";
		return;
	}

	chomik::scanner_internal::chomik_code_info entry{std::string(filename), line_number};

	//std::cout << "push chomik_code_info " << entry.filename << " at " << line_number << "\n";

    chomik_vector_of_buffer_data.push_back(entry);
    strncpy(chomik_current_file, filename, 1023);
	chomik_it_is_a_file = true;
}


extern "C" void chomik_push_buffer_data_string()
{
	chomik::scanner_internal::chomik_code_info entry;

	chomik_vector_of_buffer_data.push_back(entry);
	strncpy(chomik_current_file, "it is not a file", 1023);

	chomik_it_is_a_file = false;
}


extern "C" void chomik_pop_buffer_data(const char ** filename_ptr, unsigned * line_number_ptr, bool * it_is_a_file)
{
    if (chomik_vector_of_buffer_data.size()>0)
    {
		int index = chomik_vector_of_buffer_data.size()-1;

		if (chomik_vector_of_buffer_data[index].it_is_a_file)
		{
			strncpy(chomik_current_file, chomik_vector_of_buffer_data[index].filename.c_str(), 1023);
			*filename_ptr = chomik_current_file;
			*line_number_ptr = chomik_vector_of_buffer_data[index].line_number;
			*it_is_a_file = true;
		}
		else
		{
			*filename_ptr = chomik_current_file;
			*line_number_ptr = 1;
			strncpy(chomik_current_file, "it is not a file", 1023);
			*it_is_a_file = false;
		}
        chomik_vector_of_buffer_data.pop_back();
    }
}
 
extern "C" void chomik_reset_vector_of_buffer_data()
{
	chomik_vector_of_buffer_data.clear();
}
