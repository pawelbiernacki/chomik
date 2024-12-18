#include "chomik.h"

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

#define CHOMIK_STDERR(X) *chomik::machine::current_runtime_warning_stream << X

void chomik::code::execute(machine & m, std::shared_ptr<basic_generator> father) const
{
    for (auto & i: body->get_vector_of_statements())
    {
        i->execute(m, i, father);
    }
}

void chomik::type_definition_statement::execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father) const
{
    std::shared_ptr<const statement> x{i};
    m.add_type_definition_statement(std::move(x));
}

void chomik::variable_definition_statement::execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father) const
{
    std::shared_ptr<const statement> x{i};
    m.add_variable_definition_statement(std::move(x));
}


void chomik::assignment_statement::execute_if_cartesian_product_has_one_item(machine & m, std::shared_ptr<basic_generator> & g) const
{
        DEBUG("code line number " << line_number << ": in assignment for name " << *name << " got a generator " << *g);

        // it is a single variable, let us store it in memory
        
        // check whether we have it in the memory
        
        std::shared_ptr<signature> x=std::make_shared<signature>(*name, m, *g);
        x->set_line_number(line_number);

        DEBUG("code line number " << line_number << " signature " << *x);
        
        /*
        std::cout << "its signature is ";
        x->report(std::cout);        
        std::cout << "\n";
        */
        if (m.get_variable_is_represented_in_memory(*x))
        {
            //std::cout << "it is represented in memory\n";
            DEBUG("code line number " << line_number << ": is represented in memory");
            
            // update its value
            switch (m.get_actual_memory_representation_type_of_the_variable(*x))
            {
                case variable_with_value::actual_memory_representation_type::INTEGER:
                    DEBUG("for value " << *value << " it is integer of value " << value->get_actual_integer_value(m, *g));
                    m.get_variable_with_value(*x).assign_value_integer(value->get_actual_integer_value(m, *g));
                    break;
                    
                case variable_with_value::actual_memory_representation_type::FLOAT:
                    //std::cout << "it is float\n";
                    DEBUG(m.get_variable_with_value(*x).get_debug_type_name() << " signature " << *x << " is assigned with " << value->get_actual_float_value(m, *g));
                    m.get_variable_with_value(*x).assign_value_float(value->get_actual_float_value(m, *g));
                    break;
                    
                case variable_with_value::actual_memory_representation_type::STRING:
                    DEBUG("for value " << *value << " it is a string of value " << value->get_actual_string_value(m, *g));
                    m.get_variable_with_value(*x).assign_value_string(value->get_actual_string_value(m, *g));
                    break;
                    
                case variable_with_value::actual_memory_representation_type::CODE:
                {
                    DEBUG("code line number " << line_number << ": it is code " << *value);
                    
                    code ci;
                    replacing_policy_literal p;
                    value->get_actual_code_value(m, *g, p, ci);

                    DEBUG("the code " << ci);

                    m.get_variable_with_value(*x).assign_value_code(ci);
                    
                    DEBUG("code line number " << line_number << ": the result code " << ci);
                }
                    break;
                    
                case variable_with_value::actual_memory_representation_type::ENUM:
                    //std::cout << "it is enum\n";
                    DEBUG("code line number " << line_number << ": it is an enum " << value->get_actual_enum_value(m, *g));
                    m.get_variable_with_value(*x).assign_value_enum(value->get_actual_enum_value(m, *g));
                    
                    break;                    
            }
            
        }
        else
        {
            //std::cout << "it is not represented in memory\n";
            DEBUG("code line number " << line_number << ": is NOT represented in memory");
            
            // TODO: but should be declared, check its declaration type
            
            // check the value representation type
            switch (value->get_actual_memory_representation_type(m, *g))
            {
                case variable_with_value::actual_memory_representation_type::INTEGER:
                {
                    //std::cout << "it should be integer\n";
                    auto y{std::make_shared<simple_variable_with_value_integer>(std::move(x), value->get_actual_integer_value(m, *g))};
                    m.add_variable_with_value(std::move(y));

                }
                    break;
                    
                case variable_with_value::actual_memory_representation_type::FLOAT:
                {
                    //std::cout << "it should be float\n";

                    DEBUG("for " << *value << " got float value " << value->get_actual_float_value(m, *g));

                    auto y{std::make_shared<simple_variable_with_value_float>(std::move(x), value->get_actual_float_value(m, *g))};
                    m.add_variable_with_value(std::move(y));                    
                }
                    break;
                    
                case variable_with_value::actual_memory_representation_type::STRING:
                {
                    //DEBUG("for value " << *value << " it is a string of value " << value->get_actual_string_value(m, *g));
                    auto y{std::make_shared<simple_variable_with_value_string>(std::move(x), value->get_actual_string_value(m, *g))};                    
                    m.add_variable_with_value(std::move(y));                    
                }
                    break;
                    
                case variable_with_value::actual_memory_representation_type::CODE:
                {
                    auto yc{std::make_unique<code>()};
                    replacing_policy_literal p;

                    DEBUG("get actual code value from " << *value);

                    value->get_actual_code_value(m, *g, p, *yc);
                                        
                    DEBUG("code line number " << line_number << ": got code " << *yc);
                                        
                    auto y{std::make_shared<variable_with_value_code>(std::move(x), std::move(yc))};
                    
                    m.add_variable_with_value(std::move(y));
                }
                    break;
                    
                case variable_with_value::actual_memory_representation_type::ENUM:
                    {
                    //std::cout << "it should be enum\n";
                                        
                    auto y{std::make_shared<simple_variable_with_value_enum>(std::move(x), value->get_actual_text_representation(m, *g))};
                    m.add_variable_with_value(std::move(y));
                    }    
                    break;                    
            }
        }
}


