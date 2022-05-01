#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

std::ostream & operator<<(std::ostream & s, const chomik::assignment_source & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::assignment_event & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::type_definition & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::type_definition_body & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::generic_type & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::signature_item & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::statement & x)
{
    x.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::generic_value & v)
{
    v.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::code & c)
{
    c.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::generic_name & n)
{
    n.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::generator & g)
{
    g.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::generic_name_item & i)
{
    i.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::signature & x)
{
    x.report(s);
    return s;
}



int chomik::generic_stream_random_number_stream::read_integer()
{
    int r = dist(rng);
    return r;
}

const std::string chomik::predefined_types::array_of_predefined_types[]=
{
    "integer", "float", "string", "code"
};

const std::string chomik::predefined_variables::array_of_predefined_variables[]=
{
    "print", "create", "get", "read", "compare", "add", "substract", "the"
};

const std::vector<std::unique_ptr<chomik::type_instance_enum_value>>::const_iterator chomik::type_instance::dummy;
const std::vector<std::shared_ptr<chomik::type_definition>> chomik::statement::dummy;

void chomik::assignment_statement::get_copy(std::shared_ptr<statement> & target) const
{
    DEBUG("code line number " << line_number << ": " << *this << " - get_copy");    
    target = std::make_shared<assignment_statement>(*name, *value, line_number);
    
    DEBUG("code line number " << line_number << ": got " << *target);
}


chomik::assignment_statement::assignment_statement(const generic_name & gn, const generic_value & gv, unsigned new_line_number): name{std::make_unique<generic_name>(gn)}, statement{new_line_number}
{
    std::unique_ptr<generic_value> i;
    gv.get_copy(i);
    value = std::move(i);
}

void chomik::variable_with_value_code::get_value_code(code & target) const
{
    DEBUG("copying:" << *value);
    
    for (auto & i: value->get_body().get_vector_of_statements())
    {
        std::shared_ptr<statement> s;
        i->get_copy(s);
        target.add_statement(std::move(s));
    }
    
}

std::string chomik::placeholder_name_item::get_actual_text_representation(machine & m, generator & g) const
{
    return g.get_actual_text_representation_of_a_placeholder(placeholder);
}

std::string chomik::variable_value_name_item::get_actual_text_representation(machine & m, generator & g) const
{
    signature s{*name, m, g};
    return m.get_actual_text_representation_of_a_variable(s);
}        

std::string chomik::machine::get_actual_text_representation_of_a_variable(const signature & vn) const
{
    std::stringstream s;
    
    switch (get_actual_memory_representation_type_of_the_variable(vn))
    {
        case variable_with_value::actual_memory_representation_type::INTEGER:
            s << get_variable_value_integer(vn);
            break;
            
        case variable_with_value::actual_memory_representation_type::FLOAT:
            s << get_variable_value_float(vn);
            break;
            
        case variable_with_value::actual_memory_representation_type::STRING:
            s << get_variable_value_string(vn);
            break;
        
        case variable_with_value::actual_memory_representation_type::ENUM:
            s << get_variable_value_enum(vn);
            break;
            
        case variable_with_value::actual_memory_representation_type::CODE:
        {
            s << "some_code";
        }
            break;
    }
    return s.str();
}


bool chomik::predefined_types::get_type_is_predefined(const std::string & type_name)
{
    for (auto &i: array_of_predefined_types)
    {
        if (i == type_name)
        {
            return true;
        }
    }
    return false;
}


bool chomik::predefined_variables::get_variable_is_predefined(const std::string & prefix)
{
    for (auto & i: array_of_predefined_variables)
    {
        if (i == prefix)
        {
            return true;
        }
    }
    return false;
}



std::string chomik::generic_value_placeholder::get_actual_text_representation(machine & m, generator & g) const
{
    return g.get_actual_text_representation_of_a_placeholder(placeholder);
}

std::string chomik::generic_name::get_actual_text_representation(machine & m, generator & g) const
{
    std::stringstream s;
    for (auto & i: vector_of_name_items)
    {
        s << i->get_actual_text_representation(m, g);
    }
    return s.str();
}

std::string chomik::generic_literal_code::get_actual_text_representation(machine & m, generator & g) const
{
    std::stringstream s;
    my_code_pointer->report(s);
    return s.str();
}

extern "C" void chomik_open_file(const char * f);
extern "C" void chomik_close_file();
extern "C" int yyparse();

int chomik::parser::parse(const char * filename)
{
    chomik_open_file(filename);

    int i = yyparse();

    if (i != 0)
        std::cout << "error" << "\n";

    //chomik_close_file(); no need to close it, it is going to be closed by the scanner

    return i;
}

chomik::parser::parser(program & p): my_program{p}
{
    
}


void chomik::parser::copy_list_of_statements_to_the_program(list_of_statements * const l)
{
    for (auto & i: l->get_vector_of_statements())
    {
        std::shared_ptr<statement> x{i};
        my_program.add_statement(std::move(x));
    }    
}

        
void chomik::signature::add_content(std::shared_ptr<signature_item> && i)
{
    vector_of_items.push_back(std::move(i));
}

void chomik::name_item_string::add_content_to_signature(signature & target, machine & m, generator & g) const
{
    target.add_content(std::make_shared<simple_value_string_signature_item>(my_value));
}


void chomik::identifier_name_item::add_content_to_signature(signature & target, machine & m, generator & g) const
{
    target.add_content(std::make_shared<simple_value_enum_signature_item>(identifier));
}

void chomik::identifier_name_item::add_content_to_signature(signature & target) const
{
    target.add_content(std::make_shared<simple_value_enum_signature_item>(identifier));
}

std::string chomik::machine::get_variable_value_enum(const signature & vn) const
{
    return get_variable_with_value(vn).get_value_enum();
}

std::string chomik::machine::get_variable_value_string(const signature & vn) const
{
    return get_variable_with_value(vn).get_value_string();
}

int chomik::machine::get_variable_value_integer(const signature & vn) const
{
    return get_variable_with_value(vn).get_value_integer();
}
        
double chomik::machine::get_variable_value_float(const signature & vn) const
{
    return get_variable_with_value(vn).get_value_float();
}
        
