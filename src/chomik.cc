#include "chomik.h"

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
    
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <filename>\n";
        return_value = 1;
    }
    else
    {
        if (the_parser.parse(argv[1]) == 0)
        {            
            chomik::machine m;
            m.create_predefined_types();
            m.create_predefined_variables();
            m.create_predefined_streams();
            the_program.execute(m);
            //the_program.report(std::cout);
            //m.report(std::cout);
            
            
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
    return return_value;
}
