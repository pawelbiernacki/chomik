#include "chomik.h"

void chomik::variable_with_value_code::report(std::ostream & s) const
{
    actual_name->report(s); 
    s << "= value code {";
    value->report(s);
    s << "}\n";
}


void chomik::variable_value_name_item::report(std::ostream & s) const
{
    s << '<';
    name->report(s);
    s << '>';
}


void chomik::type_definition_statement::report(std::ostream & s) const
{
    bool first = true;
    s << "type ";
    for (auto & i: vector_of_type_definitions)
    {
        if (!first) s << ',';
        i->report(s);
        first = false;
    }
    s << ';';
}

void chomik::execute_value_statement::report(std::ostream & s) const
{
    s << "execute ";
    value->report(s);
    s << ';';
}

void chomik::execute_variable_value_statement::report(std::ostream & s) const
{
    s << "execute <";
    name->report(s);
    s << ">;";
}

void chomik::type_definition::report(std::ostream & s) const
{
    s << name << "=";
    body->report(s);
    s << "";
}

void chomik::type_definition_body_enum::report(std::ostream & s) const
{
    bool first = true;
    s << '{';
    for (auto & i: vector_of_names)
    {
        if (!first) s << ',';
        i->report(s);
        first = false;
    }
    s << '}';
}        


void chomik::generic_name::report(std::ostream & s) const
{
    for (auto & i: vector_of_name_items)
    {
        i->report(s);
        s << ' ';
    }
}


void chomik::variable_definition_statement::report(std::ostream & s) const
{
    bool first = true;
    s << "variable ";
    for (auto & i: vector_of_variable_definitions)
    {
        if (!first) s << ',';
        i->report(s);
        first = false;
    }
    s << ';';    
}

void chomik::assignment_statement::report(std::ostream & s) const
{
    s << "let variable ";
    name->report(s);
    s << '=';
    value->report(s);
    s << ';';
}

void chomik::generic_range_boundary_variable_value::report(std::ostream & s) const
{
    s << '<';
    name->report(s);
    s << '>';
}

 

void chomik::signature::report(std::ostream & s) const
{
    for (auto & i: vector_of_items)
    {
        i->report(s);
        s << " ";
    }
}

void chomik::machine::report(std::ostream & s) const
{
    s << "machine:\n";
    s << "type instances:\n";
    for (auto & i: vector_of_type_instances)
    {
        i->report(s);
    }
    s << "memory:\n";
    for (auto & i: memory)
    {
        i->report(s);
    }
}

void chomik::generic_literal_code::report(std::ostream & s) const
{
    s << "{";
    my_code_pointer->report(s);
    s << '}';
}


void chomik::generic_literal_placeholder::report(std::ostream & s) const
{
    s << "[(" << placeholder << ':' << *type_name << ")]";
}


void chomik::generator::report(std::ostream & s) const
{
    bool first = true;
    
    s << "created in " << my_filename << " line " << line_number << ":";
    
    for (auto & i: vector_of_placeholders)
    {
        if (!first) s << " x ";
        s << i->get_name();
        first = false;
    }
    
    if (get_the_cartesian_product_of_placeholder_types_is_empty())
    {
        s << ", the cartesian product is empty";
    }
    else
    if (get_the_cartesian_product_of_placeholder_types_has_one_item())
    {
        s << ", the cartesian product has one item";
    }
    else    
    if (get_the_cartesian_product_of_placeholder_types_is_finite())
    {
        s << ", the cartesian product is finite ";
    }
    if (memory.size() > 0)
    {
        s << " ";
        s << "placeholders memory:";
        for (auto & i: memory)
        {
            i->report(s);
            s << " ";
        }
    }
    if (auto o=my_father.lock())
    {
        s << "(father: " << *o << ")";        
    }
}

