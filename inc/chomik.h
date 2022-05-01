#ifndef CHOMIK_H
#define CHOMIK_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

namespace chomik
{    
    class signature;
    class code;
    
    /**
     * This class is never instantiated. Its purpose is to provide the static method checking whether the type of the given name is predefined. 
     */
    class predefined_types
    {
    private:
        static const std::string array_of_predefined_types[];
    public:
        static bool get_type_is_predefined(const std::string & type_name);
    };
    
    /**
     * This class is never instantiated. Its purpose is to provide the static method checking whether the variable (family of variables) of the given prefix is predefined.
     */
    class predefined_variables
    {
    private:
        static const std::string array_of_predefined_variables[];
    public:
        static bool get_variable_is_predefined(const std::string & prefix);
    };
    
    /**
     * This class is used to represent a variable with its value in a machine.
     */ 
    class variable_with_value
    {
    protected:
        std::shared_ptr<signature> actual_name;
    public:        
        enum class actual_memory_representation_type { NONE, INTEGER, FLOAT, STRING, CODE, ENUM };
        
        variable_with_value(std::shared_ptr<signature> && n): actual_name{std::move(n)} {}
        virtual ~variable_with_value() {}
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual actual_memory_representation_type get_representation_type() const = 0;
        
        std::string get_signature_string_representation() const;
        
        virtual int get_value_integer() const { return 0; }
        
        virtual double get_value_float() const { return 0.0; }
        
        virtual std::string get_value_string() const { return ""; }
        
        virtual std::string get_value_enum() const { return ""; }        
        
        virtual void get_value_code(code & target) const {}
        
        virtual void assign_value_integer(int v) {}
        
        virtual void assign_value_float(double v) {}
        
        virtual void assign_value_string(std::string v) {}
        
        virtual void assign_value_enum(std::string v) {}
        
        virtual void assign_value_code(const code & ci) {}
    };
    
    
    /**
     * Somewhat similar to the variable_with_value class, this class is used to represent placeholders with their values in generators.
     */
    class placeholder_with_value
    {
    protected:
        const std::string placeholder;
    public:
        placeholder_with_value(const std::string & n): placeholder{n} {}
        
        const std::string & get_name() const { return placeholder; }
        
        virtual ~placeholder_with_value() {}
        
        virtual void report(std::ostream & s) const = 0;

        virtual variable_with_value::actual_memory_representation_type get_representation_type() const = 0;        
        
        virtual bool get_is_valid() const { return true; }
        
        virtual bool get_is_terminated() const { return false; }
        
        virtual void increment() {}
        
        virtual int get_value_integer() const { return 0; }
        
        virtual double get_value_float() const { return 0.0; }
        
        virtual std::string get_value_string() const { return ""; }
        
        virtual std::string get_value_enum() const { return ""; }
    };
    
    /**
     * This is a small helper class used for matching signatures.
     */
    class matching_protocol
    {
    private:
        bool successful;
        std::map<std::string, int> map_placeholder_names_to_integer;
        std::map<std::string, double> map_placeholder_names_to_float;
        std::map<std::string, std::string> map_placeholder_names_to_string;
        std::map<std::string, std::string> map_placeholder_names_to_identifier;
        std::map<std::string, bool> map_placeholder_names_to_flag_has_been_bound;
    public:
        matching_protocol(): successful{true} {}
        bool get_is_placeholder_bound_as_integer(const std::string & p) const { return map_placeholder_names_to_integer.find(p)!=map_placeholder_names_to_integer.end(); }
        bool get_is_placeholder_bound_as_float(const std::string & p) const { return map_placeholder_names_to_float.find(p)!=map_placeholder_names_to_float.end(); }
        bool get_is_placeholder_bound_as_string(const std::string & p) const { return map_placeholder_names_to_string.find(p)!=map_placeholder_names_to_string.end(); }
        bool get_is_placeholder_bound_as_identifier(const std::string & p) const { return map_placeholder_names_to_identifier.find(p)!=map_placeholder_names_to_identifier.end(); }
        int get_placeholder_value_integer(const std::string & p) const { return map_placeholder_names_to_integer.at(p); }
        double get_placeholder_value_float(const std::string & p) const { return map_placeholder_names_to_float.at(p); }
        std::string get_placeholder_value_string(const std::string & p) const { return map_placeholder_names_to_string.at(p); }
        std::string get_placeholder_value_identifier(const std::string & p) const { return map_placeholder_names_to_identifier.at(p); }
        
        void bind_placeholder_as_integer(const std::string & p, int v) 
        { 
            auto [it, s] = map_placeholder_names_to_integer.insert(std::pair(p,v));
            if (!s) throw std::runtime_error("failed to bind a placeholder");
        }
        void bind_placeholder_as_float(const std::string & p, double v) 
        { 
            auto [it, s] = map_placeholder_names_to_float.insert(std::pair(p,v));
            if (!s) throw std::runtime_error("failed to bind a placeholder");
        }
        void bind_placeholder_as_string(const std::string & p, std::string v) 
        { 
            auto [it, s] = map_placeholder_names_to_string.insert(std::pair(p,v));
            if (!s) throw std::runtime_error("failed to bind a placeholder");
        }
        void bind_placeholder_as_identifier(const std::string & p, std::string v) 
        { 
            auto [it, s] = map_placeholder_names_to_identifier.insert(std::pair(p,v));
            if (!s) throw std::runtime_error("failed to bind a placeholder");
        }        
        bool get_is_successful() const { return successful; }
        
    };
    
    class generic_name_item;
    class machine;
    class generator;
    
    /**
     * This is a base class for all items of a signature class.
     */
    class signature_item
    {
    public:
        virtual ~signature_item() {}
        virtual void report(std::ostream & s) const = 0;
        
        virtual void print(std::ostream & s) const { report(s); } // printing is usually the same as reporting
        
        virtual bool get_is_predefined() const { return false; }
        
        virtual bool get_it_is_identifier(const std::string & pattern) const { return false; }
        
        virtual bool get_it_is_string() const { return false; }
        
        virtual bool get_it_is_integer() const { return false; }
        
        virtual bool get_it_is_float() const { return false; }
        
        virtual int get_value_integer() const { return 0; }
        
        virtual double get_value_float() const { return 0.0; }
        
        virtual std::string get_value_string() const { return ""; }

        virtual bool get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const = 0;
    };
    
    /**
     * This template class is used to create signature items of a given underlying type.
     * It is used with the simple types.
     */
    template <typename TYPE>
    class simple_value_signature_item: public signature_item
    {
    protected:
        const TYPE value;
    public:
        simple_value_signature_item(const TYPE v): value{v} {}
        virtual void report(std::ostream & s) const override
        {
            s << value;
        }
    };
    
    class simple_value_integer_signature_item: public simple_value_signature_item<int>
    {
    public:
        simple_value_integer_signature_item(const int v): simple_value_signature_item<int>(v) {}
        virtual bool get_it_is_integer() const override { return true; }
        virtual int get_value_integer() const override { return value; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const override;
    };

    class simple_value_float_signature_item: public simple_value_signature_item<double>
    {
    public:
        simple_value_float_signature_item(const double v): simple_value_signature_item<double>(v) {}
        virtual bool get_it_is_float() const override { return true; }
        virtual double get_value_float() const override { return value; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const override;        
    };
    
    /**
     * This class slightly modifies the report method of the simple_value_signature_item instantiated for std::string.
     * It is preferred to use this class rather than its base class (the template class).
     */
    class simple_value_string_signature_item: public simple_value_signature_item<std::string>
    {
    public:
        simple_value_string_signature_item(const std::string v): simple_value_signature_item<std::string>{v} {}
        virtual void report(std::ostream & s) const override
        {
            s << '\"' << value << '\"';
        }        
        virtual void print(std::ostream & s) const override 
        { 
            s << value;             
        } 
        
        virtual bool get_it_is_string() const override { return true; }
        
        virtual std::string get_value_string() const override { return value; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const override;        
    };
    
    
    /**
     * This class is used to represent an identifier withing a variable's signature.
     */
    class simple_value_enum_signature_item: public simple_value_signature_item<std::string>
    {
    public:
        simple_value_enum_signature_item(const std::string & i): simple_value_signature_item<std::string>(i) {}
        
        virtual bool get_is_predefined() const override { return predefined_variables::get_variable_is_predefined(value); }
        
        virtual bool get_it_is_identifier(const std::string & pattern) const { return value == pattern; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const generator & g, matching_protocol & target) const override;        
    };
    
    class generic_name;
    class machine;
    class generator;
    
    /**
     * A signature is an actual "name" of a variable.
     * It also contains the code to execute the variables of the predefined signatures.
     */
    class signature
    {
    private:
        std::vector<std::shared_ptr<signature_item>> vector_of_items;
        
        bool get_it_has_prefix(const std::string & pattern) const;
        
        void execute_predefined_print(machine & m) const;
        void execute_predefined_create(machine & m) const;
        void execute_predefined_get(machine & m) const;
        void execute_predefined_read(machine & m) const;
        void execute_predefined_compare(machine & m) const;
        void execute_predefined_add(machine & m) const;
        void execute_predefined_substract(machine & m) const;
    public:
        signature(const generic_name & gn, machine & m, generator & g);
        signature(const generic_name & gn);
        
        void report(std::ostream & s) const;
        
        void add_content(std::shared_ptr<signature_item> && i);
        
        std::string get_string_representation() const;
        
        bool get_is_predefined(const machine & m) const;
        
        void execute_predefined(machine & m) const;
        