void chomik::assignment_statement::execute_if_cartesian_product_is_finite_and_small(machine & m, std::shared_ptr<basic_generator> & g) const
{
        DEBUG("code line number " << line_number << ": in assignment for name " << *name << " got a generator " << *g);
                                
                std::shared_ptr<signature> x=std::make_shared<signature>(*name, m, *g);
                x->set_line_number(line_number);

                /*
                std::cout << "its signature is ";
                x->report(std::cout);        
                std::cout << "\n";
                */
                
                if (m.get_variable_is_represented_in_memory(*x))
                {
                    //std::cout << "it is represented in memory\n";
                    // update its value
                    switch (m.get_actual_memory_representation_type_of_the_variable(*x))
                    {
                        case variable_with_value::actual_memory_representation_type::INTEGER:
                            //std::cout << "it is integer\n";
                            m.get_variable_with_value(*x).assign_value_integer(value->get_actual_integer_value(m, *g));
                            break;
                    
                        case variable_with_value::actual_memory_representation_type::FLOAT:
                            //std::cout << "it is float\n";
                            m.get_variable_with_value(*x).assign_value_float(value->get_actual_float_value(m, *g));
                            break;
                    
                        case variable_with_value::actual_memory_representation_type::STRING:
                            //std::cout << "it is string\n";
                            m.get_variable_with_value(*x).assign_value_string(value->get_actual_string_value(m, *g));
                            break;
                    
                        case variable_with_value::actual_memory_representation_type::CODE:
                        {
                            //std::cout << "it is code\n";
                            auto yc{std::make_unique<code>()};
                            replacing_policy_literal p;
                            value->get_actual_code_value(m, *g, p, *yc);
                                        
                            auto y{std::make_shared<variable_with_value_code>(std::move(x), std::move(yc))};
                    
                            m.add_variable_with_value(std::move(y));
                        }
                            break;
                    
                        case variable_with_value::actual_memory_representation_type::ENUM:
                            //std::cout << "it is enum\n";
                            m.get_variable_with_value(*x).assign_value_enum(value->get_actual_enum_value(m, *g));
                    
                            //std::cout << "assigned " << value->get_actual_enum_value(m, g) << "\n";
                            break;                    
                    }
                }
                else
                {
                    //std::cout << "it is not represented in memory\n";
                    switch (value->get_actual_memory_representation_type(m, *g))
                    {
                        case variable_with_value::actual_memory_representation_type::INTEGER:
                        {
                            //std::cout << "it should be integer\n";
                            auto y{std::make_shared<simple_variable_with_value_integer>(std::move(x), value->get_actual_integer_value(m, *g))};
                            m.add_variable_with_value(std::move(y));
                        }
                        break;
                    
                        case variable_with_value::actual_memory_representation_type::FLOAT:
                        {
                            //std::cout << "it should be float\n";
                            auto y{std::make_shared<simple_variable_with_value_float>(std::move(x), value->get_actual_float_value(m, *g))};
                            m.add_variable_with_value(std::move(y));                    
                        }
                        break;
                    
                        case variable_with_value::actual_memory_representation_type::STRING:
                        {
                            //std::cout << "it should be string\n";
                            auto y{std::make_shared<simple_variable_with_value_string>(std::move(x), value->get_actual_string_value(m, *g))};                    
                            m.add_variable_with_value(std::move(y));                    
                        }
                        break;
                    
                        case variable_with_value::actual_memory_representation_type::CODE:
                        {
                            //std::cout << "it should be code\n";
                            auto yc{std::make_unique<code>()};
                            replacing_policy_literal p;
                            value->get_actual_code_value(m, *g, p, *yc);
                    
                            auto y{std::make_shared<variable_with_value_code>(std::move(x), std::move(yc))};
                    
                            m.add_variable_with_value(std::move(y));
                        }
                        break;
                    
                        case variable_with_value::actual_memory_representation_type::ENUM:
                        {
                            //std::cout << "it should be enum\n";                                                                    
                            auto y{std::make_shared<simple_variable_with_value_enum>(std::move(x), value->get_actual_text_representation(m, *g))};
                            m.add_variable_with_value(std::move(y));
                        }    
                        break;                    
                    }
                }                                    
}

