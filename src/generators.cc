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

    debug();
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


void chomik::generator::finalize(machine & m)
{
    m.destroy_ad_hoc_type_instances_above(initial_amount_of_ad_hoc_type_instances);
}


void chomik::generator::initialize(machine & m)
{    
    initial_amount_of_ad_hoc_type_instances = m.get_amount_of_ad_hoc_type_instances();

    DEBUG("generator::initialize, amount of placeholders " << vector_of_placeholders.size());

    for (auto & i: vector_of_placeholders)
    {
        DEBUG("generator::initialize - add placeholder " << i->get_name() << " of type " << i->get_placeholder_type() << " to the generator");

        if (i->get_placeholder_type().get_is_an_ad_hoc_type())
        {
            DEBUG("generator::initialize - it is an ad hoc type");

            const std::string type_name = i->get_placeholder_type().get_generic_type_name();

            DEBUG("the type name is " << type_name);

            auto * my_type_instance = m.create_an_ad_hoc_type(i->get_placeholder_type(), *this, type_name);

            switch (i->get_placeholder_type().get_actual_memory_representation_type(m))
            {
                case variable_with_value::actual_memory_representation_type::INTEGER:
                {
                    DEBUG("it is an integer range");

                    int f,l;
                    m.get_first_and_last_iterators_for_ad_hoc_range_type(type_name, f, l);

                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_range>(i->get_name(), f, l, my_type_instance)};

                    DEBUG("it is a range " << f << ".." << l);

                    add_placeholder_with_value(std::move(x));
                };

                default:;
            }
        }
        else
        {
            switch (i->get_placeholder_type().get_actual_memory_representation_type(m))
            {
                case variable_with_value::actual_memory_representation_type::INTEGER:
                {
                const std::string type_name = i->get_placeholder_type().get_type_name(m, *this);
                int f, l;
                if (m.get_type_instance_is_known(type_name))
                {
                    m.get_first_and_last_iterators_for_range_type(type_name, f, l);
                
                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_range>(i->get_name(), f, l, nullptr)};
                                        
                    DEBUG("it is a range " << f << ".." << l);
                
                    // std::cout << "it is a range " << f << ".." << l << "\n";
        
                    add_placeholder_with_value(std::move(x));
                }
                else
                {
                    // TODO we just assume it is infinite because there is no type instance - we should check whether the assumption is correct                                        
                    DEBUG("it is infinite");
                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_infinite_range>(i->get_name(), nullptr)};
                    add_placeholder_with_value(std::move(x));
                }
            }
            break;

            case variable_with_value::actual_memory_representation_type::CODE:
            {
                const std::string type_name = i->get_placeholder_type().get_type_name(m, *this);

                DEBUG("NOT IMPLEMENTED");
            }
                break;

            case variable_with_value::actual_memory_representation_type::FLOAT:
            {
                std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_with_value_and_report<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>>(i->get_name(), 0.0, nullptr)};
        
                add_placeholder_with_value(std::move(x));
            }
            break;
            case variable_with_value::actual_memory_representation_type::STRING:
            {
                DEBUG("it is a string (" << i->get_name() << ")");
                std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_with_value_and_report<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>>(i->get_name(), "", nullptr)};
        

                DEBUG("add placeholder with value " << *x << " to the generator " << *this);

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
                    std::unique_ptr<placeholder_with_value> x{std::make_unique<simple_placeholder_for_enum>(i->get_name(), f, l, nullptr)};
        
                    add_placeholder_with_value(std::move(x));
                }
            }
            break;            
            }
        }
    }
}