void chomik::machine::get_variable_value_code(const signature & vn, code & target) const
{
    DEBUG("get code for " << vn);
    get_variable_with_value(vn).get_value_code(target);
}

const chomik::variable_with_value & chomik::machine::get_variable_with_value(const signature & vn) const
{
    const std::string s = vn.get_string_representation();
    auto x = map_signature_string_representation_to_variable_with_value.find(s);
    
    if (x != map_signature_string_representation_to_variable_with_value.end())
        return *x->second;
    
    std::stringstream s2;
    s2 << "unable to find the variable " << s;
    
    throw std::runtime_error(s2.str());
}

chomik::variable_with_value & chomik::machine::get_variable_with_value(const signature & vn)
{
    const std::string s = vn.get_string_representation();
    auto x = map_signature_string_representation_to_variable_with_value.find(s);
    
    if (x != map_signature_string_representation_to_variable_with_value.end())
        return *x->second;
    
    std::stringstream s2;
    s2 << "unable to find the variable " << s;
    
    throw std::runtime_error(s2.str());
}

bool chomik::machine::get_variable_is_represented_in_memory(const signature & vn) const
{
    const std::string s = vn.get_string_representation();

/*    
    std::cout << "checking whether the variable ";
    vn.report(std::cout);
    std::cout << " is represented in the memory\n";
    */

    auto x = map_signature_string_representation_to_variable_with_value.find(s);
    
    if (x != map_signature_string_representation_to_variable_with_value.end())
        return true;
    
    return false;
}

chomik::variable_with_value::actual_memory_representation_type chomik::machine::get_actual_memory_representation_type_of_the_variable(const signature & vn) const
{
    const std::string s = vn.get_string_representation();
        
    auto x = map_signature_string_representation_to_variable_with_value.find(s);
    
    if (x != map_signature_string_representation_to_variable_with_value.end())
    {
        return x->second->get_representation_type();
    }   
    
    return variable_with_value::actual_memory_representation_type::NONE; // this happens when the variable is not represented in the memory
}