void chomik::assignment_statement::execute_if_cartesian_product_is_large_or_infinite(machine & m, std::shared_ptr<basic_generator> & g) const
{
    // TODO - make the family of variables
    
    DEBUG("code line number " << line_number << ": in assignment for name " << *name << " got a generator " << *g);

    auto d{std::make_unique<description_of_a_cartesian_product>(*g)};
    auto t{std::make_unique<generic_name>(*name)};

    DEBUG("is it a literal " << (value->get_is_literal() ? "true": "false"));
    
    if (value->get_is_literal())
    {    
        // we distinguish two cases - either a code with placeholders or (the second case) - any literal (float, integer, enum) or a code without placeholders
        if (value->get_is_code_with_placeholders(m, *g))
        {        
            std::unique_ptr<generic_literal> v;
            value->get_literal_copy(v);
            auto s{std::make_unique<assignment_source_code_pattern>(std::move(v))};
            auto a{std::make_unique<assignment_event>(std::move(t), std::move(d), std::move(s))};
            DEBUG("code line number " << line_number << ": an assignment " << *a);        
            
            m.add_assignment_event(std::move(a));
        }   
        else
        {
            std::unique_ptr<generic_literal> v;
            value->get_literal_copy(v);
            auto s{std::make_unique<assignment_source_literal_value>(std::move(v), v->get_actual_memory_representation_type(m, *g))};
            auto a{std::make_unique<assignment_event>(std::move(t), std::move(d), std::move(s))};
            DEBUG("code line number " << line_number << ": an assignment " << *a);        
            
            m.add_assignment_event(std::move(a));
        }    
    }
    else
    {
        // it must be therefore a variable value
        std::unique_ptr<generic_value> v;
        value->get_copy(v);
                
        auto s{std::make_unique<assignment_source_variable_value>(std::move(v))};
        auto a{std::make_unique<assignment_event>(std::move(t), std::move(d), std::move(s))};
        DEBUG("code line number " << line_number << ": an assignment " << *a);        
        
        m.add_assignment_event(std::move(a));
                
        // TODO - update all the values matching the signature for the variables stored in memory
    }
}

