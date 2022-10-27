#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

#define CHOMIK_STDERR(X) *chomik::machine::current_runtime_warning_stream << X

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


bool chomik::generator::get_does_not_exceed_level(int max_level) const
{
    for (auto & i: memory)
    {
        if (i->get_exceeds_level(max_level))
        {
            DEBUG(*i << " exceeds level " << max_level << ", it has level " << i->get_level());
            return false;
        }
    }
    return true;
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
                    // TODO we just assume it is infinite because there is no type instance - we should check whether the assumption is correct                                        
                    DEBUG("it is infinite");
                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_infinite_range>(i->get_name())};
                    add_placeholder_with_value(std::move(x));
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

void chomik::generator::increment(machine & m)
{
    for (auto & i: memory)
    {
        i->increment();
        if (i->get_is_valid())
        {
            return; // we DONT't increment the father
        }
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

void chomik::mapping_generator::initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const
{
#warning "it may be necessary"    
}

void chomik::generator::initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const
{    
    for (auto & a: vector_of_placeholders)
    {
        DEBUG("adding placeholder to the description of a cartesian product");
        target.add_dimension(std::make_unique<cartesian_product_dimension>(a->get_name(), a->get_placeholder_type().get_low_level_type_name(), 
                                                                           a->get_placeholder_type().get_is_finite()));        
    }    
}

bool chomik::generator::get_has_placeholder(const std::string & p) const
{
    bool result = false;
    if (auto o=my_father.lock())
    {
        result = o->get_has_placeholder_with_value(p);
    }
    if (!result)
        result = map_placeholder_names_to_placeholders.find(p)!=map_placeholder_names_to_placeholders.end();
    return result;
}
        
bool chomik::generator::get_has_placeholder_with_value(const std::string & p) const
{
    bool result = false;
    if (auto o=my_father.lock())
    {
        result = o->get_has_placeholder_with_value(p);
    }
    if (!result)
        result = map_placeholder_names_to_placeholders_with_value.find(p)!=map_placeholder_names_to_placeholders_with_value.end();
    return result;
}
        
chomik::placeholder_with_value& chomik::generator::get_placeholder_with_value(const std::string & p)
{
    DEBUG("get_placeholder_with_value for " << p);
    
    
    if (auto o=my_father.lock())
    {
        if (o->get_has_placeholder_with_value(p))
        {
            DEBUG("father knows it " << p);
            
            return o->get_placeholder_with_value(p);
        }
    }
    
    DEBUG("let us find " << p << " locally");
    
    auto f = map_placeholder_names_to_placeholders_with_value.find(p); 
    if (f == map_placeholder_names_to_placeholders_with_value.end())
    {
        std::stringstream message;
        message << "no placeholder " << p << " found, got only ";
        
        for (auto q=map_placeholder_names_to_placeholders_with_value.begin(); q!=map_placeholder_names_to_placeholders_with_value.end(); q++)
        {
            message << q->first << " ";
        }        
        
        throw std::runtime_error(message.str());
    }
    
    return *f->second;
}

const chomik::placeholder_with_value& chomik::generator::get_placeholder_with_value(const std::string & p) const
{
    if (auto o=my_father.lock())
    {
        if (o->get_has_placeholder_with_value(p))
        {
            return o->get_placeholder_with_value(p);
        }
    }
    return *map_placeholder_names_to_placeholders_with_value.find(p)->second;
}

chomik::simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)> chomik::basic_generator::dummy{"", 0};

bool chomik::mapping_generator::get_has_placeholder_with_value(const std::string & p) const
{ 
    DEBUG("check whether the mapping generator " << *this << " has placeholder with value " << p);
        
    if (auto o=my_father.lock())
    {
        DEBUG("we locked the father, let's check " << convert_childs_placeholder_name_to_father(p));        
        
        bool result = o->get_has_placeholder_with_value(convert_childs_placeholder_name_to_father(p));
        
        DEBUG("the result is " << (result ? "true" : "false"));
        
        return result;
    }
    return false;             
}

chomik::placeholder_with_value& chomik::mapping_generator::get_placeholder_with_value(const std::string & p)
{
    if (auto o=my_father.lock())
    {
        DEBUG("we locked the father, let's check " << convert_childs_placeholder_name_to_father(p));        
                
        return o->get_placeholder_with_value(convert_childs_placeholder_name_to_father(p));
    }
    return dummy;
}

const chomik::placeholder_with_value& chomik::mapping_generator::get_placeholder_with_value(const std::string & p) const
{
    if (auto o=my_father.lock())
    {
        DEBUG("we locked the father, let's check " << convert_childs_placeholder_name_to_father(p));        
        
        return o->get_placeholder_with_value(convert_childs_placeholder_name_to_father(p));
    }
    return dummy;
}


void chomik::mapping_generator::initialize(machine & m)
{
    DEBUG("mapping generator initialize");
        
    if (auto o = my_father.lock())
    {
        o->initialize(m);
    }
}

void chomik::mapping_generator::increment(machine & m)
{
    DEBUG("we increment the mapping generator");
    
    if (auto o = my_father.lock())
    {
        o->increment(m);
        DEBUG("the father of the mapping generator is " << *o);
    }
}



int chomik::basic_generator::get_placeholder_value_integer(const std::string & p) const 
{ 
    DEBUG("get placeholder " << p << " value integer");
        
    return get_placeholder_with_value(p).get_value_integer();     
}
        
double chomik::basic_generator::get_placeholder_value_float(const std::string & p) const 
{ 
    DEBUG("get placeholder " << p << " value float");
    
    return get_placeholder_with_value(p).get_value_float(); 
}
        
std::string chomik::basic_generator::get_placeholder_value_string(const std::string & p) const 
{ 
    DEBUG("get placeholder " << p << " value string");
    
    return get_placeholder_with_value(p).get_value_string(); 
}
        
std::string chomik::basic_generator::get_placeholder_value_enum(const std::string & p) const 
{
    DEBUG("get placeholder " << p << " value enum");
    
    return get_placeholder_with_value(p).get_value_enum();     
}        

        
const std::string chomik::mapping_generator::convert_childs_placeholder_name_to_father(const std::string & cpn) const
{
    auto f = map_child_placeholder_to_father_placeholder.find(cpn);
    if (f != map_child_placeholder_to_father_placeholder.end())
        return f->second;
    
    return cpn;
}


void chomik::mapping_generator::initialize_mapping(const matching_protocol & mp)
{
    DEBUG("initialize mapping");
    mp.initialize_mapping(*this);    
}
