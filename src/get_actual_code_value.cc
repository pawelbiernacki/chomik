#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

void chomik::statement::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    std::shared_ptr<statement> c;
    make_copy_with_replacements(m, g, p, c);
    
    DEBUG("got " << *c);
    
    target.add_statement(std::move(c));
}

void chomik::list_of_statements::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    DEBUG("list_of_statements::get_actual_code_value");
    for (auto & i: vector_of_statements)
    {
        i->get_actual_code_value(m, g, p, target);
    }
}

void chomik::code::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    DEBUG("code::get_actual_code_value");
    body->get_actual_code_value(m, g, p, target);
}

void chomik::generic_literal_code::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    //std::cout << "we get the actual code value\n";
    DEBUG("get actual code value from " << *my_code_pointer);

    my_code_pointer->get_actual_code_value(m, g, p, target);

    DEBUG("got " << target);
}

 
void chomik::assignment_source_variable_value::get_actual_code_value(const machine & m, const basic_generator & g, code & target) const
{
    replacing_policy_literal p;

    my_value->get_actual_code_value(m, g, p, target);

    DEBUG("got " << target);
}

void chomik::assignment_source_code_pattern::get_actual_code_value(const machine & m, const basic_generator & g, code & target) const
{
    replacing_policy_literal p;

    my_value->get_actual_code_value(m, g, p, target);

    DEBUG("got " << target);
}

void chomik::assignment_source_literal_value::get_actual_code_value(const machine & m, const basic_generator & g, code & target) const
{
    DEBUG("get_actual_code_value");

    replacing_policy_literal p;
    my_value->get_actual_code_value(m, g, p, target);

    DEBUG("got " << target);
}


void chomik::generic_value_variable_value::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    DEBUG("generic_value_variable_value::get_actual_code_value");
}

void chomik::generic_value_literal::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    DEBUG("generic_value_literal::get_actual_code_value");
    literal->get_actual_code_value(m, g, p, target);
}

void chomik::generic_literal_placeholder::get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const
{
    DEBUG("generic_literal_placeholder::get_actual_code_value");

    if (g.get_has_placeholder(placeholder))
    {
        if (g.get_has_placeholder_with_value(placeholder))
        {
            const placeholder_with_value& x{g.get_placeholder_with_value(placeholder)};
            if (x.get_representation_type() == variable_with_value::actual_memory_representation_type::CODE)
            {
                DEBUG("it is a code !!!");

                using pp = simple_placeholder_with_value_and_report<code, static_cast<int>(variable_with_value::actual_memory_representation_type::CODE)>;
                const pp& x2{static_cast<const pp&>(x)};

                target = x2.get_value();
            }
        }
    }
}
