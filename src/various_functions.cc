#include "chomik.h"
#include "config.h"
#include <cstring>
#include <algorithm>

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

#define CHOMIK_STDERR(X) *chomik::machine::current_runtime_warning_stream << X

chomik::dictionary_of_identifiers chomik::base_class_with_dictionary::our_dictionary{"ALPHA"};


int chomik::dictionary_of_identifiers::get_identifier_index(const std::string id)
{
    int result = 0;
    auto s = std::find(vector_of_identifiers.begin(), vector_of_identifiers.end(), id);
    if (s == vector_of_identifiers.end())
    {
        vector_of_identifiers.push_back(id);
        //std::cout << debug_name << " add id " << id << " at " << (vector_of_identifiers.size()-1) << "\n";
        return vector_of_identifiers.size()-1;
    }
    return s-vector_of_identifiers.begin();
}

const std::string chomik::dictionary_of_identifiers::get_identifier_by_index(int index) const
{
    return vector_of_identifiers[index];
}

std::string chomik::signature_item::get_string_representation() const
{
    std::stringstream s;
    report(s);
    return s.str();
}


std::string chomik::machine::get_signature_item_representation(int var_index, int item_index) const
{
// TODO error handling!
    if (var_index >= 0 && var_index < memory.size())
    {
        if (item_index >= 0 && item_index < memory[var_index]->get_amount_of_signature_items())
        {
            return memory[var_index]->get_signature_item_representation(item_index);
        }
    }
    return "";
}

std::string chomik::machine::get_signature_item_type_name(int var_index, int item_index) const
{
// TODO error handling!
    if (var_index >= 0 && var_index < memory.size())
    {
        if (item_index >= 0 && item_index < memory[var_index]->get_amount_of_signature_items())
        {
            return memory[var_index]->get_signature_item_type_name(item_index);
        }
    }
    return "";
}

std::string chomik::variable_with_value::get_signature_item_type_name(int item_index) const
{
    return actual_name->get_item_type_name(item_index);
}





std::string chomik::variable_with_value::get_signature_item_representation(int item_index) const
{
    return actual_name->get_item_representation(item_index);
}

int chomik::variable_with_value::get_amount_of_signature_items() const { return actual_name->get_amount_of_items(); }

std::ostream & operator<<(std::ostream & s, const chomik::generic_literal & l)
{
    l.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::basic_generator & g)
{
    g.report(s);
    return s;
}

std::ostream & operator<<(std::ostream & s, const chomik::placeholder_with_value & x)
{
    x.report(s);
    return s;
}

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


std::ostream & operator<<(std::ostream & s, const chomik::matching_protocol & x)
{
    x.report(s);
    return s;
}

void chomik::code::add_content_to_signature(const generic_name_item & s, signature & target, const machine & m, const basic_generator & g) const
{
    target.add_content(std::make_shared<code_signature_item>(s, *this));
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
    "print", "create", "get", "read", "compare", "add", "subtract", "multiply", "divide", "set", "getline", "execution", "match", "modulo"
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

void chomik::name_item_code::add_placeholders_to_generator(basic_generator & g) const
{
    my_code_pointer->add_placeholders_to_generator(g);
}


void chomik::name_item_code::add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const
{
    my_code_pointer->add_content_to_signature(*this, target, m, g);
}


std::string chomik::name_item_code::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    return my_code_pointer->get_actual_text_representation(m, g);
}

void chomik::name_item_code::get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const
{
    my_code_pointer->get_result_replacing_placeholders(m, g, p, target);
}

void chomik::name_item_code::get_copy(std::shared_ptr<generic_name_item> & gni) const
{
    gni = std::make_shared<name_item_code>();
}                        


chomik::name_item_code::name_item_code(const code & cp): my_code_pointer(std::make_unique<code>(cp))
{
    
}


chomik::code::code(const code & c): body{c.body}, is_main{c.is_main}
{
    DEBUG("created code from " << c);
}


chomik::list_of_statements::list_of_statements(statement * const s, list_of_statements * const l): is_main{false}
{
    if (s)
    {
        std::shared_ptr<statement> x{s};
        vector_of_statements.push_back(std::move(x));
    }

    if (l)
    {
        for (auto & i: l->vector_of_statements)
        {
            std::shared_ptr<statement> y{i};
            vector_of_statements.push_back(std::move(y));
        }
    }
}


chomik::list_of_statements::list_of_statements(const list_of_statements & s): is_main{false}
{
    for (const auto & i: s.vector_of_statements)
    {
        std::shared_ptr<statement> y;
        i->get_copy(y);
        DEBUG("got statement " << *y);
        vector_of_statements.push_back(std::move(y));
    }
}


void chomik::code_signature_item::get_copy(std::shared_ptr<signature_item> & target) const
{
    target = std::make_shared<code_signature_item>(*this);
}


std::string chomik::code::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    std::stringstream s;
    report(s);
    return s.str();
}

chomik::code_name_item::code_name_item(const code & c): my_code{std::make_unique<code>(c)}
{
}


std::string chomik::placeholder_name_item::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    return g.get_actual_text_representation_of_a_placeholder(placeholder);
}

std::string chomik::variable_value_name_item::get_actual_text_representation(const machine & m, const basic_generator & g) const
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



std::string chomik::generic_value_placeholder::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    return g.get_actual_text_representation_of_a_placeholder(placeholder);
}

std::string chomik::generic_name::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    std::stringstream s;
    for (auto & i: vector_of_name_items)
    {
        s << i->get_actual_text_representation(m, g);
    }
    return s.str();
}

std::string chomik::generic_literal_code::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    std::stringstream s;
    my_code_pointer->report(s);
    return s.str();
}

extern "C" void chomik_open_file(const char * f);
extern "C" void chomik_close_file();
extern "C" int yyparse();

std::ostream *chomik::machine::current_compilation_error_stream=&std::cerr, *chomik::machine::current_runtime_warning_stream = &std::cerr;
extern "C" int chomik_standard_yyerror_on;
extern "C" void chomik_read_from_string(const char * b, int len);

int chomik::parser::parse(const char * filename)
{
    chomik_standard_yyerror_on = 1;
    
    chomik_open_file(filename);
    
    int i = yyparse();

    if (i != 0)
        std::cout << "error" << "\n";

    //chomik_close_file(); no need to close it, it is going to be closed by the scanner

    return i;
}




int chomik::parser::parse_string(const std::string & code, std::ostream & error_stream)
{
    chomik::machine::current_compilation_error_stream = &error_stream;
    chomik_standard_yyerror_on = 0;
    
    chomik_read_from_string(code.c_str(), code.length());
    
    int i = yyparse();

    if (i != 0)
        error_stream << "error" << "\n";
    
    return i;
}