void chomik::placeholder_name_item::add_content_to_signature(signature & target, machine & m, generator & g) const
{    
    if (g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_content(std::make_shared<simple_value_integer_signature_item>(g.get_placeholder_value_integer(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_content(std::make_shared<simple_value_float_signature_item>(g.get_placeholder_value_float(placeholder)));
                break;

            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_content(std::make_shared<simple_value_string_signature_item>(g.get_placeholder_value_string(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_content(std::make_shared<simple_value_enum_signature_item>(g.get_placeholder_value_enum(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::CODE:
                // TODO - implement it
                break;                
        }
    }
    
}


bool chomik::simple_value_integer_signature_item::get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const
{
    if (gni.get_is_integer())
    {
        return gni.get_match_integer(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        if (target.get_is_placeholder_bound_as_integer(gni.get_placeholder_name()))             // this is necessary since we might use the same placeholder twice
        {
            return value == target.get_placeholder_value_integer(gni.get_placeholder_name());
        }
        else
        {
            target.bind_placeholder_as_integer(gni.get_placeholder_name(), value);
            return target.get_is_successful();
        }
    }    
    return false;
}


bool chomik::simple_value_float_signature_item::get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const
{
    if (gni.get_is_float())
    {
        return gni.get_match_float(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        if (target.get_is_placeholder_bound_as_float(gni.get_placeholder_name()))           // this is necessary since we might use the same placeholder twice
        {
            return value == target.get_placeholder_value_float(gni.get_placeholder_name());   
        }
        else
        {
            target.bind_placeholder_as_float(gni.get_placeholder_name(), value);
            return target.get_is_successful();
        }
    }    
    return false;
}


bool chomik::simple_value_string_signature_item::get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const
{
    if (gni.get_is_string())
    {
        return gni.get_match_string(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        if (target.get_is_placeholder_bound_as_string(gni.get_placeholder_name()))           // this is necessary since we might use the same placeholder twice
        {
            return value == target.get_placeholder_value_string(gni.get_placeholder_name());   
        }
        else
        {
            target.bind_placeholder_as_string(gni.get_placeholder_name(), value);
            return target.get_is_successful();
        }
    }    
    return false;
}


bool chomik::simple_value_enum_signature_item::get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const
{
    if (gni.get_is_identifier())
    {
        return gni.get_match_identifier(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        if (target.get_is_placeholder_bound_as_identifier(gni.get_placeholder_name()))           // this is necessary since we might use the same placeholder twice
        {
            return value == target.get_placeholder_value_identifier(gni.get_placeholder_name());   
        }
        else
        {
            target.bind_placeholder_as_identifier(gni.get_placeholder_name(), value);
            return target.get_is_successful();
        }
    }    
    return false;
}

bool chomik::assignment_event::get_match(const signature & s, const machine & m, const generator & g) const
{
    DEBUG("checking whether it is matching the signature " << s);
    DEBUG("my event - " << *this);
    
    auto sv{s.get_vector_of_items()};   // this signature must contain actual values
    auto sq{my_assignment_target->get_vector_of_name_items()};  // this signature may contain placeholders
    matching_protocol mp;
    
    if (sv.size() != sq.size())
    {
        return false;
    }
    
    auto sv_iterator = sv.begin();
    auto sq_iterator = sq.begin();
    bool result=true;
    for (;(sv_iterator!=sv.end()) && (sq_iterator!=sq.end()); sv_iterator++, sq_iterator++)     // only one of the conditions must be there, but it does no harm to have both
    {
        DEBUG("check whether " << **sv_iterator << " matches " << **sq_iterator);        
        
        if (!(*sv_iterator)->get_match(**sq_iterator, m, g, mp))
        {
            result = false;
            break;
        }        
    }        
    return result;
}


void chomik::variable_value_name_item::add_content_to_signature(signature & target, machine & m, generator & g) const
{
    signature x{*name, m, g};
    
    if (m.get_variable_is_represented_in_memory(x))
    {
        switch (m.get_actual_memory_representation_type_of_the_variable(x))
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_content(std::make_shared<simple_value_integer_signature_item>(m.get_variable_value_integer(x)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_content(std::make_shared<simple_value_float_signature_item>(m.get_variable_value_float(x)));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_content(std::make_shared<simple_value_string_signature_item>(m.get_variable_value_string(x)));
                break;
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_content(std::make_shared<simple_value_enum_signature_item>(m.get_variable_value_enum(x)));
                break;
                                
            case variable_with_value::actual_memory_representation_type::CODE:
                // TODO - implement it
                break;
        }
    }
    else
    {
        // iterate backwards through the assignment events - try to identify the first assignment for this variable
        DEBUG("iterate backwards through the assignment events (there are " << m.get_vector_of_assignment_events().size() << " of them)");
        
        for (int i=m.get_vector_of_assignment_events().size()-1; i>=0; i--)
        {
            const std::unique_ptr<assignment_event>& a{m.get_vector_of_assignment_events()[i]};
            DEBUG("found " << *a);
            
            if (a->get_match(x, m, g))
            {
                DEBUG("it is matching our signature !!!");
                                
                switch (a->get_source().get_actual_memory_representation_type())
                {
                    case variable_with_value::actual_memory_representation_type::INTEGER:
                        target.add_content(std::make_shared<simple_value_integer_signature_item>(a->get_source().get_actual_integer_value(m, g)));
                        break;
                        
                    case variable_with_value::actual_memory_representation_type::FLOAT:
                        target.add_content(std::make_shared<simple_value_float_signature_item>(a->get_source().get_actual_float_value(m, g)));
                        break;
                        
                    case variable_with_value::actual_memory_representation_type::STRING:
                        target.add_content(std::make_shared<simple_value_string_signature_item>(a->get_source().get_actual_string_value(m, g)));
                        break;
                        
                    case variable_with_value::actual_memory_representation_type::ENUM:
                        target.add_content(std::make_shared<simple_value_enum_signature_item>(a->get_source().get_actual_identifier_value(m, g)));
                        break;
                                            
                    case variable_with_value::actual_memory_representation_type::CODE:
                        break;
                }
                
                break;
            }            
        }                
    }
    
}


chomik::signature::signature(const generic_name & gn, machine & m, generator & g)
{
    for (auto &i: gn.get_vector_of_name_items())
    {
        i->add_content_to_signature(*this, m, g);
    }
}

chomik::signature::signature(const generic_name & gn)
{
    for (auto &i: gn.get_vector_of_name_items())
    {
        i->add_content_to_signature(*this);
    }
}

chomik::variable_with_value::actual_memory_representation_type chomik::generic_value_placeholder::get_actual_memory_representation_type(machine & m, generator & g) const
{
    return type_name->get_actual_memory_representation_type(m);
}


chomik::variable_with_value::actual_memory_representation_type chomik::generic_value_variable_value::get_actual_memory_representation_type(machine & m, generator & g) const
{
    signature s{*name, m, g};
    
    if (m.get_variable_is_represented_in_memory(s))
    {
        return m.get_actual_memory_representation_type_of_the_variable(s);
    }
        
    // TODO sometimes we can find the type even if the variable is not represented in memory
    return variable_with_value::actual_memory_representation_type::NONE;
}

std::string chomik::variable_with_value::get_signature_string_representation() const 
{ 
    return actual_name->get_string_representation();     
}

void chomik::variable_with_value_code::assign_value_code(const code & ci)
{
    value = std::make_unique<code>(ci);
}


void chomik::machine::create_predefined_variables()
{
    DEBUG("machine::create_predefined_variables");
    
    generic_name gn;
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("target"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
    std::shared_ptr<signature> the_print_target_stream_index=std::make_shared<signature>(gn);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_print_target_stream_index), 0));
        
    generic_name gn2;
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_created_stream_index=std::make_shared<signature>(gn2);    
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_created_stream_index), 0));
    
    generic_name gn3;
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("result"));    
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));    
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("index"));    
    std::shared_ptr<signature> the_get_from_stream_result_stream_index=std::make_shared<signature>(gn3);    
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_from_stream_result_stream_index), 0));

    generic_name gn4;
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("result"));    
    std::shared_ptr<signature> the_get_from_stream_result=std::make_shared<signature>(gn4);    
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_get_from_stream_result), ""));


    generic_name gn5;
    gn5.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn5.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    gn5.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));
    std::shared_ptr<signature> the_print_separator=std::make_shared<signature>(gn5);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_print_separator), " "));

    generic_name gn6;
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("end"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("line"));
    std::shared_ptr<signature> the_print_end_of_line=std::make_shared<signature>(gn6);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_print_end_of_line), "\n"));

    generic_name gn7;
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("source"));        
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
    std::shared_ptr<signature> the_read_from_stream_source_stream_index=std::make_shared<signature>(gn7);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_read_from_stream_source_stream_index), 2));  // standard input

    generic_name gn8;
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn8.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_read_from_stream_result_integer=std::make_shared<signature>(gn8);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_read_from_stream_result_integer), 0));

    generic_name gn9;
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn9.add_generic_name_item(std::make_shared<name_item_string>("string"));
    std::shared_ptr<signature> the_read_from_stream_result_string=std::make_shared<signature>(gn9);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_read_from_stream_result_string), ""));

    generic_name gn10;
    gn10.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn10.add_generic_name_item(std::make_shared<identifier_name_item>("compare"));
    gn10.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_compare_result=std::make_shared<signature>(gn10);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_compare_result), ""));
    
    generic_name gn11;
    gn11.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn11.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
    gn11.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn11.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_add_result_integer=std::make_shared<signature>(gn11);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_add_result_integer), 0));

    generic_name gn12;
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("substract"));
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn12.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_substract_result_integer=std::make_shared<signature>(gn12);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_substract_result_integer), 0));
    
}

