
%define api.pure full

%parse-param {yyscan_t yyscanner} {void * extra_chomik_parameter}
%lex-param {yyscan_t yyscanner} {void *extra_chomik_parameter}

%locations

%{

#define YY_DECL int chomik_scannerlex (CHOMIK_PARSERSTYPE * chomik_scannerlval_param, CHOMIK_PARSERLTYPE * yyloc_param, yyscan_t yyscanner, void *extra_chomik_parameter)

#define yyparse chomik_parserparse

typedef void* yyscan_t;

int yyparse (yyscan_t yyscanner, void *extra_chomik_parameter);

#include <stdio.h>

int chomik_parserlex(void * yylval_param, void * yyloc_param, yyscan_t scanner, void *extra_chomik_parameter);
void chomik_parsererror(void * yyloc_param, yyscan_t scanner, void * chomik_extra_parameter, const char* msg);

int chomik_scannerlex(void * yylval_param, void * yyloc_param, yyscan_t scanner, void *extra_chomik_parameter);
void chomik_scannererror(void * yyloc_param, yyscan_t scanner, void * chomik_extra_parameter, const char* msg);

int chomik_get_current_yylineno(yyscan_t scanner, void * extra_chomik_parameter);

#define YYERROR_VERBOSE 1
#define YYMAXDEPTH 1000000


void * chomik_create_generic_type_named(const char * const type_name);
void * chomik_create_generic_type_list(void * const l);
void * chomik_create_generic_complex_name_type_named(void * const complex_type_name);
void * chomik_create_complex_name_generic_type_definition(void * const complex_type_name, void * t);
void * chomik_create_generic_range(void * const l, void * const r);
void * chomik_create_generic_range_boundary_int_literal(int i);
void * chomik_create_generic_range_boundary_variable_value(void * const gn);
void * chomik_create_generic_name(void * const l);
void * chomik_create_type_definition(const char * const type_name, void * type_definition_body);
void * chomik_create_list_of_type_definitions(void * const td, void * const l);
void * chomik_create_list_of_generic_names(void * const first_name, void * const other_names);
void * chomik_create_type_definition_body_enum(void * const l);
void * chomik_create_type_definition_statement(void * const l, unsigned new_line_number);
void * chomik_create_name_item_float(double d);
void * chomik_create_name_item_string(const char * const s);
void * chomik_create_name_item_int(int i);
void * chomik_create_name_item_variable_value(void * const l);
void * chomik_create_name_item_identifier(const char * const identifier, void * scanner, void *extra_chomik_parameter);
void * chomik_create_name_item_placeholder(const char * const placeholder, void * const gt);
void * chomik_create_name_item_code(void * const l);
void * chomik_create_value_variable_value(void * const gn);
void * chomik_create_expand_statement(int d, unsigned new_line_number);
void * chomik_create_assignment_statement(void * const t, void * const v, unsigned new_line_number);
void * chomik_create_generic_value_code(void * const l);
void * chomik_create_list_of_statements(void * const s, void * const l);
void * chomik_create_list_of_name_items(void * const i, void * const l);
void * chomik_create_value_literal_float(double v);
void * chomik_create_value_literal_int(int i);
void * chomik_create_value_literal_enum(void * const n);
void * chomik_create_variable_definition_statement(void * const l, unsigned new_line_number);
void * chomik_create_list_of_variable_definitions(void * const vd, void * const l);
void * chomik_create_variable_definition(void * const n, void * const t);
void * chomik_create_generic_type_range(void * const r);
void * chomik_create_type_definition_body_range(void * const r);
void * chomik_create_value_literal(void * const l);
void * chomik_create_enum_literal(const char * const type_name, void * const l);
void * chomik_create_integer_literal(int v);
void * chomik_create_float_literal(double v);
void * chomik_create_string_literal(const char * const v);
void * chomik_create_code_literal(void * const l);
void * chomik_create_complex_name_enum_literal(void * const complex_type_name, void * const l);
void * chomik_create_complex_name_enum_literal_placeholder(void * const complex_type_name, const char * const p, void * const l);
void * chomik_create_execute_variable_value_statement(void * const l, unsigned new_line_number);
void * chomik_create_execute_value_statement(void * const l, unsigned new_line_number);
void * chomik_create_integer_literal_placeholder(const char * const p, void * const l);
void * chomik_create_enum_literal_placeholder(const char * const p, void * const l, const char * const t);
void * chomik_create_string_literal_placeholder(const char * const p, void * const l);
void * chomik_create_code_literal_placeholder(const char * const p, void * const l);
void * chomik_create_float_literal_placeholder(const char * const p, void * const l);
void * chomik_create_variable_value(void * const l);
void chomik_destroy_list_of_statements(void * const l);
void chomik_destroy_list_of_type_definitions(void * const l);
void chomik_destroy_list_of_names(void * const l);
void chomik_destroy_list_of_name_items(void * const l);
void chomik_destroy_name(void * const l);
void chomik_destroy_list_of_variable_definitions(void * const l);

void chomik_copy_list_of_statements_to_the_program(void * const l, void * scanner, void * extra_chomik_parameter);

%}