extern "C" void chomik_nonstandard_yyerror(unsigned line_number, const char * message)
{
    *chomik::machine::current_compilation_error_stream << "line " << line_number << ": " << message << "\n";    
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

void chomik::name_item_string::add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const
{
    target.add_content(std::make_shared<simple_value_string_signature_item>(*this, my_value));
}

std::string chomik::machine::get_variable_value_enum(const signature & vn) const
{
    if (get_variable_is_represented_in_memory(vn))
    {            
        return get_variable_with_value(vn).get_value_enum();
    }
    else
    {
        DEBUG("it is NOT in memory");                
        DEBUG("iterate backwards through the assignment events (there are " << get_vector_of_assignment_events().size() << " of them)");
        
        generator our_generator(__FILE__, __LINE__);
        
        
        for (int i=get_vector_of_assignment_events().size()-1; i>=0; i--)
        {
            const std::unique_ptr<assignment_event>& a{get_vector_of_assignment_events()[i]};
            DEBUG("found " << *a);
            
            matching_protocol mprotocol;
            if (a->get_match(vn, *this, our_generator, mprotocol))
            {
                DEBUG("it is matching our signature !!!");
                return a->get_source().get_actual_identifier_value(*this, our_generator);
            }
            else
            {
                DEBUG("it is not matching the signature !!!, got matching_protocol " << mprotocol);
                
                mprotocol.copy_bound_placeholders(our_generator);                                
                
                
                matching_protocol mprotocol2;
                
                if (a->get_match(vn, *this, our_generator, mprotocol2))
                {                    
                    DEBUG("it is matching for the generator " << our_generator);
                                        
                    switch (a->get_source().get_actual_memory_representation_type())
                    {
                        case variable_with_value::actual_memory_representation_type::ENUM:
                            DEBUG("the enum is " << a->get_source().get_actual_identifier_value(*this, our_generator));
                                                        
                            return a->get_source().get_actual_identifier_value(*this, our_generator);
                            
                        default:;
                            // TODO
                    }
                }
                
            }
        }                                
    }    
    
    return "";
}


void chomik::matching_protocol::copy_bound_placeholders(generator & target) const
{
    DEBUG("copy placeholders");

    for (auto a=map_placeholder_names_to_integer.begin(); a!=map_placeholder_names_to_integer.end(); a++)
    {
        DEBUG("make a placeholder " << a->first << " with value " << a->second);
        
        auto b=std::make_shared<generic_type_named>("integer");
        target.add_placeholder(a->first, std::move(b));
        
        auto c = std::make_shared<simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>>(a->first, a->second, nullptr);
                
        target.add_placeholder_with_value(std::move(c));
    }

    for (auto a=map_placeholder_names_to_code.begin(); a!=map_placeholder_names_to_code.end(); a++)
    {
        DEBUG("make a placeholder " << a->first << " with value " << a->second);
        auto b=std::make_shared<generic_type_named>("code");
        target.add_placeholder(a->first, std::move(b));

        auto c = std::make_shared<simple_placeholder_with_value_and_report<code, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::CODE)>>(a->first, a->second, nullptr);

        target.add_placeholder_with_value(std::move(c));
    }
    
    DEBUG("produced " << target);
    
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
    DEBUG("get float for " << vn << ", got " << get_variable_with_value(vn).get_value_float() << ", an object of class " << get_variable_with_value(vn).get_debug_type_name());
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


chomik::variable_with_value & chomik::machine::get_variable_with_value_by_index(int index)
{
    // TODO error handling
    return *memory[index];
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



void chomik::placeholder_name_item::add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const
{    
    DEBUG("checking whether " << g << " has placeholder " << placeholder);
    
    if (g.get_has_placeholder_with_value(placeholder))
    {
        switch (g.get_placeholder_with_value(placeholder).get_representation_type())
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_content(std::make_shared<simple_value_integer_signature_item>(*this, g.get_placeholder_value_integer(placeholder)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_content(std::make_shared<simple_value_float_signature_item>(*this, g.get_placeholder_value_float(placeholder)));
                break;

            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_content(std::make_shared<simple_value_string_signature_item>(*this, g.get_placeholder_value_string(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, g.get_placeholder_value_enum(placeholder)));
                break;                
                
            case variable_with_value::actual_memory_representation_type::CODE:
                {
                code c;
                g.get_placeholder_value_code(placeholder, c);
                target.add_content(std::make_shared<code_signature_item>(*this, c));
                }
                break;                
        }
    }
    else
    {
        DEBUG("no, it doesn't");

        //g.debug();

        
        //target.add_content(std::make_shared<simple_value_integer_signature_item>(*this, 1));
        // TODO fixme

    }
    
}



bool chomik::code_name_item::get_match_code(const code & v) const
{
    return *my_code == v;
}


bool chomik::code::operator==(const code & c) const
{
    return c.body == body;
}


void chomik::generic_value_literal::get_copy(std::unique_ptr<generic_value> & target) const
{
    DEBUG("get copy of a literal " << *literal);

    std::unique_ptr<generic_literal> i;
    literal->get_copy(i);
    target = std::make_unique<generic_value_literal>(std::move(i));
}


bool chomik::generic_name::operator==(const generic_name & n) const
{
    if (vector_of_name_items.size() != n.vector_of_name_items.size())
        return false;

    // TODO implement me

    return true;
}

bool chomik::variable_definition::operator==(const variable_definition & d) const
{
    return name == d.name && type_name->get_is_equal(*d.type_name);
}


bool chomik::variable_definition_statement::operator==(const variable_definition_statement & s) const
{
    if (vector_of_variable_definitions.size() != s.vector_of_variable_definitions.size())
        return false;

    for (int i=0; i<vector_of_variable_definitions.size(); i++)
    {
        const variable_definition& d1=static_cast<const variable_definition&>(*vector_of_variable_definitions[i]);
        const variable_definition& d2=static_cast<const variable_definition&>(*s.vector_of_variable_definitions[i]);
        if (d1 == d2)
            continue;
        return false;
    }

    return true;
}

bool chomik::list_of_statements::operator==(const list_of_statements & l) const
{
    if (l.vector_of_statements.size() != vector_of_statements.size())
        return false;

    for (int i=0; i<vector_of_statements.size(); i++)
    {
        if (vector_of_statements[i]->get_statement_type() != l.vector_of_statements[i]->get_statement_type())
        {
            return false;
        }

        switch (vector_of_statements[i]->get_statement_type())
        {
            case statement::statement_type::TYPE_DEFINITION:
                // TODO implement me
                break;

            case statement::statement_type::VARIABLE_DEFINITION:
                {
                    const variable_definition_statement& s1=static_cast<const variable_definition_statement&>(*vector_of_statements[i]);
                    const variable_definition_statement& s2=static_cast<const variable_definition_statement&>(*l.vector_of_statements[i]);
                    if (s1 == s2)
                        continue;
                    return false;
                }
                break;

            case statement::statement_type::ASSIGNMENT:
                // TODO implement me
                break;

            case statement::statement_type::EXECUTE:
                // TODO implement me
                break;

            case statement::statement_type::EXPAND:
                // TODO implement me
                break;
        }
    }

    return true;
}



void chomik::matching_protocol::bind_placeholder_as_code(const std::string & p, const code & c)
{
    auto [it, s] = map_placeholder_names_to_code.insert(std::pair(p,c));
    if (!s) throw std::runtime_error("failed to bind a placeholder");
}


bool chomik::code_signature_item::get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const
{
    DEBUG("get match for " << gni << " against " << *my_code);
    if (gni.get_is_code())
    {
        DEBUG(gni << " is a code");
        return gni.get_match_code(*my_code);
    }
    else
    if (gni.get_is_placeholder())
    {
        const auto * gni2 = static_cast<const placeholder_name_item*>(&gni);

        DEBUG(gni << " is a placeholder of type " << gni2->get_type());
        if (g.get_has_placeholder_with_value(gni.get_placeholder_name()))
        {
            DEBUG("it has a placeholder with value " << g << ", namely the placeholder " << gni.get_placeholder_name());

            switch (gni2->get_type().get_actual_memory_representation_type(m))
            {
                case variable_with_value::actual_memory_representation_type::CODE:
                    {
                    code c;
                    g.get_placeholder_value_code(gni.get_placeholder_name(), c);

                    DEBUG("placeholder value " << c);
                    return c == *my_code;
                    }
                    break;
            }
        }
        else
        {
            DEBUG("the generator " << g << " has no placeholder " << gni.get_placeholder_name());

            if (source.get_placeholder_name() != "")
            {
                DEBUG("bind " << gni.get_placeholder_name() << " to " << source.get_placeholder_name());
                target.bind_placeholder_as_placeholder(gni.get_placeholder_name(), source.get_placeholder_name());
            }
            else
            {
                DEBUG("bind " << gni.get_placeholder_name() << " as " << *my_code);
                target.bind_placeholder_as_code(gni.get_placeholder_name(), *my_code);
            }
            return false;
        }
    }
    return false;
}



bool chomik::simple_value_integer_signature_item::get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const
{
    DEBUG("get_match for " << gni << " against " << value);
    
    
    if (gni.get_is_integer())
    {
        DEBUG(gni << " is an integer");
        return gni.get_match_integer(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        const auto * gni2 = static_cast<const placeholder_name_item*>(&gni);
                
        DEBUG(gni << " is a placeholder of type " << gni2->get_type());
        
        if (g.get_has_placeholder_with_value(gni.get_placeholder_name()))
        {
            DEBUG("it has a placeholder with value " << g << ", namely the placeholder " << gni.get_placeholder_name() << " has value " <<      
                g.get_placeholder_value_integer(gni.get_placeholder_name()));
            
            switch (gni2->get_type().get_actual_memory_representation_type(m))
            {
                case variable_with_value::actual_memory_representation_type::INTEGER:
                    if (target.get_is_placeholder_bound_as_integer(gni.get_placeholder_name()))      // this is necessary since we might use the same placeholder twice
                    {                                                
                        return value == target.get_placeholder_value_integer(gni.get_placeholder_name());
                    }
                    else
                    {                        
                        DEBUG("looking for " << gni.get_placeholder_name() << ", translated to " 
                            << g.convert_childs_placeholder_name_to_father(gni.get_placeholder_name()));
                        
                        DEBUG("placeholder value " << g.get_placeholder_value_integer(gni.get_placeholder_name()));
                        
                        if (value == g.get_placeholder_value_integer(gni.get_placeholder_name()))
                        {
                            target.bind_placeholder_as_integer(gni.get_placeholder_name(), value);
                            return target.get_is_successful();                            
                        }
                        else
                        {
                            target.bind_placeholder_as_integer(gni.get_placeholder_name(), value);
                        }
                        return false;
                    }
                    break;
                    
                default:
                    // TODO - implement me
                    break;
            }
        }
        else
        {
            DEBUG("the generator " << g << " has no placeholder " << gni.get_placeholder_name());                        
            
            if (source.get_placeholder_name() != "")
            {            
                DEBUG("bind " << gni.get_placeholder_name() << " to " << source.get_placeholder_name());
                target.bind_placeholder_as_placeholder(gni.get_placeholder_name(), source.get_placeholder_name());
            }
            else
            {
                DEBUG("bind " << gni.get_placeholder_name() << " as " << value);
                target.bind_placeholder_as_integer(gni.get_placeholder_name(), value);
            }
            return false;
        }
    }    
    return false;
}


bool chomik::simple_value_float_signature_item::get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const
{
    DEBUG("get_match for " << gni << " against " << value);

    if (gni.get_is_float())
    {
        return gni.get_match_float(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        if (target.get_is_placeholder_bound_as_float(gni.get_placeholder_name()))           // this is necessary since we might use the same placeholder twice
        {
            DEBUG(gni.get_placeholder_name() << " has already been bound");
            return value == target.get_placeholder_value_float(gni.get_placeholder_name());
        }
        else
        {
            DEBUG("bind " << gni.get_placeholder_name() << " as " << std::showpoint << value);
            target.bind_placeholder_as_float(gni.get_placeholder_name(), value);
            return target.get_is_successful();
        }
    }    
    return false;
}


bool chomik::simple_value_string_signature_item::get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const
{
    DEBUG("get_match for " << gni << " against " << value);
    
    if (gni.get_is_string())
    {
        return gni.get_match_string(value);
    }
    else
    if (gni.get_is_placeholder())
    {
        const auto * gni2 = static_cast<const placeholder_name_item*>(&gni);
                
        DEBUG(gni << " is a placeholder of type " << gni2->get_type());
        
        
        if (g.get_has_placeholder_with_value(gni.get_placeholder_name()))
        {
            const simple_placeholder_with_value_and_report<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)> & pwv = static_cast<const simple_placeholder_with_value_and_report<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>&>(g.get_placeholder_with_value(gni.get_placeholder_name()));
            
            DEBUG("g has this placeholder with value " << pwv);
                        
            return value == pwv.get_value();
        }
        else
        {                        
            if (target.get_is_placeholder_bound_as_string(gni.get_placeholder_name()))           // this is necessary since we might use the same placeholder twice
            {
                DEBUG("the placeholder " << gni.get_placeholder_name() << " is bound as string");
                DEBUG("check whether " << value << "==" << target.get_placeholder_value_string(gni.get_placeholder_name()));            
                return value == target.get_placeholder_value_string(gni.get_placeholder_name());   
            }
            else
            {
                DEBUG("bind " << gni.get_placeholder_name() << " as " << value);
                target.bind_placeholder_as_string(gni.get_placeholder_name(), value);
                return target.get_is_successful();
            }
        }
    }    
    return false;
}


bool chomik::simple_value_enum_signature_item::get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const
{
    DEBUG("get_match for " << gni << " against " << get_enum());
    
    if (gni.get_is_identifier())
    {
        return gni.get_match_identifier(get_enum());
    }
    else
    if (gni.get_is_placeholder())
    {
        if (target.get_is_placeholder_bound_as_identifier(gni.get_placeholder_name()))           // this is necessary since we might use the same placeholder twice
        {
            return get_enum() == target.get_placeholder_value_identifier(gni.get_placeholder_name());
        }
        else
        {
            target.bind_placeholder_as_identifier(gni.get_placeholder_name(), get_enum());
            return target.get_is_successful();
        }
    }    
    return false;
}

bool chomik::assignment_event::get_match(const signature & s, const machine & m, const basic_generator & g, matching_protocol & target) const
{
    DEBUG("checking whether it is matching the signature " << s << " for " << g);
    DEBUG("my event - " << *this);
    
    auto sv{s.get_vector_of_items()};   // this signature must contain actual values
    auto sq{my_assignment_target->get_vector_of_name_items()};  // this signature may contain placeholders    
    
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
        
        if (!(*sv_iterator)->get_match(**sq_iterator, m, g, target))
        {
            result = false;
            DEBUG("no, " << **sv_iterator << " does not match " << **sq_iterator);
            // we had a break here, now we don't, since we might use the matching protocol
        }        
    }        
    
    DEBUG("matching result = " << (result ? "true":"false"));
    
    return result;
}


void chomik::variable_value_name_item::add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const
{
    signature x{*name, m, g};
    
    DEBUG("signature " << x << " add variable value to signature");
    
    if (m.get_variable_is_represented_in_memory(x))
    {
        switch (m.get_actual_memory_representation_type_of_the_variable(x))
        {
            case variable_with_value::actual_memory_representation_type::INTEGER:
                target.add_content(std::make_shared<simple_value_integer_signature_item>(*this, m.get_variable_value_integer(x)));
                break;
                
            case variable_with_value::actual_memory_representation_type::FLOAT:
                target.add_content(std::make_shared<simple_value_float_signature_item>(*this, m.get_variable_value_float(x)));
                break;
                
            case variable_with_value::actual_memory_representation_type::STRING:
                target.add_content(std::make_shared<simple_value_string_signature_item>(*this, m.get_variable_value_string(x)));
                break;
                
            case variable_with_value::actual_memory_representation_type::ENUM:
                target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, m.get_variable_value_enum(x)));
                break;
                                
            case variable_with_value::actual_memory_representation_type::CODE:
                {
                code c;
                m.get_variable_value_code(x, c);
                target.add_content(std::make_shared<code_signature_item>(*this, c));
                }
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
            
            matching_protocol mprotocol;
                        
            if (a->get_match(x, m, g, mprotocol))
            {
                DEBUG("it is matching our signature !!!");
                                
                switch (a->get_source().get_actual_memory_representation_type())
                {
                    case variable_with_value::actual_memory_representation_type::INTEGER:
                        target.add_content(std::make_shared<simple_value_integer_signature_item>(*this, a->get_source().get_actual_integer_value(m, g)));
                        break;
                        
                    case variable_with_value::actual_memory_representation_type::FLOAT:
                        target.add_content(std::make_shared<simple_value_float_signature_item>(*this, a->get_source().get_actual_float_value(m, g)));
                        break;
                        
                    case variable_with_value::actual_memory_representation_type::STRING:
                        target.add_content(std::make_shared<simple_value_string_signature_item>(*this, a->get_source().get_actual_string_value(m, g)));
                        break;
                        
                    case variable_with_value::actual_memory_representation_type::ENUM:
                        target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, a->get_source().get_actual_identifier_value(m, g)));


                        break;
                                            
                    case variable_with_value::actual_memory_representation_type::CODE:
                        break;
                }
                
                break;
            }
            else
            {
                DEBUG("it is not matching our signature !!!");
                DEBUG("the matching protocol is " << mprotocol);
                
            }
        }                
    }
    
}