void chomik::assignment_statement::execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father) const
{
    std::shared_ptr<basic_generator> g=std::make_shared<generator>(*name, __FILE__, __LINE__);
    machine_finalization_guard<basic_generator> guard{m, *g};

    g->set_father(father);
        
    DEBUG("code line number " << line_number << ": " << *this);

    g->initialize(m);

    DEBUG("code line number " << line_number << ": in assignment for name " << *name << " got a generator " << *g);
    DEBUG("code line number " << line_number << ": the assigned value is " << *value);

    if (g->get_the_cartesian_product_of_placeholder_types_is_empty())
    {
        // nothing to be done
        DEBUG("code line number " << line_number << ": it is empty");
    }
    else
    if (g->get_the_cartesian_product_of_placeholder_types_has_one_item())
    {
        DEBUG("code line number " << line_number << ": it has one item");
        execute_if_cartesian_product_has_one_item(m, g);
        
    }
    else
    if (g->get_the_cartesian_product_of_placeholder_types_is_finite() && g->get_the_cartesian_product_of_placeholder_types_is_small(m))
    {
        //std::cout << "it is an array of variables\n";
            DEBUG("code line number " << line_number << ": it is finite and small");
        
            for (; !g->get_terminated(); g->increment(m))
            {
                if (g->get_is_valid())
                {
                    execute_if_cartesian_product_is_finite_and_small(m, g);
                }
            }
    }
    else
    if (g->get_the_cartesian_product_of_placeholder_types_is_finite()) // but is not small
    {
        // it may be a code - then execute it as a large set, or else it may be not code - then execute it in a loop
        if (i->get_is_code_assignment(m))
        {
            execute_if_cartesian_product_is_large_or_infinite(m, g);
        }
        else
        {
            DEBUG("code line number " << line_number << ": it is finite and large, but it is not a code");

            for (; !g->get_terminated(); g->increment(m))
            {
                if (g->get_is_valid())
                {
                    execute_if_cartesian_product_is_finite_and_small(m, g);
                }
            }
        }
    }
    else
    {
        // infinite assignment event        

            DEBUG("code line number " << line_number << ": it is infinite assignment");
        
            execute_if_cartesian_product_is_large_or_infinite(m, g);
    }
}



