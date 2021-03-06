#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

void chomik::statement::get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const
{
    std::shared_ptr<statement> c;
    make_copy_with_replacements(m, g, p, c);
    target.add_statement(std::move(c));
}

void chomik::list_of_statements::get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const
{
    //DEBUG("list_of_statements::get_actual_code_value");
    for (auto & i: vector_of_statements)
    {
        i->get_actual_code_value(m, g, p, target);
    }
}

void chomik::code::get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const
{
    body->get_actual_code_value(m, g, p, target);
}

void chomik::generic_literal_code::get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const
{
    //std::cout << "we get the actual code value\n";
    
    my_code_pointer->get_actual_code_value(m, g, p, target);    
}

 
