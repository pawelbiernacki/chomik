#include "chomik.h"
#include "config.h"
#include <cstring>

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif


enum class execution_mode_enum { UNKNOWN, COMMON, INSTANT };
enum class source_type_enum { UNKNOWN, FILE, STRING };

std::unique_ptr<chomik::machine> common_machine;
std::vector<std::string> vector_of_chomik_string_values;


int execute_the_program_on_the_machine(chomik::parser & my_parser, chomik::program & my_program, chomik::machine & m, bool memory_report_flag)
{
	int return_value = 0;

	my_program.execute(m);

	if (memory_report_flag)
		m.report(std::cout);

	chomik::generic_name gn;
	gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
	gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("program"));
	gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("return"));
	chomik::signature s0{gn};

	return_value = m.get_variable_value_integer(s0);

	my_program.clear();
	my_parser.reset();

	return return_value;
}

int parse_the_source_and_execute_the_code(chomik::parser & my_parser, chomik::program & my_program, execution_mode_enum execution_mode, source_type_enum source_type, const std::string &source, bool memory_report_flag)
{
	int return_value = 0;
	if (execution_mode == execution_mode_enum::COMMON && source_type == source_type_enum::FILE)
	{
		if (my_parser.parse(source.c_str()) == 0)
		{
			my_program.set_is_main(true);

			chomik::machine::current_error_stream = &std::cerr;

			if (!common_machine.get())
			{
				//std::cout << "creating the common machine\n";
				common_machine = std::make_unique<chomik::machine>(vector_of_chomik_string_values);
				common_machine->create_predefined_types();
				common_machine->create_predefined_variables();
				common_machine->create_predefined_streams();
			}

			return_value = execute_the_program_on_the_machine(my_parser, my_program, *common_machine, memory_report_flag);

			if (return_value != 0)
			{
				//std::cout << "the program return is " << return_value << "\n";
				my_program.clear();
				my_parser.reset();
				return return_value;
			}

			my_program.clear();
			my_parser.reset();
			return 0;
		}
		else
		{
			my_program.clear();
			my_parser.reset();
			std::cerr << "failed to parse the file\n";
			return -1;
		}
		my_program.clear();
		my_parser.reset();
		return 0;
	}
	else
	if (execution_mode == execution_mode_enum::COMMON && source_type == source_type_enum::STRING)
	{
		if (my_parser.parse_string(source) == 0)
		{
			my_program.set_is_main(true);

			chomik::machine::current_error_stream = &std::cerr;

			if (!common_machine.get())
			{
				//std::cout << "creating the common machine\n";
				common_machine = std::make_unique<chomik::machine>(vector_of_chomik_string_values);
				common_machine->create_predefined_types();
				common_machine->create_predefined_variables();
				common_machine->create_predefined_streams();
			}

			return_value = execute_the_program_on_the_machine(my_parser, my_program, *common_machine, memory_report_flag);

			if (return_value != 0)
			{
				//std::cout << "the program return is " << return_value << "\n";
				my_program.clear();
				my_parser.reset();
				return return_value;
			}

			my_program.clear();
			my_parser.reset();
			return 0;
		}
		else
		{
			my_program.clear();
			my_parser.reset();
			std::cerr << "failed to parse the string\n";
			return -1;
		}
		my_program.clear();
		my_parser.reset();
		return 0;
	}
	else
	if (execution_mode == execution_mode_enum::INSTANT && source_type == source_type_enum::FILE)
	{
		if (my_parser.parse(source.c_str()) == 0)
		{
			my_program.set_is_main(true);

			chomik::machine::current_error_stream = &std::cerr;

			chomik::machine m(vector_of_chomik_string_values);
			m.create_predefined_types();
			m.create_predefined_variables();
			m.create_predefined_streams();

			return_value = execute_the_program_on_the_machine(my_parser, my_program, m, memory_report_flag);

			if (return_value != 0)
			{
				my_program.clear();
				my_parser.reset();
				//std::cout << "the program return is " << return_value << "\n";
				return return_value;
			}
			my_program.clear();
			my_parser.reset();
			return 0;
		}
		else
		{
			my_program.clear();
			my_parser.reset();
			std::cerr << "failed to parse the file\n";
			return -1;
		}
		my_program.clear();
		my_parser.reset();
		return 0;
	}
	else
	if (execution_mode == execution_mode_enum::INSTANT && source_type == source_type_enum::STRING)
	{
		if (my_parser.parse_string(source) == 0)
		{
			my_program.set_is_main(true);

			chomik::machine::current_error_stream = &std::cerr;

			chomik::machine m(vector_of_chomik_string_values);
			m.create_predefined_types();
			m.create_predefined_variables();
			m.create_predefined_streams();

			return_value = execute_the_program_on_the_machine(my_parser, my_program, m, memory_report_flag);

			if (return_value != 0)
			{
				my_program.clear();
				my_parser.reset();
				//std::cout << "the program return is " << return_value << "\n";
				return return_value;
			}
		}
		else
		{
			my_program.clear();
			my_parser.reset();
			std::cerr << "failed to parse the string\n";
			return -1;
		}
		my_program.clear();
		my_parser.reset();
		return 0;
	}
	else
	{
		my_program.clear();
		my_parser.reset();
		return -1;
	}
	my_program.clear();
	my_parser.reset();
	return 0;
}