void chomik::machine::create_predefined_types()
{
    std::shared_ptr<type_instance> boolean_type = std::make_shared<type_instance_enum>("boolean");
    
    generic_name gn_false;
    gn_false.add_generic_name_item(std::make_shared<identifier_name_item>("false"));
    signature signature_false{gn_false};
    boolean_type->add_type_instance_enum_value(signature_false);
    
    generic_name gn_true;
    gn_true.add_generic_name_item(std::make_shared<identifier_name_item>("true"));
    signature signature_true{gn_true};    
    boolean_type->add_type_instance_enum_value(signature_true);
    
    add_type_instance(std::move(boolean_type));
    
    
    std::shared_ptr<type_instance> comparison_result_type = std::make_shared<type_instance_enum>("compare_result");
    generic_name gn_lower;
    gn_lower.add_generic_name_item(std::make_shared<identifier_name_item>("lower"));
    signature signature_lower{gn_lower};
    comparison_result_type->add_type_instance_enum_value(signature_lower);
    
    generic_name gn_equal;
    gn_equal.add_generic_name_item(std::make_shared<identifier_name_item>("equal"));
    signature signature_equal{gn_equal};
    comparison_result_type->add_type_instance_enum_value(signature_equal);

    generic_name gn_greater;
    gn_greater.add_generic_name_item(std::make_shared<identifier_name_item>("greater"));
    signature signature_greater{gn_greater};
    comparison_result_type->add_type_instance_enum_value(signature_greater);

    add_type_instance(std::move(comparison_result_type));
    
}

void chomik::machine::create_predefined_streams()
{
    DEBUG("machine::create_predefined_streams");
    
    add_stream(std::make_unique<generic_stream_standard_output_stream>());
    add_stream(std::make_unique<generic_stream_standard_error_stream>());
    add_stream(std::make_unique<generic_stream_standard_input_stream>());    
    
}

int chomik::generic_literal_placeholder::get_actual_integer_value(machine & m, generator & g) const
{
    if (g.get_has_placeholder_with_value(placeholder))
    {
        DEBUG("got " << g.get_placeholder_value_integer(placeholder));
        return g.get_placeholder_value_integer(placeholder);
    }    
    
    std::stringstream s;
    s << "for placeholder " << placeholder << " the generator " << g << " does not contain any value";
    throw std::runtime_error(s.str());
        
    return 0;
}


std::string chomik::generic_literal_placeholder::get_actual_string_value(machine & m, generator & g) const
{ 
    return g.get_placeholder_with_value(placeholder).get_value_enum();     
}



int chomik::generic_value_variable_value::get_actual_integer_value(machine & m, generator & g) const
{
    signature s{*name, m, g};
    return m.get_variable_value_integer(s);
}


std::string chomik::generic_value_variable_value::get_actual_string_value(machine & m, generator & g) const
{
    signature s{*name, m, g};
    return m.get_variable_value_string(s);
}

std::string chomik::generic_value_variable_value::get_actual_enum_value(machine & m, generator & g) const
{
    signature s{*name, m, g};
    return m.get_variable_value_enum(s);
}


void chomik::signature::execute_predefined_print(machine & m) const
{
        // check the print target stream index
        generic_name gn;
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("target"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
        signature the_print_target_stream_index{gn};
                
        int index = m.get_variable_with_value(the_print_target_stream_index).get_value_integer();
        
        DEBUG("value of the print target stream index " << index);
        
        if (index >= 0 && index < m.get_amount_of_streams())
        {
            generic_name gn2;
            gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
            gn2.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
            gn2.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));
            signature the_print_separator{gn2};
            
            std::string separator = m.get_variable_with_value(the_print_separator).get_value_string();
            
            DEBUG("the print separator is \"" << separator << "\"");

            generic_name gn3;
            gn3.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
            gn3.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
            gn3.add_generic_name_item(std::make_shared<identifier_name_item>("end"));
            gn3.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
            gn3.add_generic_name_item(std::make_shared<identifier_name_item>("line"));
            signature the_print_end_of_line{gn3};
            
            std::string end_of_line = m.get_variable_with_value(the_print_end_of_line).get_value_string();
            
            generic_stream& gs{m.get_stream(index)};
            
            if (vector_of_items.size() == 3 
                && vector_of_items[1]->get_it_is_identifier("memory")
                && vector_of_items[2]->get_it_is_identifier("report"))
            {
                m.report(gs.get_output_stream());
            }
            else
            {            
                bool first = true;
                for (auto & i: vector_of_items)
                {
                    if (first)
                    {
                        first = false;
                        continue;
                    }
                    i->print(gs.get_output_stream());
                    gs.get_output_stream() << separator;
                }    
                gs.get_output_stream() << end_of_line;
            }
        }    
}

void chomik::signature::execute_predefined_create(machine & m) const
{
        if (vector_of_items.size() == 5)
        {
            if (vector_of_items[1]->get_it_is_identifier("new") 
                && vector_of_items[2]->get_it_is_identifier("output") 
                && vector_of_items[3]->get_it_is_identifier("filestream")
                && vector_of_items[4]->get_it_is_string())
            {                
                m.add_stream(std::make_unique<generic_stream_file_output>(vector_of_items[4]->get_value_string()));
                
                generic_name gn2;
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
                signature the_created_stream_index{gn2};    
                
                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                
                return;
            }
            else
            if (vector_of_items[1]->get_it_is_identifier("new") 
                && vector_of_items[2]->get_it_is_identifier("input") 
                && vector_of_items[3]->get_it_is_identifier("stringstream")
                && vector_of_items[4]->get_it_is_string())
            {
                m.add_stream(std::make_unique<generic_stream_stringstream>(vector_of_items[4]->get_value_string()));

                generic_name gn2;
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
                signature the_created_stream_index{gn2};    
                
                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                
                return;
            }
        }
        else
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_identifier("new") 
                && vector_of_items[2]->get_it_is_identifier("output") 
                && vector_of_items[3]->get_it_is_identifier("stringstream"))
            {
                m.add_stream(std::make_unique<generic_stream_stringstream>());
                
                generic_name gn2;
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                gn2.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
                signature the_created_stream_index{gn2};    
                
                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                
                return;
            }
        }
        else
        if (vector_of_items.size() == 9)
        {
            if (vector_of_items[1]->get_it_is_identifier("new") 
                && vector_of_items[2]->get_it_is_identifier("input") 
                && vector_of_items[3]->get_it_is_identifier("random")
                && vector_of_items[4]->get_it_is_identifier("number")
                && vector_of_items[5]->get_it_is_identifier("stream")
                && vector_of_items[6]->get_it_is_string()
                && vector_of_items[7]->get_it_is_integer()
                && vector_of_items[8]->get_it_is_integer()
            )
            {
                if (vector_of_items[6]->get_value_string() == "integer")
                {                
                    m.add_stream(std::make_unique<generic_stream_random_number_stream>(vector_of_items[7]->get_value_integer(), vector_of_items[8]->get_value_integer()));
                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
                    signature the_created_stream_index{gn2};    
                
                    DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                    m.get_variable_with_value(the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());                
                    return;
                }
            }
        }
            
        std::cerr << "warning: unknown create variable\n";        
        for (auto & i: vector_of_items)
        {
            std::cerr << *i << ' ';
        }    
        std::cerr << '\n';    
}


