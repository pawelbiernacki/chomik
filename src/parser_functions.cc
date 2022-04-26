#include "chomik.h"

extern chomik::parser the_parser;


extern "C" void * chomik_create_generic_type_named(const char * const type_name)
{
    return new chomik::generic_type_named(type_name);
}

extern "C" void * chomik_create_generic_range_boundary_int_literal(int i)
{
    return new chomik::generic_range_boundary_int_literal(i);
}

extern "C" void * chomik_create_generic_range_boundary_variable_value(void * const gn)
{
    return new chomik::generic_range_boundary_variable_value(static_cast<chomik::generic_name*>(gn));
}

extern "C" void * chomik_create_generic_name(void * const l)
{
    return new chomik::generic_name(static_cast<chomik::list_of_generic_name_items*>(l));
}

extern "C" void * chomik_create_type_definition(const char * const type_name, void * t)
{
    return new chomik::type_definition(type_name, static_cast<chomik::type_definition_body*>(t));
}

extern "C" void chomik_copy_list_of_statements_to_the_program(void * const l)
{
    the_parser.copy_list_of_statements_to_the_program(static_cast<chomik::list_of_statements*>(l));    
}

extern "C" void chomik_destroy_list_of_statements(void * const l)
{
    delete static_cast<chomik::list_of_statements*>(l);
}

extern "C" void chomik_destroy_list_of_type_definitions(void * const l)
{
    delete static_cast<chomik::list_of_type_definitions*>(l);
}

extern "C" void chomik_destroy_list_of_names(void * const l)
{
    delete static_cast<chomik::list_of_generic_names*>(l);
}

extern "C" void chomik_destroy_list_of_name_items(void * const l)
{
    delete static_cast<chomik::list_of_generic_name_items*>(l);
}

extern "C" void chomik_destroy_list_of_variable_definitions(void * const l)
{
    delete static_cast<chomik::list_of_variable_definitions*>(l);
}


extern "C" void chomik_destroy_name(void * const l)
{
    delete static_cast<chomik::generic_name*>(l);
}

extern "C" void * chomik_create_type_definition_body_enum(void * const l)
{
    return new chomik::type_definition_body_enum(static_cast<chomik::list_of_generic_names*>(l));
}

extern "C" void * chomik_create_list_of_type_definitions(void * const td, void * const l)
{
    return new chomik::list_of_type_definitions(static_cast<chomik::type_definition*>(td), static_cast<chomik::list_of_type_definitions*>(l));
}

extern "C" void * chomik_create_generic_range(void * const l, void * const r)
{
    return new chomik::generic_range(static_cast<chomik::generic_range_boundary*>(l), static_cast<chomik::generic_range_boundary*>(r));
}

extern "C" void * chomik_create_type_definition_statement(void * const l, unsigned new_line_number)
{
    return new chomik::type_definition_statement(static_cast<chomik::list_of_type_definitions*>(l), new_line_number);
}

extern "C" void * chomik_create_list_of_generic_names(void * const first_name, void * const other_names)
{
    return new chomik::list_of_generic_names(static_cast<chomik::generic_name*>(first_name), static_cast<chomik::list_of_generic_names*>(other_names));
}

extern "C" void * chomik_create_name_item_float(double d)
{
    return new chomik::name_item_float(d);
}

extern "C" void * chomik_create_name_item_string(const char * const s)
{
    return new chomik::name_item_string(s);
}

extern "C" void * chomik_create_name_item_int(int i)
{
    return new chomik::name_item_integer(i);
}

extern "C" void * chomik_create_name_item_placeholder(const char * const placeholder, void * const gt)
{
    return new chomik::placeholder_name_item(placeholder, static_cast<chomik::generic_type*>(gt));
}

extern "C" void * chomik_create_name_item_identifier(const char * const identifier)
{
    return new chomik::identifier_name_item(identifier);
}

extern "C" void * chomik_create_name_item_variable_value(void * const l)
{
    return new chomik::variable_value_name_item(static_cast<chomik::list_of_generic_name_items*>(l));
}


extern "C" void * chomik_create_expand_statement(int d, unsigned new_line_number)
{
    return new chomik::expand_statement(d, new_line_number);
}

extern "C" void * chomik_create_assignment_statement(void * const t, void * const v, unsigned new_line_number)
{
    return new chomik::assignment_statement(static_cast<chomik::generic_name*>(t), static_cast<chomik::generic_value*>(v), new_line_number);
}