chomik::signature::signature(const generic_name & gn, const machine & m, const basic_generator & g)
{    
    for (auto &i: gn.get_vector_of_name_items())
    {
        i->add_content_to_signature(*this, m, g);
    }
    
    DEBUG("the signature constructor produced " << *this);
}

chomik::signature::signature(const generic_name & gn)
{
    for (auto &i: gn.get_vector_of_name_items())
    {
        i->add_content_to_signature(*this);
    }
    DEBUG("the signature constructor produced " << *this);
}

chomik::signature::signature()
{
}


chomik::variable_with_value::actual_memory_representation_type chomik::generic_value_placeholder::get_actual_memory_representation_type(machine & m, basic_generator & g) const
{
    return type_name->get_actual_memory_representation_type(m);
}


chomik::variable_with_value::actual_memory_representation_type chomik::generic_value_variable_value::get_actual_memory_representation_type(machine & m, basic_generator & g) const
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
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("subtract"));
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn12.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_subtract_result_integer=std::make_shared<signature>(gn12);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_subtract_result_integer), 0));

    generic_name gn13;
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("set"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("to"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));    
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("index"));    
    std::shared_ptr<signature> the_set_to_stream_stream_index=std::make_shared<signature>(gn13);    
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_set_to_stream_stream_index), 0));
    
    generic_name gn14;
    gn14.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn14.add_generic_name_item(std::make_shared<identifier_name_item>("break"));
    gn14.add_generic_name_item(std::make_shared<identifier_name_item>("flag"));
    std::shared_ptr<signature> the_break_flag=std::make_shared<signature>(gn14);    
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_break_flag), "false"));    
    
    
    generic_name gn15;
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("max"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("size"));
    std::shared_ptr<signature> the_read_from_stream_max_size=std::make_shared<signature>(gn15);    
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_read_from_stream_max_size), 0));
    
    
    generic_name gn16;
    gn16.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn16.add_generic_name_item(std::make_shared<identifier_name_item>("program"));
    gn16.add_generic_name_item(std::make_shared<identifier_name_item>("return"));
    std::shared_ptr<signature> the_program_return=std::make_shared<signature>(gn16);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_program_return), 0));
    
    
    generic_name gn17;
    gn17.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn17.add_generic_name_item(std::make_shared<identifier_name_item>("multiply"));
    gn17.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn17.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_multiply_result_integer=std::make_shared<signature>(gn17);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_multiply_result_integer), 1));

    generic_name gn18;
    gn18.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn18.add_generic_name_item(std::make_shared<identifier_name_item>("divide"));
    gn18.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn18.add_generic_name_item(std::make_shared<name_item_string>("float"));
    std::shared_ptr<signature> the_divide_result_float=std::make_shared<signature>(gn18);
    add_variable_with_value(std::make_shared<simple_variable_with_value_float>(std::move(the_divide_result_float), 1));
    
    generic_name gn19;
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("defined"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_is_defined_result=std::make_shared<signature>(gn19);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_get_is_defined_result), "false"));
    
    generic_name gn20;
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("chomik"));
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("package"));
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("version"));
    std::shared_ptr<signature> the_chomik_package_version=std::make_shared<signature>(gn20);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_chomik_package_version), PACKAGE_VERSION));
    

    generic_name gn21;
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("ad"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("hoc"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_amount_of_ad_hoc_types_result=std::make_shared<signature>(gn21);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_amount_of_ad_hoc_types_result), 0));

    generic_name gn22;
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_amount_of_variables_in_the_memory_result=std::make_shared<signature>(gn22);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_amount_of_variables_in_the_memory_result), 0));

    generic_name gn23;
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("items"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_amount_of_items_in_the_memory_variables_signature_result=std::make_shared<signature>(gn23);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_amount_of_items_in_the_memory_variables_signature_result), 0));

    generic_name gn24;
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("representation"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_signature_item_representation_result=std::make_shared<signature>(gn24);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_get_signature_item_representation_result), ""));

    generic_name gn25;
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("name"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_signature_item_types_name_result=std::make_shared<signature>(gn25);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_get_signature_item_types_name_result), ""));


    generic_name gn26;
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_getline_stream_index=std::make_shared<signature>(gn26);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_getline_stream_index), 2));  // 2 is the standard input


    generic_name gn27;
    gn27.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn27.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    gn27.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_getline_result=std::make_shared<signature>(gn27);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_getline_result), ""));

    generic_name gn28;
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("regular"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_created_signature_regular_expression_index=std::make_shared<signature>(gn28);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_created_signature_regular_expression_index), 0));  //

    for (int i = 1; i<=max_match_group_index; i++)
    {
        generic_name gn29;
        gn29.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn29.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn29.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn29.add_generic_name_item(std::make_shared<name_item_string>("integer"));
        gn29.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::shared_ptr<signature> the_match_group_integer_x=std::make_shared<signature>(gn29);
        add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_match_group_integer_x), 0));

        generic_name gn292;
        gn292.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn292.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn292.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn292.add_generic_name_item(std::make_shared<name_item_string>("boolean"));
        gn292.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::shared_ptr<signature> the_match_group_boolean_x=std::make_shared<signature>(gn292);
        add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_match_group_boolean_x), "false"));
    }

    generic_name gn30;
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_match_expression_index=std::make_shared<signature>(gn30);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_match_expression_index), 0));

    generic_name gn31;
    gn31.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn31.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    gn31.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_match_result=std::make_shared<signature>(gn31);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_match_result), "false"));


    generic_name gn32;
    gn32.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn32.add_generic_name_item(std::make_shared<identifier_name_item>("modulo"));
    gn32.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn32.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_modulo_result_integer=std::make_shared<signature>(gn32);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_modulo_result_integer), 0));

    generic_name gn33;
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("good"));
    std::shared_ptr<signature> the_stream_is_good=std::make_shared<signature>(gn33);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_stream_is_good), "true"));


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

    // also create auxilliary variables for the newly created types
    create_auxilliary_variables_for_type_instance(*boolean_type);
    
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

    // also create auxilliary variables for the newly created types
    create_auxilliary_variables_for_type_instance(*comparison_result_type);

    add_type_instance(std::move(comparison_result_type));    
}