void chomik::generator::increment(machine & m)
{
    DEBUG("generator::increment");

    // there can be dependencies within the ad-hoc ranges, we assume the placeholders at the lower indices
    // to be able to affect the higher indices placeholders
    for (int i=memory.size()-1; i>=0; i--)
    {
        memory[i]->increment();

        DEBUG("incremented " << i);

        if (memory[i]->get_is_valid())
        {
            for (int j=i+1;j<memory.size(); j++)
            {
                memory[j]->update_ad_hoc_range_type_instance(m, *this);
            }
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

bool chomik::generator::get_the_cartesian_product_of_placeholder_types_is_small(machine & m) const
{
    double size_of_the_cartesian_product = 1.0;

    for (auto & i: vector_of_placeholders)
    {
        if (get_has_placeholder(i->get_name()))
        {
            auto & t=i->get_placeholder_type();

            if (m.get_type_instance_is_known(t.get_type_name(m, *this)))
            {
                auto & ti = m.get_type_instance(t.get_type_name(m, *this));

                size_of_the_cartesian_product *= ti.get_amount_of_values();
            }
            else
            {
                DEBUG("can be an ad_hoc type " << t.get_type_name(m, *this));
                // TODO - handle the ad hoc type
            }
        }
        else
        {
            throw std::runtime_error("the generator does not have the placeholder");
        }
    }

    DEBUG("size of the cartesian product " << size_of_the_cartesian_product);

    return size_of_the_cartesian_product <= 1000; // TODO - replace with some integer variable accessible from the interpreter
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
// TODO implement me
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
    {
        result = map_placeholder_names_to_placeholders_with_value.find(p)!=map_placeholder_names_to_placeholders_with_value.end();
    }

    DEBUG("checking whether the generator created in " << my_filename << " line " << line_number << ": has placeholder with value " << p << " " << (result ? "true" : "false"));

    if (!result)
    {
        for (auto x: map_placeholder_names_to_placeholders_with_value)
        {
            DEBUG("placeholder name " << x.first);
        }

        for (auto &x: vector_of_placeholders)
        {
            DEBUG("placeholder (without value) " << x->get_name());
        }
    }

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
    DEBUG("get_placeholder_with_value for " << p);
    
    if (auto o=my_father.lock())
    {
        if (o->get_has_placeholder_with_value(p))
        {
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

        for (auto &q:vector_of_placeholders)
        {
            message << q->get_name() << "# ";
        }
        
        throw std::runtime_error(message.str());        
    }    
    
    DEBUG("got a placeholder " << *f->second);
    
    return *f->second;
}

chomik::simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)> chomik::basic_generator::dummy{"", -1, nullptr};

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

int chomik::generator::get_placeholder_value_integer(const std::string & p) const
{
    DEBUG("get placeholder " << p << " value integer");

    const placeholder_with_value &x{get_placeholder_with_value(p)};
    
    const simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)> &y{reinterpret_cast<const simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>&>(x)};
    
    int v = y.get_value();

    DEBUG("result " << v);
    
    return v;
}


void chomik::generator::get_placeholder_value_code(const std::string & p, code & target) const
{
    DEBUG("get placeholder " << p << " value code");

    const placeholder_with_value &x{get_placeholder_with_value(p)};

    const simple_placeholder_with_value_and_report<code, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::CODE)> &y{reinterpret_cast<const simple_placeholder_with_value_and_report<code, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::CODE)>&>(x)};

    target = y.get_value();

    DEBUG("result " << target);
}


int chomik::basic_generator::get_placeholder_value_integer(const std::string & p) const 
{ 
    DEBUG("get placeholder " << p << " value integer");

    const placeholder_with_value &x{get_placeholder_with_value(p)};
    
    const simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)> &y{reinterpret_cast<const simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>&>(x)};
    
    int v = y.get_value();
    
    DEBUG("result " << v);
        
    return v;     
}
        
double chomik::basic_generator::get_placeholder_value_float(const std::string & p) const 
{ 
    DEBUG("get placeholder " << p << " value float");
    
    const placeholder_with_value &x{get_placeholder_with_value(p)};
    
    const simple_placeholder_with_value_and_report<double, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::FLOAT)> &y{reinterpret_cast<const simple_placeholder_with_value_and_report<double, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::FLOAT)>&>(x)};
    
    double v = y.get_value();
    
    DEBUG("result " << v);
        
    return v;     
}
        
std::string chomik::basic_generator::get_placeholder_value_string(const std::string & p) const 
{ 
    DEBUG("get placeholder " << p << " value string");

    const placeholder_with_value &x{get_placeholder_with_value(p)};

    const simple_placeholder_with_value_and_report<std::string,
            static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::STRING)> &y{reinterpret_cast<const simple_placeholder_with_value_and_report<std::string, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::STRING)>&>(x)};
    
    std::string v{y.get_value()};
    
    DEBUG("the placeholder value is \'" << v << "\'");
    
    return v;
}
        
std::string chomik::basic_generator::get_placeholder_value_enum(const std::string & p) const 
{
    DEBUG("get placeholder " << p << " value enum");
    
    return get_placeholder_with_value(p).get_value_enum();     
}        


