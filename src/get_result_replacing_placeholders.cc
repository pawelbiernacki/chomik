#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

void chomik::placeholder_name_item::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("placeholder_name_item::get_result_replacing_placeholders");
    
    if (p.replace_known_placeholders_with_their_values() && g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_generic_name_item(std::make_shared<name_item_integer>(g.get_placeholder_value_integer(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_generic_name_item(std::make_shared<name_item_float>(g.get_placeholder_value_float(placeholder)));
                break;                                
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_generic_name_item(std::make_shared<name_item_string>(g.get_placeholder_value_string(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_generic_name_item(std::make_shared<identifier_name_item>(g.get_placeholder_value_enum(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::CODE:
                {
                code c;
                g.get_placeholder_value_code(placeholder, c);

                DEBUG("got code " << c);

                target.add_generic_name_item(std::make_shared<code_name_item>(c));
                }
                break;
        }
    }
    else
    {
        target.add_generic_name_item(std::make_shared<placeholder_name_item>(placeholder, *type_name));
    }
}

void chomik::code::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("code::get_result_replacing_placeholders");

    code c;
    body->get_actual_code_value(m, g, p, c);
    std::shared_ptr<code_name_item> cp=std::make_shared<code_name_item>(c);
    target.add_generic_name_item(std::move(cp));
}


void chomik::generic_literal_code::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("generic_literal_code::get_result_replacing_placeholders");
    my_code_pointer->get_result_replacing_placeholders(m, g, p, target);

    DEBUG("got " << target);
}

void chomik::variable_value_name_item::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    generic_name gn;

    DEBUG("variable_value_name_item::get_result_replacing_placeholders " << *this);

    name->get_result_replacing_placeholders(m, g, p, gn);
    
    DEBUG("after the placeholders are replaced - got " << gn);
    
    signature s{gn, m, g};
    
    DEBUG("the signature is " << s);
    
    if (p.replace_all_variables_with_their_values() && m.get_variable_is_represented_in_memory(s))
    {
        DEBUG("it is represented in memory");
        
        switch (m.get_actual_memory_representation_type_of_the_variable(s))
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_generic_name_item(std::make_shared<name_item_integer>(m.get_variable_value_integer(s)));
                break;
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_generic_name_item(std::make_shared<name_item_float>(m.get_variable_value_float(s)));
                break;
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_generic_name_item(std::make_shared<name_item_string>(m.get_variable_value_string(s)));                
                break;
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_generic_name_item(std::make_shared<identifier_name_item>(m.get_variable_value_enum(s)));
                break;                
            case variable_with_value::actual_memory_representation_type::CODE:
                break;                
        }
    }
    else
    {
        //std::cout << "it is not represented in memory - check families of variables\n";
        // TODO - add_generic_name_item
        DEBUG("the variable is not represented in memory or the policy forbids replacing it with its value");
                
        target.add_generic_name_item(std::make_shared<variable_value_name_item>(*name));
    }
    
    
}



void chomik::generic_value_placeholder::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("hi");
    
    if (p.replace_known_placeholders_with_their_values() && g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_generic_name_item(std::make_shared<name_item_integer>(g.get_placeholder_value_integer(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_generic_name_item(std::make_shared<name_item_float>(g.get_placeholder_value_float(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_generic_name_item(std::make_shared<name_item_string>(g.get_placeholder_value_string(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_generic_name_item(std::make_shared<identifier_name_item>(g.get_placeholder_value_enum(placeholder)));
                break;

            case variable_with_value::actual_memory_representation_type::CODE:
                {
                code c;
                g.get_placeholder_value_code(placeholder, c);
                target.add_generic_name_item(std::make_shared<code_name_item>(c));
                }
                break;
                
        }
    }
    else
    {
        //std::cout << "there is no such placeholder!!!\n";
        // TODO - add_generic_name_item
        throw std::runtime_error("the placeholder is not known");
        
    }
}

void chomik::generic_literal_placeholder::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("get result for placeholder " << placeholder);
    
    if (p.replace_known_placeholders_with_their_values() && g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_generic_name_item(std::make_shared<name_item_integer>(g.get_placeholder_value_integer(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_generic_name_item(std::make_shared<name_item_float>(g.get_placeholder_value_float(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_generic_name_item(std::make_shared<name_item_string>(g.get_placeholder_value_string(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_generic_name_item(std::make_shared<identifier_name_item>(g.get_placeholder_value_enum(placeholder)));
                break;

            case variable_with_value::actual_memory_representation_type::CODE:
                
                break;
                
        }
    }
    else
    {
        throw std::runtime_error("the placeholder is not known");
    }
}

void chomik::generic_name::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("generic_name::get_result_replacing_placeholders");
    for (auto & i: vector_of_name_items)
    {
        i->get_result_replacing_placeholders(m, g, p, target);
    }
}

void chomik::generic_value_variable_value::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    DEBUG("hi");
    //std::cout << "generic_value_variable_value\n";
    //report(std::cout);
    //std::cout << "\n";
    
    generic_name n;
                        
    name->get_result_replacing_placeholders(m, g, p, n);
            
    //std::cout << "found variable name ";
    //n.report(std::cout);
    //std::cout << "\n";
    
    signature s{n, m, g};
    
    /*
    std::cout << "its signature is ";
    s.report(std::cout);
    std::cout << "\n";
    */
            
    if (m.get_variable_is_represented_in_memory(s))
    {        
        //std::cout << "it is represented in memory\n";
        
        switch (m.get_actual_memory_representation_type_of_the_variable(s))
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_generic_name_item(std::make_shared<name_item_integer>(123));
                break;
            
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_generic_name_item(std::make_shared<name_item_float>(1.2));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_generic_name_item(std::make_shared<name_item_string>("abc"));
                break;
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_generic_name_item(std::make_shared<name_item_string>("abc"));
                break;
                
            case variable_with_value::actual_memory_representation_type::CODE:
                //std::cout << "this is a code\n";
                break;
        }
        
    }
    else
    {
        // TODO - check the families of variables
    }
}


void chomik::code_name_item::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    // TODO implement me
}