void chomik::description_of_a_cartesian_product::report(std::ostream & s) const
{
    s << "cartesian product ";    
    for (auto & a: vector_of_dimensions)
    {
        a->report(s);
        s << " ";
    }
    s << " ";
}

void chomik::assignment_event::report(std::ostream & s) const
{
    s << "assignment_event " << *my_assignment_target << " for ";
    my_cartesian_product->report(s);    
    s << " with " << *my_assignment_source;
}

void chomik::cartesian_product_dimension::report(std::ostream & s) const
{
    s << placeholder << ":" << type_name;
}
 
void chomik::assignment_source_literal_value::report(std::ostream & s) const
{
    s << "literal ";
    my_value->report(s);
}

void chomik::assignment_source_code_pattern::report(std::ostream & s) const
{
    s << "code pattern";
}

void chomik::assignment_source_variable_value::report(std::ostream & s) const
{
    s << "variable value";
}

void chomik::mapping_generator::report(std::ostream & s) const
{
    s << "created in " << my_filename << " line " << line_number << ":";

    for (auto i=map_child_placeholder_to_father_placeholder.begin(); i!=map_child_placeholder_to_father_placeholder.end(); i++)
    {
        s << i->first << "->" << i->second << " ";
    }
    
    if (auto o = my_father.lock())
    {
        s << " (";
        o->report(s);
        s << ")";
    }
}


void chomik::matching_protocol::report(std::ostream & s) const
{
    s << "matching protocol ";
    
    for (auto a=map_placeholder_names_to_integer.begin(); a!=map_placeholder_names_to_integer.end(); a++)
    {
        s << a->first << "-> integer " << a->second << " ";
    }
    for (auto a=map_placeholder_names_to_string.begin(); a!=map_placeholder_names_to_string.end(); a++)
    {
        s << a->first << "-> string " << a->second << " ";
    }
    for (auto a=map_placeholder_names_to_code.begin(); a!=map_placeholder_names_to_code.end(); a++)
    {
        s << a->first << "-> code " << a->second << " ";
    }
    for (auto a=map_placeholder_names_to_placeholder_names.begin(); a!=map_placeholder_names_to_placeholder_names.end(); a++)
    {
        s << a->first << "-> placeholder " << a->second << " ";
    }
}


void chomik::external_placeholder_generator::report(std::ostream & s) const
{
    s << "created in " << my_filename << " line " << line_number << ":";
    
    for (auto i=map_placeholder_names_to_placeholders_with_value.begin(); i!=map_placeholder_names_to_placeholders_with_value.end(); i++)
    {
        s << i->first << "->" << *i->second << " ";
    }

    if (auto o = my_father.lock())
    {
        s << " (";
        o->report(s);
        s << ")";
    }    
}

void chomik::code_name_item::report(std::ostream & s) const
{
    my_code->report(s);
}

void chomik::name_item_code::report(std::ostream & s) const
{
    my_code_pointer->report(s);
}

void chomik::code_signature_item::report(std::ostream & s) const
{
    my_code->report(s);
}

void chomik::code::report(std::ostream & s) const
{
    body->report(s);
}

void chomik::type_instance_enum::report(std::ostream & s) const
{
    bool first = true;
    s << "type instance " << name << "={";
    for (auto & i: vector_of_values)
    {
        if (!first) s << ',';
        s << i->get_name();
        first = false;
    }
    s << "};\n";
}

void chomik::type_instance_range::report(std::ostream & s) const
{
    s << "type instance " << name << '=' << min_boundary << ".." << max_boundary << ";\n";
}

void chomik::type_instance_ad_hoc_range::report(std::ostream & s) const
{
    s << "type instance " << name << "[" << my_type.get_generic_type_name() << "]" << '=' << min_boundary << ".." << max_boundary << ";\n";
}

void chomik::list_of_statements::report(std::ostream & s) const
{
    for (auto & i: vector_of_statements)
    {
        i->report(s);
        if (is_main)
        {
            s << '\n';
        }
    }
}