void chomik::machine::create_predefined_streams()
{
    DEBUG("machine::create_predefined_streams");
    
    add_stream(std::make_unique<generic_stream_standard_output_stream>());
    add_stream(std::make_unique<generic_stream_standard_error_stream>());
    add_stream(std::make_unique<generic_stream_standard_input_stream>());    
    
}

int chomik::generic_literal_placeholder::get_actual_integer_value(const machine & m, const basic_generator & g) const
{
    DEBUG("get generic literal placeholder value for " << placeholder << ", generator " << g);
    
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

double chomik::generic_literal_placeholder::get_actual_float_value(const machine & m, const basic_generator & g) const
{
    DEBUG("get generic literal placeholder value for " << placeholder << ", generator " << g);

    if (g.get_has_placeholder_with_value(placeholder))
    {
        DEBUG("got " << g.get_placeholder_value_float(placeholder));
        return g.get_placeholder_value_float(placeholder);
    }

    g.debug();

    std::stringstream s;
    s << "for placeholder " << placeholder << " the generator " << g << " does not contain any value";
    throw std::runtime_error(s.str());

    return 0;
}



std::string chomik::generic_literal_placeholder::get_actual_string_value(const machine & m, const basic_generator & g) const
{ 
    DEBUG("get actual string value for " << placeholder);
    if (g.get_has_placeholder_with_value(placeholder))
    {
        DEBUG("got placeholder with value " << g.get_placeholder_with_value(placeholder));
        DEBUG("the value is " << g.get_placeholder_with_value(placeholder).get_value_string());
    
        const simple_placeholder_with_value_and_report<std::string, 3> & x{static_cast<const simple_placeholder_with_value_and_report<std::string,3>&>(g.get_placeholder_with_value(placeholder))};
    
        return x.get_value();
    }
    return ""; // TODO - is it OK to return it here???
}



int chomik::generic_value_variable_value::get_actual_integer_value(const machine & m, const basic_generator & g) const
{
    signature s{*name, m, g};
    int v{m.get_variable_value_integer(s)};
    
    DEBUG("get integer value for " << s << ", it equals " << v);
    
    return v;
}

double chomik::generic_value_variable_value::get_actual_float_value(const machine & m, const basic_generator & g) const
{
    signature s{*name, m, g};
    double v{m.get_variable_value_float(s)};

    DEBUG("get float value for " << s << ", it equals " << std::showpoint << v);

    return v;
}




std::string chomik::generic_value_variable_value::get_actual_string_value(const machine & m, const basic_generator & g) const
{
    signature s{*name, m, g};
    std::string v{m.get_variable_value_string(s)};
    
    DEBUG("get string value for " << s << ", it equals " << v);
    
    return v;    
}

std::string chomik::generic_value_variable_value::get_actual_enum_value(const machine & m, const basic_generator & g) const
{
    signature s{*name, m, g};
    return m.get_variable_value_enum(s);
}

std::unique_ptr<chomik::signature_common_data> chomik::signature::our_common_data=std::make_unique<signature_common_data>();



chomik::signature_regular_expression::signature_regular_expression(const std::string & c): my_signature_regular_expression_code{c}
{
    parse(my_signature_regular_expression_code);
}


void chomik::signature_regular_expression::parse(const std::string & c)
{
    state my_state = state::INITIAL;
    std::stringstream s(c);
    std::stringstream id;
    std::string placeholder_name;
    char x;
    std::stringstream regular_expression_code_stream;

    my_name = std::make_unique<generic_name>();

    do
    {
        x = s.get();
        if (s.good())
        {
            //std::cout << "state " << static_cast<int>(my_state) << "\n";
            switch (my_state)
            {
                case state::INITIAL:
                    if (std::isblank(x))
                    {
                        continue;
                    }
                    else
                    if (std::isalpha(x) || x=='_')
                    {
                        my_state = state::IDENTIFIER;
                        id.str("");
                        id.clear();
                        id << x;
                        continue;
                    }
                    else
                    if (x == '(')
                    {
                        my_state = state::PLACEHOLDER_1;
                        id.str("");
                        id.clear();
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error1");
                    }
                    break;

                case state::IDENTIFIER:
                    if (std::isblank(x))
                    {
                        //std::cout << "identifier " << id.str() << "\n";
                        regular_expression_code_stream << id.str();

                        std::shared_ptr<generic_name_item> my_name_item=std::make_shared<identifier_name_item>(id.str());
                        vector_of_items.push_back(std::make_shared<simple_value_enum_signature_item>(*my_name_item, id.str()));

                        my_name->add_generic_name_item(std::move(my_name_item));

                        my_state = state::INITIAL;
                        continue;
                    }
                    else
                    if (std::isalpha(x) || x=='_')
                    {
                        id << x;
                        continue;
                    }
                    else
                    if (x == '(')
                    {
                        //std::cout << "identifier " << id.str() << "\n";
                        regular_expression_code_stream << id.str();

                        std::shared_ptr<generic_name_item> my_name_item=std::make_shared<identifier_name_item>(id.str());
                        vector_of_items.push_back(std::make_shared<simple_value_enum_signature_item>(*my_name_item, id.str()));

                        my_name->add_generic_name_item(std::move(my_name_item));

                        my_state = state::PLACEHOLDER_1;
                        id.str("");
                        id.clear();
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error2");
                    }
                    break;

                case state::PLACEHOLDER_1:
                    if (std::isblank(x))
                    {
                        continue;
                    }
                    else
                    if (std::isalpha(x) || x=='_')
                    {
                        my_state = state::PLACEHOLDER_2;
                        id.str("");
                        id.clear();
                        id << x;
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error3");
                    }
                    break;

                case state::PLACEHOLDER_2:
                    if (std::isblank(x))
                    {
                        my_state = state::PLACEHOLDER_3;
                        //std::cout << "placeholder " << id.str() << "\n";
                        placeholder_name = id.str();
                        continue;
                    }
                    else
                    if (std::isalpha(x) || x=='_')
                    {
                        id << x;
                        continue;
                    }
                    else
                    if (x == ':')
                    {
                        //std::cout << "placeholder " << id.str() << "\n";
                        placeholder_name = id.str();
                        my_state = state::PLACEHOLDER_4;
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error4");
                    }
                    break;

                case state::PLACEHOLDER_3:
                    if (std::isblank(x))
                    {
                        continue;
                    }
                    else
                    if (x == ':')
                    {
                        my_state = state::PLACEHOLDER_4;
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error5");
                    }
                    break;

                case state::PLACEHOLDER_4:
                    if (std::isblank(x))
                    {
                        continue;
                    }
                    else
                    if (std::isalpha(x) || x=='_')
                    {
                        my_state = state::PLACEHOLDER_5;
                        id.str("");
                        id.clear();
                        id << x;
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error6");
                    }
                    break;

                case state::PLACEHOLDER_5:
                    if (std::isblank(x))
                    {
                        //std::cout << "placeholder_type " << id.str() << "\n";
                        if (id.str() == "integer")
                        {
                            regular_expression_code_stream << "(\\d+)";
                        }
                        else
                        if (id.str() == "boolean")
                        {
                            regular_expression_code_stream << "(false|true)";
                        }
                        else
                        {
                            std::stringstream error_message_stream;
                            error_message_stream << "integer expected, got \"" << id.str() << "\"";
                            throw std::runtime_error(error_message_stream.str());
                        }

                        my_state = state::PLACEHOLDER_6;
                        continue;
                    }
                    else
                    if (std::isalpha(x) || x=='_')
                    {
                        id << x;
                        continue;
                    }
                    else
                    if (x == ')')
                    {
                        //std::cout << "placeholder_type " << id.str() << "\n";
                        if (id.str() == "integer")
                        {
                            regular_expression_code_stream << "(\\d+)";
                        }
                        else
                        if (id.str() == "boolean")
                        {
                            regular_expression_code_stream << "(false|true)";
                        }
                        else
                        {
                            std::stringstream error_message_stream;
                            error_message_stream << "integer expected, got \"" << id.str() << "\"";
                            throw std::runtime_error(error_message_stream.str());
                        }

                        my_state = state::INITIAL;
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error7");
                    }
                    break;

                case state::PLACEHOLDER_6:
                    if (std::isblank(x))
                    {
                        continue;
                    }
                    else
                    if (x == ')')
                    {
                        my_state = state::INITIAL;
                        continue;
                    }
                    else
                    {
                        throw std::runtime_error("error8");
                    }
                    break;
            }
        }
    }
    while (!s.eof());

    //std::cout << "regular_expression_code=" << regular_expression_code_stream.str() << "\n";

    my_regular_expression = std::make_unique<std::regex>(regular_expression_code_stream.str());


}


chomik::signature_common_data::signature_common_data()
{    
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("target"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        
            
    generic_name_the_print_separator.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_print_separator.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    generic_name_the_print_separator.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));

    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("end"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("line"));

    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("set"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("to"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));        
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        

    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));        
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        

    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));


    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("source"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));        
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));        

    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));

    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<name_item_string>("string"));
    
    
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("max"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("size"));

    generic_name_the_compare_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_compare_result.add_generic_name_item(std::make_shared<identifier_name_item>("compare"));
    generic_name_the_compare_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));                    

    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("multiply"));
    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));
                

    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("divide"));
    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<name_item_string>("float"));


    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));


    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("subtract"));
    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));


    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("defined"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("ad"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("hoc"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("items"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("representation"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("name"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_getline_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_getline_result.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    generic_name_the_getline_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("regular"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_match_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_match_result.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    generic_name_the_match_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("modulo"));
    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));

    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("good"));


    signature_the_print_target_stream_index = std::make_unique<signature>(generic_name_the_print_target_stream_index);
    signature_the_print_separator = std::make_unique<signature>(generic_name_the_print_separator);
    signature_the_print_end_of_line = std::make_unique<signature>(generic_name_the_print_end_of_line);
    signature_the_created_stream_index = std::make_unique<signature>(generic_name_the_created_stream_index);
    signature_the_set_to_stream_stream_index = std::make_unique<signature>(generic_name_the_set_to_stream_stream_index);
    signature_the_get_from_stream_stream_index = std::make_unique<signature>(generic_name_the_get_from_stream_stream_index);
    signature_the_get_from_stream_result = std::make_unique<signature>(generic_name_the_get_from_stream_result);    
    signature_the_read_from_stream_source_stream_index = std::make_unique<signature>(generic_name_the_read_from_stream_source_stream_index);    
    signature_the_read_from_stream_result_integer = std::make_unique<signature>(generic_name_the_read_from_stream_result_integer);
    signature_the_read_from_stream_result_string = std::make_unique<signature>(generic_name_the_read_from_stream_result_string);
    signature_the_read_from_stream_max_size = std::make_unique<signature>(generic_name_the_read_from_stream_max_size);    
    signature_the_compare_result = std::make_unique<signature>(generic_name_the_compare_result);
    signature_the_multiply_result_integer = std::make_unique<signature>(generic_name_the_multiply_result_integer);
    signature_the_divide_result_float = std::make_unique<signature>(generic_name_the_divide_result_float);    
    signature_the_add_result_integer = std::make_unique<signature>(generic_name_the_add_result_integer);
    signature_the_subtract_result_integer = std::make_unique<signature>(generic_name_the_subtract_result_integer);
    signature_the_get_is_defined_result = std::make_unique<signature>(generic_name_the_get_is_defined_result);
    signature_the_get_amount_of_ad_hoc_types_result = std::make_unique<signature>(generic_name_the_get_amount_of_ad_hoc_types_result);
    signature_the_get_amount_of_variables_in_the_memory_result = std::make_unique<signature>(generic_name_the_get_amount_of_variables_in_the_memory_result);
    signature_the_get_amount_of_items_in_the_memory_variables_signature_result =
        std::make_unique<signature>(generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result);
    signature_the_get_signature_item_representation_result = std::make_unique<signature>(generic_name_the_get_signature_item_representation_result);
    signature_the_get_signature_item_types_name_result = std::make_unique<signature>(generic_name_the_get_signature_item_types_name_result);
    signature_the_getline_stream_index = std::make_unique<signature>(generic_name_the_getline_stream_index);
    signature_the_getline_result = std::make_unique<signature>(generic_name_the_getline_result);
    signature_the_created_signature_regular_expression_index = std::make_unique<signature>(generic_name_the_created_signature_regular_expression_index);
    signature_the_match_expression_index = std::make_unique<signature>(generic_name_the_match_expression_index);
    signature_the_match_result = std::make_unique<signature>(generic_name_the_match_result);
    signature_the_modulo_result_integer = std::make_unique<signature>(generic_name_the_modulo_result_integer);
    signature_the_stream_is_good = std::make_unique<signature>(generic_name_the_stream_is_good);

    for (int i=1; i<=machine::max_match_group_index; i++)
    {
        generic_name gn;
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn.add_generic_name_item(std::make_shared<name_item_string>("integer"));
        gn.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::unique_ptr<signature> the_match_group_integer_x=std::make_unique<signature>(gn);
        signature_the_match_group_integer_x.push_back(std::move(the_match_group_integer_x));

        generic_name gn2;
        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn2.add_generic_name_item(std::make_shared<name_item_string>("boolean"));
        gn2.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::unique_ptr<signature> the_match_group_boolean_x=std::make_unique<signature>(gn2);
        signature_the_match_group_boolean_x.push_back(std::move(the_match_group_boolean_x));
    }
}


void chomik::signature::execute_predefined_print(machine & m) const
{
        int index = m.get_variable_with_value(*our_common_data->signature_the_print_target_stream_index).get_value_integer();
        
        DEBUG("value of the print target stream index " << index);
        
        if (index >= 0 && index < m.get_amount_of_streams())
        {                        
            std::string separator = m.get_variable_with_value(*our_common_data->signature_the_print_separator).get_value_string();
            
            DEBUG("the print separator is \"" << separator << "\"");
            
            std::string end_of_line = m.get_variable_with_value(*our_common_data->signature_the_print_end_of_line).get_value_string();
            
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
                if (m.get_can_create_files())
                {
                    m.add_stream(std::make_unique<generic_stream_file_output>(vector_of_items[4]->get_value_string()));
                }
                else
                {
                    m.add_stream(std::make_unique<generic_stream_stringstream>());
                }
                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());
                                
                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());

                set_stream_flags(m, gs);
                
                return;
            }
            else
            if (vector_of_items[1]->get_it_is_identifier("new")
                && vector_of_items[2]->get_it_is_identifier("input")
                && vector_of_items[3]->get_it_is_identifier("filestream")
                && vector_of_items[4]->get_it_is_string())
            {
                if (m.get_can_create_files())
                {
                    m.add_stream(std::make_unique<generic_stream_file_input>(vector_of_items[4]->get_value_string()));
                }
                else
                {
                    m.add_stream(std::make_unique<generic_stream_stringstream>());
                }

                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                DEBUG("assign value integer " << m.get_last_created_stream_index());

                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                set_stream_flags(m, gs);
                return;
            }
            else
            if (vector_of_items[1]->get_it_is_identifier("new") 
                && vector_of_items[2]->get_it_is_identifier("input") 
                && vector_of_items[3]->get_it_is_identifier("stringstream")
                && vector_of_items[4]->get_it_is_string())
            {
                m.add_stream(std::make_unique<generic_stream_stringstream>(vector_of_items[4]->get_value_string()));

                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());
                
                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                set_stream_flags(m, gs);
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
                
                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                
                set_stream_flags(m, gs);

                return;
            }
        }
        else
        if (vector_of_items.size() == 6)
        {
            if (vector_of_items[1]->get_it_is_identifier("new")
                && vector_of_items[2]->get_it_is_identifier("signature")
                && vector_of_items[3]->get_it_is_identifier("regular")
                && vector_of_items[4]->get_it_is_identifier("expression")
                && vector_of_items[5]->get_it_is_string()
                )
            {
                // here we parse the signature regular expression
                m.add_signature_regular_expression(std::make_unique<signature_regular_expression>(vector_of_items[5]->get_value_string()));

                DEBUG("assign value integer " << m.get_last_created_signature_regular_expression_index());

                m.get_variable_with_value(
                    *our_common_data->signature_the_created_signature_regular_expression_index)
                        .assign_value_integer(m.get_last_created_signature_regular_expression_index());
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

                    generic_stream & gs = m.get_stream(m.get_last_created_stream_index());
                
                    DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                    m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());                

                    set_stream_flags(m, gs);

                    return;
                }
            }
        }
        else
        if (vector_of_items.size() == 7)
        {
            if (vector_of_items[1]->get_it_is_identifier("new") 
                && vector_of_items[2]->get_it_is_identifier("input") 
                && vector_of_items[3]->get_it_is_identifier("random")
                && vector_of_items[4]->get_it_is_identifier("enum")
                && vector_of_items[5]->get_it_is_identifier("stream")
                && vector_of_items[6]->get_it_is_string())  // enum type name
            {
                m.add_stream(std::make_unique<generic_stream_random_enum_stream>(vector_of_items[6]->get_value_string(), m));
                
                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                DEBUG("assign value integer " << m.get_last_created_stream_index());
                
                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());                                

                set_stream_flags(m, gs);
                return;
            }
        }
            
        CHOMIK_STDERR("warning: unknown create variable\n");        
        for (auto & i: vector_of_items)
        {
            CHOMIK_STDERR(*i << ' ');
        }    
        CHOMIK_STDERR('\n');    
}