chomik::program the_program;
chomik::parser the_parser{the_program};

int main(int argc, char * argv[])
{
    int return_value = 0;
    int i;
    bool program_report_flag = false, version_flag=false, memory_report_flag=false;
    std::string filename="";
	bool file_provided = false, file_must_be_provided = true;

    chomik::parser::register_parser(&the_parser);
    
    if (argc < 2)
    {
        std::cerr << "usage:    " << argv[0] << " "
        << "[-p|--program-report] [-v|--version] "
		<< "[-m|--memory-report] "
		<< "[-s <integer> <string> | --set-the-chomik-string <integer> <string>] "
		<< "[-e <string> | --execute <string>] "
		<< "[-f (common|instant) (file <string>|string <string>) | --execute-for (common|instant) (file <string>|string <string>)] "
		<< "<filename>\n";
        return_value = 1;
    }
    else
    {
		vector_of_chomik_string_values.resize(chomik::machine::max_chomik_string_index);
		for (int i = 1; i<=chomik::machine::max_chomik_string_index; i++)
		{
			vector_of_chomik_string_values[i-1] = std::string("chomik");
		}


        for (i=1; i<argc; i++)
        {
            if (!strcmp(argv[i], "-v") || !strcmp(argv[i],"--version"))
            {
                version_flag=true;
            }
            else
            if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--program-report"))
            {                
                program_report_flag=true;
            }            
            else
            if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--memory-report"))
            {
                memory_report_flag=true;
            }
            else
			if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--execute"))
			{
				if (i >= argc)
				{
					std::cerr << "parameter expected after -e or --execute\n";
					return -1;
				}
				const char * p = argv[++i];

				if (p == nullptr)
				{
					std::cerr << "parameter expected after -e or --execute\n";
					return -1;
				}

				std::string code_to_be_executed(p);

				if (the_parser.parse_string(code_to_be_executed) == 0)
				{
					the_program.set_is_main(true);

					chomik::machine::current_error_stream = &std::cerr;
					chomik::machine m{vector_of_chomik_string_values};

					m.create_predefined_types();
					m.create_predefined_variables();
					m.create_predefined_streams();

					return_value = execute_the_program_on_the_machine(the_parser, the_program, m, memory_report_flag);

					if (return_value != 0)
					{
						//std::cout << "the program return is " << return_value << "\n";
						return return_value;
					}

					file_must_be_provided = false;
				}
				else
				{
					std::cerr << "failed to parse the command\n";
					return -1;
				}
			}
			else
			if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--execute-for"))
			{
				if (i >= argc)
				{
					std::cerr << "parameter (common|instant) expected after -f or --execute-for\n";
						return -1;
				}
				const char * execution_mode_char_ptr = argv[++i];

				if (execution_mode_char_ptr == nullptr)
				{
					std::cerr << "parameter (common|instant) expected after -f or --execute-for\n";
					return -1;
				}

				std::string execution_mode_name{execution_mode_char_ptr};
				execution_mode_enum execution_mode = execution_mode_enum::UNKNOWN;

				if (execution_mode_name == "common")
				{
					execution_mode = execution_mode_enum::COMMON;
				}
				else
				if (execution_mode_name == "instant")
				{
					execution_mode = execution_mode_enum::INSTANT;
				}
				else
				{
					std::cerr << "the command line option -f or --execute-for requires arguments: (common|instant) (file <string>|string <string>)\n";
					return -1;
				}

				if (i >= argc)
				{
					std::cerr << "parameter (file <string>|string <string>) expected after -f (common|instant) or --execute-for (common|instant)\n";
					return -1;
				}
				const char * source_type_char_ptr = argv[++i];
				if (source_type_char_ptr == nullptr)
				{
					std::cerr << "(file <string>|string <string>) expected after -f (common|instant) or --execute-for (common|instant)\n";
					return -1;
				}

				std::string source_type_name{source_type_char_ptr};
				source_type_enum source_type = source_type_enum::UNKNOWN;

				if (source_type_name == "file")
				{
					source_type = source_type_enum::FILE;
				}
				else
				if (source_type_name == "string")
				{
					source_type = source_type_enum::STRING;
				}
				else
				{
					std::cerr << "the command line option -f or --execute-for requires arguments: (common|instant) (file|string)  <string>\n";
					return -1;
				}

				if (i >= argc)
				{
					std::cerr << "parameter <string> expected after -f (common|instant) (file|string) or --execute-for (common|instant) (file|string)\n";
					return -1;
				}
				const char * source_char_ptr = argv[++i];

				if (source_char_ptr == nullptr)
				{
					std::cerr << "parameter <string> expected after -f (common|instant) (file|string) or --execute-for (common|instant) (file|string)\n";
					return -1;
				}

				std::string source{source_char_ptr};

				return_value = parse_the_source_and_execute_the_code(the_parser, the_program, execution_mode, source_type, source, memory_report_flag);

				if (return_value != 0)
				{
					//std::cout << "the program return is " << return_value << "\n";
					return return_value;
				}

				file_must_be_provided = false;
			}
			else
			if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--set-the-chomik-string"))
			{
				if (i >= argc)
				{
					std::cerr << "parameter expected after -s or --set-the-chomik-string\n";
					return -1;
				}
				int index = std::stoi(std::string(argv[++i]));

				//std::cout << "index = " << index << "\n";

				if (index >= 1 && index <= chomik::machine::max_chomik_string_index)
				{
					vector_of_chomik_string_values[index-1] = std::string(argv[++i]);

					//std::cout << "string " << (index - 1) << "=" << vector_of_chomik_string_values[index-1] << "\n";
				}
				else
				{
					std::cerr << "the index must be a value " << 1 << ".." << chomik::machine::max_chomik_string_index << "\n";
					return -1;
				}
			}
            else
            {
                filename = std::string(argv[i]);

				file_provided = true;

				//std::cout << "got filename " << filename << "\n";
            }
        }
        
        if (version_flag)
        {
            std::cout << "chomik by Pawel Biernacki, Vantaa 2022-2026\n";
            std::cout << "version " << PACKAGE_VERSION << "\n";
			return return_value;
        }



        if (program_report_flag)    // only parse, don't execute
        {
			if (file_must_be_provided && file_provided)
			{
				if (the_parser.parse(filename.c_str()) == 0)
				{
					the_program.report(std::cout);
				}
				else
				{
					return_value = 1;
				}
			}
        }
        else
        {
			//std::cout << "now we parse a file " << filename << "\n";

			if (file_provided)
			{
            if (the_parser.parse(filename.c_str()) == 0)
            {
				chomik::machine::current_error_stream = &std::cerr;

                chomik::machine m{vector_of_chomik_string_values};
				m.create_predefined_types();
				m.create_predefined_variables();
				m.create_predefined_streams();

				return_value = execute_the_program_on_the_machine(the_parser, the_program, m, memory_report_flag);
            }
            else
            {
				std::cerr << "failed to parse " << filename << "\n";
                return_value = -1;
            }
			}
			else
			if (file_must_be_provided && !file_provided)
			{
				std::cerr << "no file provided!\n";
				return_value = -1;
			}
        }
    }
    return return_value;
}