void chomik::basic_generator::get_placeholder_value_code(const std::string & p, code & target) const
{
    DEBUG("get placeholder " << p << " value code");


    using placeholder_with_code = simple_placeholder_with_value_and_report<code, static_cast<int>(variable_with_value::actual_memory_representation_type::CODE)>;

    const placeholder_with_code& pp{static_cast<const placeholder_with_code&>(get_placeholder_with_value(p))};

    target = pp.get_value();

    DEBUG("result is " << target);
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


bool chomik::basic_generator::get_the_cartesian_product_of_placeholder_types_is_finite() const 
{     
    if (auto o=my_father.lock())
    {
        return o->get_the_cartesian_product_of_placeholder_types_is_finite();
    }
    return false;
}
        
bool chomik::basic_generator::get_the_cartesian_product_of_placeholder_types_is_small(machine & m) const
{ 
    if (auto o=my_father.lock())
    {
        return o->get_the_cartesian_product_of_placeholder_types_is_small(m);
    }
    return false; 
}
        
bool chomik::basic_generator::get_the_cartesian_product_of_placeholder_types_is_empty() const 
{ 
    if (auto o=my_father.lock())
    {
        return o->get_the_cartesian_product_of_placeholder_types_is_empty();
    }
    return false; 
}
        
bool chomik::basic_generator::get_the_cartesian_product_of_placeholder_types_has_one_item() const 
{ 
    if (auto o=my_father.lock())
    {
        return o->get_the_cartesian_product_of_placeholder_types_has_one_item();
    }
    return false; 
}


bool chomik::generator::get_the_cartesian_product_of_placeholder_types_has_one_item() const
{
    DEBUG("the generator's vector of placeholders size equals " << vector_of_placeholders.size());
    return vector_of_placeholders.size()==0;
}

chomik::external_placeholder_generator::external_placeholder_generator(const std::string & filename, unsigned new_line_number):
    my_filename{filename},
    line_number{new_line_number}
{
}


void chomik::external_placeholder_generator::initialize(machine & m)
{
    // nothing to be done
}


void chomik::external_placeholder_generator::increment(machine & m)
{
    // nothing to be done
}

void chomik::external_placeholder_generator::initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const
{
}


void chomik::external_placeholder_generator::initialize_mapping(const matching_protocol & mp)
{
    DEBUG("initialize mapping");
    mp.initialize_mapping(*this);    
}


void chomik::external_placeholder_generator::clear_mappings()
{
    DEBUG("clear mappings");
    memory.clear();
    map_placeholder_names_to_placeholders_with_value.clear();
}

void chomik::generator::debug() const
{
    DEBUG("line number " << line_number << ", filename " << my_filename);

    DEBUG("checking whether " << *this << " has following placeholders:");
    for (auto & a: vector_of_placeholders)
    {
        DEBUG("placeholder " << a->get_name());
    }
    DEBUG("memory:");
    for (auto & a: memory)
    {
        DEBUG("placeholder " << a->get_name());
    }
}



bool chomik::external_placeholder_generator::get_has_placeholder_with_value(const std::string & p) const
{
    DEBUG("get_has_placeholder_with_value");
    return map_placeholder_names_to_placeholders_with_value.find(p) != map_placeholder_names_to_placeholders_with_value.end();
}

chomik::placeholder_with_value& chomik::external_placeholder_generator::get_placeholder_with_value(const std::string & p)
{
    DEBUG("get_placeholder_with_value");
    return *map_placeholder_names_to_placeholders_with_value.find(p)->second;
}

const chomik::placeholder_with_value& chomik::external_placeholder_generator::get_placeholder_with_value(const std::string & p) const
{
    DEBUG("get_placeholder_with_value");
    return *map_placeholder_names_to_placeholders_with_value.find(p)->second;
}


void chomik::external_placeholder_generator::add_placeholder_with_value(std::shared_ptr<placeholder_with_value> && p)
{
    DEBUG("add_placeholder_with_value");

    std::shared_ptr<placeholder_with_value> p2{p};
    auto [it, success] = map_placeholder_names_to_placeholders_with_value.insert(std::pair(p->get_name(), std::move(p2)));
    if (!success)
    {
        throw std::runtime_error("failed to insert a placeholder with value");
    }

    memory.push_back(std::move(p));
}
