%{
#define YYSTYPE         CHOMIK_PARSERSTYPE

#define YY_DECL int chomik_scannerlex( CHOMIK_PARSERSTYPE * yylval_param, CHOMIK_PARSERLTYPE * yyloc_param, void * yyscanner, void * extra_chomik_parameter)
#define YYLTYPE int

#include "chomik_parser.tab.h"
#include <stdbool.h>

void chomik_push_buffer_data_file(const char * const filename, unsigned line_number);
void chomik_push_buffer_data_string();
void chomik_pop_buffer_data(const char ** filename_ptr, unsigned * line_number_ptr, bool * it_is_a_file);

void chomik_open_file(const char * f, void * scanner, void * extra_chomik_parameter);
void chomik_open_file_by_include(const char * f, void * yyscanner, void * extra_chomik_parameter);
void chomik_close_file_if_it_is_a_file(void * scanner);

int chomik_standard_yyerror_on = 1;

void chomik_nonstandard_yyerror(unsigned line_number, const char * message, void * scanner);

char chomik_current_file[1024] = "";
bool chomik_it_is_a_file = true;
bool chomik_scanner_can_open_files = true;

%}

%option reentrant bison-bridge

%option noyywrap
%option yylineno
%option stack


%x COMMENT INCLUDE


DIGIT    [0-9]
ID       [a-z][a-z0-9]*
BLANK           [ \t]
ANYTHING_BUT_EOL [^\n]
LETTER          [a-zA-Z_]

LETTER_OR_DIGIT ({DIGIT}|{LETTER})
IDENTIFIER		{LETTER}({LETTER_OR_DIGIT})*

%%


"#"                 yy_push_state( COMMENT, yyscanner);
     
<COMMENT>\n          yy_pop_state(yyscanner);
<COMMENT>[^\n]+      

".."                    return T_DOUBLE_POINT;

";"						return ';';
","						return ',';
":"						return ':';
"{"						return '{';
"}"						return '}';
"("						return '(';
")"						return ')';
"."						return '.';
"-"                     return '-';
"?"						return '?';
"!"						return '!';
"["                     return '[';
"]"                     return ']';
"<"                     return '<';
">"                     return '>';
"="						return '=';
     
"type"				    { return T_TYPE; }
"variable"              { return T_VARIABLE; }
"expand"                { return T_EXPAND; }
"value"                 { return T_VALUE; }
"let"                   { return T_LET; }
"execute"               { return T_EXECUTE; }
"integer"               { return T_INTEGER; }
"float"                 { return T_FLOAT; }
"string"                { return T_STRING; }
"code"                  { return T_CODE; }
"complex"               { return T_COMPLEX; }
"range"                 { return T_RANGE; }
"list"                  { return T_LIST; }


"include"               {
                            BEGIN(INCLUDE); 
                        }

{IDENTIFIER}  			{ yylval_param->value_string = strdup(yytext); return T_IDENTIFIER; }

-?{DIGIT}+\.{DIGIT}+e(-|\+){DIGIT}+	{ yylval_param->value_float = atof(yytext); return T_FLOAT_LITERAL; }

-?{DIGIT}+\.{DIGIT}+	{ yylval_param->value_float = atof(yytext); return T_FLOAT_LITERAL; }

-?{DIGIT}+				{ yylval_param->value_int = atoi(yytext); return T_INT_LITERAL; }



<INCLUDE>\"([^\\\"]|\\.)*\" {                                                             
                                if (chomik_standard_yyerror_on)
                                {
                                const char * p = yytext+1;                        
                                char * name_of_file_to_include = malloc(strlen(yytext+1)+1); 
                                char *t = name_of_file_to_include;
                                while (*p != '\0')
                                {
                                    if (*p == '\"')
                                    {
                                        *t = '\0';
                                        break;
                                    }
                                    else
                                    {
                                        *t++ = *p++;
                                    }
                                }
                                
                                /*printf("including %s\n", name_of_file_to_include);*/

                                chomik_open_file_by_include(name_of_file_to_include, yyscanner, extra_chomik_parameter);
                                free(name_of_file_to_include);
                                
                                yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE, yyscanner ), yyscanner);
                                                                                                                                
                                yylineno = 1;
                                
                                    BEGIN(INITIAL);
                                }
                                else
                                {
                                    /*fprintf(stderr, "include %s", yytext);*/
                                    BEGIN(INITIAL);
                                }
                            }
                            
<INCLUDE>.
                            