void chomik::signature::execute_predefined_get(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_identifier("from") 
                && vector_of_items[2]->get_it_is_identifier("stream") 
                && vector_of_items[3]->get_it_is_identifier("result"))
            {
                
                //the get from stream result stream index
                generic_name gn;
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("result"));        
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));        
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
                
                signature the_get_from_stream_result_stream_index{gn};
                
                int index = m.get_variable_with_value(the_get_from_stream_result_stream_index).get_value_integer();
        
                DEBUG("value of the print target stream index " << index);
                if (index >= 0 && index < m.get_amount_of_streams())
                {
                    generic_stream& gs{m.get_stream(index)};                        
                                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
                
                    signature the_get_from_stream_result{gn2};    
                
                    DEBUG("assign value string " << gs.get_result());
                
                    m.get_variable_with_value(the_get_from_stream_result).assign_value_string(gs.get_result());                                    
                    return;
                }
                return;
            }
        }
                
        std::cerr << "warning: unknown get variable\n";        
        for (auto & i: vector_of_items)
        {
            std::cerr << *i << ' ';
        }    
        std::cerr << '\n';         
}

void chomik::signature::execute_predefined_read(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_identifier("from") 
                && vector_of_items[2]->get_it_is_identifier("stream")
                && vector_of_items[3]->get_it_is_string())
            {
                
                if (vector_of_items[3]->get_value_string() == "integer")
                {
                generic_name gn;
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("source"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));        
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
                
                signature the_read_from_stream_source_stream_index{gn};
                
                int index = m.get_variable_with_value(the_read_from_stream_source_stream_index).get_value_integer();
        
                DEBUG("value of the read from stream source stream index " << index);
                    if (index >= 0 && index < m.get_amount_of_streams())
                    {
                    generic_stream& gs{m.get_stream(index)};                        
                                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
                    gn2.add_generic_name_item(std::make_shared<name_item_string>("integer"));
                    signature the_read_from_stream_result{gn2};    
                    
                    int s = gs.read_integer();
                                    
                    DEBUG("read " << s);
                
                    m.get_variable_with_value(the_read_from_stream_result).assign_value_integer(s);
                    return;
                    }                
                }
                else
                if (vector_of_items[3]->get_value_string() == "string")
                {                
                
                generic_name gn;
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("source"));
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));        
                gn.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
                
                signature the_read_from_stream_source_stream_index{gn};
                
                int index = m.get_variable_with_value(the_read_from_stream_source_stream_index).get_value_integer();
        
                DEBUG("value of the print target stream index " << index);
                    if (index >= 0 && index < m.get_amount_of_streams())
                    {
                    generic_stream& gs{m.get_stream(index)};                        
                                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
                    gn2.add_generic_name_item(std::make_shared<name_item_string>("string"));
                
                    signature the_read_from_stream_result{gn2};    
                    
                    std::string s = gs.read_string();
                                    
                    DEBUG("read " << s);
                
                    m.get_variable_with_value(the_read_from_stream_result).assign_value_string(s);
                    return;
                    }
                }
                return;
            }
        }
        std::cerr << "warning: unknown get variable\n";        
        for (auto & i: vector_of_items)
        {
            std::cerr << *i << ' ';
        }    
        std::cerr << '\n';         
}

void chomik::signature::execute_predefined_compare(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {                
                if (vector_of_items[1]->get_value_string() == "integer")
                {                                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("compare"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));                    
                
                    signature the_compare_result{gn2};    
                    
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("got " << a << " and " << b);
                    
                    if (a < b)
                    {
                        s = "lower";
                    }
                    else
                    if (a > b)
                    {
                        s = "greater";
                    }
                    else
                    {
                        s = "equal";
                    }
                
                    m.get_variable_with_value(the_compare_result).assign_value_enum(s);                                        
                    return;
                }
            }
        }
    std::cerr << "warning: unknown compare variable\n";        
    for (auto & i: vector_of_items)
    {
        std::cerr << *i << ' ';
    }    
    std::cerr << '\n';         
}


void chomik::signature::execute_predefined_add(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {                
                if (vector_of_items[1]->get_value_string() == "integer")
                {                                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
                    gn2.add_generic_name_item(std::make_shared<name_item_string>("integer"));
                
                    signature the_add_result{gn2};    
                    
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("signature::execute_predefined_add got " << a << " and " << b);
                                    
                    m.get_variable_with_value(the_add_result).assign_value_integer(a+b);                                        
                    return;
                }
            }
        }
    std::cerr << "warning: unknown add variable\n";        
    for (auto & i: vector_of_items)
    {
        std::cerr << *i << ' ';
    }    
    std::cerr << '\n';         
}

void chomik::signature::execute_predefined_substract(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {                
                if (vector_of_items[1]->get_value_string() == "integer")
                {                                
                    generic_name gn2;
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("substract"));
                    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
                    gn2.add_generic_name_item(std::make_shared<name_item_string>("integer"));
                
                    signature the_add_result{gn2};    
                    
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("got " << a << " and " << b);
                                    
                    m.get_variable_with_value(the_add_result).assign_value_integer(a-b);                                        
                    return;
                }
            }
        }
    std::cerr << "warning: unknown substract variable\n";        
    for (auto & i: vector_of_items)
    {
        std::cerr << *i << ' ';
    }    
    std::cerr << '\n';         
}