void chomik::signature::execute_predefined_set(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_identifier("to") 
                && vector_of_items[2]->get_it_is_identifier("stream") 
                && vector_of_items[3]->get_it_is_string())
            {
                
                //the set to stream result stream index

                int index = m.get_variable_with_value(*our_common_data->signature_the_set_to_stream_stream_index).get_value_integer();
        
                DEBUG("value of the set to stream stream index " << index);
                if (index >= 0 && index < m.get_amount_of_streams())
                {
                    generic_stream& gs{m.get_stream(index)};                                                        
                    
                    gs.set_result(vector_of_items[3]->get_value_string());
                                    
                    DEBUG("assign value string " << gs.get_result());

                    set_stream_flags(m, gs);

                    return;
                }
                return;
            }
        }
        
    CHOMIK_STDERR("warning: unknown set variable\n");        
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }    
    CHOMIK_STDERR('\n');             
}

void chomik::signature::set_stream_flags(machine & m, generic_stream & gs) const
{
    if (gs.get_is_good()) m.get_variable_with_value(*our_common_data->signature_the_stream_is_good).assign_value_enum("true");
    else m.get_variable_with_value(*our_common_data->signature_the_stream_is_good).assign_value_enum("false");
}


void chomik::signature::execute_predefined_getline(machine & m) const
{
    if (vector_of_items.size() == 1)
    {
        //the get from stream result stream index
        int index = m.get_variable_with_value(*our_common_data->signature_the_getline_stream_index).get_value_integer();

        DEBUG("value of the getline stream index " << index);
        if (index >= 0 && index < m.get_amount_of_streams())
        {
            generic_stream& gs{m.get_stream(index)};

            gs.getline();

            std::string result = gs.getline_result();

            DEBUG("assign value string " << result);

            m.get_variable_with_value(*our_common_data->signature_the_getline_result).assign_value_string(result);

            set_stream_flags(m, gs);

            return;
        }
        return;
    }

    CHOMIK_STDERR("warning: unknown getline variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }
    CHOMIK_STDERR('\n');

}

void chomik::signature::execute_predefined_get(machine & m) const
{
        if (vector_of_items.size() == 3)
        {
            if (vector_of_items[1]->get_it_is_identifier("from") 
                && vector_of_items[2]->get_it_is_identifier("stream"))
            {
                
                //the get from stream result stream index                
                int index = m.get_variable_with_value(*our_common_data->signature_the_get_from_stream_stream_index).get_value_integer();
        
                DEBUG("value of the get from stream stream index " << index);
                if (index >= 0 && index < m.get_amount_of_streams())
                {
                    generic_stream& gs{m.get_stream(index)};                        
                                                
                    DEBUG("assign value string " << gs.get_result());
                
                    m.get_variable_with_value(*our_common_data->signature_the_get_from_stream_result).assign_value_string(gs.get_result());                                    

                    set_stream_flags(m, gs);
                    return;
                }
                return;
            }
        }


        if (vector_of_items.size() == 6)
        {
            if (vector_of_items[1]->get_it_is_identifier("amount")
                && vector_of_items[2]->get_it_is_identifier("of")
                && vector_of_items[3]->get_it_is_identifier("ad")
                && vector_of_items[4]->get_it_is_identifier("hoc")
                && vector_of_items[5]->get_it_is_identifier("types")
            )
            {
                m.get_variable_with_value(*our_common_data->signature_the_get_amount_of_ad_hoc_types_result).assign_value_integer(m.get_amount_of_ad_hoc_type_instances());
                return;
            }

            // get signature item representation (VARIABLE_INDEX:integer) (ITEM_INDEX:integer)
            if (vector_of_items[1]->get_it_is_identifier("signature")
                && vector_of_items[2]->get_it_is_identifier("item")
                && vector_of_items[3]->get_it_is_identifier("representation")
                && vector_of_items[4]->get_it_is_integer()  // index of the memory variable
                && vector_of_items[5]->get_it_is_integer()  // index of the item
            )
            {
                m.get_variable_with_value(*our_common_data->
                    signature_the_get_signature_item_representation_result).assign_value_string(
                        m.get_signature_item_representation(vector_of_items[4]->get_value_integer(), vector_of_items[5]->get_value_integer()));
                return;
            }

        }

        if (vector_of_items.size() == 7)
        {
            if (vector_of_items[1]->get_it_is_identifier("amount")
                && vector_of_items[2]->get_it_is_identifier("of")
                && vector_of_items[3]->get_it_is_identifier("variables")
                && vector_of_items[4]->get_it_is_identifier("in")
                && vector_of_items[5]->get_it_is_identifier("the")
                && vector_of_items[6]->get_it_is_identifier("memory")
            )
            {
                m.get_variable_with_value(*our_common_data->signature_the_get_amount_of_variables_in_the_memory_result)
                    .assign_value_integer(m.get_amount_of_variables_in_the_memory());
                return;
            }

            if (vector_of_items[1]->get_it_is_identifier("signature")
                && vector_of_items[2]->get_it_is_identifier("item")
                && vector_of_items[3]->get_it_is_identifier("types")
                && vector_of_items[4]->get_it_is_identifier("name")
                && vector_of_items[5]->get_it_is_integer() // index of the memory variable
                && vector_of_items[6]->get_it_is_integer() // index of the item
            )
            {
                m.get_variable_with_value(*our_common_data->signature_the_get_signature_item_types_name_result)
                    .assign_value_string(m.get_signature_item_type_name(vector_of_items[5]->get_value_integer(), vector_of_items[6]->get_value_integer()));
                return;
            }
        }


        if (vector_of_items.size() == 10)
        {
            if (vector_of_items[1]->get_it_is_identifier("amount")
                && vector_of_items[2]->get_it_is_identifier("of")
                && vector_of_items[3]->get_it_is_identifier("items")
                && vector_of_items[4]->get_it_is_identifier("in")
                && vector_of_items[5]->get_it_is_identifier("the")
                && vector_of_items[6]->get_it_is_identifier("memory")
                && vector_of_items[7]->get_it_is_identifier("variables")
                && vector_of_items[8]->get_it_is_identifier("signature")
                && vector_of_items[9]->get_it_is_integer())
            {
                m.get_variable_with_value(*our_common_data->signature_the_get_amount_of_items_in_the_memory_variables_signature_result)
                    .assign_value_integer(m.get_amount_of_items_in_the_memory_variables_signature(vector_of_items[9]->get_value_integer()));
                return;
            }
        }

            /* here we implement the family of variables <get is defined ...> which sets the boolean value <the get is defined result>. */
            if (vector_of_items.size() >= 4)
            {
                if (vector_of_items[1]->get_it_is_identifier("is")
                    && vector_of_items[2]->get_it_is_identifier("defined"))
                {                    
                                        
                    signature sn;
                    for (auto i = 3; i<vector_of_items.size(); i++)
                    {
                        std::shared_ptr<signature_item> c;
                        vector_of_items[i]->get_copy(c);
                        sn.add_content(std::move(c));                        
                    }                    
                    if (m.get_variable_is_represented_in_memory(sn) && m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).get_value_enum() == "true")
                    {
                        //m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).assign_value_enum("true");
                    }
                    else
                    {
                        m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).assign_value_enum("false");
                        
                        /*
                        std::cout << "the variable is represented in memory: " << m.get_variable_is_represented_in_memory(sn) << " and the result is "
                            << m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).get_value_enum() << "\n";
                            */
                    }
                    return;
                }
            }
                
        CHOMIK_STDERR("warning: unknown get variable\n");        
        for (auto & i: vector_of_items)
        {
            CHOMIK_STDERR(*i << ' ');
        }    
        CHOMIK_STDERR('\n');         
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
                int index = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_source_stream_index).get_value_integer();
        
                DEBUG("value of the read from stream source stream index " << index);
                    if (index >= 0 && index < m.get_amount_of_streams())
                    {
                    generic_stream& gs{m.get_stream(index)};                        
                                                    
                    int s = gs.read_integer();
                                    
                    DEBUG("read " << s);
                
                    m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_result_integer).assign_value_integer(s);

                    set_stream_flags(m, gs);
                    return;
                    }                
                }
                else
                if (vector_of_items[3]->get_value_string() == "string")
                {                                
                int index = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_source_stream_index).get_value_integer();
        
                DEBUG("value of the print target stream index " << index);
                    if (index >= 0 && index < m.get_amount_of_streams())
                    {
                    generic_stream& gs{m.get_stream(index)};                                                    
                    
                    int max_size = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_max_size).get_value_integer();
                                                            
                    gs.set_max_size(max_size);
                    
                    std::string s = gs.read_string();
                                    
                    DEBUG("read " << s);
                
                    m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_result_string).assign_value_string(s);

                    set_stream_flags(m, gs);
                    return;
                    }
                }
                else
                {
                    int index = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_source_stream_index).get_value_integer();
        
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
                        gn2.add_generic_name_item(std::make_shared<name_item_string>(vector_of_items[3]->get_value_string()));
                        signature the_read_from_stream_result{gn2};    
                        
                        std::string s = gs.read_string();
                                    
                        DEBUG("read " << s);
                
                        m.get_variable_with_value(the_read_from_stream_result).assign_value_enum(s);
                        set_stream_flags(m, gs);
                    }
                }
                return;
            }
        }
        CHOMIK_STDERR("warning: unknown read variable\n");        
        for (auto & i: vector_of_items)
        {
            CHOMIK_STDERR(*i << ' ');
        }    
        CHOMIK_STDERR('\n');         
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
                
                    m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);                                        
                    return;
                }
            }
            else
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_float()
                && vector_of_items[3]->get_it_is_float())
            {                
                if (vector_of_items[1]->get_value_string() == "float")
                {                                
                    std::string s = "";
                    
                    double a = vector_of_items[2]->get_value_float();
                    double b = vector_of_items[3]->get_value_float();
                    
                    DEBUG("got " << std::showpoint << a << " and " << std::showpoint << b);
                    
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
                        s = "equal";    // comparison of floats (in fact doubles) for equality makes little sense, but we allow it
                    }
                
                    m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);                                        
                    return;
                }
            }                
            else
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_enum()
                && vector_of_items[3]->get_it_is_enum())
            {                
                    std::string s = "";
                    std::string a = vector_of_items[2]->get_value_enum();
                    std::string b = vector_of_items[3]->get_value_enum();
                    
                    auto x{strcmp(a.c_str(), b.c_str())};
                    
                    if (x < 0)
                    {
                        s = "lower";
                    }
                    else
                    if (x > 0)
                    {
                        s = "greater";
                    }
                    else
                    {
                        s = "equal";
                    }                    
                    
                    m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);
                    return;
            }
            else
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_string()
                && vector_of_items[3]->get_it_is_string())
            {                                    
                    std::string s = "";
                    std::string a = vector_of_items[2]->get_value_string();
                    std::string b = vector_of_items[3]->get_value_string();
                    
                    auto x{strcmp(a.c_str(), b.c_str())};
                    
                    if (x < 0)
                    {
                        s = "lower";
                    }
                    else
                    if (x > 0)
                    {
                        s = "greater";
                    }
                    else
                    {
                        s = "equal";
                    }                    
                    
                    m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);
                    return;
            }
        }
    CHOMIK_STDERR("warning: unknown compare variable\n");        
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }    
    CHOMIK_STDERR('\n');         
}


