%{
#include "parser.tab.h"
#define YY_DECL int yylex( YYSTYPE * yylval_param, void * yyscanner )
#define YYLTYPE int

void chomik_push_buffer_data(const char * const filename, unsigned line_number);
void chomik_pop_buffer_data(const char ** filename_ptr, unsigned * line_number_ptr);

void chomik_open_file(const char * f);
void chomik_close_file();

int chomik_standard_yyerror_on = 1;

void chomik_nonstandard_yyerror(unsigned line_number, const char * message);

%}


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


"#"                 yy_push_state( COMMENT);
     
<COMMENT>\n          yy_pop_state();
<COMMENT>[^\n]+      

".."                    return T_RANGE;

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

                                chomik_open_file(name_of_file_to_include);
                                free(name_of_file_to_include);
                                
                                yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
                                                                                                                                
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
                                chomik_close_file();
                                yypop_buffer_state();                        
                        
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


void chomik_open_file(const char * f)
{
    if (chomik_standard_yyerror_on)
    {    
        yyin = fopen( f, "r" );
        if (yyin ==  NULL)
        {
            fprintf(stderr, "unable to open file %s\n", f);
            exit(EXIT_FAILURE);
        }
        chomik_push_buffer_data(f, yylineno);
    }
    else
    {
        chomik_nonstandard_yyerror(0, "no include allowed here");
    }
	
	/*printf("opening file %s\n", f);*/
	
}

void chomik_close_file()
{
    const char * f = "unknown";
        
    if (chomik_standard_yyerror_on)
    {    
        chomik_pop_buffer_data(&f,&yylineno);
    
    /*printf("closing file %s\n", f);*/
    
        fclose(yyin);
    }
    else
    {
        /* we read from a buffer */
    }
}

char chomik_current_file[1024] = "";

void chomik_standard_yyerror(char const *s)
{
    fprintf (stderr, "file %s, line %i: %s\n", chomik_current_file, yylineno, s);
}

void chomik_read_from_string(const char * b, int len)
{    
    if (yy_scan_string(b) == NULL)
    {
        chomik_nonstandard_yyerror(0, "unable to read the buffer");    
    }
}

void yyerror (char const *s) {
   if (chomik_standard_yyerror_on)
   {
        chomik_standard_yyerror(s);
   }
   else
   {
        chomik_nonstandard_yyerror(yylineno, s);
   }
}

