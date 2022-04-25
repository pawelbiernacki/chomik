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
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <filename>\n";
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
        }
    }
    return 0;
}