void chomik::signature::execute_predefined_multiply(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {                
                if (vector_of_items[1]->get_value_string() == "integer")
                {                                                    
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("signature::execute_predefined_multiply got " << a << " and " << b);
                                    
                    m.get_variable_with_value(*our_common_data->signature_the_multiply_result_integer).assign_value_integer(a*b);
                    return;
                }
            }
        }
    CHOMIK_STDERR("warning: unknown multiply variable\n");        
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }    
    CHOMIK_STDERR('\n');             
}


void chomik::signature::execute_predefined_modulo(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {
                if (vector_of_items[1]->get_value_string() == "integer")
                {
                    std::string s = "";

                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();

                    DEBUG("signature::execute_predefined_modulo got " << a << " and " << b);

                    if (b!=0)
                    {
                        m.get_variable_with_value(*our_common_data->signature_the_modulo_result_integer).assign_value_integer(a%b);
                    }
                    return;
                }
            }
        }
    CHOMIK_STDERR("warning: unknown modulo variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }
    CHOMIK_STDERR('\n');

}



void chomik::signature::execute_predefined_divide(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {                
                if (vector_of_items[1]->get_value_string() == "float")
                {                                
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("signature::execute_predefined_divide got " << a << " and " << b);
                    
                    if (b!=0)
                    {
                        m.get_variable_with_value(*our_common_data->signature_the_divide_result_float).assign_value_float(static_cast<double>(a)/static_cast<double>(b));                        
                    }
                                    
                    return;
                }
            }
        }
    CHOMIK_STDERR("warning: unknown divide variable\n");        
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }    
    CHOMIK_STDERR('\n');

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
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("signature::execute_predefined_add got " << a << " and " << b);
                                    
                    m.get_variable_with_value(*our_common_data->signature_the_add_result_integer).assign_value_integer(a+b);
                    return;
                }
            }
        }
    CHOMIK_STDERR("warning: unknown add variable\n");        
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }    
    CHOMIK_STDERR('\n');         
}