void chomik::execute_variable_value_statement::execute_if_cartesian_product_has_one_item(machine & m, std::shared_ptr<basic_generator> & g) const
{
        signature s{*name, m, *g};    
        s.set_line_number(line_number);

        DEBUG("code line number " << line_number << ": got signature " << s);
        
        if (s.get_is_predefined(m))
        {
            DEBUG("it is predefined");
            s.execute_predefined(m);
        }
        else
        {
            if (m.get_variable_is_represented_in_memory(s))
            {
                DEBUG("code line number " << line_number << ": it is in memory");

                switch (m.get_actual_memory_representation_type_of_the_variable(s))
                {
                    case variable_with_value::actual_memory_representation_type::INTEGER:
                        CHOMIK_STDERR("warning: cannot execute an integer!\n");
                        break;
                                
                    case variable_with_value::actual_memory_representation_type::FLOAT:
                        CHOMIK_STDERR("warning: cannot execute a float!\n");
                        break;

                    case variable_with_value::actual_memory_representation_type::STRING:
                        CHOMIK_STDERR("warning: cannot execute a string!\n");
                        break;
                                
                    case variable_with_value::actual_memory_representation_type::ENUM:
                        CHOMIK_STDERR("warning: cannot execute an enum!\n");
                        break;
                                
                    case variable_with_value::actual_memory_representation_type::CODE:
                        {
                            code ci;
                            DEBUG("code line number " << line_number << ": get code " << s);
                            m.get_variable_value_code(s, ci); 
                            
                            DEBUG("code line number " << line_number << ": code to be executed: " << ci);
                            ci.execute(m);
                        }
                        break;                                                                
                }                
            }
            else
            {
                DEBUG("code line number " << line_number << ": it is not in memory");
                DEBUG("iterate backwards through the assignment events (there are " << m.get_vector_of_assignment_events().size() << " of them)");
                
                std::shared_ptr<basic_generator> mg=std::make_shared<mapping_generator>(__FILE__, __LINE__);
                
                mg->set_father(g);
                
                for (int i=m.get_vector_of_assignment_events().size()-1; i>=0; i--)
                {
                    const std::unique_ptr<assignment_event>& a{m.get_vector_of_assignment_events()[i]};
                    DEBUG("found " << *a);
                                
                    matching_protocol mprotocol;
                    
                    if (a->get_match(s, m, *g, mprotocol))
                    {
                        DEBUG("it is matching our signature !!!");

                        DEBUG("the source is " << a->get_source());
                        DEBUG("the generator is " << *mg);

                        std::shared_ptr<basic_generator> mg2=std::make_shared<external_placeholder_generator>(__FILE__, __LINE__);
                        mg2->set_father(mg);

                        mg2->initialize_mapping(mprotocol);

                        DEBUG("now the generator is " << *mg2);

                        code c;

                        a->get_source().get_actual_code_value(m, *mg2, c);

                        DEBUG("code line number " << line_number << ": it is a code " << c);

                        c.execute(m, mg2);
                    }
                    else
                    {
                        DEBUG("it is NOT matching our signature !!!");
                        DEBUG("matching protocol is " << mprotocol);
                        
                        
                        std::shared_ptr<basic_generator> mg2=std::make_shared<external_placeholder_generator>(__FILE__, __LINE__);
                        mg2->set_father(mg);
                        
                        mg2->initialize_mapping(mprotocol);
                        
                        DEBUG("now the generator is " << *mg2);
 
                        matching_protocol mprotocol2;
                        
                        if (a->get_match(s, m, *mg2, mprotocol2))
                        {
                            DEBUG("now it is matching our signature !!!");
                            switch (a->get_source().get_actual_memory_representation_type())
                            {
                                case variable_with_value::actual_memory_representation_type::CODE:
                                {
                                    code c;
                                                                
                                    a->get_source().get_actual_code_value(m, *mg, c);
                                
                                    DEBUG("code line number " << line_number << ": it is a code " << c);
                                                                                        
                                    c.execute(m, mg2);
                                }
                                break;
                        
                                default:
                                DEBUG("code line number " << line_number << ": error - we may only execute code!");
                                break;
                            }
                        
                        }
                        else
                        {
                            DEBUG("it is still not matching our signature !!!");
                        }
                    }
                }
            }
        }    
}