        const std::vector<std::shared_ptr<signature_item>> & get_vector_of_items() const { return vector_of_items; }
        
    };
    
    
    /**
     * This class is a base class of all classes representing the types.
     */
    class generic_type
    {
    public:
        virtual ~generic_type() {}
        virtual void report(std::ostream & s) const = 0;
        virtual bool get_is_finite() const = 0;
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m) const = 0;
        virtual void add_placeholders_to_generator(generator & g) const = 0;
        
        virtual std::string get_type_name(machine & m, generator & g) const = 0;
        
        virtual std::string get_low_level_type_name() const = 0;
        
        virtual void get_copy(std::shared_ptr<generic_type> & target) const = 0;
        
        virtual void get_copy(std::unique_ptr<generic_type> & target) const = 0;
    };
    
    
    
    class generic_type_named: public generic_type
    {
    private:
        const std::string name;
    public:
        generic_type_named(const char * const n): name{n} {}
        
        generic_type_named(const std::string & n): name{n} {}
        
        virtual std::string get_type_name(machine & m, generator & g) const override
        {
            return name;
        }
        
        virtual std::string get_low_level_type_name() const override 
        {
            return name;
        }
        
        virtual void report(std::ostream & s) const
        {
            s << name;
        }
        
        virtual bool get_is_finite() const override
        {            
            if (name == "integer" || name == "float" || name=="string" || name=="code")   // these types are built-in and infinite
                return false;
            return true;
        }
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override {}
        
        virtual void get_copy(std::shared_ptr<generic_type> & target) const override
        {
            target = std::make_shared<generic_type_named>(name);
        }
        
        virtual void get_copy(std::unique_ptr<generic_type> & target) const override
        {
            target = std::make_unique<generic_type_named>(name);
        }
    };        

    
    class generic_range_boundary
    {
    public:
        virtual ~generic_range_boundary() {}
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual int get_value(machine & m) const = 0;

        virtual void add_placeholders_to_generator(generator & g) const {}    
        
        virtual void get_copy(std::unique_ptr<generic_range_boundary> & target) const = 0;
    };
    
    class generic_range_boundary_int_literal: public generic_range_boundary
    {
    private:
        const int b;
    public:
        generic_range_boundary_int_literal(const int i): b{i} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << b;
        }
        
        virtual int get_value(machine & m) const override
        {
            return b;
        }
        
        virtual void get_copy(std::unique_ptr<generic_range_boundary> & target) const override
        {
            target = std::make_unique<generic_range_boundary_int_literal>(b);
        }
    };
    
    class generic_range_boundary_variable_value: public generic_range_boundary
    {
    private:
        std::unique_ptr<generic_name> name;
    
    public:
        /**
         * The constructor owns the first parameter, it must not be destroyed by the parser! 
         */
        generic_range_boundary_variable_value(generic_name * gn): name{gn} {}
        
        generic_range_boundary_variable_value(const generic_name & gn);
        
        virtual void report(std::ostream & s) const override;

        virtual int get_value(machine & m) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override;
                
        virtual void get_copy(std::unique_ptr<generic_range_boundary> & target) const override
        {
            target = std::make_unique<generic_range_boundary_variable_value>(*name);
        }
        
    };    
    
    
    class generic_range
    {
    private:
        std::unique_ptr<generic_range_boundary> min_boundary, max_boundary;
    public:
        /* This constructor owns the first and second parameter, the parser must not destroy them! */
        generic_range(generic_range_boundary * const min_b, generic_range_boundary * const max_b):
            min_boundary{min_b}, max_boundary{max_b}
        {
        }
        
        generic_range(std::unique_ptr<generic_range_boundary> && a, std::unique_ptr<generic_range_boundary> && b):
            min_boundary{std::move(a)}, max_boundary{std::move(b)} {}
        
        void report(std::ostream & s) const
        {
            min_boundary->report(s);
            s << "..";
            max_boundary->report(s);
        }
        
        int get_min_value(machine & m) const
        {
            return min_boundary->get_value(m);
        }
        int get_max_value(machine & m) const
        {
            return max_boundary->get_value(m);
        }
        
        void add_placeholders_to_generator(generator & g) const
        {
            min_boundary->add_placeholders_to_generator(g);
            max_boundary->add_placeholders_to_generator(g);
        }
        
        void get_copy(std::unique_ptr<generic_range> & target) const
        {
            std::unique_ptr<generic_range_boundary> a, b;            
            min_boundary->get_copy(a);
            max_boundary->get_copy(b);
            target = std::make_unique<generic_range>(std::move(a), std::move(b));
        }
    };
    
    class generic_type_range: public generic_type
    {
    private:
        std::unique_ptr<generic_range> r;
    public:
        /**
         * This constructor owns its first parameter. It must not be destroyed by the parser!
         */
        generic_type_range(generic_range * const nr): r{nr} {}
        
        generic_type_range(const generic_range & rn)
        {
            std::unique_ptr<generic_range> i;
            rn.get_copy(i);
            r = std::move(i);
        }
        
        virtual void report(std::ostream & s) const
        {
            r->report(s);
        }
        virtual bool get_is_finite() const override
        {
            return true;
        }
        
        int get_min_value(machine & m) const
        {
            return r->get_min_value(m);
        }
        int get_max_value(machine & m) const
        {
            return r->get_max_value(m);
        }
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m) const override 
        { return variable_with_value::actual_memory_representation_type::INTEGER; }
        
        virtual void add_placeholders_to_generator(generator & g) const
        {
            r->add_placeholders_to_generator(g);
        }
        
        virtual std::string get_type_name(machine & m, generator & g) const override
        {
            std::stringstream s;
            s << get_min_value(m) << ".." << get_max_value(m);
            return s.str();
        }
        virtual std::string get_low_level_type_name() const override 
        {
            return "integer";
        }

        virtual void get_copy(std::shared_ptr<generic_type> & target) const override
        {
            target = std::make_shared<generic_type_range>(*r);
        }
        
        virtual void get_copy(std::unique_ptr<generic_type> & target) const override
        {
            target = std::make_unique<generic_type_range>(*r);
        }        
    };
            
    class replacing_policy;
    class type_instance;
    class type_definition;
    
    /**
     * This class is a base class for all statements in the chomik programming language.
     */
    class statement
    {    
    protected:
        unsigned line_number;
        
        static const std::vector<std::shared_ptr<type_definition>> dummy;
        
    public:
        enum class statement_type { NONE, TYPE_DEFINITION, VARIABLE_DEFINITION, ASSIGNMENT, EXECUTE, EXPAND };
        
        statement(unsigned l): line_number{l} {}
        
        virtual ~statement() {}
        
        virtual statement_type get_statement_type() const = 0;
        virtual void report(std::ostream & s) const = 0;
        
        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const = 0;                
        
        virtual void expand(machine & m, int depth) const {}
        
        virtual void add_placeholders_to_generator(generator & g) const = 0;
        