void chomik::signature::execute_predefined_subtract(machine & m) const
{
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_string() 
                && vector_of_items[2]->get_it_is_integer()
                && vector_of_items[3]->get_it_is_integer())
            {                
                if (vector_of_items[1]->get_value_string() == "integer")
                {                                                    
                    std::string s = "";
                    
                    int a = vector_of_items[2]->get_value_integer();
                    int b = vector_of_items[3]->get_value_integer();
                    
                    DEBUG("got " << a << " and " << b);
                                    
                    m.get_variable_with_value(*our_common_data->signature_the_subtract_result_integer).assign_value_integer(a-b);                                        
                    return;
                }
            }
        }
    CHOMIK_STDERR("warning: unknown subtract variable\n");        
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }    
    CHOMIK_STDERR('\n');         
}

void chomik::signature::execute_predefined_execution(machine & m) const
{
        if (vector_of_items.size() == 6)
        {
            if (vector_of_items[1]->get_it_is_identifier("of")
                && vector_of_items[2]->get_it_is_identifier("the")
                && vector_of_items[3]->get_it_is_identifier("memory")
                && vector_of_items[4]->get_it_is_identifier("variables")
                && vector_of_items[5]->get_it_is_integer())
            {
                int index = vector_of_items[5]->get_value_integer();


                if (index >= 0 && index <m.get_amount_of_variables_in_the_memory())
                {
                    DEBUG("execution of the memory variables " << index);

                        code c;
                        m.get_variable_with_value_by_index(index).get_value_code(c);
                        c.execute(m);
                    return;
                }
            }
        }

    CHOMIK_STDERR("warning: unknown execution variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }
    CHOMIK_STDERR('\n');
}


void chomik::signature::execute_predefined_match(machine & m) const
{
    if (vector_of_items.size() >= 2)
    {
        int index = m.get_variable_with_value(*our_common_data->signature_the_match_expression_index).get_value_integer();

        DEBUG("value of the match expression index " << index);
        if (index >= 0 && index < m.get_amount_of_signature_regular_expressions())
        {
            std::stringstream matched_string_stream;
            for (int i=1; i<vector_of_items.size(); i++)
            {
                matched_string_stream << vector_of_items[i]->get_string_representation();
            }
            std::string matched_string{matched_string_stream.str()};

            signature_regular_expression& se{m.get_signature_regular_expression(index)};
            std::smatch my_smatch;

            bool result = std::regex_match(matched_string, my_smatch, se.get_regular_expression());

            if (result)
            {
                for (int i=1; i<my_smatch.size(); i++)
                {
                    if (i < machine::max_match_group_index)
                    {
                        //std::cout << "got " << my_smatch[i] << "\n";

                        try {

                            if (my_smatch[i] == "false" || my_smatch[i] == "true")
                            {
                                m.get_variable_with_value(*our_common_data->signature_the_match_group_boolean_x[i - 1]).assign_value_enum(my_smatch[i]);
                            }
                            else
                            {
                                m.get_variable_with_value(*our_common_data->signature_the_match_group_integer_x[i - 1]).assign_value_integer(std::stoi(my_smatch[i]));
                            }
                        }
                        catch (...)
                        {
                            CHOMIK_STDERR("error on " << my_smatch[i] << '\n');
                        }
                    }
                }
            }
            m.get_variable_with_value(*our_common_data->signature_the_match_result).assign_value_enum(result ? "true" : "false");
        }
        return;
    }

    CHOMIK_STDERR("warning: unknown match variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_STDERR(*i << ' ');
    }
    CHOMIK_STDERR('\n');
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
    if (get_it_has_prefix("modulo"))
    {
        execute_predefined_modulo(m);
    }
    else
    if (get_it_has_prefix("subtract"))
    {
        execute_predefined_subtract(m);
    }
    else
    if (get_it_has_prefix("multiply"))
    {
        execute_predefined_multiply(m);
    }
    else
    if (get_it_has_prefix("divide"))
    {
        execute_predefined_divide(m);
    }
    else
    if (get_it_has_prefix("set"))
    {
        execute_predefined_set(m);
    }
    else
    if (get_it_has_prefix("getline"))
    {
        execute_predefined_getline(m);
    }
    else
    if (get_it_has_prefix("execution"))
    {
        execute_predefined_execution(m);
    }
    else
    if (get_it_has_prefix("match"))
    {
        execute_predefined_match(m);
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


void chomik::execute_variable_value_statement::add_placeholders_to_generator(basic_generator & g) const
{
    name->add_placeholders_to_generator(g);
}


std::string chomik::generic_literal_placeholder::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    // TODO - it does not look properly
    return "hallo";
}

void chomik::generic_literal_placeholder::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("adding literal placeholder to the generator");
    std::shared_ptr<generic_type> s;
    type_name->get_copy(s);
    g.add_placeholder(placeholder, std::move(s));
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
                generator g{__FILE__, __LINE__};

                std::shared_ptr<type_instance> y{std::make_shared<type_instance_range>(k->get_name(), k->get_body().get_min_value(*this, g), k->get_body().get_max_value(*this, g))};
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
            {
                DEBUG("expanding type definition for level " << k);
                j->expand(*this, k, temporary_vector_of_type_instances[l]);
                create_auxilliary_variables_for_type_instance(*temporary_vector_of_type_instances[l]);
            }
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


void chomik::machine::create_auxilliary_variables_for_type_instance(type_instance & ti)
{
    
    if (ti.get_mode() == type_instance::type_instance_mode::ENUM)
    {    
        generic_name gn;
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("result"));    
        gn.add_generic_name_item(std::make_shared<name_item_string>(ti.get_name()));    
    
        std::shared_ptr<signature> the_read_from_stream_result=std::make_shared<signature>(gn);    
            
        if (!get_variable_is_represented_in_memory(*the_read_from_stream_result))
        {
            DEBUG("creating the read from stream result " << ti.get_name() << " variable");
            add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_read_from_stream_result), ""));    
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
    if (g.get_the_cartesian_product_of_placeholder_types_is_finite() && g.get_the_cartesian_product_of_placeholder_types_is_small(m))
    {
        for (g.initialize(m); !g.get_terminated(); g.increment(m))
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
        // unlimited or large variable family
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

void chomik::generic_range_boundary_variable_value::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");

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


void chomik::placeholder_name_item::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");

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

void chomik::variable_value_name_item::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    name->add_placeholders_to_generator(g);
}

void chomik::generic_name::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");

    DEBUG("amount of name items " << vector_of_name_items.size());

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
        DEBUG("name item " << *i);
        i->add_placeholders_to_generator(g);
    }
}

void chomik::generic_literal_code::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    my_code_pointer->add_placeholders_to_generator(g);
}

void chomik::code::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    //std::cout << "adding placeholders to generator - code ";
    //body->report(std::cout);
    //std::cout << "\n";
    body->add_placeholders_to_generator(g);
}

void chomik::list_of_statements::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    for (auto & i: vector_of_statements)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::type_definition_statement::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    for (auto & i: vector_of_type_definitions)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::type_definition::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    body->add_placeholders_to_generator(g);
}

void chomik::variable_definition_statement::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    for (auto & i: vector_of_variable_definitions)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::type_definition_body_enum::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    for (auto & i: vector_of_names)
    {
        i->add_placeholders_to_generator(g);
    }
}

void chomik::execute_value_statement::add_placeholders_to_generator(basic_generator & g) const
{
    DEBUG("add_placeholders_to_generator");
    value->add_placeholders_to_generator(g);
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
                DEBUG("add type instance enum " << x << " for level 1");
            }
        }        
        else
        if (g.get_the_cartesian_product_of_placeholder_types_is_finite())
        {
            for (g.initialize(m); !g.get_terminated(); g.increment(m))
            {
                if (g.get_is_valid())
                {
                    DEBUG("for " << *this << " got a generator " << g);
                    
                    if (g.get_does_not_exceed_level(depth-1))
                    {                                        
                        signature x{*i, m, g};
                        temporary_vector_of_type_instance_enums.push_back(x.get_string_representation());
                        
                        DEBUG("add type instance enum " << x << " for level " << depth);
                    }
                    else
                    {
                        DEBUG("the generator " << g << " exceeds level " << (depth-1));
                    }
                }
            }
        }        
    }
    
    
    for (auto & i: temporary_vector_of_type_instance_enums)
    {
        e->add_type_instance_enum_value(i, depth);
    }
    
}


chomik::variable_with_value::actual_memory_representation_type chomik::generic_type_named::get_actual_memory_representation_type(const machine & m) const 
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
        CHOMIK_STDERR("warning: type_instance " << name << " is unknown\n");
    }
    
    return variable_with_value::actual_memory_representation_type::NONE;
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