void chomik::signature::execute_predefined(machine & m) const
{
    if (m.get_is_user_defined_executable(*this))
    {
        m.execute_user_defined_executable(*this);
    }    
    else
    if (get_it_has_prefix("print"))
    {        
        execute_predefined_print(m);
    }
    else
    if (get_it_has_prefix("create"))
    {
        execute_predefined_create(m);
    }
    else
    if (get_it_has_prefix("get"))
    {
        execute_predefined_get(m);
    }
    else
    if (get_it_has_prefix("read"))
    {
        execute_predefined_read(m);
    }
    else
    if (get_it_has_prefix("compare"))
    {
        execute_predefined_compare(m);
    }
    else
    if (get_it_has_prefix("add"))
    {
        execute_predefined_add(m);
    }
    else
    if (get_it_has_prefix("substract"))
    {
        execute_predefined_substract(m);
    }
    
}

bool chomik::signature::get_it_has_prefix(const std::string & pattern) const
{
    if (vector_of_items.size() >= 1)
    {
        return vector_of_items[0]->get_it_is_identifier(pattern);
    }
    return false;
}


bool chomik::signature::get_is_predefined(const machine & m) const
{
    if (vector_of_items.size()>=1)
    {
        return vector_of_items[0]->get_is_predefined() || m.get_is_user_defined_executable(*this);
    }
    
    return false;
}


void chomik::execute_variable_value_statement::add_placeholders_to_generator(generator & g) const
{
    name->add_placeholders_to_generator(g);
}


std::string chomik::generic_literal_placeholder::get_actual_text_representation(machine & m, generator & g) const
{
    return "hallo";
}

void chomik::generic_literal_placeholder::add_placeholders_to_generator(generator & g) const
{
}

void chomik::type_definition_statement::expand(machine & m, int depth) const
{
}

bool chomik::type_definition::get_is_range() const
{
    return body->get_is_range();
}


void chomik::machine::expand(int i)
{
    std::vector<std::shared_ptr<type_definition>> temporary_vector_of_type_definitions;
    std::vector<std::shared_ptr<type_instance>> temporary_vector_of_type_instances;
    std::vector<bool> temporary_vector_of_flags_type_is_new;
    
    for (auto & j: vector_of_type_definiton_statements)
    {
        for (auto & k: j->get_vector_of_type_definitions())
        {
            std::shared_ptr<type_definition> x{k};
            temporary_vector_of_type_definitions.push_back(std::move(x));
            
            if (k->get_is_range())
            {
                std::shared_ptr<type_instance> y{std::make_shared<type_instance_range>(k->get_name(), k->get_body().get_min_value(*this), k->get_body().get_max_value(*this))};
                std::shared_ptr<type_instance> y2{y};   // here we keep the second pointer to the same object, but the first one will be stored in the temporary vector only
            
                temporary_vector_of_type_instances.push_back(std::move(y));
                temporary_vector_of_flags_type_is_new.push_back(!get_type_instance_is_known(k->get_name()));
                
                if (!get_type_instance_is_known(k->get_name()))  // we may call "expand()" several times, this should not be a problem
                    add_type_instance(std::move(y2));
            }
            else
            {            
                std::shared_ptr<type_instance> y{std::make_shared<type_instance_enum>(k->get_name())};
                std::shared_ptr<type_instance> y2{y};   // here we keep the second pointer to the same object, but the first one will be stored in the temporary vector only
            
                temporary_vector_of_type_instances.push_back(std::move(y));
                temporary_vector_of_flags_type_is_new.push_back(!get_type_instance_is_known(k->get_name()));
                
                if (!get_type_instance_is_known(k->get_name()))  // we may call "expand()" several times, this should not be a problem
                    add_type_instance(std::move(y2));
            }
        }
    }
    
    for (int k=1; k<=i; k++)
    {   
        int l=0;
        for (auto & j: temporary_vector_of_type_definitions)
        {
            if (temporary_vector_of_flags_type_is_new[l])   // skip the types that are already known
                j->expand(*this, k, temporary_vector_of_type_instances[l]);
            l++;
        }
    }

    for (int k=1; k<=i; k++)
    {    
        for (auto & j: vector_of_variable_definition_statements)
        {
            j->expand(*this, k);
        }    
    }
}


void chomik::variable_definition::expand(machine & m, int depth) const
{
    generator g{*name, __FILE__, __LINE__};
    
    /*
    std::cout << "for name ";
    name->report(std::cout);
    std::cout << " got a generator ";
    g.report(std::cout);
    std::cout << "\n";
    */
        
    if (g.get_the_cartesian_product_of_placeholder_types_is_empty())
    {
        // skip
    }
    else
    if (g.get_the_cartesian_product_of_placeholder_types_has_one_item())
    {
        if (depth == 1)
        {
            // add variable family with one member
        }
    }
    else
    if (g.get_the_cartesian_product_of_placeholder_types_is_finite() && g.get_the_cartesian_product_of_placeholder_types_is_small())
    {
        for (g.initialize(m); !g.get_terminated(); g.increment())
        {
            if (g.get_is_valid())
            {
                /*
                std::cout << "got a generator ";
                g.report(std::cout);
                std::cout << "\n";
                */
            }
        }
    }
    else
    {
        // unlimited variable family
    }
}

void chomik::variable_definition_statement::expand(machine & m, int depth) const
{
    for (auto & i: vector_of_variable_definitions)
    {
        i->expand(m, depth);
    }
}

void chomik::type_definition::expand(machine & m, int depth, std::shared_ptr<type_instance> & e) const
{
    DEBUG("expanding type " << name << " for depth " << depth << " - " << *body);          
    body->expand(m, depth, name, e);    
}

void chomik::generic_range_boundary_variable_value::add_placeholders_to_generator(generator & g) const
{
    for (auto & i: name->get_vector_of_name_items())
    {
        i->add_placeholders_to_generator(g);
    }
}


void chomik::type_definition_body_range::expand(machine & m, int depth, const std::string & n, std::shared_ptr<chomik::type_instance>& e) const
{    
    if (predefined_types::get_type_is_predefined(n))
    {
        throw std::runtime_error("this type is predefinied");
    }            
    
    generator g{*r, __FILE__, __LINE__};

    if (g.get_the_cartesian_product_of_placeholder_types_is_empty())
    {
    }
    else
    {
        if (depth == 1)
        {
            //std::shared_ptr<type_instance> e=std::make_shared<type_instance_range>(n, r->get_min_value(m, g), r->get_max_value(m, g));
            //m.add_type_instance(std::move(e));
        }
    }
}