%union {
char * value_string;
double value_float;
int value_int;
void * value_pointer;
}


%token T_TYPE T_VARIABLE T_LET T_EXECUTE T_EXPAND T_VALUE T_DOUBLE_POINT T_INTEGER T_FLOAT T_STRING T_CODE T_COMPLEX T_RANGE T_LIST

%token<value_string> T_IDENTIFIER T_STRING_LITERAL
%token<value_float> T_FLOAT_LITERAL
%token<value_int> T_INT_LITERAL

%type <value_pointer> type_definition_body range min_boundary max_boundary type_name name type_definition nonempty_list_of_type_definitions
%type <value_pointer> statement multiple_type_definition multiple_variable_definition assignment execute expand nonempty_list_of_names
%type <value_pointer> name_item nonempty_list_of_name_items value constant_value_literal code_literal list_of_statements
%type <value_pointer> nonempty_list_of_variable_definitions variable_definition 

%start program

%%

program: list_of_statements { chomik_copy_list_of_statements_to_the_program($1, yyscanner, extra_chomik_parameter); chomik_destroy_list_of_statements($1); }

list_of_statements: statement list_of_statements { $$ = chomik_create_list_of_statements($1, $2); chomik_destroy_list_of_statements($2); }
            | { $$ = chomik_create_list_of_statements(NULL, NULL); }

statement: multiple_type_definition | multiple_variable_definition | assignment | execute | expand

multiple_type_definition: T_TYPE nonempty_list_of_type_definitions ';' { $$ = chomik_create_type_definition_statement($2, chomik_get_current_yylineno(yyscanner, extra_chomik_parameter)); chomik_destroy_list_of_type_definitions($2); }

nonempty_list_of_type_definitions: type_definition ',' nonempty_list_of_type_definitions { $$=chomik_create_list_of_type_definitions($1, $3); chomik_destroy_list_of_type_definitions($3); /* no need to destroy $1 !!! */ }
        | type_definition { $$=chomik_create_list_of_type_definitions($1, NULL); /* no need to destroy $1 !!! */ } 

type_definition: T_IDENTIFIER '=' type_definition_body { $$ = chomik_create_type_definition($1, $3); free($1); /* no need to destroy $3 !!! */ }
        | T_COMPLEX '[' name ']' '=' type_definition_body { $$ = chomik_create_complex_name_generic_type_definition($3, $6); /* no need to destroy $3 or $6 !!! */ }

type_definition_body: '{' nonempty_list_of_names '}' { $$=chomik_create_type_definition_body_enum($2); chomik_destroy_list_of_names($2); } 
            | optional_range_keyword range { $$=chomik_create_type_definition_body_range($2); /* no need to destroy $2 !!! */ }