void chomik::execute_variable_value_statement::execute_if_cartesian_product_is_finite_and_small(machine & m, std::shared_ptr<basic_generator> & g) const
{
                generic_name name2;
                replacing_policy_exhaustive p;
                name->get_result_replacing_placeholders(m, *g, p, name2);
                                
                DEBUG("name2 = " << name2);
    
                /*
                std::cout << "result replacing placeholders ";
                name2.report(std::cout);
                std::cout << "\n";
                */
                
                signature s{name2, m, *g};
                s.set_line_number(line_number);
                //g.report(std::cout);
                //s.report(std::cout);
                
                DEBUG("signature = " << s);
                
                
                if (s.get_is_predefined(m))
                {
                    s.execute_predefined(m);
                }
                else
                {
                    if (m.get_variable_is_represented_in_memory(s))
                    {
                        //std::cout << "it is in memory\n";
                        
                        switch (m.get_actual_memory_representation_type_of_the_variable(s))
                        {
                            case variable_with_value::actual_memory_representation_type::INTEGER:
                                CHOMIK_STDERR("warning: cannot execute an integer!\n");
                                break;
                                
                            case variable_with_value::actual_memory_representation_type::FLOAT:
                                CHOMIK_STDERR("warning: cannot execute a float!\n");
                                break;

                            case variable_with_value::actual_memory_representation_type::STRING:
                                CHOMIK_STDERR("warning: cannot execute a string!\n");
                                break;
                                
                            case variable_with_value::actual_memory_representation_type::ENUM:
                                CHOMIK_STDERR("warning: cannot execute an enum!\n");
                                break;
                                
                            case variable_with_value::actual_memory_representation_type::CODE:
                            {
                                code ci;
                                //std::cout << "executing code\n";
                                m.get_variable_value_code(s, ci); 
                                
                                DEBUG("code line number " << line_number << ": code to be executed: " << ci);
                                ci.execute(m, g);
                            }
                                break;                                                                
                        }
                    }
                    else
                    {
                        DEBUG("the signature is not represented in the memory");
                        
                        DEBUG("iterate backwards through the assignment events (there are " << m.get_vector_of_assignment_events().size() << " of them)");
        
                
                        std::shared_ptr<basic_generator> mg=std::make_shared<mapping_generator>(__FILE__, __LINE__);                
                
                        mg->set_father(g);
                
                        for (int i=m.get_vector_of_assignment_events().size()-1; i>=0; i--)
                        {
                            const std::unique_ptr<assignment_event>& a{m.get_vector_of_assignment_events()[i]};
                            DEBUG("found " << *a);
                                
                            matching_protocol mprotocol;
                    
                            if (a->get_match(s, m, *g, mprotocol))
                            {
                                DEBUG("it is matching our signature !!!");

                                switch (a->get_source().get_actual_memory_representation_type())
                                {
                                    case variable_with_value::actual_memory_representation_type::CODE:
                                    {
                                    code c;
                                                                
                                    a->get_source().get_actual_code_value(m, *g, c);
                                
                                    DEBUG("code line number " << line_number << ": it is a code " << c);
                                                                                        
                                    c.execute(m, mg);
                                    }    
                                    break;
                                
                                    default:
                                    DEBUG("code line number " << line_number << ": error - we may only execute code!");
                                    break;
                                }
                
                                break;                                
                            }
                            else
                            {
                                DEBUG("it is not matching our signature !!!");
                                std::shared_ptr<basic_generator> mg2=std::make_shared<external_placeholder_generator>(__FILE__, __LINE__);
                                mg2->set_father(mg);
                        
                                mg2->initialize_mapping(mprotocol);
                        
                                DEBUG("now the generator is " << *mg2);
 
                                matching_protocol mprotocol2;
                        
                                if (a->get_match(s, m, *mg2, mprotocol2))
                                {
                                    DEBUG("now it is matching our signature !!!");
                                    switch (a->get_source().get_actual_memory_representation_type())
                                    {
                                        case variable_with_value::actual_memory_representation_type::CODE:
                                        {
                                            code c;
                                                                
                                            a->get_source().get_actual_code_value(m, *mg2, c);
                                
                                            DEBUG("code line number " << line_number << ": it is a code " << c);
                                                                                        
                                            c.execute(m, mg2);
                                        }
                                        break;
                        
                                        default:
                                            DEBUG("code line number " << line_number << ": error - we may only execute code!");
                                        break;
                                    }
                        
                                }
                                else
                                {
                                    DEBUG("it is still not matching our signature !!!");
                                }                                
                            }
                        }
                    }
                }
}