void chomik::placeholder_name_item::add_placeholders_to_generator(generator & g) const
{
    if (!g.get_has_placeholder(placeholder))
    {
        std::shared_ptr<generic_type> x{type_name};
        g.add_placeholder(placeholder, std::move(x));
    }
    else
    {
        // TODO the placeholder is already there, check that it has identical type
        //std::cout << "check that the placeholders have identical type\n";
        
    }
}

std::string chomik::signature::get_string_representation() const
{
    std::stringstream s;
    
    for (auto & i: vector_of_items)
    {
        i->report(s);
    }
    
    //DEBUG("signature::get_string_representation - result " << s.str());
    
    return s.str();
}

void chomik::variable_value_name_item::add_placeholders_to_generator(generator & g) const
{
    name->add_placeholders_to_generator(g);
}

void chomik::generic_name::add_placeholders_to_generator(generator & g) const
{
    /*
    std::cout << "generic_name::add_placeholders_to_generator ";
    report(std::cout);
    std::cout << ", amount of name items " << vector_of_name_items.size() << "\n";
    */
    
    for (auto & i: vector_of_name_items)
    {     
        /*
        std::cout << "the name item ";
        i->report(std::cout);
        std::cout << "\n";
        
        */
        i->add_placeholders_to_generator(g);
    }
}

void chomik::generic_literal_code::add_placeholders_to_generator(generator & g) const
{
    my_code_pointer->add_placeholders_to_generator(g);
}

void chomik::code::add_placeholders_to_generator(generator & g) const
{
    //std::cout << "adding placeholders to generator - code ";
    //body->report(std::cout);
    //std::cout << "\n";
    body->add_placeholders_to_generator(g);
}

void chomik::list_of_statements::add_placeholders_to_generator(generator & g) const
{
    for (auto & i: vector_of_statements)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::type_definition_statement::add_placeholders_to_generator(generator & g) const
{
    for (auto & i: vector_of_type_definitions)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::type_definition::add_placeholders_to_generator(generator & g) const
{
    body->add_placeholders_to_generator(g);
}

void chomik::variable_definition_statement::add_placeholders_to_generator(generator & g) const
{
    for (auto & i: vector_of_variable_definitions)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::type_definition_body_enum::add_placeholders_to_generator(generator & g) const
{
    for (auto & i: vector_of_names)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::execute_value_statement::add_placeholders_to_generator(generator & g) const
{
    value->add_placeholders_to_generator(g);
}

chomik::generator::generator(const generic_range & gr, const std::string & filename, unsigned new_line_number): my_filename{filename}, line_number{new_line_number}
{
    gr.add_placeholders_to_generator(*this);
}

chomik::generator::generator(const generic_value & gv, const std::string & filename, unsigned new_line_number): my_filename{filename}, line_number{new_line_number}
{
    gv.add_placeholders_to_generator(*this);
}

chomik::generator::generator(const std::string & filename, unsigned new_line_number): my_filename{filename}, line_number{new_line_number}
{
}

chomik::generator::generator(const generic_name & gn, const std::string & filename, unsigned new_line_number): my_filename{filename}, line_number{new_line_number}
{
    DEBUG("created a new generator in " << filename << " at " << line_number);
    gn.add_placeholders_to_generator(*this);    
    
    DEBUG("generator::generator got a generic name " << gn);
    DEBUG("created a new generator in " << filename << " at " << line_number << ", after initialization it is " << *this);
}

bool chomik::generator::get_is_valid()
{
    for (auto & i: memory)
    {
        if (!i->get_is_valid())
            return false;
    }
    return true;
}
        
void chomik::generator::add_placeholder(const std::string & p, std::shared_ptr<generic_type> && t)
{    
    //std::cout << "adding placeholder " << p << " to the generator\n";
    
    // TODO - check that it does not exist, or if it exists that it has identical type
    
    std::shared_ptr<generator_placeholder> x = std::make_shared<generator_placeholder>(p, std::move(t));
    std::shared_ptr<generator_placeholder> x2{x};
            
    vector_of_placeholders.push_back(std::move(x));
    auto [i, success] = map_placeholder_names_to_placeholders.insert(std::pair(p, std::move(x2)));
    
    if (!success)
    {
        throw std::runtime_error("unable to insert placeholder");
    }    
}


void chomik::type_definition_body_enum::expand(machine & m, int depth, const std::string & n, std::shared_ptr<type_instance> & e) const
{   
    std::vector<std::string> temporary_vector_of_type_instance_enums;
    
    if (predefined_types::get_type_is_predefined(n))
    {
        throw std::runtime_error("this type is predefinied");
    }        
    
    for (auto & i: vector_of_names)
    {    
        generator g{*i, __FILE__, __LINE__};
    
        if (g.get_the_cartesian_product_of_placeholder_types_is_empty())
        {
        }
        else
        if (g.get_the_cartesian_product_of_placeholder_types_has_one_item())
        {
            if (depth == 1)
            {
                signature x{*i, m, g};
                e->add_type_instance_enum_value(x, depth);
            }
        }        
        else
        if (g.get_the_cartesian_product_of_placeholder_types_is_finite() && g.get_the_cartesian_product_of_placeholder_types_is_small())
        {
            for (g.initialize(m); !g.get_terminated(); g.increment())
            {
                if (g.get_is_valid())
                {
                    DEBUG("for " << *this << " got a generator " << g);
                    
                    signature x{*i, m, g};
                    temporary_vector_of_type_instance_enums.push_back(x.get_string_representation());
                }
            }
        }        
    }
    
    
    for (auto & i: temporary_vector_of_type_instance_enums)
    {
        e->add_type_instance_enum_value(i, depth);
    }
    
}


chomik::variable_with_value::actual_memory_representation_type chomik::generic_type_named::get_actual_memory_representation_type(machine & m) const 
{
    if (name == "integer")
        return variable_with_value::actual_memory_representation_type::INTEGER;
    if (name == "float")
        return variable_with_value::actual_memory_representation_type::FLOAT;
    if (name == "string")
        return variable_with_value::actual_memory_representation_type::STRING;
    if (name == "code")
        return variable_with_value::actual_memory_representation_type::CODE;
            
    
    if (m.get_type_instance_is_known(name))
    {
        switch (m.get_type_instance(name).get_mode())
        {
            case type_instance::type_instance_mode::INTEGER:
                return variable_with_value::actual_memory_representation_type::INTEGER;
                
            case type_instance::type_instance_mode::ENUM:
                return variable_with_value::actual_memory_representation_type::ENUM;
        }
    }
    else
    {
        std::cerr << "warning: type_instance " << name << " is unknown\n";
    }
    
    return variable_with_value::actual_memory_representation_type::NONE;
}


void chomik::generator::initialize(machine & m)
{
    for (auto & i: vector_of_placeholders)
    {
        DEBUG("generator::initialize - add placeholder " << i->get_name() << " of type " << i->get_placeholder_type() << " to the generator");
        
        switch (i->get_placeholder_type().get_actual_memory_representation_type(m))
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
            {
                const std::string type_name = i->get_placeholder_type().get_type_name(m, *this);
                int f, l;
                if (m.get_type_instance_is_known(type_name))
                {
                    m.get_first_and_last_iterators_for_range_type(type_name, f, l);
                
                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_range>(i->get_name(), f, l)};
                                        
                    DEBUG("it is a range " << f << ".." << l);
                
                    // std::cout << "it is a range " << f << ".." << l << "\n";
        
                    add_placeholder_with_value(std::move(x));
                }
                else
                {
                    throw std::runtime_error("type_instance is not known");
                }
            }
            break;
            case variable_with_value::actual_memory_representation_type::FLOAT:
            {
                std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_with_value_and_report<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>>(i->get_name(), 123.456)};
        
                add_placeholder_with_value(std::move(x));
            }
            break;
            case variable_with_value::actual_memory_representation_type::STRING:
            {
                std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_with_value_and_report<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>>(i->get_name(), "hallo")};
        
                add_placeholder_with_value(std::move(x));
            }
            break;
            
            case variable_with_value::actual_memory_representation_type::ENUM:
            {
                const std::string type_name = i->get_placeholder_type().get_type_name(m, *this);
                
                std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator f,l;
                
                // std::cout << "it is an enum of type " << type_name << "\n";
                
                if (m.get_type_instance_is_known(type_name))
                {
                    m.get_first_and_last_iterators_for_enum_type(type_name, f, l);
                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_enum>(i->get_name(), f, l)};
        
                    add_placeholder_with_value(std::move(x));
                }
            }
            break;
            
        }
    }
}