extern "C" void * chomik_create_code_literal(void * const l)
{
    static_cast<chomik::list_of_statements*>(l)->set_is_main(false);    // this is useful to know for pretty printing
    auto cl = new chomik::generic_literal_code(static_cast<chomik::list_of_statements*>(l));    
    //cl->report(std::cout);
    return cl;
}

extern "C" void * chomik_create_list_of_statements(void * const s, void * const l)
{
    return new chomik::list_of_statements(static_cast<chomik::statement*>(s), static_cast<chomik::list_of_statements*>(l));
}


extern "C" void * chomik_create_list_of_name_items(void * const i, void * const l)
{
    return new chomik::list_of_generic_name_items(static_cast<chomik::generic_name_item*>(i), static_cast<chomik::list_of_generic_name_items*>(l));
}

extern "C" void * chomik_create_float_literal(double v)
{
    return new chomik::simple_literal_float(v);
}

extern "C" void * chomik_create_integer_literal(int i)
{
    return new chomik::simple_literal_integer(i);
}

extern "C" void * chomik_create_string_literal(const char * const v)
{
    return new chomik::simple_literal_string(v);
}


extern "C" void * chomik_create_enum_literal(const char * const type_name, void * const n)
{
    return new chomik::generic_literal_enum(type_name, static_cast<chomik::generic_name*>(n));
}


extern "C" void * chomik_create_value_placeholder(const char * const placeholder, void * const type_name)
{
    return new chomik::generic_value_placeholder(placeholder, static_cast<chomik::generic_type*>(type_name));
}

extern "C" void * chomik_create_value_variable_value(void * const gn)
{
    return new chomik::generic_value_variable_value(static_cast<chomik::generic_name*>(gn));
}

extern "C" void * chomik_create_variable_definition_statement(void * const l, unsigned new_line_number)
{
    return new chomik::variable_definition_statement(static_cast<chomik::list_of_variable_definitions*>(l), new_line_number);
}

extern "C" void * chomik_create_list_of_variable_definitions(void * const vd, void * const l)
{
    return new chomik::list_of_variable_definitions(static_cast<chomik::variable_definition*>(vd), static_cast<chomik::list_of_variable_definitions*>(l));
}

extern "C" void * chomik_create_variable_definition(void * const n, void * const type_name)
{
    std::shared_ptr<chomik::generic_name> xn{static_cast<chomik::generic_name*>(n)};
    std::shared_ptr<chomik::generic_type> yn{static_cast<chomik::generic_type*>(type_name)};
    return new chomik::variable_definition(std::move(xn), std::move(yn));
}

extern "C" void * chomik_create_generic_type_range(void * const r)
{
    return new chomik::generic_type_range(static_cast<chomik::generic_range*>(r));
}

extern "C" void * chomik_create_type_definition_body_range(void * const r)
{
    return new chomik::type_definition_body_range(static_cast<chomik::generic_range*>(r));
}

extern "C" void * chomik_create_value_literal(void * const l)
{
    return new chomik::generic_value_literal(static_cast<chomik::generic_literal*>(l));
}

extern "C" void * chomik_create_execute_variable_value_statement(void * const l, unsigned new_line_number)
{
    std::unique_ptr<chomik::generic_name> xn{static_cast<chomik::generic_name*>(l)};
    return new chomik::execute_variable_value_statement(std::move(xn), new_line_number);
}

extern "C" void * chomik_create_execute_value_statement(void * const l, unsigned new_line_number)
{
    std::unique_ptr<chomik::generic_value> xn{static_cast<chomik::generic_value*>(l)};
    return new chomik::execute_value_statement(std::move(xn), new_line_number);
}

extern "C" void * chomik_create_integer_literal_placeholder(const char * const p, void * const l)
{
    return new chomik::generic_literal_placeholder(p, static_cast<chomik::generic_type*>(l), chomik::variable_with_value::actual_memory_representation_type::INTEGER);
}

extern "C" void * chomik_create_enum_literal_placeholder(const char * const p, void * const l, const char * const t)
{
    return new chomik::generic_literal_placeholder(p, static_cast<chomik::generic_type*>(l), chomik::variable_with_value::actual_memory_representation_type::ENUM);
}

extern "C" void * chomik_create_string_literal_placeholder(const char * const p, void * const l)
{
    return new chomik::generic_literal_placeholder(p, static_cast<chomik::generic_type*>(l), chomik::variable_with_value::actual_memory_representation_type::STRING);
}

extern "C" void * chomik_create_variable_value(void * const l)
{
    return new chomik::generic_value_variable_value(static_cast<chomik::generic_name*>(l));
}