void chomik::execute_variable_value_statement::execute_if_cartesian_product_is_large_or_infinite(machine & m, std::shared_ptr<basic_generator> & g) const
{
    signature s{*name, m, *g};    
    s.set_line_number(line_number);

    DEBUG("code line number " << line_number << ": got signature " << s << ", while the generator is " << *g);
        
        if (s.get_is_predefined(m))
        {
            s.execute_predefined(m);
        }
        else
        {
            if (m.get_variable_is_represented_in_memory(s))
            {
                DEBUG("code line number " << line_number << ": it is in memory");   // this should never happen, since we do not have infinite memory!!!
                                
                throw std::runtime_error("internal error");
            }
            else
            {
                DEBUG("code line number " << line_number << ": it is NOT in memory");
                
                DEBUG("iterate backwards through the assignment events (there are " << m.get_vector_of_assignment_events().size() << " of them)");
        
                
                std::shared_ptr<basic_generator> mg=std::make_shared<mapping_generator>(__FILE__, __LINE__);                
                
                mg->set_father(g);
                
                for (int i=m.get_vector_of_assignment_events().size()-1; i>=0; i--)
                {
                    const std::unique_ptr<assignment_event>& a{m.get_vector_of_assignment_events()[i]};
                    DEBUG("found " << *a);
                                
                    matching_protocol mprotocol;
                    
                    if (a->get_match(s, m, *g, mprotocol))
                    {
                        DEBUG("it is matching our signature !!!");
                                
                        switch (a->get_source().get_actual_memory_representation_type())
                        {
                            case variable_with_value::actual_memory_representation_type::CODE:
                            {
                                code c;
                                                                
                                a->get_source().get_actual_code_value(m, *g, c);
                                
                                DEBUG("code line number " << line_number << ": it is a code " << c);
                                                                                        
                                c.execute(m, mg);
                            }    
                            break;
                        
                        default:
                            DEBUG("code line number " << line_number << ": error - we may only execute code!");
                            break;
                        }
                
                        break;
                    }
                    else
                    {
                        DEBUG("it is not matching the signature !!!");
                        DEBUG("our matching protocol is " << mprotocol);
                        mg->initialize_mapping(mprotocol);
 
                        matching_protocol mprotocol2;
                        
                        if (a->get_match(s, m, *mg, mprotocol2))
                        {                    
                            switch (a->get_source().get_actual_memory_representation_type())
                            {
                                case variable_with_value::actual_memory_representation_type::CODE:
                                {
                                    code c;
                                                                
                                    a->get_source().get_actual_code_value(m, *mg, c);
                                
                                    DEBUG("code line number " << line_number << ": it is a code " << c);
                                                                                        
                                    c.execute(m, mg);
                                }    
                                break;
                        
                                default:
                                DEBUG("code line number " << line_number << ": error - we may only execute code!");
                                break;
                            }
                
                            break;
                        }
                    }
                }                
            }
        }    

}
 