nonempty_list_of_names: name ',' nonempty_list_of_names { $$ = chomik_create_list_of_generic_names($1, $3); /* no need to destroy $1 !!! */ chomik_destroy_list_of_names($3); } 
    | name { $$ = chomik_create_list_of_generic_names($1, NULL); /* no need to destroy $1 !!! */ }    
    
name_item: T_IDENTIFIER { $$ = chomik_create_name_item_identifier($1, yyscanner, extra_chomik_parameter); free($1); }
            | '(' T_IDENTIFIER ':' type_name ')' { $$ = chomik_create_name_item_placeholder($2, $4); free($2); /* no need to destroy $4 !!! */ } 
            | T_INT_LITERAL { $$ = chomik_create_name_item_int($1); }
            | T_FLOAT_LITERAL { $$ = chomik_create_name_item_float($1); } 
            | T_STRING_LITERAL { $$ = chomik_create_name_item_string($1); free($1); }
            | '{' list_of_statements '}' { $$ = chomik_create_name_item_code($2); /* no need to destroy $2 !!! */ }
            | '<' nonempty_list_of_name_items '>' { $$ = chomik_create_name_item_variable_value($2); chomik_destroy_list_of_name_items($2); }                                    
            
type_name: T_INTEGER { $$=chomik_create_generic_type_named("integer"); } 
        | T_FLOAT { $$=chomik_create_generic_type_named("float"); }
        | T_STRING { $$=chomik_create_generic_type_named("string"); }
        | T_CODE { $$=chomik_create_generic_type_named("code"); }
        | T_IDENTIFIER { $$=chomik_create_generic_type_named($1); free($1); } 
        | optional_range_keyword range { $$=chomik_create_generic_type_range($2); }
        | T_COMPLEX '[' name ']' { $$=chomik_create_generic_complex_name_type_named($3); }
        | T_LIST '{' nonempty_list_of_names '}' { $$=chomik_create_generic_type_list($3); }


optional_range_keyword: T_RANGE |

range: min_boundary T_DOUBLE_POINT max_boundary { $$=chomik_create_generic_range($1, $3); /* no need to destroy $1 and $3 !!! */ }

min_boundary: T_INT_LITERAL { $$=chomik_create_generic_range_boundary_int_literal($1); } 
        | '<' name '>' { $$=chomik_create_generic_range_boundary_variable_value($2); /* no need to destroy $2 !!! */ }

max_boundary: T_INT_LITERAL { $$=chomik_create_generic_range_boundary_int_literal($1); } 
        | '<' name '>' { $$=chomik_create_generic_range_boundary_variable_value($2); /* no need to destroy $2 !!! */ }

multiple_variable_definition: T_VARIABLE nonempty_list_of_variable_definitions ';' { $$ = chomik_create_variable_definition_statement($2, chomik_get_current_yylineno(yyscanner, extra_chomik_parameter)); chomik_destroy_list_of_variable_definitions($2); }

nonempty_list_of_variable_definitions: variable_definition ',' nonempty_list_of_variable_definitions 
        { $$ = chomik_create_list_of_variable_definitions($1, $3); chomik_destroy_list_of_variable_definitions($3); }
        
    | variable_definition { $$ = chomik_create_list_of_variable_definitions($1, NULL); }
    
variable_definition: name ':' type_name { $$ = chomik_create_variable_definition($1, $3); /* no need to destroy $1 and $3 !!! */ }

name: nonempty_list_of_name_items { $$ = chomik_create_generic_name($1); chomik_destroy_list_of_name_items($1); } 

nonempty_list_of_name_items: name_item nonempty_list_of_name_items 
            { $$ = chomik_create_list_of_name_items($1, $2); chomik_destroy_list_of_name_items($2); /* no need to destroy $1 !!! */} 
            
        | name_item { $$ = chomik_create_list_of_name_items($1, NULL); /* no need to destroy $1 !!! */ }

        