\"([^\\\"]|\\.)*\"		{ 

                        const char * p = yytext+1;                        
                        
                        if (strlen(yytext+1) > 65536)
                        {
                            /* looks like an attack */
                            yytext="the string is too long";                            
                        }
                        
                        
                        yylval_param->value_string = malloc(strlen(yytext+1)+1);

                        char * t = yylval_param->value_string;
                        while (*p != '\0')
                        {
                            if (*p == '\"')
                            {
                                *t = '\0';
                                break;
                            }
                            else
                            if (*p == '\\')
                            {
                                p++;
                                switch (*p)
                                {
                                    case '\\': *t++ = '\\'; break;
                                    case '\"': *t++ = '\"'; break;
                                    case 'n': *t++ = '\n'; break;
                                }
                                p++;
                            }                            
                            else
                            {
                                *t++ = *p++;
                            }
                        }

                        return T_STRING_LITERAL; 
                        }

.|\n


 <<EOF>>                {
                            if (chomik_standard_yyerror_on)
                            {
								chomik_close_file_if_it_is_a_file(yyscanner);
                                yypop_buffer_state(yyscanner);
                        
                                if ( !YY_CURRENT_BUFFER )
                                {
                                    yyterminate();
                                }
                            }
                            else
                            {
                                    yyterminate();                            
                            }
                        }

%%


void chomik_open_file_by_include(const char * f, void * yyscanner, void * extra_chomik_parameter)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

    if (!chomik_scanner_can_open_files)
    {
		return;
	}

	yyin = fopen( f, "r" );
    if (yyin ==  NULL)
    {
		fprintf(stderr, "unable to open file %s\n", f);
        exit(EXIT_FAILURE);
	}
    chomik_push_buffer_data_file(f, yylineno);
    chomik_it_is_a_file = true;

	/*printf("opening file %s\n", f);*/
}



void chomik_open_file(const char * f, void * yyscanner, void * extra_chomik_parameter)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

    if (!chomik_scanner_can_open_files)
    {
		return;
	}

	yyin = fopen( f, "r" );
    if (yyin ==  NULL)
    {
		fprintf(stderr, "unable to open file %s\n", f);
        exit(EXIT_FAILURE);
	}
    chomik_push_buffer_data_file(f, 1);
    chomik_it_is_a_file = true;

	/*printf("opening file %s\n", f);*/
	
}

void chomik_close_file_if_it_is_a_file(void * yyscanner)
{
    const char * f = "unknown";
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	unsigned line_number;

    if (chomik_standard_yyerror_on)
    {    
		if (chomik_it_is_a_file)
		{
			fclose(yyin);
		}

        chomik_pop_buffer_data(&f, &line_number, &chomik_it_is_a_file);

        yyset_lineno(line_number, yyscanner);
    }
    else
    {
        /* we read from a buffer */
    }
}

void chomik_standard_yyerror(char const *s, void * yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	if (chomik_it_is_a_file)
	{
		fprintf (stderr, "file %s, line %i: %s\n", chomik_current_file, yylineno, s);
	}
	else
	{
		fprintf (stderr, "string: %s\n", s);
	}
}

void chomik_read_from_string(const char * b, int len, void * yyscanner)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	if (b == NULL)
	{
		if (chomik_standard_yyerror_on)
		{
			chomik_standard_yyerror("provided empty string for scanning", yyscanner);
		}
		else
		{
			chomik_nonstandard_yyerror(0, "provided empty string for scanning", yyscanner);
		}
        return;
	}

	if (yyscanner == NULL)
	{
		if (chomik_standard_yyerror_on)
		{
			chomik_standard_yyerror("provided empty scanner for scanning", yyscanner);
		}
		else
		{
			chomik_nonstandard_yyerror(0, "provided empty scanner for scanning", yyscanner);
		}
        return;
	}

	chomik_push_buffer_data_string();
    chomik_it_is_a_file = false;

    if (chomik_scanner_scan_string(b, yyscanner) == NULL)
    {
		if (chomik_standard_yyerror_on)
		{
			chomik_standard_yyerror("unable to read the buffer", yyscanner);
		}
		else
		{
			chomik_nonstandard_yyerror(1, "unable to read the buffer", yyscanner);
		}
    }
}

void chomik_scannererror (void *loc, void *yyscanner, void * extra_chomik_parameter, char const *message)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	if (chomik_standard_yyerror_on)
	{
        chomik_standard_yyerror(message, yyscanner);
	}
	else
	{
        chomik_nonstandard_yyerror(yylineno, message, yyscanner);
	}
}

void chomik_reset_scanner()
{
	yylex_destroy;
}

int chomik_get_current_yylineno(void * yyscanner, void * extra_chomik_parameter)
{
	struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

	return yylineno;
}

