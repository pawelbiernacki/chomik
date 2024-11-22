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
#include <iomanip>
#include <regex>

//#define CHOMIK_DONT_USE_OPTIMIZATIONS
// disable this flag to build an optimized chomik

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

        std::string get_signature_item_representation(int item_index) const;
        std::string get_signature_item_type_name(int item_index) const;
        int get_amount_of_signature_items() const;
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual actual_memory_representation_type get_representation_type() const = 0;
        
        std::string get_signature_string_representation() const;
        
        virtual int get_value_integer() const { return 0; }
        
        virtual double get_value_float() const { return 0.0; }
        
        virtual std::string get_value_string() const { return "?"; }
        
        virtual std::string get_value_enum() const { return ""; }        
        
        virtual void get_value_code(code & target) const {}
        
        virtual void assign_value_integer(int v) {}
        
        virtual void assign_value_float(double v) {}
        
        virtual void assign_value_string(std::string v) {}
        
        virtual void assign_value_enum(std::string v) {}
        
        virtual void assign_value_code(const code & ci) {}

        virtual std::string get_debug_type_name() const { return "variable_with_value"; }
    };
    
    
    class machine;
    class basic_generator;

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
        
        virtual std::string get_value_string() const { return ""; }
        
        virtual std::string get_value_enum() const { return ""; }

        virtual void get_value_code(code & target) const {}
        
        virtual bool get_exceeds_level(int max_level) const { return false; }
        
        virtual int get_level() const { return 0; }

        virtual void update_ad_hoc_range_type_instance(machine & m, basic_generator & g) {}

        virtual void update_int_value(int f, int l) {}

    };
    
    class mapping_generator;
    class external_placeholder_generator;
    class generator;
    
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
        std::map<std::string, code> map_placeholder_names_to_code;
        std::map<std::string, bool> map_placeholder_names_to_flag_has_been_bound;
        
        std::map<std::string, std::string> map_placeholder_names_to_placeholder_names;
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
        void bind_placeholder_as_code(const std::string & p, const code & c);
        void bind_placeholder_as_placeholder(const std::string & p, const std::string & p2)
        {
            auto [it, s] = map_placeholder_names_to_placeholder_names.insert(std::pair(p,p2));
            if (!s) throw std::runtime_error("failed to bind a placeholder");
        }
        
        bool get_is_successful() const { return successful; }
                
        void report(std::ostream & s) const;
        
        void initialize_mapping(mapping_generator & target) const;
        
        void initialize_mapping(external_placeholder_generator & target) const;
        
        void copy_bound_placeholders(generator & target) const;
    };

    
    class generic_name_item;
    class machine;
    class basic_generator;
    
    /**
     * This is a base class for all items of a signature class.
     */
    class signature_item
    {
    protected:
        const generic_name_item & source;
    public:
        signature_item(const generic_name_item & s): source{s} {}
        
        virtual ~signature_item() {}
        virtual void report(std::ostream & s) const = 0;

        std::string get_string_representation() const;

        virtual std::string get_type_name() const = 0;
        
        virtual void print(std::ostream & s) const { report(s); } // printing is usually the same as reporting
        
        virtual bool get_is_predefined() const { return false; }
        
        virtual bool get_it_is_identifier(const std::string & pattern) const { return false; }
        
        virtual bool get_it_is_string() const { return false; }
        
        virtual bool get_it_is_integer() const { return false; }
        
        virtual bool get_it_is_float() const { return false; }
        
        virtual bool get_it_is_enum() const { return false; }

        virtual bool get_it_is_code() const { return false; }
        
        virtual int get_value_integer() const { return 0; }
        
        virtual double get_value_float() const { return 0.0; }
        
        virtual std::string get_value_string() const { return ""; }
        
        virtual std::string get_value_enum() const { return ""; }

        virtual void get_value_code(std::unique_ptr<code> & target) {}

        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const = 0;
        
        virtual void get_copy(std::shared_ptr<signature_item> & target) const = 0;

        virtual std::string get_debug_type_name() const { return "signature_item"; }
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
        simple_value_signature_item(const generic_name_item & s, const TYPE v): signature_item{s}, value{v} {}

        virtual void report(std::ostream & s) const override
        {
            s << value;
        }
    };
    
    class simple_value_integer_signature_item: public simple_value_signature_item<int>
    {
    public:
        simple_value_integer_signature_item(const generic_name_item & s, const int v): simple_value_signature_item<int>(s, v) {}
        virtual bool get_it_is_integer() const override { return true; }
        virtual int get_value_integer() const override { return value; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const override;
        
        virtual void get_copy(std::shared_ptr<signature_item> & target) const override 
        {
            target = std::make_shared<simple_value_integer_signature_item>(source, value);
        }

        virtual std::string get_debug_type_name() const { return "simple_value_integer_signature_item"; }

        virtual std::string get_type_name() const override { return "integer"; }
    };

    class simple_value_float_signature_item: public simple_value_signature_item<double>
    {
    public:
        simple_value_float_signature_item(const generic_name_item & s, const double v): simple_value_signature_item<double>(s, v) {}
        virtual bool get_it_is_float() const override { return true; }
        virtual double get_value_float() const override { return value; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const override;        
        
        virtual void get_copy(std::shared_ptr<signature_item> & target) const override 
        {
            target = std::make_shared<simple_value_float_signature_item>(source, value);
        }

        virtual std::string get_debug_type_name() const { return "simple_value_float_signature_item"; }

        virtual std::string get_type_name() const override { return "float"; }
    };
    
    /**
     * This class slightly modifies the report method of the simple_value_signature_item instantiated for std::string.
     * It is preferred to use this class rather than its base class (the template class).
     */
    class simple_value_string_signature_item: public simple_value_signature_item<std::string>
    {
    public:
        simple_value_string_signature_item(const generic_name_item & s, const std::string v): simple_value_signature_item<std::string>{s, v} {}
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
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const override;        
        
        virtual void get_copy(std::shared_ptr<signature_item> & target) const override 
        {
            target = std::make_shared<simple_value_string_signature_item>(source, value);
        }

        virtual std::string get_debug_type_name() const { return "simple_value_string_signature_item"; }

        virtual std::string get_type_name() const override { return "string"; }
    };

    /**
     * This is a dictionary of identifiers. It is used only to minimize the memory usage of the identifier name items.
     */
    class dictionary_of_identifiers
    {
    private:
        std::vector<std::string> vector_of_identifiers;
        const std::string debug_name; // only for debugging
    public:
        dictionary_of_identifiers(const std::string & d): debug_name{d} {}
        int get_identifier_index(const std::string id); // if necessary - create a new identifier
        const std::string get_identifier_by_index(int index) const;
    };


    class base_class_with_dictionary
    {
    protected:
        static dictionary_of_identifiers our_dictionary;
    public:

    };


#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
    /**
     * This class is used to represent an identifier withing a variable's signature.
     */
    class simple_value_enum_signature_item: public simple_value_signature_item<std::string>
    {
    public:
        simple_value_enum_signature_item(const generic_name_item & s, const std::string & i): simple_value_signature_item<std::string>(s, i) {}
        
        virtual bool get_is_predefined() const override { return predefined_variables::get_variable_is_predefined(value); }
        
        virtual bool get_it_is_identifier(const std::string & pattern) const { return value == pattern; }
        
        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const override;        
        
        virtual bool get_it_is_enum() const override { return true; }
        
        virtual std::string get_value_enum() const override { return value; }
        
        virtual void get_copy(std::shared_ptr<signature_item> & target) const override 
        {
            target = std::make_shared<simple_value_enum_signature_item>(source, value);
        }

        virtual std::string get_debug_type_name() const { return "simple_value_enum_signature_item"; }

        const std::string get_enum() const { return value; }

        virtual std::string get_type_name() const override { return "identifier"; }
    };
#else
    class simple_value_enum_signature_item: public simple_value_signature_item<int>, private base_class_with_dictionary
    {
    public:
        simple_value_enum_signature_item(const generic_name_item & s, const std::string & i): simple_value_signature_item<int>(s, our_dictionary.get_identifier_index(i)) {}

        virtual void print(std::ostream & s) const override
        {
            s << our_dictionary.get_identifier_by_index(value);
        }

        virtual void report(std::ostream & s) const override
        {
            s << our_dictionary.get_identifier_by_index(value);
        }

        virtual bool get_is_predefined() const override { return predefined_variables::get_variable_is_predefined(our_dictionary.get_identifier_by_index(value)); }

        virtual bool get_it_is_identifier(const std::string & pattern) const { return our_dictionary.get_identifier_by_index(value) == pattern; }

        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const override;

        virtual bool get_it_is_enum() const override { return true; }

        virtual std::string get_value_enum() const override { return our_dictionary.get_identifier_by_index(value); }

        virtual void get_copy(std::shared_ptr<signature_item> & target) const override
        {
            const std::string id = our_dictionary.get_identifier_by_index(value);
            target = std::make_shared<simple_value_enum_signature_item>(source, id);
        }

        virtual std::string get_debug_type_name() const { return "simple_value_enum_signature_item"; }

        const std::string get_enum() const { return our_dictionary.get_identifier_by_index(value); }

        virtual std::string get_type_name() const override { return "identifier"; }
    };
#endif
    
    
    class code_signature_item: public signature_item
    {
    private:
        std::unique_ptr<code> my_code;
    public:
        code_signature_item(const generic_name_item & s, const code & c): signature_item{s}, my_code{std::make_unique<code>(c)} {}

        code_signature_item(const code_signature_item & s): signature_item{s.source}, my_code{std::make_unique<code>(*s.my_code)} {}

        virtual void report(std::ostream & s) const override;

        virtual bool get_it_is_code() const override { return true; }

        virtual bool get_match(const generic_name_item & gni, const machine & m, const basic_generator & g, matching_protocol & target) const override;

        virtual void get_copy(std::shared_ptr<signature_item> & target) const;

        virtual std::string get_type_name() const override { return "code"; }
    };
    
    class generic_name;
    class machine;
    class signature_common_data;
    class generic_stream;
    
    
    /**
     * A signature is an actual "name" of a variable.
     * It also contains the code to execute the variables of the predefined signatures.
     */
    class signature
    {
    private:       
        static std::unique_ptr<signature_common_data> our_common_data;
        
        std::vector<std::shared_ptr<signature_item>> vector_of_items;
        
        bool get_it_has_prefix(const std::string & pattern) const;
        
        void execute_predefined_print(machine & m) const;
        void execute_predefined_create(machine & m) const;
        void execute_predefined_get(machine & m) const;
        void execute_predefined_read(machine & m) const;
        void execute_predefined_compare(machine & m) const;
        void execute_predefined_add(machine & m) const;
        void execute_predefined_subtract(machine & m) const;
        void execute_predefined_multiply(machine & m) const;
        void execute_predefined_divide(machine & m) const;        
        void execute_predefined_set(machine & m) const;
        void execute_predefined_getline(machine & m) const;
        void execute_predefined_execution(machine & m) const;
        void execute_predefined_match(machine & m) const;
        void execute_predefined_modulo(machine & m) const;
        void execute_predefined_cast(machine & m) const;

        void set_stream_flags(machine & m, generic_stream & gs) const;

    public:
        signature(const generic_name & gn, const machine & m, const basic_generator & g);
        signature(const generic_name & gn);
        signature();

        int get_amount_of_items() const { return vector_of_items.size(); }
        
        void report(std::ostream & s) const;
        
        void add_content(std::shared_ptr<signature_item> && i);
        
        std::string get_string_representation() const;

        std::string get_item_representation(int item_index) const { return vector_of_items[item_index]->get_string_representation(); }
        std::string get_item_type_name(int item_index) const  { return vector_of_items[item_index]->get_type_name(); }
        
        bool get_is_predefined(const machine & m) const;
        
        void execute_predefined(machine & m) const;
        
        const std::vector<std::shared_ptr<signature_item>> & get_vector_of_items() const { return vector_of_items; }            
    };

    /**
     * This is a kind of "regular expression" used to match signatures.
     */
    class signature_regular_expression
    {
    private:
        const std::string my_signature_regular_expression_code;

        std::unique_ptr<generic_name> my_name;

        std::vector<std::shared_ptr<signature_item>> vector_of_items;

        std::unique_ptr<std::regex> my_regular_expression;

        enum class state { INITIAL, IDENTIFIER, PLACEHOLDER_1, PLACEHOLDER_2, PLACEHOLDER_3, PLACEHOLDER_4, PLACEHOLDER_5, PLACEHOLDER_6 };

        void parse(const std::string & c);

    public:
        signature_regular_expression(const std::string & c);

        const std::vector<std::shared_ptr<signature_item>> & get_vector_of_items() const { return vector_of_items; }

        std::regex& get_regular_expression() { return *my_regular_expression; }
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
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(const machine & m) const = 0;
        virtual void add_placeholders_to_generator(basic_generator & g) const = 0;
        
        virtual std::string get_type_name(const machine & m, const basic_generator & g) const = 0;

        virtual std::string get_generic_type_name() const = 0;

        virtual std::string get_low_level_type_name() const = 0;
        
        virtual void get_copy(std::shared_ptr<generic_type> & target) const = 0;
        
        virtual void get_copy(std::unique_ptr<generic_type> & target) const = 0;

        virtual bool get_is_equal(const generic_type & t) const = 0;

        virtual bool get_is_an_ad_hoc_type() const = 0;

        virtual void update_boundaries(machine & m, int & f, int & l, basic_generator & g) const {}
    };
    
    
    
    class generic_type_named: public generic_type
    {
    private:
        const std::string name;
    public:
        generic_type_named(const char * const n): name{n} {}
        
        generic_type_named(const std::string & n): name{n} {}
        
        virtual std::string get_type_name(const machine & m, const basic_generator & g) const override
        {
            return name;
        }

        virtual std::string get_generic_type_name() const override
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
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(const machine & m) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override {}
        
        virtual void get_copy(std::shared_ptr<generic_type> & target) const override
        {
            target = std::make_shared<generic_type_named>(name);
        }
        
        virtual void get_copy(std::unique_ptr<generic_type> & target) const override
        {
            target = std::make_unique<generic_type_named>(name);
        }

        virtual bool get_is_equal(const generic_type & t) const override
        {
            // TODO fix me
            return false;
        }

        virtual bool get_is_an_ad_hoc_type() const override
        {
            return false;// The named types are not "ad hoc" types
        }
    };        

    
    class generic_range_boundary
    {
    public:
        virtual ~generic_range_boundary() {}
        
        virtual void report(std::ostream & s) const = 0;
        
        virtual int get_value(const machine & m, const basic_generator & g) const = 0;

        virtual void add_placeholders_to_generator(basic_generator & g) const {}    
        
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
        
        virtual int get_value(const machine & m, const basic_generator & g) const override
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

        virtual int get_value(const machine & m, const basic_generator & g) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
                
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
        
        int get_min_value(const machine & m, const basic_generator & g) const
        {
            return min_boundary->get_value(m, g);
        }
        int get_max_value(const machine & m, const basic_generator & g) const
        {
            return max_boundary->get_value(m, g);
        }
        
        void add_placeholders_to_generator(basic_generator & g) const
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
        
        int get_min_value(const machine & m, const basic_generator & g) const
        {
            return r->get_min_value(m, g);
        }
        int get_max_value(const machine & m, const basic_generator & g) const
        {
            return r->get_max_value(m, g);
        }
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(const machine & m) const override 
        { return variable_with_value::actual_memory_representation_type::INTEGER; }
        
        virtual void add_placeholders_to_generator(basic_generator & g) const
        {
            r->add_placeholders_to_generator(g);
        }
        
        virtual std::string get_type_name(const machine & m, const basic_generator & g) const override
        {
            std::stringstream s;
            s << get_min_value(m, g) << ".." << get_max_value(m, g);
            return s.str();
        }

        virtual std::string get_generic_type_name() const override
        {
            std::stringstream s;
            r->report(s);
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

        virtual bool get_is_equal(const generic_type & t) const override
        {
            // TODO fix me
            return false;
        }

        virtual bool get_is_an_ad_hoc_type() const override
        {
            return true;
        }

        virtual void update_boundaries(machine & m, int & f, int & l, basic_generator & g) const override
        {
            f = get_min_value(m, g);
            l = get_max_value(m, g);
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
        
        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const = 0;                
        
        virtual void expand(machine & m, int depth) const {}
        
        virtual void add_placeholders_to_generator(basic_generator & g) const = 0;
        
        void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const;

        virtual bool get_is_code_assignment(machine & m) const { return false; }
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & s) const = 0;
        
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
        
        virtual void add_placeholders_to_generator(basic_generator & g) const = 0;
                
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const = 0;
        
        virtual void add_content_to_signature(signature & target) const {}
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const = 0;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const = 0;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const = 0;
        
        virtual bool get_is_identifier() const { return false; }
        
        virtual bool get_is_integer() const { return false; }
        
        virtual bool get_is_float() const { return false; }
        
        virtual bool get_is_string() const { return false; }

        virtual bool get_is_code() const { return false; }
        
        virtual bool get_is_placeholder() const { return false; }
        
        virtual std::string get_placeholder_name() const { return ""; }
        
        virtual bool get_match_integer(int v, const machine & m, const basic_generator & g) const { return false; }
        virtual bool get_match_float(double v, const machine & m, const basic_generator & g) const { return false; }
        virtual bool get_match_string(const std::string & v, const machine & m, const basic_generator & g) const { return false; }
        virtual bool get_match_identifier(const std::string & v, const machine & m, const basic_generator & g) const { return false; }
        virtual bool get_match_code(const code & v, const machine & m, const basic_generator & g) const { return false; }

        virtual bool get_is_variable_value() const { return false; }
    };

    class list_of_generic_name_items;
    
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

        std::string get_actual_text_representation(const machine & m, const basic_generator & g) const;

        void add_placeholders_to_generator(basic_generator & g) const;

        void add_generic_name_item(std::shared_ptr<generic_name_item> && i)
        {
            vector_of_name_items.push_back(std::move(i));
        }

        void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const;

        bool operator==(const generic_name & n) const;
    };

    /**
     *
     */
    class identifier_name_item: public generic_name_item, private base_class_with_dictionary
    {
    private:
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
        const std::string identifier;
#else
        const int index;
#endif


    public:
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
        identifier_name_item(const char * const i): identifier{i} {}
        
        identifier_name_item(const std::string & i): identifier{i} {}
#else
        identifier_name_item(const char * const i): index{our_dictionary.get_identifier_index(i)} {}

        identifier_name_item(const std::string & i): index{our_dictionary.get_identifier_index(i)} {}
#endif
        
        virtual void report(std::ostream & s) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
            s << identifier;
#else
            s << our_dictionary.get_identifier_by_index(index);
#endif
        }
                
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
    target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, identifier));