optional_variable: T_VARIABLE |        
        
assignment: T_LET optional_variable name '=' value ';' { $$ = chomik_create_assignment_statement($3, $5, chomik_get_current_yylineno(yyscanner, extra_chomik_parameter)); /* no need to destroy $3 and $5 !!! */ }

value: T_VALUE constant_value_literal { $$ = chomik_create_value_literal($2); /* no need to destroy $2 !!! */ }
    | '<' name '>' { $$ = chomik_create_variable_value($2); /* no need to destroy $2 !!! */ }
        
constant_value_literal: 
        T_IDENTIFIER name { $$ = chomik_create_enum_literal($1, $2); free($1); /* no need to destroy $2 !!! */ }
        | T_IDENTIFIER '[' '(' T_IDENTIFIER ':' type_name ')' ']' { $$ = chomik_create_enum_literal_placeholder($4, $6, $1); free($1); free($4); /* no need to destroy $6 !!! */ } 
        | T_CODE code_literal { $$ = $2; }
        | T_CODE '[' '(' T_IDENTIFIER ':' type_name ')' ']' { $$ = chomik_create_code_literal_placeholder($4, $6); free($4); /* no need to destroy $6 !!! */ }
        | T_INTEGER T_INT_LITERAL { $$ = chomik_create_integer_literal($2); }
        | T_INTEGER '[' '(' T_IDENTIFIER ':' type_name ')' ']' { $$ = chomik_create_integer_literal_placeholder($4, $6); free($4); /* no need to destroy $6 !!! */ }
        | T_FLOAT T_FLOAT_LITERAL { $$ = chomik_create_float_literal($2); }
        | T_FLOAT '[' '(' T_IDENTIFIER ':' type_name ')' ']' { $$ = chomik_create_float_literal_placeholder($4, $6); free($4); /* no need to destroy $6 !!! */ }
        | T_STRING T_STRING_LITERAL { $$ = chomik_create_string_literal($2); free($2); }
        | T_STRING '[' '(' T_IDENTIFIER ':' type_name ')' ']' { $$ = chomik_create_string_literal_placeholder($4, $6); free($4); /* no need to destroy $6 !!! */ }
        | T_COMPLEX '[' name ']' name { $$ = chomik_create_complex_name_enum_literal($3, $5); }
        | T_COMPLEX '[' name ']' '[' '(' T_IDENTIFIER ':' type_name ')' ']' { $$ = chomik_create_complex_name_enum_literal_placeholder($3, $7, $9); free($7); /* no need to destroy $3 or $9 !!! */ }

        
code_literal: '{' list_of_statements '}' { $$ = chomik_create_code_literal($2); chomik_destroy_list_of_statements($2); }


optional_execute: T_EXECUTE |


execute: optional_execute '<' name '>' ';' { $$ = chomik_create_execute_variable_value_statement($3, chomik_get_current_yylineno(yyscanner, extra_chomik_parameter)); /* no need to destroy $3 !!! */ }
        | optional_execute value ';' { $$ = chomik_create_execute_value_statement($2, chomik_get_current_yylineno(yyscanner, extra_chomik_parameter)); /* no need to destroy $2 !!! */ }

expand: T_EXPAND '(' T_INT_LITERAL ')' ';' { $$ = chomik_create_expand_statement($3, chomik_get_current_yylineno(yyscanner, extra_chomik_parameter)); }

%%
 
void chomik_reset_parser()
{
	// just for case we have some globals to be reset
}

int chomik_parserlex(void * yylval_param, void * yyloc_param, yyscan_t scanner, void *extra_chomik_parameter)
{
	return chomik_scannerlex(yylval_param, yyloc_param, scanner, extra_chomik_parameter);
}
void chomik_parsererror(void * yyloc_param, yyscan_t scanner, void * chomik_extra_parameter, const char* msg)
{
	chomik_scannererror(yyloc_param, scanner, chomik_extra_parameter, msg);
}

