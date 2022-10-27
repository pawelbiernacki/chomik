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
    
    if (argc < 2)
    {
        std::cerr << "usage:    " << argv[0] << " [-p|--program-report] [-v|--version] [-m|--memory-report] <filename>\n";
        return_value = 1;
    }
    else
    {
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
            {
                filename = std::string(argv[i]);
            }
        }
        
        if (version_flag)
        {
            std::cout << "chomik by Pawel Biernacki, Vantaa 2022\n";
            std::cout << "version " << PACKAGE_VERSION << "\n";
        }
        else
        if (program_report_flag)    // only parse, don't execute
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
        else
        {
            if (the_parser.parse(filename.c_str()) == 0)
            {
                chomik::machine m;
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
            }
            else
            {
                return_value = 1;
            }
        }
    }
    return return_value;
}