int chomik::generic_range_boundary_variable_value::get_value(const machine & m, const basic_generator & g) const
{        
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


void chomik::program::execute(machine & m) const 
{ 
    auto s=std::make_shared<generator>(__FILE__, __LINE__);
    
    my_code.execute(m, s);             
}        


chomik::description_of_a_cartesian_product::description_of_a_cartesian_product(const basic_generator & g)
{    
    g.initialize_description_of_a_cartesian_product(*this);
    
}

std::string chomik::generic_value_variable_value::get_actual_text_representation(const machine & m, const basic_generator & g) const
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
            
        case variable_with_value::actual_memory_representation_type::ENUM:
            return m.get_variable_value_enum(s);
            break;

    }
            
    return "unknown";
}


chomik::generic_stream_random_enum_stream::generic_stream_random_enum_stream(const std::string & n, machine & m): 
    type_name{n}, my_machine{m}, generic_stream_random_number_stream{0, m.get_max_enum_type_index(n)} 
{
}

std::string chomik::generic_stream_random_enum_stream::read_string() 
{ 
    return my_machine.get_enum_type_item(type_name, read_integer()); 
}

int chomik::machine::get_max_enum_type_index(const std::string & tn) const 
{ 
    if (!get_type_instance_is_known(tn))
    {
        throw std::runtime_error("type instance is not known");
    }
    return map_type_name_to_type_instance.at(tn)->get_amount_of_values()-1;
}

std::string chomik::machine::get_enum_type_item(const std::string & tn, int i) const 
{ 
    if (!get_type_instance_is_known(tn))
    {
        throw std::runtime_error("type instance is not known");
    }
        
    //std::cout << "GET ENUM " << tn << " OFFSET " << i << "\n";
    
    return map_type_name_to_type_instance.at(tn)->get_enum_item(i);
}


void chomik::type_instance_enum::add_type_instance_enum_value(const signature & n, unsigned new_level) 
{
    std::string sr = n.get_string_representation();
    if (std::find_if(vector_of_values.begin(), vector_of_values.end(), [&sr](auto & x){return x->get_name()==sr;})==vector_of_values.end())
        vector_of_values.push_back(std::make_unique<type_instance_enum_value>(sr, new_level));
}                
        
void chomik::type_instance_enum::add_type_instance_enum_value(const std::string & n, unsigned int new_level)
{
    if (std::find_if(vector_of_values.begin(), vector_of_values.end(), [&n](auto & x){return x->get_name()==n;})==vector_of_values.end())
        vector_of_values.push_back(std::make_unique<type_instance_enum_value>(n, new_level));
}


void chomik::matching_protocol::initialize_mapping(mapping_generator & target) const
{
    DEBUG("initialize mapping");
    
    target.clear_mappings();
    for (auto a=map_placeholder_names_to_placeholder_names.begin(); a!=map_placeholder_names_to_placeholder_names.end(); a++)
    {
        DEBUG("add mapping " << a->second << " -> " << a->first);
        target.add_mapping(a->second, a->first);
    }    
}

void chomik::matching_protocol::initialize_mapping(external_placeholder_generator & target) const
{
    DEBUG("initialize mapping");
    target.clear_mappings();
    
    for (auto a=map_placeholder_names_to_integer.begin(); a!=map_placeholder_names_to_integer.end(); a++)
    {
        DEBUG("add placeholder integer " << a->second << " -> " << a->first);
        
        target.add_placeholder_with_value(std::make_shared<simple_placeholder_with_value_and_report<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>>(a->first, a->second, nullptr));
    }    
    for (auto a=map_placeholder_names_to_float.begin(); a!=map_placeholder_names_to_float.end(); a++)
    {
        DEBUG("add placeholder float " << std::showpoint << a->second << " -> " << a->first);

        target.add_placeholder_with_value(std::make_shared<simple_placeholder_with_value_and_report<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>>(a->first, a->second, nullptr));
    }
    for (auto a=map_placeholder_names_to_string.begin(); a!=map_placeholder_names_to_string.end(); a++)
    {
        DEBUG("add placeholder string " << a->second << " -> " << a->first);
        
        target.add_placeholder_with_value(std::make_shared<simple_placeholder_with_value_and_report<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>>(a->first, a->second, nullptr));
    }        
    for (auto a=map_placeholder_names_to_code.begin(); a!=map_placeholder_names_to_code.end(); a++)
    {
        DEBUG("add placeholder code " << a->second << " -> " << a->first);

        target.add_placeholder_with_value(std::make_shared<simple_placeholder_with_value_and_report<code, static_cast<int>(variable_with_value::actual_memory_representation_type::CODE)>>(a->first, a->second, nullptr));
    }
}

void chomik::generic_stream::read_string_of_x_characters(std::string & target, unsigned x)
{
    std::stringstream target_stream;
    unsigned char c;
    unsigned len{0};
    
    while (read_char(c))
    {
        //std::cout << "got char " << c << ", len" << len << "\n";
        if (c < 0b10000000) 
        {
            if (len == 0 && (isblank(c) || c=='\n'))
            {
                continue;
            }            
            
            ++len;
            if (isblank(c) || c=='\n')
            {
                //std::cout << c << " break\n";
                break;
            }            
            target_stream << c;
            if (len==x)
            {
                break;
            }
        }
        else if (c < 0b11000000) { 
            // TODO - UTF-8 error
            return;
        }
        else if (c < 0b11100000) {
            ++len;
            target_stream << c;
            if (read_char(c))
            {
                target_stream << c;
            }
            if (len==x)
            {
                break;
            }
        }
        else if (c < 0b11110000) {
            ++len;
            target_stream << c;
            if (read_char(c))
            {
                target_stream << c;
                if (read_char(c))
                {
                    target_stream << c;
                }
            }
            if (len==x)
            {
                break;
            }            
        }
        else if (c < 0b11111000) {
            ++len;
            target_stream << c;
            if (read_char(c))
            {
                target_stream << c;
                if (read_char(c))
                {
                    target_stream << c;
                    if (read_char(c))
                    {
                        target_stream << c;
                    }
                }
            }
            if (len==x)
            {
                break;
            }            
        }
        else {
            // TODO - UTF-8 error!
            return;
        }        
    }
                
    target = target_stream.str();
}


std::string chomik::generic_value_literal::get_actual_string_value(const machine & m, const basic_generator & g) const
{ 
    auto x{literal->get_actual_string_value(m, g)};
    DEBUG("actual string value \'" << x << "\'");
    return x;
}


void chomik::code_name_item::add_placeholders_to_generator(basic_generator & g) const
{
    my_code->add_placeholders_to_generator(g);
}

void chomik::code_name_item::add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const
{
    my_code->add_content_to_signature(*this, target, m, g);
}

std::string chomik::code_name_item::get_actual_text_representation(const machine & m, const basic_generator & g) const
{
    return my_code->get_actual_text_representation(m, g);
}


void chomik::machine::destroy_ad_hoc_type_instances_above(int amount)
{
    vector_of_ad_hoc_type_instances.erase(vector_of_ad_hoc_type_instances.begin()+amount, vector_of_ad_hoc_type_instances.end());
}

chomik::type_instance * chomik::machine::create_an_ad_hoc_type(const generic_type & t, generator & g, const std::string & tn)
{
    DEBUG("create an ad hoc type " << t << " as " << tn);
    int f,l;

    t.update_boundaries(*this, f, l, g);

    std::shared_ptr<type_instance_ad_hoc_range> x = std::make_shared<type_instance_ad_hoc_range>(t, tn, f, l);
    type_instance * y = dynamic_cast<type_instance*>(&*x);
//TODO fixme
    add_ad_hoc_type(std::move(x));
    return y;
}

void chomik::machine::add_ad_hoc_type(std::shared_ptr<type_instance_ad_hoc_range> && t)
{
    DEBUG("add ad hoc type " << t->get_name());

    vector_of_ad_hoc_type_instances.push_back(t);
}

void chomik::machine::get_first_and_last_iterators_for_ad_hoc_range_type(const std::string & type_name, int & f, int & l) const
{
    DEBUG("looking for ad hoc range type " << type_name);

    for (int x=vector_of_ad_hoc_type_instances.size()-1;x>=0;x--)
    {
        if (vector_of_ad_hoc_type_instances[x]->get_name() == type_name)
        {
            f = vector_of_ad_hoc_type_instances[x]->get_first_iterator_for_range();
            l = vector_of_ad_hoc_type_instances[x]->get_last_iterator_for_range();
            return;
        }
    }
}

chomik::placeholder_with_value& chomik::basic_generator::get_placeholder_with_value(const std::string & p)
{
    return dummy;
}

const chomik::placeholder_with_value& chomik::basic_generator::get_placeholder_with_value(const std::string & p) const { return dummy; }


void chomik::type_instance_ad_hoc_range::update(placeholder_with_value & v, machine & m, basic_generator & g)
{
    DEBUG("update type instance " << name);

    my_type.update_boundaries(m, min_boundary, max_boundary, g);

    DEBUG("updated type instance " << name << " as " << min_boundary << ".." << max_boundary);

    v.update_int_value(min_boundary, max_boundary);
}


void chomik::machine::add_variable_with_value(std::shared_ptr<variable_with_value> && vv)
{
    auto [it, success] = map_signature_string_representation_to_variable_with_value.insert(std::pair(vv->get_signature_string_representation(), vv));

    if (!success)
    {
        std::stringstream s;
        s << "failed to insert a pair into machine's memory " << vv->get_signature_string_representation();
        throw std::runtime_error(s.str());
    }

    std::shared_ptr<variable_with_value> v2{vv};

            //std::cout << "add_variable_with_value\n";
            //vv->report(std::cout);
            //std::cout << "\n";

    memory.push_back(std::move(vv));
}


void chomik::simple_placeholder_for_range::update_int_value(int f, int l)
{
    DEBUG("update int value " << f << ".." << l);
    first = f;
    last = l+1;
    value = f;
}

bool chomik::generic_value_variable_value::get_is_code(machine & m) const
{
    // TODO it may be true for some variables
    return false;
}