void chomik::execute_variable_value_statement::execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father) const
{
    //std::cout << "execute_variable_value_statement::execute\n";
    DEBUG("code line number " << line_number << " make a generator for name " << *name);
    
    std::shared_ptr<basic_generator> g=std::make_shared<generator>(*name, __FILE__, __LINE__);
    machine_finalization_guard<basic_generator> guard{m, *g};

    g->set_father(father);

    g->initialize(m);

    //DEBUG("got father generator " << *father);
    //DEBUG("created a generator " << *g);

    // "the break flag" is a predefined boolean variable used to terminate the implicit loops
    bool the_break_flag=false;
    generic_name gn_the_break_flag;
    gn_the_break_flag.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn_the_break_flag.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("break"));
    gn_the_break_flag.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("flag"));

    signature s{gn_the_break_flag, m, *g};
    s.set_line_number(line_number);
    
    if (g->get_the_cartesian_product_of_placeholder_types_is_empty())
    {
        // nothing to be done
    }
    else
    if (g->get_the_cartesian_product_of_placeholder_types_has_one_item())
    {
        DEBUG("code line number " << line_number << ": execute variable value (single item)");
        
        execute_if_cartesian_product_has_one_item(m, g);
    }
    else
    if (g->get_the_cartesian_product_of_placeholder_types_is_finite())
    {
        DEBUG("code line number " << line_number << ": execute variable value (small and finite loop)");
        for (; !g->get_terminated(); g->increment(m))
        {
            if (g->get_is_valid())
            {
                DEBUG("generator " << *g);
                execute_if_cartesian_product_is_finite_and_small(m, g);
            }
            
                        
            switch (m.get_actual_memory_representation_type_of_the_variable(s))
            {
                case variable_with_value::actual_memory_representation_type::ENUM:
                    the_break_flag = (m.get_variable_value_enum(s) == "true");
                    break;
                    
                default:
                    CHOMIK_STDERR("internal error\n"); // this  should never happen
            }
                                                
            if (the_break_flag)
                break;            
        }
    }    
    else
    {
        // infinite or large
        // TODO - check the families of variables
        DEBUG("code line number " << line_number << ": execute variable value (infinite loop)");

        for (;!g->get_terminated(); g->increment(m))
        {
            if (g->get_is_valid())
            {                                
                execute_if_cartesian_product_is_large_or_infinite(m, g);
            }
                        
            switch (m.get_actual_memory_representation_type_of_the_variable(s))
            {
                case variable_with_value::actual_memory_representation_type::ENUM:
                    the_break_flag = (m.get_variable_value_enum(s) == "true");
                    break;
                    
                default:
                    CHOMIK_STDERR("internal error\n"); // this  should never happen
            }
                                                
            if (the_break_flag)
                break;
        }
    }
}


void chomik::execute_value_statement::execute_if_cartesian_product_has_one_item(machine & m, basic_generator & g) const
{    
    DEBUG("code line number " << line_number << ": for " << *this << " got generator " << g);
        
    code c;
    replacing_policy_exhaustive p;
    value->get_actual_code_value(m, g, p, c);

    DEBUG("code line number " << line_number << ": got code " << c);

    c.execute(m);    
}


void chomik::execute_value_statement::execute_if_cartesian_product_is_finite_and_small(machine & m, basic_generator & g) const
{
    DEBUG("code line number " << line_number << ": for " << *this << " got generator " << g);
                                
                
    code c;
    replacing_policy_exhaustive p;
    value->get_actual_code_value(m, g, p, c);
    
    DEBUG("code line number " << line_number << ": got code " << c);
    
    c.execute(m);
}


void chomik::execute_value_statement::execute_if_cartesian_product_is_large_or_infinite(machine & m, basic_generator & g) const
{
    // TODO implement
}

void chomik::execute_value_statement::execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father) const
{
    generator g{*value, __FILE__, __LINE__};
    machine_finalization_guard<basic_generator> guard{m, g};

    g.set_father(father);
    
    g.initialize(m);

    if (g.get_the_cartesian_product_of_placeholder_types_is_empty())
    {
        // skip - there is nothing to execute
    }
    else
    if (g.get_the_cartesian_product_of_placeholder_types_has_one_item())
    {
        execute_if_cartesian_product_has_one_item(m, g);
    }
    else
    if (g.get_the_cartesian_product_of_placeholder_types_is_finite() && g.get_the_cartesian_product_of_placeholder_types_is_small(m))
    {
        /*
        std::cout << "for ";
        report(std::cout);
    
        std::cout << "got generator ";
        g.report(std::cout);
        std::cout << "\n";        
        */
        
        for (; !g.get_terminated(); g.increment(m))
        {
            if (g.get_is_valid())
            {                
                execute_if_cartesian_product_is_finite_and_small(m, g);
            }
        }
    }
    else
    if (g.get_the_cartesian_product_of_placeholder_types_is_finite())
    {
        for (; !g.get_terminated(); g.increment(m))
        {
            if (g.get_is_valid())
            {
                execute_if_cartesian_product_is_finite_and_small(m, g);
            }
        }
    }
    else
    {
        // unlimited loop?
        execute_if_cartesian_product_is_large_or_infinite(m, g);
    }    
}

void chomik::expand_statement::execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father) const
{
    m.expand(depth);
}
