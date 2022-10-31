#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif


void chomik::expand_statement::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const
{
    target = std::make_shared<expand_statement>(depth, line_number);
}

void chomik::execute_value_statement::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const
{
    std::unique_ptr<generic_literal_code> c = std::make_unique<generic_literal_code>();        
    
    DEBUG("original value " << *value);
    
    value->get_actual_code_value(m, g, p, c->get_code());
    
    DEBUG("got actual code " << c->get_code());
    
    std::unique_ptr<generic_literal> c2 = std::move(c);
    

    auto y = std::make_unique<generic_value_literal>(std::move(c2));
    auto x = std::make_shared<execute_value_statement>(std::move(y), line_number);    
    
    target = std::move(x);
}

void chomik::type_definition_statement::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const
{
    target = std::make_shared<type_definition_statement>(line_number);
}

void chomik::variable_definition_statement::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const
{
    target = std::make_shared<variable_definition_statement>(line_number);
}

void chomik::execute_variable_value_statement::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const
{
    auto x = std::make_shared<execute_variable_value_statement>(line_number);
    
    for (auto & i: name->get_vector_of_name_items())
    {      
        i->get_result_replacing_placeholders(m, g, p, *x->name);
    }
    target = std::move(x);
}

void chomik::assignment_statement::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const
{
    std::shared_ptr<generic_value> t;
    value->make_copy_with_replacements(m, g, p, t);
       
    auto x = std::make_shared<assignment_statement>(*t, line_number);
    
    for (auto & i: name->get_vector_of_name_items())
    {      
        i->get_result_replacing_placeholders(m, g, p, *x->name);
    }

    DEBUG("code line number " << line_number << ": got assignment " << *x);
    
    target = std::move(x);
}


void chomik::generic_value_variable_value::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const
{
    signature s{*name, m, g};
    
    if (p.replace_all_variables_with_their_values() && m.get_variable_is_represented_in_memory(s))
    {
        switch (m.get_actual_memory_representation_type_of_the_variable(s))
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target = std::make_shared<generic_value_literal>(std::make_unique<simple_literal_integer>(m.get_variable_value_integer(s)));
                break;
            case variable_with_value::actual_memory_representation_type::FLOAT:                
                target = std::make_shared<generic_value_literal>(std::make_unique<simple_literal_float>(m.get_variable_value_float(s)));
                break;
            case variable_with_value::actual_memory_representation_type::STRING:
                target = std::make_shared<generic_value_literal>(std::make_unique<simple_literal_string>(m.get_variable_value_string(s)));
                break;
            case variable_with_value::actual_memory_representation_type::ENUM:
                //target = std::make_shared<generic_value_literal>(std::make_unique<generic_literal_enum>("enum", m.get_variable_value_enum(s)));
                break;                
            case variable_with_value::actual_memory_representation_type::CODE:
                break;                
        }
    }
    else
    {
        target = std::make_shared<generic_value_variable_value>(*name);
    }
}

void chomik::generic_value_literal::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const
{
    std::unique_ptr<generic_literal> x;
    literal->make_copy_with_replacements(m, g, p, x);
    target = std::make_shared<generic_value_literal>(std::move(x));
}

void chomik::generic_literal_placeholder::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const
{
    if (p.replace_known_placeholders_with_their_values() && g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target = std::make_unique<simple_literal_integer>(g.get_placeholder_value_integer(placeholder));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target = std::make_unique<simple_literal_float>(g.get_placeholder_value_float(placeholder));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target = std::make_unique<simple_literal_string>(g.get_placeholder_value_string(placeholder));
                break;
                
            case variable_with_value::actual_memory_representation_type::ENUM:
            {
                generic_name gn;
                gn.add_generic_name_item(std::make_shared<identifier_name_item>(g.get_placeholder_value_enum(placeholder)));
                target = std::make_unique<generic_literal_enum>(type_name->get_type_name(m, g), gn);
            }
                break;
                
            case variable_with_value::actual_memory_representation_type::CODE:
                // TODO implement
                break;
        }
    }
    else
    {
        get_copy(target);
    }
}

void chomik::generic_literal_code::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const
{
    code c;
    my_code_pointer->get_actual_code_value(m, g, p, c);
    
    target = std::make_unique<generic_literal_code>(c);
}


void chomik::generic_value_placeholder::make_copy_with_replacements(const machine & m, basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const
{
    if (p.replace_known_placeholders_with_their_values() && g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target = std::make_shared<generic_value_literal>(std::make_unique<simple_literal_integer>(g.get_placeholder_value_integer(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target = std::make_shared<generic_value_literal>(std::make_unique<simple_literal_float>(g.get_placeholder_value_float(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target = std::make_shared<generic_value_literal>(std::make_unique<simple_literal_string>(g.get_placeholder_value_string(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                //target = std::make_shared<generic_value_literal>(std::make_unique<generic_literal_enum>(g.get_placeholder_value_enum(placeholder)));
                break;

            case variable_with_value::actual_memory_representation_type::CODE:
                
                break;
                
        }
    }
    else
    {
        std::unique_ptr<generic_value> t;
        get_copy(t);
        target = std::move(t);
    }
}