#else
    target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, our_dictionary.get_identifier_by_index(index)));
#endif
        }
        
        virtual void add_content_to_signature(signature & target) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
            target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, identifier));
#else
            target.add_content(std::make_shared<simple_value_enum_signature_item>(*this, our_dictionary.get_identifier_by_index(index)));
#endif
        }
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
            return identifier;
#else
            return our_dictionary.get_identifier_by_index(index);
#endif
        }
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override {}
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
            target.add_generic_name_item(std::make_unique<identifier_name_item>(get_actual_text_representation(m, g)));
#else
            target.add_generic_name_item(std::make_unique<identifier_name_item>(our_dictionary.get_identifier_by_index(index)));
#endif
        }
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
            gni = std::make_shared<identifier_name_item>(identifier);
#else
            gni = std::make_shared<identifier_name_item>(our_dictionary.get_identifier_by_index(index));
#endif
        }
        
        virtual bool get_is_identifier() const override { return true; }
        
        virtual bool get_match_identifier(const std::string & v, const machine & m, const basic_generator & g) const override
        {
#ifdef CHOMIK_DONT_USE_OPTIMIZATIONS
            return identifier==v;
#else
            return our_dictionary.get_identifier_by_index(index)==v;
#endif
        }
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
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
                
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<placeholder_name_item>(placeholder, *type_name);
        }
        
        virtual bool get_is_placeholder() const override { return true; }
        
        virtual std::string get_placeholder_name() const { return placeholder; }
        
        generic_type & get_type() const { return *type_name; }
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

        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<variable_value_name_item>(*name);
        }        

        virtual bool get_match_integer(int v, const machine & m, const basic_generator & g) const override;
        virtual bool get_match_float(double v, const machine & m, const basic_generator & g) const override;
        virtual bool get_match_string(const std::string & v, const machine & m, const basic_generator & g) const override;
        virtual bool get_match_identifier(const std::string & v, const machine & m, const basic_generator & g) const override;
        virtual bool get_match_code(const code & v, const machine & m, const basic_generator & g) const override;

        virtual bool get_is_variable_value() const override { return true; }
    };

    class code_name_item: public generic_name_item
    {
    private:
        std::unique_ptr<code> my_code;
    public:
        code_name_item(): my_code{std::make_unique<code>()} {}
        code_name_item(const code & c);

        virtual void report(std::ostream & s) const override;

        virtual bool get_is_code() const override { return true; }

        virtual bool get_match_code(const code & v, const machine & m, const basic_generator & g) const override;

        virtual void add_placeholders_to_generator(basic_generator & g) const override;

        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override;

        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;

        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;

        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<code_name_item>(*my_code);
        }
    };



    
    /**
     * These class contains constant objects and for the optimization purpose their creation has been moved to the class constructor.
     */
    class signature_common_data
    {
    private:
        friend class signature;
        generic_name        generic_name_the_print_target_stream_index,
                            generic_name_the_print_separator,
                            generic_name_the_print_end_of_line,
                            generic_name_the_created_stream_index,
                            generic_name_the_set_to_stream_stream_index,
                            generic_name_the_get_from_stream_stream_index,
                            generic_name_the_get_from_stream_result,
                            generic_name_the_read_from_stream_source_stream_index,
                            generic_name_the_read_from_stream_result_integer,
                            generic_name_the_read_from_stream_result_string,
                            generic_name_the_read_from_stream_max_size,
                            generic_name_the_compare_result,
                            generic_name_the_multiply_result_integer,
                            generic_name_the_divide_result_float,
                            generic_name_the_add_result_integer,
                            generic_name_the_subtract_result_integer,
                            generic_name_the_get_is_defined_result,
                            generic_name_the_get_amount_of_ad_hoc_types_result,
                            generic_name_the_get_amount_of_variables_in_the_memory_result,
                            generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result,
                            generic_name_the_get_signature_item_representation_result,
                            generic_name_the_get_signature_item_types_name_result,
                            generic_name_the_getline_stream_index,
                            generic_name_the_getline_result,
                            generic_name_the_created_signature_regular_expression_index,
                            generic_name_the_match_expression_index,
                            generic_name_the_match_result,
                            generic_name_the_modulo_result_integer,
                            generic_name_the_stream_is_good,
                            generic_name_the_cast_result_integer,
                            generic_name_the_multiply_result_float,
                            generic_name_the_cast_result_float,
                            generic_name_the_add_result_float,
                            generic_name_the_subtract_result_float;
                            
        std::unique_ptr<signature>  signature_the_print_target_stream_index,
                                    signature_the_print_separator,
                                    signature_the_print_end_of_line,
                                    signature_the_created_stream_index,
                                    signature_the_set_to_stream_stream_index,
                                    signature_the_get_from_stream_stream_index,
                                    signature_the_get_from_stream_result,
                                    signature_the_read_from_stream_source_stream_index,
                                    signature_the_read_from_stream_result_integer,
                                    signature_the_read_from_stream_result_string,
                                    signature_the_read_from_stream_max_size,
                                    signature_the_compare_result,
                                    signature_the_multiply_result_integer,
                                    signature_the_divide_result_float,
                                    signature_the_add_result_integer,
                                    signature_the_subtract_result_integer,
                                    signature_the_get_is_defined_result,
                                    signature_the_get_amount_of_ad_hoc_types_result,
                                    signature_the_get_amount_of_variables_in_the_memory_result,
                                    signature_the_get_amount_of_items_in_the_memory_variables_signature_result,
                                    signature_the_get_signature_item_representation_result,
                                    signature_the_get_signature_item_types_name_result,
                                    signature_the_getline_stream_index,
                                    signature_the_getline_result,
                                    signature_the_created_signature_regular_expression_index,
                                    signature_the_match_expression_index,
                                    signature_the_match_result,
                                    signature_the_modulo_result_integer,
                                    signature_the_stream_is_good,
                                    signature_the_cast_result_integer,
                                    signature_the_multiply_result_float,
                                    signature_the_cast_result_float,
                                    signature_the_add_result_float,
                                    signature_the_subtract_result_float;

        std::vector<std::unique_ptr<signature>> signature_the_match_group_integer_x;
        std::vector<std::unique_ptr<signature>> signature_the_match_group_boolean_x;

    public:
        signature_common_data();
        ~signature_common_data() {}
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
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override
        {
            std::stringstream s;
            s << my_value;
            return s.str();
        }
        
        virtual void add_placeholders_to_generator(basic_generator & g) const {}                

    };
    
    
    class name_item_integer: public simple_name_item<int>
    {
    public:
        name_item_integer(int v): simple_name_item<int>{v} {}
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override
        {
            target.add_content(std::make_shared<simple_value_integer_signature_item>(*this, my_value));
        }
        
        virtual void add_content_to_signature(signature & target) const override
        {
            target.add_content(std::make_shared<simple_value_integer_signature_item>(*this, my_value));
        }
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
            target.add_generic_name_item(std::make_shared<name_item_integer>(my_value));
        }
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<name_item_integer>(my_value);
        }                
        
        virtual bool get_is_integer() const override { return true; }
        
        virtual bool get_match_integer(int v, const machine & m, const basic_generator & g) const override { return my_value == v; }
    };


    class name_item_float: public simple_name_item<double>
    {
    public:
        name_item_float(double v): simple_name_item<double>{v} {}
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override
        {
            target.add_content(std::make_shared<simple_value_float_signature_item>(*this, my_value));
        }
        virtual void add_content_to_signature(signature & target) const override
        {
            target.add_content(std::make_shared<simple_value_float_signature_item>(*this, my_value));
        }
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
            target.add_generic_name_item(std::make_shared<name_item_float>(my_value));
        }
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<name_item_float>(my_value);
        }                
        
        virtual bool get_is_float() const override { return true; }
        
        virtual bool get_match_float(double v, const machine & m, const basic_generator & g) const override { return my_value == v; }  // comparing doubles for equality is questionable
    };
    
    class name_item_string: public simple_name_item<std::string>
    {
        public:
        name_item_string(std::string v): simple_name_item<std::string>{v} {}
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
            target.add_generic_name_item(std::make_shared<name_item_string>(my_value));
        }
        virtual void report(std::ostream & s) const override
        {
            s << '\"' << my_value << '\"';
        }
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override;
        
        virtual void add_content_to_signature(signature & target) const override
        {
            target.add_content(std::make_shared<simple_value_string_signature_item>(*this, my_value));
        }

        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override
        {
            gni = std::make_shared<name_item_string>(my_value);
        }        
        
        virtual bool get_is_string() const override { return true; }
        
        virtual bool get_match_string(const std::string & v, const machine & m, const basic_generator & g) const override { return my_value == v; }
    };
    
    
    class code;
    class list_of_statements;
    
    class name_item_code: public generic_name_item
    {
    private:
        std::unique_ptr<code> my_code_pointer;
    public:
        /**
         * This constructor takes ownership of the list of statements. No need to destroy it!
         */
        name_item_code(list_of_statements * const l): my_code_pointer{std::make_unique<code>(l)} {}

        name_item_code(const code & cp);

        name_item_code(): my_code_pointer{std::make_unique<code>()} {}
        
        virtual void report(std::ostream & s) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void add_content_to_signature(signature & target, const machine & m, const basic_generator & g) const override;

        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::shared_ptr<generic_name_item> & gni) const override;
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
        
        void add_placeholders_to_generator(basic_generator & g) const;
        
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
        
        virtual void add_placeholders_to_generator(basic_generator & g) const = 0;
        
        virtual bool get_is_range() const = 0;
        
        virtual int get_min_value(machine & m, basic_generator & g) const { return 0; }
        
        virtual int get_max_value(machine & m, basic_generator & g) const { return 0; }
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
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override
        {
            r->add_placeholders_to_generator(g);
        }
        
        virtual bool get_is_range() const override 
        {
            return true;
        }
        virtual int get_min_value(machine & m, basic_generator & g) const override { return r->get_min_value(m, g); }
        
        virtual int get_max_value(machine & m, basic_generator & g) const override { return r->get_max_value(m, g); }
    };
    
    class list_of_generic_names;
    
    class mapping_generator_placeholder
    {
    private:
        const std::string placeholder_name_in_father, placeholder_name_in_son;
    public:
        mapping_generator_placeholder(const std::string & fn, const std::string sn): placeholder_name_in_father{fn}, placeholder_name_in_son{sn} {}
    };
    
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
    
    
    template <typename TYPE, int REPRESENTATION_TYPE> class simple_placeholder_with_value;
    template <typename TYPE, int REPRESENTATION_TYPE> class simple_placeholder_with_value_and_report;

    class description_of_a_cartesian_product;
    
    /**
     * This is a base class for the generators. There are currently three types of generators:
     * - the mapping generators
     * - the generators (regular ones)
     * - the external generators
     */
    class basic_generator
    {
    protected:
        std::weak_ptr<basic_generator> my_father; // this pointer is used when the generators are nested        
        
        static simple_placeholder_with_value_and_report<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)> dummy;
        
    public:
        virtual ~basic_generator()
        {
        }
        
        void set_father(std::shared_ptr<basic_generator> f) { my_father = f; }
        
        virtual const std::string convert_childs_placeholder_name_to_father(const std::string & cpn) const { return cpn; }
        
        virtual bool get_has_placeholder_with_value(const std::string & p) const { return false; }

        virtual placeholder_with_value& get_placeholder_with_value(const std::string & p);

        virtual const placeholder_with_value& get_placeholder_with_value(const std::string & p) const;
        
        virtual void report(std::ostream & s) const {};
        
        virtual void add_placeholder(const std::string & p, std::shared_ptr<generic_type> && t) {}
        
        virtual bool get_can_have_placeholders() const { return false; }
                
        virtual int get_placeholder_value_integer(const std::string & p) const;
        
        virtual double get_placeholder_value_float(const std::string & p) const;
        
        std::string get_placeholder_value_string(const std::string & p) const;
        
        std::string get_placeholder_value_enum(const std::string & p) const;

        virtual void get_placeholder_value_code(const std::string & p, code & target) const;
        
        virtual bool get_the_cartesian_product_of_placeholder_types_is_finite() const;
        
        virtual bool get_the_cartesian_product_of_placeholder_types_is_small(machine & m) const;
        
        virtual bool get_the_cartesian_product_of_placeholder_types_is_empty() const;
        
        virtual bool get_the_cartesian_product_of_placeholder_types_has_one_item() const;
        
        virtual void initialize(machine & m) = 0;
        
        virtual bool get_is_valid() { return true; }
        
        virtual void increment(machine & m) = 0;
        
        virtual bool get_terminated() const  { return true; }
        
        virtual std::string get_actual_text_representation_of_a_placeholder(const std::string & placeholder) const { return ""; }
        
        virtual bool get_has_placeholder(const std::string & p) const { return false; }
                                
        virtual void initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const = 0;
        
        virtual void add_mapping(const std::string & f, const std::string & s) {}
        
        virtual void initialize_mapping(const matching_protocol & mp) {}

        virtual void debug() const {}

        virtual void finalize(machine & m) {}

    };
    
    /**
     * This is a mapping generator - it may redefine the father's placeholder names.
     */    
    class mapping_generator: public basic_generator
    {
    private:        
        std::map<std::string, std::string> map_child_placeholder_to_father_placeholder;

        const std::string my_filename;
        const unsigned line_number;
        
    public:
        mapping_generator(const std::string & filename, unsigned new_line_number): my_filename{filename}, line_number{new_line_number} {}
                        
        virtual const std::string convert_childs_placeholder_name_to_father(const std::string & cpn) const override;
        
        virtual void report(std::ostream & s) const override;
        
        virtual bool get_has_placeholder_with_value(const std::string & p) const override;
        
        virtual placeholder_with_value& get_placeholder_with_value(const std::string & p) override;

        virtual const placeholder_with_value& get_placeholder_with_value(const std::string & p) const override;        
        
        virtual void initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const override;
        
        virtual void initialize(machine & m) override;
        
        virtual void increment(machine & m) override;
        
        virtual void add_mapping(const std::string & f, const std::string & s) 
        { 
            auto [it, success] = map_child_placeholder_to_father_placeholder.insert(std::pair(s, f));
            if (!success)
            {
                throw std::runtime_error("failed to add mapping to a mapping generator");
            }
        }
        
        void clear_mappings() { map_child_placeholder_to_father_placeholder.clear(); }
        
        virtual void initialize_mapping(const matching_protocol & mp) override;


    };


    template <typename GENERATOR_TYPE> class machine_finalization_guard
    {
    private:
        GENERATOR_TYPE & my_generator;
        machine & my_machine;
    public:
        machine_finalization_guard(machine & m, GENERATOR_TYPE & p): my_machine{m}, my_generator{p} {}
        ~machine_finalization_guard() { my_generator.finalize(my_machine); }
    };
    
    /**
     * This class creates all possible tuples of a finite cartesian product of the given sets.
     * It can be used almost as an iterator.
     */
    class generator: public basic_generator
    {
    private:
        std::vector<std::shared_ptr<generator_placeholder>> vector_of_placeholders;
        std::map<std::string, std::shared_ptr<generator_placeholder>> map_placeholder_names_to_placeholders;
        
        std::vector<std::shared_ptr<placeholder_with_value>> memory;
        std::map<std::string, std::shared_ptr<placeholder_with_value>> map_placeholder_names_to_placeholders_with_value;
        
        int initial_amount_of_ad_hoc_type_instances; // we need to remember this value, it will be necessary to clean the ad hoc types afterwards
        
        const std::string my_filename;
        const unsigned line_number;
        
    public:
        generator(const generic_name & gn, const std::string & filename, unsigned new_line_number);
        generator(const generic_range & gr, const std::string & filename, unsigned new_line_number);
        generator(const generic_value & gv, const std::string & filename, unsigned new_line_number);
        generator(const std::string & filename, unsigned new_line_number);

        virtual void initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const override;
        
        virtual bool get_can_have_placeholders() const { return true; }
        
        virtual bool get_has_placeholder(const std::string & p) const override;
        
        virtual bool get_has_placeholder_with_value(const std::string & p) const override;
        
        virtual placeholder_with_value& get_placeholder_with_value(const std::string & p) override;

        virtual const placeholder_with_value& get_placeholder_with_value(const std::string & p) const override;
        
        virtual int get_placeholder_value_integer(const std::string & p) const override;

        virtual double get_placeholder_value_float(const std::string & p) const override;

        virtual void get_placeholder_value_code(const std::string & p, code & target) const override;
        
        virtual std::string get_actual_text_representation_of_a_placeholder(const std::string & placeholder) const override
        {
            return "unknown_placeholder";
        }
                
        virtual void add_placeholder(const std::string & p, std::shared_ptr<generic_type> && t) override;
        
        void add_placeholder_with_value(std::shared_ptr<placeholder_with_value> && p)
        {
            std::shared_ptr<placeholder_with_value> p2{p};
            auto [it, success] = map_placeholder_names_to_placeholders_with_value.insert(std::pair(p->get_name(), std::move(p2)));
            if (!success)
            {
                throw std::runtime_error("failed to insert a placeholder with value");
            }
            
            memory.push_back(std::move(p));
        }
        
        virtual void report(std::ostream & s) const override;
        
        virtual bool get_the_cartesian_product_of_placeholder_types_is_finite() const override;

        virtual bool get_the_cartesian_product_of_placeholder_types_is_small(machine & m) const override;
        
        // TODO - if there is at least one empty type then the whole cartesian product is empty
        virtual bool get_the_cartesian_product_of_placeholder_types_is_empty() const override { return false; }
        
        // TODO - it is also possible for some types like range 1..1
        virtual bool get_the_cartesian_product_of_placeholder_types_has_one_item() const override;
        
        virtual void initialize(machine & m) override;
        
        virtual bool get_is_valid() override;
        
        virtual void increment(machine & m) override;
        
        virtual bool get_terminated() const override;
        
        bool get_does_not_exceed_level(int max_level) const;

        virtual void debug() const override;

        virtual void finalize(machine & m) override;
    };
    
    /**
     * This is a kind of generator used when the external placeholders are involved.
     * It is not really iterable.
     */
    class external_placeholder_generator: public basic_generator
    {
    private:
        std::vector<std::shared_ptr<placeholder_with_value>> memory;
        std::map<std::string, std::shared_ptr<placeholder_with_value>> map_placeholder_names_to_placeholders_with_value;
        
        const std::string my_filename;
        const unsigned line_number;
        
    public:
        external_placeholder_generator(const std::string & filename, unsigned new_line_number);
        
        virtual void report(std::ostream & s) const override;        
        
        virtual void initialize(machine & m) override;
        
        virtual void increment(machine & m) override;
        
        virtual void initialize_description_of_a_cartesian_product(description_of_a_cartesian_product & target) const override;
        
        virtual void initialize_mapping(const matching_protocol & mp) override;
        
        virtual bool get_has_placeholder_with_value(const std::string & p) const override;

        virtual placeholder_with_value& get_placeholder_with_value(const std::string & p) override;

        virtual const placeholder_with_value& get_placeholder_with_value(const std::string & p) const override;
        
        void clear_mappings();
        
        void add_placeholder_with_value(std::shared_ptr<placeholder_with_value> && p);
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
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
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

        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const override;                
        
        virtual void expand(machine & m, int depth) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
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
        
        void add_placeholders_to_generator(basic_generator & g) const
        {
            name->add_placeholders_to_generator(g);
            type_name->add_placeholders_to_generator(g);
        }

        bool operator==(const variable_definition & d) const;
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
        
        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const override;        
        
        virtual void expand(machine & m, int depth) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override
        {
            // TODO - implement
        }        

        bool operator==(const variable_definition_statement & s) const;
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
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const = 0;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const = 0;
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const { return 0; }
        
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const { return 0.0; }
        
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const { return ""; }
        
        virtual std::string get_actual_enum_value(const machine & m, const basic_generator & g) const { return ""; }
        
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const {}
        
        virtual void add_placeholders_to_generator(basic_generator & g) const = 0;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const = 0;
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const = 0;
        
        virtual void get_copy(std::unique_ptr<generic_value> & target) const = 0;
        
        virtual void get_literal_copy(std::unique_ptr<generic_literal> & target) const {}
        
        virtual bool get_is_code_with_placeholders(machine & m, basic_generator & g) const { return false; }
        
        virtual bool get_is_literal() const { return false; }

        virtual bool get_is_code(machine & m) const { return false; }
                
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
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override
        {
            /*
            std::cout << "generic_value_variable_value - adding placeholders from ";
            name->report(std::cout);
            std::cout << " to the generator\n";
            */
            name->add_placeholders_to_generator(g);
        }
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const override;
        
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const override;
        
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const override;
        
        virtual std::string get_actual_enum_value(const machine & m, const basic_generator & g) const override;
        
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const override;
        
        virtual void get_copy(std::unique_ptr<generic_value> & target) const override
        {
            target = std::make_unique<generic_value_variable_value>(*name);
        }
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const override;
                

        virtual bool get_is_code(machine & m) const override;
    };
    
    
    /**
     * This class represents the literals of all built-in types and user defined enums.
     */
    class generic_literal
    {
    public:
        virtual ~generic_literal() {}
        virtual void report(std::ostream & s) const = 0;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const = 0;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const = 0;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const {}
        
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const {}

        virtual std::string get_actual_enum_value(const machine & m, const basic_generator & g) const { return ""; }
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const { return 0; }
        
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const { return 0.0; }
        
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const { return ""; }
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const = 0;
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const = 0;
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const = 0;

        virtual bool get_is_code() const { return false; }
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
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override { return expected_type; }
        
        virtual void report(std::ostream & s) const override;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const override;
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const override;

        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const override;

        virtual void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const override;
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            std::unique_ptr<generic_type> t;
            type_name->get_copy(t);
            target = std::make_unique<generic_literal_placeholder>(placeholder, std::move(t), expected_type);
        }
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override;
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
        
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override
        {
            return literal->get_actual_memory_representation_type(m, g);
        }
        virtual void report(std::ostream & s) const override
        {
            s << "literal ";
            literal->report(s);
        }        
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override
        {
            return literal->get_actual_text_representation(m, g);            
        }
        virtual void add_placeholders_to_generator(basic_generator & g) const override
        {
            literal->add_placeholders_to_generator(g);
        }
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
            literal->get_result_replacing_placeholders(m, g, p, target);
        }
        
        virtual std::string get_actual_enum_value(const machine & m, const basic_generator & g) const override { return literal->get_actual_enum_value(m, g); }
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const override { return literal->get_actual_integer_value(m, g); }
        
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const override { return literal->get_actual_float_value(m, g); }
        
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const override;
                
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const override;

        virtual void get_copy(std::unique_ptr<generic_value> & target) const override;

        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const override;
        
        virtual bool get_is_literal() const override { return true; }
        
        virtual void get_literal_copy(std::unique_ptr<generic_literal> & target) const 
        {
            literal->get_copy(target);
        }

        virtual bool get_is_code(machine & m) const override { return literal->get_is_code(); }
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
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override
        {
            return static_cast<variable_with_value::actual_memory_representation_type>(REPRESENTATION_TYPE);
        }
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override
        {
            std::stringstream s;
            s << my_value;
            return s.str();
        }
        virtual void add_placeholders_to_generator(basic_generator & g) const override {}
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
            // left empty intentionally
        }        

    };   
    
    class simple_literal_integer: public simple_literal<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    public:
        simple_literal_integer(int v): simple_literal<int, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::INTEGER)>(v) {}
        virtual int get_value_integer() const { return my_value; }        
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const override { return my_value; }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            target = std::make_unique<simple_literal_integer>(my_value);
        }                
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
        {
            get_copy(target);
        }
    };
    
    class simple_literal_float: public simple_literal<double, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::FLOAT)>
    {
    public:
        simple_literal_float(double v): simple_literal<double, static_cast<int>(chomik::variable_with_value::actual_memory_representation_type::FLOAT)>(v) {}
        virtual double get_value_float() const { return my_value; }
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const override { return my_value; }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            target = std::make_unique<simple_literal_float>(my_value);
        }
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
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
        
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const override { return my_value; }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const
        {
            target = std::make_unique<simple_literal_string>(my_value);
        }

        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
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

        generic_literal_code(std::unique_ptr<code> && c): my_code_pointer{std::move(c)} {}

        
        code & get_code() { return *my_code_pointer; }
        
        virtual void report(std::ostream & s) const override;
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override 
        { 
            return variable_with_value::actual_memory_representation_type::CODE;
        }
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const override;
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const
        {
            target = std::make_unique<generic_literal_code>(*my_code_pointer);
        }

        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override;

        virtual bool get_is_code() const override { return true; }
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

        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override 
        { 
            return variable_with_value::actual_memory_representation_type::ENUM;
        }        
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override
        {
            return name->get_actual_text_representation(m, g);
        }
        virtual void add_placeholders_to_generator(basic_generator & g) const override
        {
            name->add_placeholders_to_generator(g);
        }
        
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override
        {
        }        
        
        virtual std::string get_actual_enum_value(const machine & m, const basic_generator & g) const override { return name->get_actual_text_representation(m, g); }
        
        virtual void get_copy(std::unique_ptr<generic_literal> & target) const override
        {
            target = std::make_unique<generic_literal_enum>(type_name, *name);
        }        

        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::unique_ptr<generic_literal> & target) const override
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
        
        virtual variable_with_value::actual_memory_representation_type get_actual_memory_representation_type(machine & m, basic_generator & g) const override;
        
        virtual std::string get_actual_text_representation(const machine & m, const basic_generator & g) const override;
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override
        {
            std::shared_ptr<generic_type> t{type_name};
            g.add_placeholder(placeholder, std::move(t));
            type_name->add_placeholders_to_generator(g);
        }
        virtual void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const override;
                
        virtual void get_copy(std::unique_ptr<generic_value> & target) const override
        {
            std::shared_ptr<generic_type> i;
            type_name->get_copy(i);
            target = std::make_unique<generic_value_placeholder>(placeholder, std::move(i));
        }
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<generic_value> & target) const override;
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
                
        void execute_if_cartesian_product_has_one_item(machine & m, std::shared_ptr<basic_generator> & g) const;        
        
        void execute_if_cartesian_product_is_finite_and_small(machine & m, std::shared_ptr<basic_generator> & g) const;
        
        void execute_if_cartesian_product_is_large_or_infinite(machine & m, std::shared_ptr<basic_generator> & g) const;
        
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

        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const override;        

        virtual void add_placeholders_to_generator(basic_generator & g) const override
        {
            name->add_placeholders_to_generator(g);
            // we do not do value->add_placeholders_to_generator(g); !!! this is intentional!
        }
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
        virtual void get_copy(std::shared_ptr<statement> & target) const override;

        virtual bool get_is_code_assignment(machine & m) const override { return value->get_is_code(m); }
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
        
        void execute_if_cartesian_product_has_one_item(machine & m, basic_generator & g) const;
        
        void execute_if_cartesian_product_is_finite_and_small(machine & m, basic_generator & g) const;
        
        void execute_if_cartesian_product_is_large_or_infinite(machine & m, basic_generator & g) const;
        
    public:                
        execute_value_statement(std::unique_ptr<generic_value> && v, unsigned new_line_number): value{std::move(v)}, execute_statement{new_line_number} {}
        
        //execute_value_statement(unsigned new_line_number): value{std::make_unique<generic_value_literal>()}, execute_statement{new_line_number} {}
        
        virtual statement_type get_statement_type() const override { return statement_type::EXECUTE; }        

        virtual void report(std::ostream & s) const override;
                
        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const override;                
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;

        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
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
        
        void execute_if_cartesian_product_has_one_item(machine & m, std::shared_ptr<basic_generator> & g) const;
        
        void execute_if_cartesian_product_is_finite_and_small(machine & m, std::shared_ptr<basic_generator> & g) const;
        
        void execute_if_cartesian_product_is_large_or_infinite(machine & m, std::shared_ptr<basic_generator> & g) const;
        
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
        
        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const override;        
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override;
        
        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
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

        virtual void execute(machine & m, std::shared_ptr<const statement> && i, std::shared_ptr<basic_generator> father=nullptr) const override;        
        
        virtual void add_placeholders_to_generator(basic_generator & g) const override {}

        virtual void make_copy_with_replacements(const machine & m, const basic_generator & g, const replacing_policy & p, std::shared_ptr<statement> & target) const override;
        
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
        list_of_statements(statement * const s, list_of_statements * const l);
        list_of_statements(const list_of_statements & s);

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
        
        void add_placeholders_to_generator(basic_generator & g) const;
        
        void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const;

        bool operator==(const list_of_statements & l) const;
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
        std::string name;
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
        
        virtual int get_amount_of_values() const = 0;
        
        virtual std::string get_enum_item(int i) { return ""; }

        virtual void update(placeholder_with_value & v, machine & m, basic_generator & g) {}

        void update_name(const std::string & n) { name = n; }
    };
    


    template <typename TYPE, int REPRESENTATION_TYPE> class simple_placeholder_with_value: public placeholder_with_value
    {
    protected:
        TYPE value;
        type_instance * my_type_instance;
    public:
        simple_placeholder_with_value(const std::string & p, const TYPE v, type_instance * ti):
            placeholder_with_value{p}, value{v}, my_type_instance{ti} {}

        virtual variable_with_value::actual_memory_representation_type get_representation_type() const override
        {
            return static_cast<variable_with_value::actual_memory_representation_type>(REPRESENTATION_TYPE);
        }

        TYPE get_value() const { return value; }

        virtual void update_int_value(TYPE f, TYPE l) { value = f; }


        virtual void update_ad_hoc_range_type_instance(machine & m, basic_generator & g) override
        {
            // if the placeholder type instance is an ad hoc range
            // then update its type instance
            if (my_type_instance) my_type_instance->update(*this, m, g);
        }
    };

    template <typename TYPE, int REPRESENTATION_TYPE> class simple_placeholder_with_value_and_report: public simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>
    {
    public:
        simple_placeholder_with_value_and_report(const std::string & p, const TYPE v, type_instance * ti): simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>{p, v, ti} {}

        virtual void report(std::ostream & s) const override
        {
            auto & x=simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>::value;

            s << simple_placeholder_with_value<TYPE, REPRESENTATION_TYPE>::placeholder << '=';
            //x,report(s);
            s << ' ';
        }
    };


    class type_instance_enum_value
    {
    private:
        const std::string name;
        const unsigned level;
    public:
        type_instance_enum_value(const std::string & n, unsigned new_level): name{n}, level{new_level} {}
        
        const std::string & get_name() const { return name; }
        
        unsigned get_level() const { return level; }
    };
    
    class type_instance_enum: public type_instance
    {
    private:
        std::vector<std::unique_ptr<type_instance_enum_value>> vector_of_values;
    public:
        type_instance_enum(const std::string & n): type_instance{n} {}
        
        virtual void add_type_instance_enum_value(const signature & n, unsigned new_level) override;
        
        virtual void add_type_instance_enum_value(const std::string & n, unsigned int new_level) override;
        
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
        
        virtual int get_amount_of_values() const override { return vector_of_values.size(); }
        
        virtual std::string get_enum_item(int i) { return vector_of_values[i]->get_name(); }
    };
    
    class type_instance_range: public type_instance
    {
    protected:
        int min_boundary, max_boundary;
    public:
        type_instance_range(const std::string & n, int b1, int b2): type_instance{n}, min_boundary{b1}, max_boundary{b2} {}
        
        virtual void report(std::ostream & s) const override;
        
        virtual int get_first_iterator_for_range() const override { return min_boundary; }
        
        virtual int get_last_iterator_for_range() const override { return max_boundary; }
        
        virtual type_instance_mode get_mode() const { return type_instance_mode::INTEGER; }
        
        virtual int get_amount_of_values() const override { return max_boundary - min_boundary + 1; }

    };


    class type_instance_ad_hoc_range: public type_instance_range
    {
    private:
        const generic_type & my_type;

    public:
        type_instance_ad_hoc_range(const generic_type & t, const std::string & n, int b1, int b2):
            my_type{t}, type_instance_range{n, b1, b2}
            {
            }

        virtual void update(placeholder_with_value & v, machine & m, basic_generator & g) override;

        virtual void report(std::ostream & s) const override;
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
        description_of_a_cartesian_product(const basic_generator & g);
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
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const { return 0; }
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const { return 0.0; }
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const { return ""; }
        virtual std::string get_actual_identifier_value(const machine & m, const basic_generator & g) const { return ""; }
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, code & target) const {}
        
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
        
        virtual int get_actual_integer_value(const machine & m, const basic_generator & g) const override { return my_value->get_actual_integer_value(m, g); }
        virtual double get_actual_float_value(const machine & m, const basic_generator & g) const override { return my_value->get_actual_float_value(m, g); }
        virtual std::string get_actual_string_value(const machine & m, const basic_generator & g) const override { return my_value->get_actual_string_value(m, g); }
        virtual std::string get_actual_identifier_value(const machine & m, const basic_generator & g) const override { return my_value->get_actual_enum_value(m, g); }
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, code & target) const;
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
                
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, code & target) const override;
    };
    
    class assignment_source_variable_value: public assignment_source
    {
    private:
        std::unique_ptr<generic_value> my_value;
    public:
        assignment_source_variable_value(std::unique_ptr<generic_value> && v): my_value{std::move(v)} {}
        virtual void report(std::ostream & s) const override;
        
        
        virtual void get_actual_code_value(const machine & m, const basic_generator & g, code & target) const override;
        
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
        bool get_match(const signature & s, const machine & m, const basic_generator & g, matching_protocol & target) const;
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

        virtual std::string get_debug_type_name() const override { return "simple_variable_with_value<>"; }
    };
    
    class simple_variable_with_value_integer: public simple_variable_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    public:
        simple_variable_with_value_integer(std::shared_ptr<signature> && s, int v): simple_variable_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>(std::move(s), v) {}
        
        virtual int get_value_integer() const override { return value; }
        
        virtual void assign_value_integer(int v) override { value = v; }

        virtual std::string get_debug_type_name() const override { return "simple_variable_with_value_integer"; }
        
    };
    
    class simple_variable_with_value_float: public simple_variable_with_value<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>
    {
    public:
        simple_variable_with_value_float(std::shared_ptr<signature> && s, double v): simple_variable_with_value<double, static_cast<int>(variable_with_value::actual_memory_representation_type::FLOAT)>(std::move(s), v) {}        
        
        virtual double get_value_float() const override { return value; }
        
        virtual void assign_value_float(double v) override { value = v; }

        virtual std::string get_debug_type_name() const override { return "simple_variable_with_value_float"; }
    };

    class simple_variable_with_value_string: public simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>
    {
    public:
        simple_variable_with_value_string(std::shared_ptr<signature> && s, std::string v): simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::STRING)>(std::move(s), v) {}        

        virtual std::string get_value_string() const override { return value; }        
        
        virtual void assign_value_string(std::string v) override { value = v; }

        virtual std::string get_debug_type_name() const override { return "simple_variable_with_value_string"; }
    };
    
    class simple_variable_with_value_enum: public simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>
    {
    public:
        simple_variable_with_value_enum(std::shared_ptr<signature> && s, std::string v): simple_variable_with_value<std::string, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>(std::move(s), v) {}        
        
        virtual std::string get_value_enum() const override { return value; }
        
        virtual void assign_value_enum(std::string v) override { value = v; }

        virtual std::string get_debug_type_name() const override { return "simple_variable_with_value_enum"; }
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

        virtual std::string get_debug_type_name() const override { return "simple_variable_with_value_code"; }
    };
    
    
    class simple_placeholder_for_enum: public simple_placeholder_with_value<std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>
    {
    private:
        using iterator_type = std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator;
        const iterator_type first, last;
    public:
        simple_placeholder_for_enum(const std::string & p, const iterator_type f, const iterator_type l, type_instance * ti):
            simple_placeholder_with_value<std::vector<std::unique_ptr<type_instance_enum_value>>::const_iterator, static_cast<int>(variable_with_value::actual_memory_representation_type::ENUM)>
            {p, f, ti}, first{f}, last{l} {}
            
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
        
        virtual bool get_exceeds_level(int max_level) const
        {
            return (*value)->get_level() > max_level;
        }
        
        virtual int get_level() const { return (*value)->get_level(); }
    };
    
    
    class simple_placeholder_for_infinite_range: public simple_placeholder_with_value<int, 
        static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    public:
        simple_placeholder_for_infinite_range(const std::string & p, type_instance * ti): simple_placeholder_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>{p, 0, ti} {}
        
        virtual void report(std::ostream & s) const override
        {
            s << placeholder << '=' << value << "\n";
        }
        virtual bool get_is_valid() const override
        {
            return true;
        }
        virtual bool get_is_terminated() const override
        {
            return false;
        }        
        virtual void increment() override { value++; }
        
    };
    
    
    class simple_placeholder_for_range: public simple_placeholder_with_value<int,
        static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>
    {
    private:
        int first, last;

    public:
        simple_placeholder_for_range(const std::string & p, const int f, const int l, type_instance * ti): simple_placeholder_with_value<int, static_cast<int>(variable_with_value::actual_memory_representation_type::INTEGER)>{p, f, ti}, first{f}, last{l+1} {}
        
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


        virtual void update_int_value(int f, int l) override;

    };
    
    /**
     * The base class of all streams used by a chomik program.
     */
    class generic_stream
    {
    protected:
        int max_size; // for reading
        
        /**
         * 
         */
        void read_string_of_x_characters(std::string & target, unsigned x);
        
    public:
        generic_stream(): max_size{0} {}
        void set_max_size(int ms) { max_size = ms; }                
        virtual ~generic_stream() {}
        virtual bool get_allows_input() const { return false; }
        virtual bool get_allows_output() const { return false; }
        virtual bool get_should_be_opened() const { return false; }
        virtual bool get_should_be_closed() const { return false; }
        virtual std::ostream& get_output_stream() { return std::cout; }
        virtual std::istream& get_input_stream() { return std::cin; }        
        virtual std::string get_result() const { return ""; }
        virtual std::string getline_result() const { return ""; }
        virtual void set_result(const std::string & r) {}
        virtual int read_integer() { return 0; }
        virtual bool read_char(unsigned char & target) { return false; }
        virtual std::string read_string() { std::string t; read_string_of_x_characters(t, max_size); return t; }
        virtual void getline() {}

        virtual bool get_is_good() { return true; }
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
    private:
        std::string my_getline_result;
    public:
        virtual bool get_allows_input() const override { return true; }
        virtual int read_integer() override { int n; std::cin >> std::setw(max_size) >> n; return n; }
        virtual bool read_char(unsigned char & target) override { target = std::cin.get(); return std::cin.operator bool(); }

        virtual void getline() override { std::getline(std::cin, my_getline_result); } // TODO error handling
        virtual std::string getline_result() const override { return my_getline_result; }

        virtual bool get_is_good() override { return std::cin.good(); }
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

        virtual bool get_is_good() override { return file_stream.good(); }
    };
    
    class generic_stream_file_input: public generic_stream_file
    {
    private:
        std::ifstream file_stream;
        std::string my_getline_result;
    public:
        generic_stream_file_input(const std::string & file_name): file_stream{file_name} {}

        virtual bool get_allows_input() const { return true; }
        virtual bool get_should_be_opened() const override { return true; }        
        virtual bool get_should_be_closed() const override { return true; }
        virtual std::istream& get_input_stream() override { return file_stream; }
        
        virtual bool read_char(unsigned char & target) override { target = file_stream.get(); return file_stream.operator bool(); }
        virtual void getline() override { std::getline(file_stream, my_getline_result); } // TODO error handling
        virtual std::string getline_result() const override { return my_getline_result; }

        virtual int read_integer() override { int result = 0; file_stream >> result; return result; }

        virtual bool get_is_good() override { return file_stream.good(); }
    };    
    
    class generic_stream_stringstream: public generic_stream
    {
    private:
        std::stringstream string_stream;
        std::string my_getline_result;
    public:
        generic_stream_stringstream() {}
        generic_stream_stringstream(const std::string & s): string_stream{s} {}
        
        virtual bool get_allows_input() const { return true; }
        virtual bool get_allows_output() const { return true; }
        virtual std::ostream& get_output_stream() override { return string_stream; }
        virtual std::istream& get_input_stream() override { return string_stream; }
        
        virtual std::string get_result() const override { return string_stream.str(); }
        virtual void set_result(const std::string & r) override { string_stream.clear(); string_stream.str(r); }
                
        virtual int read_integer() override { int v; string_stream >> v; return v; }
        
        virtual bool read_char(unsigned char & target) override { target = string_stream.get(); return string_stream.operator bool(); }        

        virtual void getline() override { std::getline(string_stream, my_getline_result); } // TODO error handling
        virtual std::string getline_result() const override { return my_getline_result; }

        virtual bool get_is_good() override { return string_stream.good(); }
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
        virtual bool get_allows_input() const override { return true; }
        virtual int read_integer() override;
    };
    
    class generic_stream_random_enum_stream: public generic_stream_random_number_stream
    {
    private:
        const std::string type_name;
        machine & my_machine;
    public:
        generic_stream_random_enum_stream(const std::string & n, machine & m);
        virtual bool get_allows_input() const override { return true; }
        virtual std::string read_string() override;
    };
    
    
    /**
     * This class represents the memory used by the program while executing it.
     */
    class machine
    {
    private:
        friend class signature_common_data;
        friend class signature;
        static constexpr int max_match_group_index = 10;

    protected:
        std::vector<std::shared_ptr<const statement>> vector_of_type_definiton_statements;
        std::vector<std::shared_ptr<const statement>> vector_of_variable_definition_statements;
        
        std::vector<std::shared_ptr<type_instance>> vector_of_type_instances, vector_of_ad_hoc_type_instances;
        std::map<std::string, std::shared_ptr<type_instance>> map_type_name_to_type_instance;
        
        std::vector<std::shared_ptr<family_of_variables>> vector_of_variable_families;
        
        std::vector<std::unique_ptr<assignment_event>> vector_of_assignment_events;
        
        std::vector<std::shared_ptr<variable_with_value>> memory;
                
        std::map<std::string, std::shared_ptr<variable_with_value>> map_signature_string_representation_to_variable_with_value;
        
        std::vector<std::unique_ptr<generic_stream>> vector_of_streams;

        std::vector<std::unique_ptr<signature_regular_expression>> vector_of_signature_regular_expressions;
                
    public:
        // these methods are useful for certain reflection-like capabilities
        int get_amount_of_variables_in_the_memory() const { return memory.size(); }
        int get_amount_of_items_in_the_memory_variables_signature(int i) const
        {
            if (i>=0 && i<memory.size()) return memory[i]->get_amount_of_signature_items();
            return 0;
        } // TODO - error handling
        std::string get_signature_item_representation(int var_index, int item_index) const;
        std::string get_signature_item_type_name(int var_index, int item_index) const;

        // The machine should be prevented to create files in sandbox environments
        virtual bool get_can_create_files() const { return true; }
        
        int get_max_enum_type_index(const std::string & tn) const;
        
        std::string get_enum_type_item(const std::string & tn, int i) const;
        
        const std::vector<std::unique_ptr<assignment_event>> & get_vector_of_assignment_events() const { return vector_of_assignment_events; }
        
        void add_signature_regular_expression(std::unique_ptr<signature_regular_expression> && e)
        {
            vector_of_signature_regular_expressions.push_back(std::move(e));
        }
        
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

        void get_first_and_last_iterators_for_ad_hoc_range_type(const std::string & type_name, int & f, int & l) const;

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
        
        /**
         * For each newly created type instance some special variables of that type are created.
         */
        void create_auxilliary_variables_for_type_instance(type_instance & ti);
        
        void add_variable_with_value(std::shared_ptr<variable_with_value> && vv);
        
        void expand(int i);
        
        void report(std::ostream & s) const;
        
        bool get_variable_is_represented_in_memory(const signature & vn) const;
        
        const variable_with_value & get_variable_with_value(const signature & vn) const;
        
        variable_with_value & get_variable_with_value(const signature & vn);

        variable_with_value & get_variable_with_value_by_index(int index);

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

        int get_last_created_signature_regular_expression_index() const { return vector_of_signature_regular_expressions.size()-1; } // because we only create signature regular expressions, we do not delete them (this may change!!!)
        
        int get_amount_of_streams() const { return vector_of_streams.size(); }

        int get_amount_of_signature_regular_expressions() const { return vector_of_signature_regular_expressions.size(); }
        
        generic_stream& get_stream(int i) { return *vector_of_streams[i]; }

        signature_regular_expression& get_signature_regular_expression(int i) { return *vector_of_signature_regular_expressions[i]; }

        type_instance* create_an_ad_hoc_type(const generic_type & t, generator & g, const std::string & tn);

        void add_ad_hoc_type(std::shared_ptr<type_instance_ad_hoc_range> && t);

        void destroy_ad_hoc_type_instances_above(int amount);

        int get_amount_of_ad_hoc_type_instances() const { return vector_of_ad_hoc_type_instances.size(); }

        static std::ostream *current_compilation_error_stream, *current_runtime_warning_stream;
    };
    
    /**
     * This class contains a list of statements, it represents a code and can be executed.
     * It is used as a built-in type code to represent its values.
     */
    class code
    {
    private:
        std::shared_ptr<list_of_statements> body;
        
        bool is_main; // this is used for pretty-printing only
        
    public:
        /**
         * The parameter should be destroyed by the parser!
         */
        code(list_of_statements * const l): body{std::make_shared<list_of_statements>(nullptr, l)}, is_main{false} 
        {
            body->set_is_main(is_main);
        }
        
        code(): body{std::make_shared<list_of_statements>(nullptr, nullptr)}, is_main{false}
        {
            body->set_is_main(is_main);
        }

        code(const code & c);
                        
        void execute(machine & m, std::shared_ptr<basic_generator> father=nullptr) const;
                        
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
        
        void add_placeholders_to_generator(basic_generator & g) const;
        
        void get_actual_code_value(const machine & m, const basic_generator & g, const replacing_policy & p, code & target) const;

        void add_content_to_signature(const generic_name_item & s, signature & target, const machine & m, const basic_generator & g) const;

        std::string get_actual_text_representation(const machine & m, const basic_generator & g) const;

        void get_result_replacing_placeholders(const machine & m, const basic_generator & g, const replacing_policy & p, generic_name & target) const;

        bool operator==(const code & c) const;
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
        
        void execute(machine & m) const;
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

        static parser * the_parser_pointer;

    public:
        static void register_parser(parser * p) { the_parser_pointer = p; }
        static parser * get_parser_pointer() { return the_parser_pointer; }

        parser(program & p);
                
        int parse(const char * filename);
        int parse_string(const std::string & code, std::ostream & error_stream);
                
        void copy_list_of_statements_to_the_program(list_of_statements * const l);
    };
    
}

std::ostream & operator<<(std::ostream & s, const chomik::generic_value & v);
std::ostream & operator<<(std::ostream & s, const chomik::code & c);
std::ostream & operator<<(std::ostream & s, const chomik::generic_name & n);
std::ostream & operator<<(std::ostream & s, const chomik::basic_generator & g);
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
std::ostream & operator<<(std::ostream & s, const chomik::placeholder_with_value & x);
std::ostream & operator<<(std::ostream & s, const chomik::matching_protocol & x);
std::ostream & operator<<(std::ostream & s, const chomik::generic_literal & l);
#endif