        void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const;
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & s) const = 0;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const = 0;
                
        
        virtual const std::vector<std::shared_ptr<type_definition>> & get_vector_of_type_definitions() const { return dummy; }
    };    

    /**
     * An item of a generic name - this is a base class of all such item classes.
     */
    class generic_name_item
    {
    public:
        virtual ~generic_name_item() {}
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual void add_placeholders_to_generator(generator & g) const = 0;
                
        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const = 0;
        
        virtual void add_content_to_signature(signature & target) const {}
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const = 0;
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const = 0;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const = 0;
        
        virtual bool get_is_identifier() const { return false; }
        
        virtual bool get_is_integer() const { return false; }
        
        virtual bool get_is_float() const { return false; }
        
        virtual bool get_is_string() const { return false; }
        
        virtual bool get_is_placeholder() const { return false; }
        
        virtual std::string get_placeholder_name() const { return ""; }
        
        virtual bool get_match_integer(int v) const { return false; }
        virtual bool get_match_float(double v) const { return false; }
        virtual bool get_match_string(const std::string & v) const { return false; }
        virtual bool get_match_identifier(const std::string & v) const { return false; }
    };
    
    class identifier_name_item: public generic_name_item
    {
    private:
        const std::string identifier;
    public:
        identifier_name_item(const char * const i): identifier{i} {}
        
        identifier_name_item(const std::string & i): identifier{i} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << identifier;
        }
                
        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const override;
        
        virtual void add_content_to_signature(signature & target) const override;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override
        {
            return identifier;
        }
        
        virtual void add_placeholders_to_generator(generator & g) const override {}
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<identifier_name_item>(identifier);
        }
        
        virtual bool get_is_identifier() const override { return true; }
        
        virtual bool get_match_identifier(const std::string & v) const { return identifier==v; }
    };
    
    class placeholder_name_item: public generic_name_item
    {
    private:
        const std::string placeholder;
        std::shared_ptr<generic_type> type_name;
    public:
        /**
         * This constructor owns the second parameter, it must not be destroyed by the parser!
         */
        placeholder_name_item(const char * const p, generic_type * const t): placeholder{p}, type_name{t} {}
        
        placeholder_name_item(const std::string & p, const generic_type & t): placeholder{p}
        {
            std::shared_ptr<generic_type> i;
            t.get_copy(i);
            type_name = std::move(i);
        }
        
        virtual void report(std::ostream & s) const override
        {
            s << '(' << placeholder << ':';
            type_name->report(s);
            s << ')';
        }
        
        virtual void add_placeholders_to_generator(generator & g) const override;
                
        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const override;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override;
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<placeholder_name_item>(placeholder, *type_name);
        }
        
        virtual bool get_is_placeholder() const override { return true; }
        
        virtual std::string get_placeholder_name() const { return placeholder; }
    };
    
    class list_of_generic_name_items;
    
    class variable_value_name_item: public generic_name_item
    {
    private:
        std::shared_ptr<generic_name> name;
    public:
        /**
         * This constructor does not own the first parameter, it must be destroyed by the parser!
         */
        variable_value_name_item(list_of_generic_name_items * const l);
        
        variable_value_name_item(const generic_name & gn);
        
        virtual void report(std::ostream & s) const override;

        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const override;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const;
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<variable_value_name_item>(*name);
        }        
    };


    /**
     * This class denotes a "name" consisting of a sequence of generic name items.
     */
    class generic_name
    {
    private:
        std::vector<std::shared_ptr<generic_name_item>> vector_of_name_items;
    public:
        /**
         * This constructor only copies the first parameter, it should be destroyed by the parser!
         */
        generic_name(list_of_generic_name_items * const l);
        
        generic_name();
        
        generic_name(const generic_name & gn);
        
        void report(std::ostream & s) const;

        const std::vector<std::shared_ptr<generic_name_item>> & get_vector_of_name_items() const { return vector_of_name_items; }
        
        std::string get_actual_text_representation(machine & m, generator & g) const;
        
        void add_placeholders_to_generator(generator & g) const;
        
        void add_generic_name_item(std::shared_ptr<generic_name_item> && i)
        {
            vector_of_name_items.push_back(std::move(i));
        }
        
        void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const;        
                
    };

    template <typename TYPE>    
    class simple_name_item: public generic_name_item
    {
    protected:
        const TYPE my_value;
    public:
        simple_name_item(TYPE v): my_value{v} {}
        virtual void report(std::ostream & s) const override
        {
            s << my_value;
        }
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override
        {
            std::stringstream s;
            s << my_value;
            return s.str();
        }
        
        virtual void add_placeholders_to_generator(generator & g) const {}                

    };
    
    
    class name_item_integer: public simple_name_item<int>
    {
    public:
        name_item_integer(int v): simple_name_item<int>{v} {}
        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const override
        {
            target.add_content(std::make_shared<simple_value_integer_signature_item>(my_value));
        }
        
        virtual void add_content_to_signature(signature & target) const override
        {
            target.add_content(std::make_shared<simple_value_integer_signature_item>(my_value));
        }
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
            target.add_generic_name_item(std::make_shared<name_item_integer>(my_value));
        }
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<name_item_integer>(my_value);
        }                
        
        virtual bool get_is_integer() const override { return true; }
        
        virtual bool get_match_integer(int v) const override { return my_value == v; }
    };


    class name_item_float: public simple_name_item<double>
    {
    public:
        name_item_float(double v): simple_name_item<double>{v} {}
        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const override
        {
            target.add_content(std::make_shared<simple_value_float_signature_item>(my_value));
        }
        virtual void add_content_to_signature(signature & target) const override
        {
            target.add_content(std::make_shared<simple_value_float_signature_item>(my_value));
        }
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
            target.add_generic_name_item(std::make_shared<name_item_float>(my_value));
        }
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<name_item_float>(my_value);
        }                
        
        virtual bool get_is_float() const override { return true; }
        
        virtual bool get_match_float(double v) const override { return my_value == v; }  // comparing doubles for equality is questionable
    };
    
    class name_item_string: public simple_name_item<std::string>
    {
        public:
        name_item_string(std::string v): simple_name_item<std::string>{v} {}
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
            target.add_generic_name_item(std::make_shared<name_item_string>(my_value));
        }
        virtual void report(std::ostream & s) const override
        {
            s << '\"' << my_value << '\"';
        }
        virtual void add_content_to_signature(signature & target, machine & m, generator & g) const override;
        
        virtual void add_content_to_signature(signature & target) const override
        {
            target.add_content(std::make_shared<simple_value_string_signature_item>(my_value));
        }

        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<name_item_string>(my_value);
        }        
        
        virtual bool get_is_string() const override { return true; }
        
        virtual bool get_match_string(const std::string & v) const override { return my_value == v; }
    };
    
    class type_definition_body;
    
    class type_definition
    {
    private:
        const std::string name;
        std::unique_ptr<type_definition_body> body;
    public:
        /**
         * This constructor takes the second parameter and owns it, the type_definition_body must not be destroyed by the parser!
         */        
        type_definition(const char * const type_name, type_definition_body * const b): name{type_name}, body{b} {}
                
        void report(std::ostream & s) const;
        
        void expand(machine & m, int depth, std::shared_ptr<type_instance> & e) const;
        
        void add_placeholders_to_generator(generator & g) const;
        
        const std::string & get_name() const { return name; }
        
        bool get_is_range() const;
        
        type_definition_body& get_body() { return *body; }
    };
    class type_instance;
    
    class type_definition_body
    {
    private:
    public:
        virtual ~type_definition_body() {}
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual void expand(machine & m, int depth, const std::string & n, std::shared_ptr<type_instance> & e) const = 0;
        
        virtual void add_placeholders_to_generator(generator & g) const = 0;
        
        virtual bool get_is_range() const = 0;
        
        virtual int get_min_value(machine & m) const { return 0; }
        
        virtual int get_max_value(machine & m) const { return 0; }
    };

    class type_definition_body_range: public type_definition_body
    {
    private:
        std::unique_ptr<generic_range> r;
    public:
        /**
         * This constructor tales the first parameter and owns it, the generic range must not be destroyed by the parser!.
         */
        type_definition_body_range(generic_range * const nr): r{nr} {}
        
        virtual void report(std::ostream & s) const override
        {
            r->report(s);
        }
        
        virtual void expand(machine & m, int depth, const std::string & n, std::shared_ptr<type_instance> & e) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const
        {
            r->add_placeholders_to_generator(g);
        }
        
        virtual bool get_is_range() const 
        {
            return true;
        }
        virtual int get_min_value(machine & m) const { return r->get_min_value(m); }
        
        virtual int get_max_value(machine & m) const { return r->get_max_value(m); }
    };
    
    class list_of_generic_names;
    
    class generator_placeholder
    {
    private:
        const std::string name;
        std::shared_ptr<generic_type> placeholder_type;
        
    public:
        generator_placeholder(const std::string & n, std::shared_ptr<generic_type> && t): name{n}, placeholder_type{std::move(t)} {}
        
        const std::string & get_name() const { return name; }
        
        bool get_type_is_finite() const
        {
            return placeholder_type->get_is_finite();
        }
        
        generic_type& get_placeholder_type() { return *placeholder_type; }
        
        const generic_type& get_placeholder_type() const { return *placeholder_type; }
    };    
    
    class generic_value;
    
    
    /**
     * This class creates all possible tuples of a finite cartesian product of the given sets.
     * It can be used almost as an iterator.
     */
    class generator
    {
    private:
        std::vector<std::shared_ptr<generator_placeholder>> vector_of_placeholders;
        std::map<std::string, std::shared_ptr<generator_placeholder>> map_placeholder_names_to_placeholders;
        
        std::vector<std::shared_ptr<placeholder_with_value>> memory;
        std::map<std::string, std::shared_ptr<placeholder_with_value>> map_placeholder_names_to_placeholders_with_value;
        
        
        const std::string my_filename;
        const unsigned line_number;
        
    public:
        generator(const generic_name & gn, const std::string & filename, unsigned new_line_number);
        generator(const generic_range & gr, const std::string & filename, unsigned new_line_number);
        generator(const generic_value & gv, const std::string & filename, unsigned new_line_number);
        generator(const std::string & filename, unsigned new_line_number);
        
        
        const std::vector<std::shared_ptr<generator_placeholder>> & get_vector_of_placeholders() const { return vector_of_placeholders; }
        
        bool get_has_placeholder(const std::string & p) const
        {
            return map_placeholder_names_to_placeholders.find(p)!=map_placeholder_names_to_placeholders.end();
        }
        
        bool get_has_placeholder_with_value(const std::string & p) const
        {
            return map_placeholder_names_to_placeholders_with_value.find(p)!=map_placeholder_names_to_placeholders_with_value.end();
        }
        
        placeholder_with_value& get_placeholder_with_value(const std::string & p)
        {
            return *map_placeholder_names_to_placeholders_with_value.find(p)->second;
        }

        const placeholder_with_value& get_placeholder_with_value(const std::string & p) const
        {
            return *map_placeholder_names_to_placeholders_with_value.find(p)->second;
        }
        
        std::string get_actual_text_representation_of_a_placeholder(const std::string & placeholder)
        {
            return "unknown_placeholder";
        }
        
        int get_placeholder_value_integer(const std::string & p) const { return get_placeholder_with_value(p).get_value_integer(); }
        
        double get_placeholder_value_float(const std::string & p) const { return get_placeholder_with_value(p).get_value_float(); }
        
        std::string get_placeholder_value_string(const std::string & p) const { return get_placeholder_with_value(p).get_value_string(); }
        
        std::string get_placeholder_value_enum(const std::string & p) { return get_placeholder_with_value(p).get_value_enum(); }
        
        void add_placeholder(const std::string & p, std::shared_ptr<generic_type> && t);
        
        void add_placeholder_with_value(std::shared_ptr<placeholder_with_value> && p)
        {
            auto [it, success] = map_placeholder_names_to_placeholders_with_value.insert(std::pair(p->get_name(), p));
            if (!success)
            {
                throw std::runtime_error("failed to insert a placeholder with value");
            }
            
            memory.push_back(std::move(p));
        }
        
        void report(std::ostream & s) const;
        
        bool get_the_cartesian_product_of_placeholder_types_is_finite() const;
        
        // small is a relative term, we should take into account some info about the available machine
        bool get_the_cartesian_product_of_placeholder_types_is_small() const;
        
        // TODO - if there is at least one empty type then the whole cartesian product is empty
        bool get_the_cartesian_product_of_placeholder_types_is_empty() const { return false; }
        
        // TODO - it is also possible for some types like range 1..1
        bool get_the_cartesian_product_of_placeholder_types_has_one_item() const { return vector_of_placeholders.size()==0; }
        
        void initialize(machine & m);
        
        bool get_is_valid();
        
        void increment();
        
        bool get_terminated() const;
    };
    
    class type_definition_body_enum: public type_definition_body
    {
    private:
        std::vector<std::shared_ptr<generic_name>> vector_of_names;
    public:
        /**
         * This constructor does not own the first parameter, it should be destroyed by the parser!
         */
        type_definition_body_enum(list_of_generic_names * const l);
                
        virtual void report(std::ostream & s) const override;
        
        virtual void expand(machine & m, int depth, const std::string & n, std::shared_ptr<type_instance> & e) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override;
        
        virtual bool get_is_range() const 
        {
            return false;
        }
    };
    
    class list_of_type_definitions
    {
    private:
        std::vector<std::shared_ptr<type_definition>> vector_of_type_definitions;
    public:
        list_of_type_definitions(type_definition * const t, list_of_type_definitions * const l)
        {
            if (t)
            {
                std::shared_ptr<type_definition> xt{t};
                vector_of_type_definitions.push_back(std::move(xt));
            }
            if (l)
            {
                for (auto & i: l->vector_of_type_definitions)
                {
                    std::shared_ptr<type_definition> yl{i};
                    vector_of_type_definitions.push_back(std::move(yl));
                }
            }
        }
        
        std::vector<std::shared_ptr<type_definition>> & get_vector_of_type_definitions() { return vector_of_type_definitions; }
    };
    
    class list_of_generic_names
    {
    private:
        std::vector<std::shared_ptr<generic_name>> vector_of_names;
    public:
        /**
         * This constructor only copies the second parameter, it must be destroyed by the parser!
         */
        list_of_generic_names(generic_name * const gn, list_of_generic_names * const l)
        {
            if (gn)
            {
                std::shared_ptr<generic_name> xt{gn};
                vector_of_names.push_back(std::move(xt));
            }
            if (l)
            {
                for (auto & i: l->vector_of_names)
                {
                    std::shared_ptr<generic_name> yl{i};
                    vector_of_names.push_back(std::move(yl));
                }
            }
        }
        
        std::vector<std::shared_ptr<generic_name>>& get_vector_of_names() { return vector_of_names; }
    };
    
    /**
     * This class represents a type definition statement. It may contain multiple type definitions.
     * It is used with the expand statement (no types will be defined without it).
     */
    class type_definition_statement: public statement
    {
    private:
        std::vector<std::shared_ptr<type_definition>> vector_of_type_definitions;
    public:
        /**
         * This constructor does not own the first parameter. It must be destroyed by the parser!
         */
        type_definition_statement(list_of_type_definitions * const l, unsigned new_line_number): statement{new_line_number}
        {
            if (l)
            {
                for (auto & i: l->get_vector_of_type_definitions())
                {
                    std::shared_ptr<type_definition> x{i};
                    vector_of_type_definitions.push_back(std::move(x));
                }
            }
        }
        
        type_definition_statement(unsigned new_line_number): statement{new_line_number}
        {
        }
                
        virtual statement_type get_statement_type() const override { return statement_type::TYPE_DEFINITION; }
                        
        virtual void report(std::ostream & s) const override;

        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const override;                
        
        virtual void expand(machine & m, int depth) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override;
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override
        {
            // TODO - implement
        }
        
        virtual const std::vector<std::shared_ptr<type_definition>> & get_vector_of_type_definitions() const override { return vector_of_type_definitions; }
    };
    
    class variable_definition
    {
    private:
        std::shared_ptr<generic_name> name;
        std::shared_ptr<generic_type> type_name;
        
    public:
        /**
         * 
         */
        variable_definition(std::shared_ptr<generic_name> && n, std::shared_ptr<generic_type> && t): name{n}, type_name{t} {}
        
        void report(std::ostream & s) const
        {
            name->report(s);
            s << ':';
            type_name->report(s);
        }
        
        void expand(machine & m, int depth) const;
        
        void add_placeholders_to_generator(generator & g) const
        {
            name->add_placeholders_to_generator(g);
            type_name->add_placeholders_to_generator(g);
        }
    };   
    
    class list_of_variable_definitions
    {
    private:
        std::vector<std::shared_ptr<variable_definition>> vector_of_variable_definitions;
    public:
        /**
         * This constructor owns the first parameter and copies the second one, the second parameter must be destroyed by the parser!
         */
        list_of_variable_definitions(variable_definition * const vd, list_of_variable_definitions * const l) 
        {
            if (vd)
            {
                std::shared_ptr<variable_definition> x{vd};
                vector_of_variable_definitions.push_back(std::move(x));
            }
            if (l)
            {
                for (auto & i: l->vector_of_variable_definitions)
                {
                    std::shared_ptr<variable_definition> y{i};
                    vector_of_variable_definitions.push_back(std::move(y));
                }
            }
        }
        
        std::vector<std::shared_ptr<variable_definition>>& get_vector_of_variable_definitions() { return vector_of_variable_definitions; }
    };
    
    
    class variable_definition_statement: public statement
    {
    private:
        std::vector<std::shared_ptr<variable_definition>> vector_of_variable_definitions;
    public:
        /**
         * This constructor copies the first parameter only, it should be destroyed by the parser !!!
         */
        variable_definition_statement(list_of_variable_definitions * const l, unsigned new_line_number):
            statement{new_line_number}
        {
            if (l)
            {
                for (auto & i: l->get_vector_of_variable_definitions())
                {
                    std::shared_ptr<variable_definition> y{i};
                    vector_of_variable_definitions.push_back(std::move(y));
                }
            }
        }
        
        variable_definition_statement(unsigned new_line_number): statement{new_line_number}
        {
        }
        
                
        virtual statement_type get_statement_type() const override { return statement_type::VARIABLE_DEFINITION; }        
        
        variable_definition& get_last_variable_definition()
        {
            return *vector_of_variable_definitions[vector_of_variable_definitions.size()-1];
        }
                
        virtual void report(std::ostream & s) const override;
        
        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const override;        
        
        virtual void expand(machine & m, int depth) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override;
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;        
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override
        {
            // TODO - implement
        }        
    };
    
    /**
     * This class is used by the generic_value and its descendent classes to control the method get_actual_code_value.
     */
    class replacing_policy
    {
    public:
        virtual bool replace_all_variables_with_their_values() const = 0;
        
        virtual bool replace_known_placeholders_with_their_values() const = 0;
    };
    
    class replacing_policy_literal: public replacing_policy
    {
    public:
        virtual bool replace_all_variables_with_their_values() const override { return false; }
        
        virtual bool replace_known_placeholders_with_their_values() const override { return true; }
    };
    
    class replacing_policy_exhaustive: public replacing_policy
    {
    public:
        virtual bool replace_all_variables_with_their_values() const override { return true; }
        
        virtual bool replace_known_placeholders_with_their_values() const override { return true; }
    };

    class generic_literal;
    
    /**
     * This is a base class for various values (integers, floats, strings, enums, code).
     */
    class generic_value
    {
    public:
        virtual ~generic_value() {}
                
        virtual void report(std::ostream & s) const = 0;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const = 0;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const = 0;
        
        virtual int get_actual_integer_value(machine & m, generator & g) const { return 0; }
        
        virtual double get_actual_float_value(machine & m, generator & g) const { return 0.0; }
        
        virtual std::string get_actual_string_value(machine & m, generator & g) const { return ""; }
        
        virtual std::string get_actual_enum_value(machine & m, generator & g) const { return ""; }
        
        virtual void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const {}
        
        virtual void add_placeholders_to_generator(generator & g) const = 0;
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const = 0;
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const = 0;
        
        virtual void get_copy(std::unique_ptr<generic_value> & target) const = 0;
        
        virtual void get_literal_copy(std::unique_ptr<generic_literal> & target) const {}
        
        virtual bool get_is_code_with_placeholders(machine & m, generator & g) const { return false; }
        
        virtual bool get_is_literal() const { return false; }
                
    };
    
    
    /**
     * This class is used when we want to represent a value of a variable (in chomik the <> construct).
     */
    class generic_value_variable_value: public generic_value
    {
    private:
        std::unique_ptr<generic_name> name;
        
    public:
        /**
         * This constructor owns the first parameter, it must not be destroyed by the parser!!!
         */
        generic_value_variable_value(generic_name* const n): name{n} {}
        
        generic_value_variable_value(std::unique_ptr<generic_name> && n): name{std::move(n)} {}
        
        generic_value_variable_value(const generic_name & gn): name{std::make_unique<generic_name>(gn)} {}
                
        virtual void report(std::ostream & s) const override
        {
            s << '<';
            name->report(s);
            s << '>';
        }
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override
        {
            /*
            std::cout << "generic_value_variable_value - adding placeholders from ";
            name->report(std::cout);
            std::cout << " to the generator\n";
            */
            name->add_placeholders_to_generator(g);
        }
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual int get_actual_integer_value(machine & m, generator & g) const override;
        
        virtual double get_actual_float_value(machine & m, generator & g) const override { return 0.0; }
        
        virtual std::string get_actual_string_value(machine & m, generator & g) const override;
        
        virtual std::string get_actual_enum_value(machine & m, generator & g) const override;
        
        virtual void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const override {}        
        
        virtual void get_copy(std::unique_ptr<generic_value> & target) const override
        {
            target = std::make_unique<generic_value_variable_value>(*name);
        }
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const override;
                
    };
    
    
    /**
     * This class represents the literals of all built-in types and user defined enums.
     */
    class generic_literal
    {
    public:
        virtual ~generic_literal() {}
        virtual void report(std::ostream & s) const = 0;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const = 0;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const = 0;
        
        virtual void add_placeholders_to_generator(generator & g) const {}
        
        virtual void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const {}

        virtual std::string get_actual_enum_value(machine & m, generator & g) const { return ""; }
        
        virtual int get_actual_integer_value(machine & m, generator & g) const { return 0; }
        
        virtual double get_actual_float_value(machine & m, generator & g) const { return 0.0; }
        
        virtual std::string get_actual_string_value(machine & m, generator & g) const { return ""; }
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const = 0;        
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const = 0;
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const = 0;
    };
    
    
    /**
     * This class represents placeholders used in value integer or value string context. It corresponds with the expression: value integer [(X:integer)].
     */
    class generic_literal_placeholder: public generic_literal
    {
    private:
        const std::string placeholder;
        std::unique_ptr<generic_type> type_name;
        const variable_with_value::actual_memory_representation_type expected_type;
    public:
        /**
         * This constructor owns the second parameter, it must not be destroyed by the parser!
         */
        generic_literal_placeholder(const char * const p, generic_type * const t, variable_with_value::actual_memory_representation_type e): 
            placeholder{p}, type_name{t}, expected_type{e} {}
            
        generic_literal_placeholder(const std::string & p, std::unique_ptr<generic_type> && t, variable_with_value::actual_memory_representation_type e): 
            placeholder{p}, type_name{std::move(t)}, expected_type{e} {}
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override { return expected_type; }
        
        virtual void report(std::ostream & s) const override;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override;
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual std::string get_actual_string_value(machine & m, generator & g) const override;
        
        virtual int get_actual_integer_value(machine & m, generator & g) const override; 
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            std::unique_ptr<generic_type> t;
            type_name->get_copy(t);
            target = std::make_unique<generic_literal_placeholder>(placeholder, std::move(t), expected_type);
        }
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override;
    };
    
    class generic_value_literal: public generic_value
    {
    private:
        std::unique_ptr<generic_literal> literal;
    public:
        /**
         * This constructor owns the first parameter, and it must not be destroyed by the parser!
         */
        generic_value_literal(generic_literal * const l): literal{l} {}
                
        generic_value_literal(std::unique_ptr<generic_literal> && l): literal{std::move(l)} {} 
        
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override
        {
            return literal->get_actual_memory_representation_type(m, g);
        }
        virtual void report(std::ostream & s) const override
        {
            literal->report(s);
        }        
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override
        {
            return literal->get_actual_text_representation(m, g);            
        }
        virtual void add_placeholders_to_generator(generator & g) const override
        {
            literal->add_placeholders_to_generator(g);
        }
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
            literal->get_result_replacing_placeholders(m, g, p, target);
        }
        
        virtual std::string get_actual_enum_value(machine & m, generator & g) const override { return literal->get_actual_enum_value(m, g); }
        
        virtual int get_actual_integer_value(machine & m, generator & g) const override { return literal->get_actual_integer_value(m, g); }
        
        virtual double get_actual_float_value(machine & m, generator & g) const override { return literal->get_actual_float_value(m, g); }
        
        virtual std::string get_actual_string_value(machine & m, generator & g) const override { return literal->get_actual_string_value(m, g); }
                
        virtual void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const 
        {
            literal->get_actual_code_value(m, g, p, target);
        }

        virtual void get_copy(std::unique_ptr<generic_value> & target) const override
        {
            std::unique_ptr<generic_literal> i;
            literal->get_copy(i);
            target = std::make_unique<generic_value_literal>(std::move(i));
        }
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const override;
        
        virtual bool get_is_literal() const override { return true; }
        
        virtual void get_literal_copy(std::unique_ptr<generic_literal> & target) const 
        {
            literal->get_copy(target);
        }
    };    
    
    template <typename TYPE, int REPRESENTATION_TYPE>
    class simple_literal: public generic_literal
    {
    protected:
        const TYPE my_value;
    public:
        simple_literal(const TYPE v): my_value{v} {}
        virtual void report(std::ostream & s) const override
        {
            s << "value " << my_value;
        }
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override
        {
            return static_cast<variable_with_value::actual_memory_representation_type>(REPRESENTATION_TYPE);
        }
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override
        {
            std::stringstream s;
            s << my_value;
            return s.str();
        }
        virtual void add_placeholders_to_generator(generator & g) const override {}
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
            // left empty intentionally
        }        

    };   
    
    class simple_literal_integer: public simple_literal<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    public:
        simple_literal_integer(int v): simple_literal<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>(v) {}
        virtual int get_value_integer() const { return my_value; }        
        
        virtual int get_actual_integer_value(machine & m, generator & g) const override { return my_value; }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            target = std::make_unique<simple_literal_integer>(my_value);
        }                
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
        {
            get_copy(target);
        }
    };
    
    class simple_literal_float: public simple_literal<double, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::FLOAT)>
    {
    public:
        simple_literal_float(double v): simple_literal<double, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::FLOAT)>(v) {}
        virtual double get_value_float() const { return my_value; }
        virtual double get_actual_float_value(machine & m, generator & g) const override { return my_value; }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            target = std::make_unique<simple_literal_float>(my_value);
        }
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
        {
            get_copy(target);
        }
    };    
    
    
    
    class simple_literal_string: public simple_literal<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>
    {
    public:
        simple_literal_string(const std::string & v): simple_literal<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>{v} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << "value string " << '\"' << my_value << '\"';
        }        
        
        virtual std::string get_value_string() const { return my_value; }
        
        virtual std::string get_actual_string_value(machine & m, generator & g) const { return my_value; }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const
        {
            target = std::make_unique<simple_literal_string>(my_value);
        }

        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
        {
            get_copy(target);
        }
    };
    

    class list_of_statements;
    
    /**
     * This class represents a literal of the built-in type code.
     */
    class generic_literal_code: public generic_literal
    {
    private:
        std::unique_ptr<code> my_code_pointer;        
    public:
        /**
         * This constructor calls code::code which owns the parameter, so it must not be destroyed by the parser!
         */
        generic_literal_code(list_of_statements * const l): my_code_pointer{std::make_unique<code>(l)}
        {
        }
        
        generic_literal_code(): my_code_pointer{std::make_unique<code>()}
        {
        }
        
        generic_literal_code(const code & c): my_code_pointer{std::make_unique<code>(c)} {}
        
        code & get_code() { return *my_code_pointer; }
        
        virtual void report(std::ostream & s) const override;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override 
        { 
            return variable_with_value::actual_memory_representation_type::CODE;
        }
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override;                
        
        virtual void add_placeholders_to_generator(generator & g) const override;
        
        virtual void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const override;
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
            //std::cout << "generic_literal_code::get_result_replacing_placeholders\n";
        }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const
        {
            target = std::make_unique<generic_literal_code>(*my_code_pointer);
        }

        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override;
    };
    
    class generic_literal_enum: public generic_literal
    {
    private:
        const std::string type_name;
        std::unique_ptr<generic_name> name;
    public:
        /**
         * This constructor owns the second parameter, it must not be destroyed by the parser!
         */
        generic_literal_enum(const char * const tn, generic_name * const gn): type_name{tn}, name{gn} {}
        
        generic_literal_enum(const std::string & tn, const generic_name & gn): type_name{tn}, name{std::make_unique<generic_name>(gn)} {}

        virtual void report(std::ostream & s) const override
        {
            s << "value " << type_name << ' ';
            name->report(s);
        }

        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override 
        { 
            return variable_with_value::actual_memory_representation_type::ENUM;
        }        
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override
        {
            return name->get_actual_text_representation(m, g);
        }
        virtual void add_placeholders_to_generator(generator & g) const override
        {
            name->add_placeholders_to_generator(g);
        }
        
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override
        {
        }        
        
        virtual std::string get_actual_enum_value(machine & m, generator & g) const override { return name->get_actual_text_representation(m, g); }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            target = std::make_unique<generic_literal_enum>(type_name, *name);
        }        

        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
        {
            get_copy(target);
        }
    };
    
    
    class generic_value_placeholder: public generic_value
    {
    private:
        const std::string placeholder;
        std::shared_ptr<generic_type> type_name;
    public:
        generic_value_placeholder(const char * const p, generic_type * const t): placeholder{p}, type_name{t} {}
        
        generic_value_placeholder(const std::string & p, std::shared_ptr<generic_type> && gt): placeholder{p}, type_name{std::move(gt)} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << "[ (" << placeholder << ':';
            type_name->report(s);            
            s << ") ]";
        }
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, generator & g) const override;
        
        virtual std::string get_actual_text_representation(machine & m, generator & g) const override;
        
        virtual void add_placeholders_to_generator(generator & g) const override
        {
            std::shared_ptr<generic_type> t{type_name};
            g.add_placeholder(placeholder, std::move(t));
            type_name->add_placeholders_to_generator(g);
        }
        virtual void get_result_replacing_placeholders(machine & m, generator & g, const replacing_policy & p, generic_name & target) const override;
                
        virtual void get_copy(std::unique_ptr<generic_value> & target) const override
        {
            std::shared_ptr<generic_type> i;
            type_name->get_copy(i);
            target = std::make_unique<generic_value_placeholder>(placeholder, std::move(i));
        }
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const override;        
    };
    
    /**
     * This class represents an assignment, i.e. instruction that assignes a value to a variable defined by a generic name.
     * It may also be a loop of assignments, with the placeholders replaced by their actual values.
     */
    class assignment_statement: public statement
    {
    private:
        std::unique_ptr<generic_name> name;
        std::unique_ptr<generic_value> value;
                
        void execute_if_cartesian_product_has_one_item(machine & m, generator & g) const;        
        
        void execute_if_cartesian_product_is_finite_and_small(machine & m, generator & g) const;
        
        void execute_if_cartesian_product_is_large_or_infinite(machine & m, generator & g) const;
        
    public:
        /**
         * This constructor owns both parameters, they must not be destroyed by the parser!
         */
        assignment_statement(generic_name * const n, generic_value * const v, unsigned new_line_number): name{n}, value{v}, statement{new_line_number} {}        
        
        assignment_statement(const generic_value & gv, unsigned new_line_number): 
            statement{new_line_number}, name{std::make_unique<generic_name>()}            
        {
            std::unique_ptr<generic_value> i;
            gv.get_copy(i);
            value = std::move(i);
        }
        
        assignment_statement(const generic_name & gn, const generic_value & gv, unsigned new_line_number);
        
        virtual statement_type get_statement_type() const override { return statement_type::ASSIGNMENT; }
                        
        virtual void report(std::ostream & s) const override;

        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const override;        

        virtual void add_placeholders_to_generator(generator & g) const override
        {
            name->add_placeholders_to_generator(g);
            // we do not do value->add_placeholders_to_generator(g); !!! this is intentional!
        }
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override;
    };
    
    
    /**
     * The base class of the execution statements.
     */
    class execute_statement: public statement
    {
    public:
        execute_statement(unsigned new_line_number): statement{new_line_number} {}
        virtual ~execute_statement() {}
    };    
    
    
    /**
     * This class represents an execution statement for a code value.
     * It is meant to execute directly a code literal.
     */
    class execute_value_statement: public execute_statement
    {
    private:
        std::unique_ptr<generic_value> value;   // it should be a code
        
        void execute_if_cartesian_product_has_one_item(machine & m, generator & g) const;
        
        void execute_if_cartesian_product_is_finite_and_small(machine & m, generator & g) const;
        
        void execute_if_cartesian_product_is_large_or_infinite(machine & m, generator & g) const;
        
    public:                
        execute_value_statement(std::unique_ptr<generic_value> && v, unsigned new_line_number): value{std::move(v)}, execute_statement{new_line_number} {}
        
        //execute_value_statement(unsigned new_line_number): value{std::make_unique<generic_value_literal>()}, execute_statement{new_line_number} {}
        
        virtual statement_type get_statement_type() const override { return statement_type::EXECUTE; }        

        virtual void report(std::ostream & s) const override;
                
        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const override;                
        
        virtual void add_placeholders_to_generator(generator & g) const override;

        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override
        {
            std::unique_ptr<generic_value> n;
            value->get_copy(n);
            target = std::make_shared<execute_value_statement>(std::move(n), line_number);
        }        
    };
    
    
    /**
     * This class represents a statement that executes a variable value (that is supposed to be code).
     * When the variable signature begins with a predefined identifier (like print) then it is a predefined code variable.
     */
    class execute_variable_value_statement: public execute_statement
    {
    private:
        std::unique_ptr<generic_name> name;
        
        void execute_if_cartesian_product_has_one_item(machine & m, generator & g) const;
        
        void execute_if_cartesian_product_is_finite_and_small(machine & m, generator & g) const;
        
        void execute_if_cartesian_product_is_large_or_infinite(machine & m, generator & g) const;
        
    public:
        execute_variable_value_statement(std::unique_ptr<generic_name> && n, unsigned new_line_number): name{std::move(n)}, execute_statement{new_line_number} {}
        
        execute_variable_value_statement(unsigned new_line_number): name{std::make_unique<generic_name>()}, execute_statement{new_line_number}
        {
        }
        
        void add_generic_name_item(std::shared_ptr<generic_name_item> && i)
        {
            name->add_generic_name_item(std::move(i));
        }
        
        virtual statement_type get_statement_type() const override { return statement_type::EXECUTE; }        
        
        virtual void report(std::ostream & s) const override;
        
        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const override;        
        
        virtual void add_placeholders_to_generator(generator & g) const override;
        
        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override
        {
            std::unique_ptr<generic_name> n=std::make_unique<generic_name>(*name);
            target = std::make_shared<execute_variable_value_statement>(std::move(n), line_number);
        }
    };
    
    
    /**
     * This class represents a special statement expanding the types and variables.
     */
    class expand_statement: public statement
    {
    private:
        const int depth;
    public:
        expand_statement(int nd, unsigned new_line_number): depth{nd}, statement{new_line_number} {}
        
        virtual statement_type get_statement_type() const override { return statement_type::EXPAND; }
        
        virtual void report(std::ostream & s) const override
        {
            s << "expand(" << depth << ");";            
        }

        virtual void execute(machine & m, std::shared_ptr<const statement> && i) const override;        
        
        virtual void add_placeholders_to_generator(generator & g) const override {}

        virtual void make_copy_with_replacements(machine & m, generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;        
        
        virtual void get_copy(std::shared_ptr<statement> & target) const 
        {
            target = std::make_shared<expand_statement>(depth, line_number);
        }
    };
    
    
    /**
     * It is a list of executable statements of the programming language chomik.
     */
    class list_of_statements
    {
    private:
        std::vector<std::shared_ptr<statement>> vector_of_statements;
        bool is_main;
    public:
        /**
         * This constructor only copies the statements (both the first one and the list of subsequent statements).
         * Its parameters should be therefore destroyed afterwards.
         */
        list_of_statements(statement * const s, list_of_statements * const l): is_main{false} 
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
        void report(std::ostream & s) const;
        
        std::vector<std::shared_ptr<statement>>& get_vector_of_statements() { return vector_of_statements; }
        
        const std::vector<std::shared_ptr<statement>>& get_vector_of_statements() const { return vector_of_statements; }
        
        void add_statement(std::shared_ptr<statement> && s)
        {
            vector_of_statements.push_back(std::move(s));
        }
        
        void set_is_main(bool m)
        {
            is_main = m;
        }
        
        void add_placeholders_to_generator(generator & g) const;
        
        void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const;
    };
    
    
    class list_of_generic_name_items
    {
    private:
        std::vector<std::shared_ptr<generic_name_item>> vector_of_name_items;
    public:
        /**
         * This constructor owns the first parameter and copies the second one, the second parameter must be destroyed by the parser!
         */        
        list_of_generic_name_items(generic_name_item * const i, list_of_generic_name_items * const l) 
        {
            if (i)
            {
                std::shared_ptr<generic_name_item> xi{i};
                vector_of_name_items.push_back(std::move(xi));
            }
            if (l)
            {
                for (auto & j: l->vector_of_name_items)
                {
                    std::shared_ptr<generic_name_item> y{j};
                    vector_of_name_items.push_back(std::move(y));
                }
            }
        }
        
        std::vector<std::shared_ptr<generic_name_item>> & get_vector_of_name_items() { return vector_of_name_items; }
    };
    
    class type_instance_enum_value;
    
    class type_instance
    {
    private:
        static const std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator dummy;
    protected:
        const std::string name;        
    public:
        enum class type_instance_mode { NONE, INTEGER, ENUM };
        
        type_instance(const std::string & n): name{n} {}
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual void add_type_instance_enum_value(const signature & n, unsigned new_level=1) {}
        
        virtual void add_type_instance_enum_value(const std::string & n, unsigned new_level=1) {}
        
        virtual std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator get_first_iterator_for_enum() const { return dummy; }
        virtual std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator get_last_iterator_for_enum() const { return dummy; }
        
        virtual int get_first_iterator_for_range() const { return 0; }
        virtual int get_last_iterator_for_range() const { return 0; }
        
        const std::string get_name() const { return name; }
        
        virtual type_instance_mode get_mode() const { return type_instance_mode::NONE; }
    };
    

    class type_instance_enum_value
    {
    private:
        const std::string name;
        const unsigned level;
    public:
        type_instance_enum_value(const std::string & n, unsigned new_level): name{n}, level{new_level} {}
        
        const std::string & get_name() const { return name; }
    };
    
    class type_instance_enum: public type_instance
    {
    private:
        std::vector<std::unique_ptr<type_instance_enum_value>> vector_of_values;
    public:
        type_instance_enum(const std::string & n): type_instance{n} {}
        
        virtual void add_type_instance_enum_value(const signature & n, unsigned new_level) override
        {
            vector_of_values.push_back(std::make_unique<type_instance_enum_value>(n.get_string_representation(), new_level));
        }                
        
        virtual void add_type_instance_enum_value(const std::string & n, unsigned int new_level) override
        {
            vector_of_values.push_back(std::make_unique<type_instance_enum_value>(n, new_level));
        }
        
        virtual void report(std::ostream & s) const override;
        
        virtual std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator get_first_iterator_for_enum() const override
        {
            return vector_of_values.begin();
        }
        virtual std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator get_last_iterator_for_enum() const override
        {
            return vector_of_values.end();
        }
        
        virtual type_instance_mode get_mode() const { return type_instance_mode::ENUM; }
    };
    
    class type_instance_range: public type_instance
    {
    private:
        const int min_boundary, max_boundary;
    public:
        type_instance_range(const std::string & n, int b1, int b2): type_instance{n}, min_boundary{b1}, max_boundary{b2} {}
        
        virtual void report(std::ostream & s) const override;
        
        virtual int get_first_iterator_for_range() const override { return min_boundary; }
        
        virtual int get_last_iterator_for_range() const override { return max_boundary; }
        
        virtual type_instance_mode get_mode() const { return type_instance_mode::INTEGER; }
    };
    
    /**
     * This class is used in the description_of_a_cartesian_product.
     */
    class cartesian_product_dimension
    {
    private:
        const std::string placeholder;
        const std::string type_name;
        const bool is_finite;           // some of the dimensions can be finite
    public:
        cartesian_product_dimension(const std::string & p, const std::string & t, bool f): placeholder{p}, type_name{t}, is_finite{f} {}
        virtual ~cartesian_product_dimension() {}
        
        void report(std::ostream & s) const;
    };
    
    /**
     * This class is used for infinite or very large cartesian products.
     */
    class description_of_a_cartesian_product
    {
    private:
        std::vector<std::unique_ptr<cartesian_product_dimension>> vector_of_dimensions;
    public:
        description_of_a_cartesian_product(const generator & g);
        void add_dimension(std::unique_ptr<cartesian_product_dimension> && d)
        {
            vector_of_dimensions.push_back(std::move(d));
        }
        void report(std::ostream & s) const;
    };    
    
    
    class family_of_variables
    {
    private:        
        std::unique_ptr<description_of_a_cartesian_product> my_cartesian_product;
    public:
        family_of_variables(std::unique_ptr<description_of_a_cartesian_product> && d): my_cartesian_product{std::move(d)} {}
        virtual ~family_of_variables() {}
    };
    
    
    class assignment_source
    {
    private:
    public:
        virtual ~assignment_source() {}
        virtual void report(std::ostream & s) const = 0;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type() const 
        { return variable_with_value::actual_memory_representation_type::NONE; }
        
        virtual int get_actual_integer_value(machine & m, generator & g) const { return 0; }
        virtual double get_actual_float_value(machine & m, generator & g) const { return 0.0; }
        virtual std::string get_actual_string_value(machine & m, generator & g) const { return ""; }
        virtual std::string get_actual_identifier_value(machine & m, generator & g) const { return ""; }
    };
    
    /**
     * This class is used as an assignment source when a literal of type integer, float or any enum is used.
     * It might also be used for code without placeholders.
     * Maybe it's cleaner not to use it for code at all.
     */
    class assignment_source_literal_value: public assignment_source
    {
    private:
        std::unique_ptr<generic_literal> my_value;
        const variable_with_value::actual_memory_representation_type actual_type;
    public:
        assignment_source_literal_value(std::unique_ptr<generic_literal> && v, variable_with_value::actual_memory_representation_type t): 
            my_value{std::move(v)}, actual_type{t} {}
        virtual void report(std::ostream & s) const override;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type() const override { return actual_type; }
        
        virtual int get_actual_integer_value(machine & m, generator & g) const override { return my_value->get_actual_integer_value(m, g); }
        virtual double get_actual_float_value(machine & m, generator & g) const override { return my_value->get_actual_float_value(m, g); }
        virtual std::string get_actual_string_value(machine & m, generator & g) const override { return my_value->get_actual_string_value(m, g); }
        virtual std::string get_actual_identifier_value(machine & m, generator & g) const override { return my_value->get_actual_enum_value(m, g); }        
    };
    
    /**
     * This class is used as an assignment source when a code is assigned.
     */
    class assignment_source_code_pattern: public assignment_source
    {
    private:
        std::unique_ptr<generic_literal> my_value;
    public:
        assignment_source_code_pattern(std::unique_ptr<generic_literal> && v): my_value{std::move(v)} {}
        virtual void report(std::ostream & s) const override;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type() const override 
        { return variable_with_value::actual_memory_representation_type::CODE; }
    };
    
    class assignment_source_variable_value: public assignment_source
    {
    private:
    public:
        assignment_source_variable_value() {}
        virtual void report(std::ostream & s) const override;
        
        // TODO - identify the variable family and implement variable_with_value::actual_memory_representation_type get_actual_memory_representation_type() const override;
    };
    
    
    /**
     * The assignment events are used when the amount of variables assigned is infinite or too large.
     */
    class assignment_event
    {
    private:
        std::unique_ptr<generic_name> my_assignment_target;
        std::unique_ptr<description_of_a_cartesian_product> my_cartesian_product;
        std::unique_ptr<assignment_source> my_assignment_source;
        
    public:
        assignment_event(std::unique_ptr<generic_name> && t, 
                         std::unique_ptr<description_of_a_cartesian_product> && p,
                         std::unique_ptr<assignment_source> && s): 
            my_assignment_target{std::move(t)},
            my_cartesian_product{std::move(p)},
            my_assignment_source{std::move(s)} {}
        virtual ~assignment_event() {}  // this may be not necessary
        void report(std::ostream & s) const;        
        bool get_match(const signature & s, const machine & m, const generator & g) const;
        const assignment_source& get_source() const { return *my_assignment_source; }
    };
    
    
    template <typename TYPE, int REPRESENTATION_TYPE> class simple_variable_with_value: public variable_with_value
    {
    protected:
        TYPE value;
    public:
        simple_variable_with_value(std::shared_ptr<signature> && s, TYPE v): variable_with_value{std::move(s)}, value{v} {}
        
        virtual void report(std::ostream & s) const override
        {
            actual_name->report(s); 
            s << '=' << value << "\n";
        }
        
        virtual actual_memory_representation_type get_representation_type() const override
        {
            return static_cast<actual_memory_representation_type>(REPRESENTATION_TYPE);
        }
    };
    
    class simple_variable_with_value_integer: public simple_variable_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    public:
        simple_variable_with_value_integer(std::shared_ptr<signature> && s, int v): simple_variable_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>(std::move(s), v) {}
        
        virtual int get_value_integer() const override { return value; }
        
        virtual void assign_value_integer(int v) override { value = v; }
        
    };
    
    class simple_variable_with_value_float: public simple_variable_with_value<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>
    {
    public:
        simple_variable_with_value_float(std::shared_ptr<signature> && s, double v): simple_variable_with_value<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>(std::move(s), v) {}        
        
        virtual double get_value_float() const override { return value; }
        
        virtual void assign_value_float(double v) override { value = v; }
    };

    class simple_variable_with_value_string: public simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>
    {
    public:
        simple_variable_with_value_string(std::shared_ptr<signature> && s, std::string v): simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>(std::move(s), v) {}        

        virtual std::string get_value_string() const override { return value; }        
        
        virtual void assign_value_string(std::string v) override { value = v; }
    };
    
    class simple_variable_with_value_enum: public simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>
    {
    public:
        simple_variable_with_value_enum(std::shared_ptr<signature> && s, std::string v): simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>(std::move(s), v) {}        
        
        virtual std::string get_value_enum() const override { return value; }
        
        virtual void assign_value_enum(std::string v) override { value = v; }
    };
    
    /**
     * This class is used to represent a variable_with_value with the value of the built-in type code.
     */
    class variable_with_value_code: public variable_with_value
    {
    private:
        std::unique_ptr<code> value;
    public:
        variable_with_value_code(std::shared_ptr<signature> && s, std::unique_ptr<code> && v): variable_with_value{std::move(s)}, value{std::move(v)} {}

        virtual void report(std::ostream & s) const override;
        
        virtual void get_value_code(code & target) const;
        
        virtual void assign_value_code(const code & ci) override;
        
        virtual actual_memory_representation_type get_representation_type() const override
        {
            return actual_memory_representation_type::CODE;
        }
    };
    
    template <typename TYPE, int REPRESENTATION_TYPE> class simple_placeholder_with_value: public placeholder_with_value
    {
    protected:
        TYPE value;
    public:
        simple_placeholder_with_value(const std::string & p, const TYPE v): placeholder_with_value{p}, value{v} {}        
        
        virtual variable_with_value::actual_memory_representation_type get_representation_type() const override
        {
            return static_cast<variable_with_value::actual_memory_representation_type>(REPRESENTATION_TYPE);
        }
    };
    
    template <typename TYPE, int REPRESENTATION_TYPE> class simple_placeholder_with_value_and_report: public simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>
    {
    public:
        simple_placeholder_with_value_and_report(const std::string & p, const TYPE v): simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>{p, v} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>::placeholder << '=' << simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>::value << '\n';
        }        
    };
    
    
    class simple_placeholder_for_enum: public simple_placeholder_with_value<std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>
    {
    private:
        using iterator_type = std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator;
        const iterator_type first, last;
    public:
        simple_placeholder_for_enum(const std::string & p, const iterator_type f, const iterator_type l):        
            simple_placeholder_with_value<std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>
            {p, f}, first{f}, last{l} {}
            
        virtual void report(std::ostream & s) const override
        {
            s << placeholder << '=' << (*value)->get_name() << "\n";
        }        
        
        virtual bool get_is_valid() const override
        {
            return value != last;
        }
        
        virtual bool get_is_terminated() const override
        {
            return value == last;
        }
        virtual void increment() override { if (value == last) value = first; else value++; }
        
        virtual std::string get_value_enum() const override { return (*value)->get_name(); }
    };
    
    class simple_placeholder_for_range: public simple_placeholder_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    private:
        const int first, last;
    public:
        simple_placeholder_for_range(const std::string & p, const int f, const int l): simple_placeholder_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>{p, f}, first{f}, last{l+1} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << placeholder << '=' << value << "\n";
        }
        virtual bool get_is_valid() const override
        {
            return value != last;
        }
        virtual bool get_is_terminated() const override
        {
            return value >= last;
        }        
        virtual void increment() override { if (value == last) value = first; else value++; }
        
        virtual int get_value_integer() const override { return value; }
    };
    
    /**
     * The base class of all streams used by a chomik program.
     */
    class generic_stream
    {
    public:        
        virtual ~generic_stream() {}
        virtual bool get_allows_input() const { return false; }
        virtual bool get_allows_output() const { return false; }
        virtual bool get_should_be_opened() const { return false; }
        virtual bool get_should_be_closed() const { return false; }
        virtual std::ostream& get_output_stream() { return std::cout; }
        virtual std::istream& get_input_stream() { return std::cin; }
        virtual std::string get_result() const { return ""; }
        virtual std::string read_string() { return ""; }
        virtual int read_integer() { return 0; }
    };
    
    class generic_stream_standard_output_stream: public generic_stream
    {
    public:
        virtual bool get_allows_output() const override { return true; }
        virtual std::ostream& get_output_stream() override { return std::cout; }
    };
    
    class generic_stream_standard_error_stream: public generic_stream
    {
    public:
        virtual bool get_allows_output() const override { return true; }        
        virtual std::ostream& get_output_stream() override { return std::cerr; }
    };
    
    class generic_stream_standard_input_stream: public generic_stream
    {
    public:
        virtual bool get_allows_input() const override { return true; }
        virtual std::string read_string() override { std::string word; std::cin >> word; return word; }
    };
    
    class generic_stream_file: public generic_stream
    {
    public:
    };
    
    class generic_stream_file_output: public generic_stream_file
    {
    private:
        std::ofstream file_stream;
    public:
        generic_stream_file_output(const std::string & file_name): file_stream{file_name} {}
        virtual bool get_allows_output() const override { return true; }
        virtual bool get_should_be_opened() const override { return true; }
        virtual bool get_should_be_closed() const override { return true; }
        virtual std::ostream& get_output_stream() override { return file_stream; }

    };
    
    class generic_stream_file_input: public generic_stream_file
    {
    private:
        std::ifstream file_stream;
    public:
        virtual bool get_allows_input() const { return true; }
        virtual bool get_should_be_opened() const override { return true; }        
        virtual bool get_should_be_closed() const override { return true; }
        virtual std::istream& get_input_stream() override { return file_stream; }
        virtual std::string read_string() override { std::string word; file_stream >> word; return word; }
    };    
    
    class generic_stream_stringstream: public generic_stream
    {
    private:
        std::stringstream string_stream;
    public:
        generic_stream_stringstream() {}
        generic_stream_stringstream(const std::string & s): string_stream{s} {}
        
        virtual bool get_allows_input() const { return true; }
        virtual bool get_allows_output() const { return true; }
        virtual std::ostream& get_output_stream() override { return string_stream; }
        virtual std::istream& get_input_stream() override { return string_stream; }
        
        virtual std::string get_result() const override { return string_stream.str(); }
        
        virtual std::string read_string() override { std::string word; string_stream >> word; return word; }
    };
    
    class generic_stream_random_number_stream: public generic_stream
    {
    private:
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution;
        
        std::random_device dev;
        std::mt19937 rng;
        std::uniform_int_distribution<std::mt19937::result_type> dist;
    public:
        generic_stream_random_number_stream(int a, int b): rng{dev()}, dist(a, b) {}
        virtual bool get_allows_input() const { return true; }
        virtual int read_integer() override;
    };
    
    /**
     * This class represents the memory used by the program while executing it.
     */
    class machine
    {
    private:
        std::vector<std::shared_ptr<const statement>> vector_of_type_definiton_statements;
        std::vector<std::shared_ptr<const statement>> vector_of_variable_definition_statements;
        
        std::vector<std::shared_ptr<type_instance>> vector_of_type_instances;
        std::map<std::string, std::shared_ptr<type_instance>> map_type_name_to_type_instance;
        
        
        std::vector<std::shared_ptr<family_of_variables>> vector_of_variable_families;
        
        std::vector<std::unique_ptr<assignment_event>> vector_of_assignment_events;
        
        std::vector<std::shared_ptr<variable_with_value>> memory;
                
        std::map<std::string, std::shared_ptr<variable_with_value>> map_signature_string_representation_to_variable_with_value;
        
        std::vector<std::unique_ptr<generic_stream>> vector_of_streams;
        
    public:
        
        const std::vector<std::unique_ptr<assignment_event>> & get_vector_of_assignment_events() const { return vector_of_assignment_events; }
        
        
        void add_stream(std::unique_ptr<generic_stream> && s)
        {
            vector_of_streams.push_back(std::move(s));
        }
        
        void add_assignment_event(std::unique_ptr<assignment_event> && e)
        {
            vector_of_assignment_events.push_back(std::move(e));
        }
        
        void add_type_definition_statement(std::shared_ptr<const statement> && s)
        {
            vector_of_type_definiton_statements.push_back(std::move(s));
        }
        
        void add_variable_definition_statement(std::shared_ptr<const statement> && s)
        {
            vector_of_variable_definition_statements.push_back(std::move(s));
        }
        
        bool get_type_instance_is_known(const std::string & type_name) const
        {
            return map_type_name_to_type_instance.find(type_name) != map_type_name_to_type_instance.end();
        }
        
        type_instance & get_type_instance(const std::string & type_name) const
        {
            return *map_type_name_to_type_instance.at(type_name);
        }
        
        void get_first_and_last_iterators_for_enum_type(const std::string & type_name, std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator & f, std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator & l) const
        {
            f = map_type_name_to_type_instance.at(type_name)->get_first_iterator_for_enum();
            l = map_type_name_to_type_instance.at(type_name)->get_last_iterator_for_enum();
        }
        
        void get_first_and_last_iterators_for_range_type(const std::string & type_name, int & f, int & l) const
        {
            f = map_type_name_to_type_instance.at(type_name)->get_first_iterator_for_range();
            l = map_type_name_to_type_instance.at(type_name)->get_last_iterator_for_range();
        }
        
        void add_type_instance(std::shared_ptr<type_instance> && i)
        {
            auto [it, success] = map_type_name_to_type_instance.insert(std::pair(i->get_name(), i));
            
            if (!success)
            {
                throw std::runtime_error("failed to insert a type instance into machine's map");
            }
            
            std::shared_ptr<type_instance> i2{i};
            vector_of_type_instances.push_back(std::move(i2));
        }
        
        void add_variable_with_value(std::shared_ptr<variable_with_value> && vv)
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
        
        void expand(int i);
        
        void report(std::ostream & s) const;
        
        bool get_variable_is_represented_in_memory(const signature & vn) const;
        
        const variable_with_value & get_variable_with_value(const signature & vn) const;
        
        variable_with_value & get_variable_with_value(const signature & vn);
        
        variable_with_value::actual_memory_representation_type get_actual_memory_representation_type_of_the_variable(const signature & vn) const;
        
        std::string get_actual_text_representation_of_a_variable(const signature & vn) const;
        
        std::string get_variable_value_enum(const signature & vn) const;

        std::string get_variable_value_string(const signature & vn) const;
        
        int get_variable_value_integer(const signature & vn) const;
        
        double get_variable_value_float(const signature & vn) const;
        
        void get_variable_value_code(const signature & vn, code & target) const;
        
        virtual void create_predefined_variables();
        
        virtual void create_predefined_streams();
        
        virtual void create_predefined_types();
        
        virtual bool get_is_user_defined_executable(const signature & s) const { return false; }
        
        virtual void execute_user_defined_executable(const signature & s) {}

        
        int get_last_created_stream_index() const { return vector_of_streams.size()-1; } // because we only create streams, we do not delete them (this may change!!!)
        
        int get_amount_of_streams() const { return vector_of_streams.size(); }
        
        generic_stream& get_stream(int i) { return *vector_of_streams[i]; }
    };
    
    /**
     * This class contains a list of statements, it represents a code and can be executed.
     * It is used as a built-in type code to represent its values.
     */
    class code
    {
    private:
        std::shared_ptr<list_of_statements> body;
        
        bool is_main; // this class is used for pretty-printing only
        
    public:
        /**
         * This constructor owns the parameter, and it must not be destroyed by the parser ! 
         */
        code(list_of_statements * const l): body{std::make_shared<list_of_statements>(nullptr, l)}, is_main{false} 
        {
            body->set_is_main(is_main);
        }
        
        code(): body{std::make_shared<list_of_statements>(nullptr, nullptr)}, is_main{false}
        {
            body->set_is_main(is_main);
        }
                        
        void execute(machine & m) const;
                        
        void add_statement(std::shared_ptr<statement> && s)
        {
            body->add_statement(std::move(s));
        }

        void set_is_main(bool m)
        {
            is_main = m;
            body->set_is_main(m);
        }
        
        const list_of_statements& get_body() const { return *body; }
        
        void report(std::ostream & s) const;
        
        void add_placeholders_to_generator(generator & g) const;
        
        void get_actual_code_value(machine & m, generator & g, const replacing_policy & p, code & target) const;
    };
    
    /**
     * An instance of this class is created in the interpreter, it is configured by the parser, can be reported and executed.
     */
    class program
    {
    private:
        code my_code;
    public:
        program();                
        
        void execute(machine & m) const { my_code.execute(m); }        
        void report(std::ostream & s) const { my_code.report(s); }
        
        void add_statement(std::shared_ptr<statement> && s);
                        
    };
    
    
    /**
     * This is a parser, it is in fact written in bison, and it cooperates with a flex scanner.
     * Its purpose is to configure the program.
     */
    class parser
    {
    private:
        program & my_program;
        
    public:
        parser(program & p);
                
        int parse(const char * filename);
                
        void copy_list_of_statements_to_the_program(list_of_statements * const l);
    };
    
}

std::ostream & operator<<(std::ostream & s, const chomik::generic_value & v);
std::ostream & operator<<(std::ostream & s, const chomik::code & c);
std::ostream & operator<<(std::ostream & s, const chomik::generic_name & n);
std::ostream & operator<<(std::ostream & s, const chomik::generator & g);
std::ostream & operator<<(std::ostream & s, const chomik::generic_name_item & i);
std::ostream & operator<<(std::ostream & s, const chomik::signature & x);
std::ostream & operator<<(std::ostream & s, const chomik::signature_item & x);
std::ostream & operator<<(std::ostream & s, const chomik::statement & x);
std::ostream & operator<<(std::ostream & s, const chomik::generic_type & x);
std::ostream & operator<<(std::ostream & s, const chomik::type_definition_body & x);
std::ostream & operator<<(std::ostream & s, const chomik::type_definition & x);
std::ostream & operator<<(std::ostream & s, const chomik::assignment_event & x);
std::ostream & operator<<(std::ostream & s, const chomik::assignment_source & x);

#endif
