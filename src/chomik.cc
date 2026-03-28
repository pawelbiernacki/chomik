#include "chomik.h"
#include "config.h"
#include <cstring>

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

chomik::program the_program;
chomik::parser the_parser{the_program};

int main(int argc, char * argv[])
{
    int return_value = 0;
    int i;
    bool program_report_flag = false, version_flag=false, memory_report_flag=false;
    std::string filename="";
	std::vector<std::string> vector_of_chomik_string_values;
	bool file_provided = false, file_must_be_provided = true;

    chomik::parser::register_parser(&the_parser);
    
    if (argc < 2)
    {
        std::cerr << "usage:    " << argv[0] << " "
        << "[-p|--program-report] [-v|--version] "
		<< "[-m|--memory-report] "
		<< "[-s <integer> <string> | --set-the-chomik-string <integer> <string>] "
		<< "[-e <string> | --execute <string>] <filename>\n";
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

					the_program.execute(m);

					if (memory_report_flag)
						m.report(std::cout);

					chomik::generic_name gn;
					gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
					gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("program"));
					gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("return"));
					chomik::signature s0{gn};

					return_value = m.get_variable_value_integer(s0);

					if (return_value != 0)
					{
						//std::cout << "the program return is " << return_value << "\n";
						return return_value;
					}

					the_program.clear();
					the_parser.reset();

					file_must_be_provided = false;
				}
				else
				{
					std::cerr << "failed to parse the command\n";
					return -1;
				}
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

				//std::cout << "new we execute the program on the machine\n";

                the_program.execute(m);
                
                if (memory_report_flag)
                    m.report(std::cout);            
            
                chomik::generic_name gn;
                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("program"));
                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("return"));
                chomik::signature s0{gn};
                        
                return_value = m.get_variable_value_integer(s0);                                    
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