void chomik::generator::increment()
{
    for (auto & i: memory)
    {
        i->increment();
        if (i->get_is_valid())
            break;
        else
            continue;
    }
}

bool chomik::generator::get_terminated() const
{
    for (auto & i: memory)
    {
        if (!i->get_is_terminated())
            return false;
    }
    return true;
}

bool chomik::generator::get_the_cartesian_product_of_placeholder_types_is_small() const
{
    // TODO - take into account the informations about the target machine
    return true;
}


bool chomik::generator::get_the_cartesian_product_of_placeholder_types_is_finite() const
{
    for (auto & i: vector_of_placeholders)
    {
        if (!i->get_type_is_finite())
        {
            return false;
        }
    }
    return true;
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


chomik::generic_range_boundary_variable_value::generic_range_boundary_variable_value(const generic_name & gn)
{
    name = std::make_unique<generic_name>(gn);
}


chomik::variable_value_name_item::variable_value_name_item(const generic_name & gn): name{std::make_shared<generic_name>(gn)}
{
}


chomik::variable_value_name_item::variable_value_name_item(list_of_generic_name_items * const l): name{std::make_shared<generic_name>(l)}
{
    // in the constructor of generic_name the list_of_generic_name_items is just copied, therefore this constructor (variable_value_name_item)
    // does not own the first parameter!
}

chomik::type_definition_body_enum::type_definition_body_enum(list_of_generic_names * const l)
{            
    if (l)
    {
        for (auto & i: l->get_vector_of_names())
        {
            std::shared_ptr<generic_name> x{i};
            vector_of_names.push_back(std::move(x));
        }
    }
}


chomik::generic_name::generic_name(const generic_name & gn)
{
    //DEBUG("copy constructor");
    for (auto & i: gn.get_vector_of_name_items())
    {
        std::shared_ptr<generic_name_item> j;
        i->get_copy(j);
        add_generic_name_item(std::move(j));
    }
}

chomik::generic_name::generic_name()
{
}

chomik::generic_name::generic_name(list_of_generic_name_items * const l)
{
    if (l)
    {
        for (auto & i: l->get_vector_of_name_items())
        {            
            std::shared_ptr<generic_name_item> x(i);
            vector_of_name_items.push_back(std::move(x));
        }
    }
}


int chomik::generic_range_boundary_variable_value::get_value(machine & m) const
{        
    generator g(__FILE__, __LINE__);
    signature x = signature(*name, m, g);
    return m.get_variable_value_integer(x);
}

chomik::program::program(): my_code{nullptr}
{
    my_code.set_is_main(true);
}

void chomik::program::add_statement(std::shared_ptr<statement> && s)
{
    my_code.add_statement(std::move(s));
}

chomik::description_of_a_cartesian_product::description_of_a_cartesian_product(const generator & g)
{
    for (auto & a: g.get_vector_of_placeholders())
    {
        vector_of_dimensions.push_back(std::make_unique<cartesian_product_dimension>(a->get_name(), a->get_placeholder_type().get_low_level_type_name(), 
                                                                                     a->get_placeholder_type().get_is_finite()));        
    }
}

std::string chomik::generic_value_variable_value::get_actual_text_representation(machine & m, generator & g) const
{
    signature s{*name, m, g};
    /*
    std::cout << "getting value of variable ";
    s.report(std::cout);
    std::cout << "\n";
    */      
    switch (m.get_actual_memory_representation_type_of_the_variable(s))
    {
        case variable_with_value::actual_memory_representation_type::INTEGER:
            break;
            
        case variable_with_value::actual_memory_representation_type::FLOAT:
            break;
            
        case variable_with_value::actual_memory_representation_type::STRING:
            break;
            
        case variable_with_value::actual_memory_representation_type::CODE:
            break;

    }
            
    return "unknown";
}

