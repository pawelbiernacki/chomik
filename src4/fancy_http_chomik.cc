#include "fancy_http_chomik.h"
#include "config.h"
#include <iostream>
#include <iomanip>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>

int my_port = 5001;
bool my_port_known = false;


//#define FANCY_HTTP_CHOMIK_DONT_FORK_FOR_TEST_PURPOSES


chomik::program the_program;
chomik::parser the_parser{the_program};


void fancy_http_chomik::server::add_html_head_content(std::ostream & message_stream)
{
    message_stream << "<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        
    << "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" "
    << "integrity=\"sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi\" crossorigin=\"anonymous\">";
    
    
}


void fancy_http_chomik::server::add_html_default_chomik_code(std::ostream & message_stream)
{
    message_stream 
    << "&lt;print \"hello world\"&gt;;\n";
}


void fancy_http_chomik::server::add_html_common_scripts(std::ostream & message_stream)
{
    message_stream << "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/js/bootstrap.bundle.min.js\" "
    << "integrity=\"sha384-OERcA2EqjJCMA+/3y+gxIOqMEjwtxJY7qPCqsdltbNJuaOe923+mo//f6V8Qbsw3\" crossorigin=\"anonymous\"></script>";
}


void fancy_http_chomik::server::add_html_navbar(std::ostream & message_stream)
{
    message_stream
        << "<nav class=\"navbar navbar-expand-lg navbar-dark bg-primary\">\n"
        << "<div class=\"container-fluid\">\n"
        << "<a class=\"navbar-brand\" href=\"/\"><img src=\"chomik.png\" alt=""></a>\n"
        
        << "<button class=\"navbar-toggler\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#navbarNavDropdown\" "
        << "aria-controls=\"navbarText\" aria-expanded=\"true\" aria-label=\"Toggle navigation\">\n"
        << "<span class=\"navbar-toggler-icon\"></span>\n"
        << "</button>\n"
        
        << "<div class=\"collapse navbar-collapse\">\n"
        << "<ul class=\"navbar-nav dropdown\">\n"
        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink\">Basic examples</a>\n"        
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink\">\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleHelloWorld(); return false;\">Hello World!</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingMultipleValues(); return false;\">Printing multiple values</a></li>\n"
        
        << "</ul>\n"
        << "</li>\n"


        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink2\">Built-in types</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink2\">\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleBuiltInTypeInteger(); return false;\">Built-in type integer</a></li>\n"                
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSimpleIntegerAssignments(); return false;\">Simple integer assignments</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingIntegerVariableValues(); return false;\">Printing integer variable values</a></li>\n"        
        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleBuiltInTypeFloat(); return false;\">Built-in type float</a></li>\n"                
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSimpleFloatAssignments(); return false;\">Simple float assignments</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingFloatVariableValues(); return false;\">Printing float variable values</a></li>\n"

        << "<li><a class=\"dropdown-item\" onclick=\"setExampleBuiltInTypeString(); return false;\">Built-in type string</a></li>\n"                
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSimpleStringAssignments(); return false;\">Simple string assignments</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingStringVariableValues(); return false;\">Printing string variable values</a></li>\n"

        << "<li><a class=\"dropdown-item\" onclick=\"setExampleBuiltInTypeBoolean(); return false;\">Built-in type boolean</a></li>\n"                
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSimpleBooleanAssignments(); return false;\">Simple boolean assignments</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingBooleanVariableValues(); return false;\">Printing boolean variable values</a></li>\n"
        
        << "</ul>\n"
        << "</li>\n"


        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink3\">Built-in type code</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink3\">\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleBuiltInTypeCode(); return false;\">Declaring variables of type code</a></li>\n" 
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSimpleCodeAssignments(); return false;\">Simple code assignments</a></li>\n" 
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleExecutingCodeStoredInAVariable(); return false;\">Executing code stored in a variable</a></li>\n" 
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleExecutingCodeLiteral(); return false;\">Executing code literal</a></li>\n" 
        << "</ul>\n"
        << "</li>\n"

        
        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink4\">Variable names</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink4\">\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleMultipleIdentifiers(); return false;\">Declaring variables with names consisting of multiple identifiers</a></li>\n" 
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleMultipleIdentifiersAndLiterals(); return false;\">Declaring variables with names containing literals</a></li>\n" 
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleAssigningVariablesWithComplexNames(); return false;\">Assigning variables with complex names</a></li>\n" 
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingVariablesWithComplexNames(); return false;\">Printing variables with complex names</a></li>\n" 
        << "</ul>\n"
        << "</li>\n"


        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink5\">Placeholders</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink5\">\n"                
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleDeclaringVariablesWithASinglePlaceholder(); return false;\">Declaring variables with a single placeholder</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleAssigningVariablesWithASinglePlaceholder(); return false;\">Assigning variables with a single placeholder - implicit loop</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingWithImplicitLoopWithASinglePlaceholder(); return false;\">Printing with implicit loop for a single placeholder</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleDeclaringVariablesWithMultiplePlaceholders(); return false;\">Declaring variables with multiple placeholders</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleAssigningVariableWithMultiplePlaceholders(); return false;\">Assigning variables with multiple placeholders - implicit loop</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingWithImplicitLoopWithMultiplePlaceholders(); return false;\">Printing with implicit loop for multiple placeholders</a></li>\n"
        << "</ul>\n"
        << "</li>\n"

        
        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink6\">Enumerations</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink6\">\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSimpleEnumerations(); return false;\">Simple enumerations</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleDeclaringVariablesOfEnumerationType(); return false;\">Declaring variables of enumeration type</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleAssigningVariablesOfEnumerationType(); return false;\">Assigning variables of enumeration type</a></li>\n"                        
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingWithImplicitLoopForASingleEnumeration(); return false;\">Printing with implicit loop for a single enumeration</a></li>\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleEnumerationWithPlaceholders(); return false;\">Enumeration with placeholders</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintingEnumerationWithPlaceholders(); return false;\">Printing enumeration with placeholders</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleRecursiveEnumerations(); return false;\">Recursive enumerations</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleCyclicRecursiveEnumerations(); return false;\">Cyclic recursive enumerations</a></li>\n"        
        << "</ul>\n"
        << "</li>\n"


        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink7\">Comparisons</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink7\">\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExamplePrintCompareResult(); return false;\">Compare_result built-in type</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleComparingIntegers(); return false;\">Comparing integers</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleComparingStrings(); return false;\">Comparing strings</a></li>\n"        
        << "</ul>\n"
        << "</li>\n"

        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink8\">Arithmetic operations</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink8\">\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleAddingIntegers(); return false;\">Adding integers</a></li>\n"        
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleSubstractingIntegers(); return false;\">Substracting integers</a></li>\n"        
        << "</ul>\n"
        << "</li>\n"
        
        << "<li class=\"nav-item dropdown\">\n"
        << "<a class=\"nav-link dropdown-toggle\" role=\"button\" data-bs-toggle=\"dropdown\" aria-current=\"page\" id=\"navbarDropdownMenuLink9\">Writing to stringstreams</a>\n"
        << "<ul class=\"dropdown-menu\" aria-labelledby=\"navbarDropdownMenuLink9\">\n"
        << "<li><a class=\"dropdown-item\" onclick=\"setExampleCreatingAStringStream(); return false;\">Creating a stringstream</a></li>\n"        
        << "</ul>\n"
        << "</li>\n"

        << "</ul>\n"
        << "</div>\n"
    
        << "</div>\n"
        << "</nav>\n";        

}


void fancy_http_chomik::server::add_html_body(std::ostream & message_stream)
{
    add_html_common_scripts(message_stream);
    
    message_stream
        << "<script language=\"JavaScript\">"
        << "function setExampleHelloWorld()\n"
        << "{ document.getElementById(\"code\").value=\"<print \\\"hello world\\\">;\\n\"; }\n"
        << "function setExamplePrintingMultipleValues()\n"
        << "{ document.getElementById(\"code\").value=\"<print \\\"hello\\\" \\\"world\\\" 2022 1 2 3 3.14159>;\\n\"; }\n"
        << "function setExampleBuiltInTypeInteger()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:integer, y:integer;\\n\"; }\n"
        << "function setExampleSimpleIntegerAssignments()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:integer;\\nlet x=value integer 100;\\n\"; }\n"
        << "function setExamplePrintingIntegerVariableValues()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:integer;\\nlet x=value integer 100;\\n<print \\\"x=\\\" <x>>;\\n\"; }\n"
        << "function setExampleBuiltInTypeFloat()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:float;\\n\"; }\n"
        << "function setExampleSimpleFloatAssignments()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:float;\\nlet x=value float 3.14159;\\n\"; }\n"
        << "function setExamplePrintingFloatVariableValues()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:float;\\nlet x=value float 3.14159;\\n<print \\\"x=\\\" <x>>;\\n\"; }\n"
        << "function setExampleBuiltInTypeString()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:string;\\n\"; }\n"
        << "function setExampleSimpleStringAssignments()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:string;\\nlet x=value string \\\"alpha\\\";\\n\"; }\n"
        << "function setExamplePrintingStringVariableValues()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:string;\\nlet x=value string \\\"alpha\\\";\\n<print \\\"x=\\\" <x>>;\\n\"; }\n"
        << "function setExampleBuiltInTypeBoolean()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:boolean;\\n\"; }\n"
        << "function setExampleSimpleBooleanAssignments()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:boolean, y:boolean;\\nlet x=value boolean false;\\nlet y=value boolean true;\\n\"; }\n"
        << "function setExamplePrintingBooleanVariableValues()\n"
        << "{ document.getElementById(\"code\").value=\"variable x:boolean, y:boolean;\\nlet x=value boolean false;\\nlet y=value boolean true;\\n<print \\\"x=\\\" <x> \\\"and y=\\\" <y>>;\\n\"; }\n"
        << "function setExampleMultipleIdentifiers()\n"        
        << "{ document.getElementById(\"code\").value=\"variable this is a legal name:integer;\\nvariable another legal name:integer;\\n\"; }\n"
        
        << "function setExampleMultipleIdentifiersAndLiterals()\n"
        << "{ document.getElementById(\"code\").value=\"variable this is a legal name \\\"even with a literal\\\" inside:integer, \\nanother legal name with a literal 123:string;\\n\"; }\n"        

        << "function setExampleAssigningVariablesWithComplexNames()\n"
        << "{ document.getElementById(\"code\").value=\"variable this is a legal name \\\"even with a literal\\\" inside:integer, \\nanother legal name with a literal 123:string;\\nlet this is a legal name \\\"even with a literal\\\" inside = value integer 456;\\nlet another legal name with a literal 123=value string \\\"alpha\\\";\\n\"; }\n"
        

        << "function setExamplePrintingVariablesWithComplexNames()\n"
        << "{ document.getElementById(\"code\").value=\"variable this is a legal name \\\"even with a literal\\\" inside:integer;\\nvariable another legal name with a literal 123:string;\\nlet this is a legal name \\\"even with a literal\\\" inside = value integer 456;\\nlet another legal name with a literal 123=value string \\\"alpha\\\";\\n<print \\\"this is a legal name \\\\\\\"even with a literal\\\\\\\" inside=\\\" <this is a legal name \\\"even with a literal\\\" inside>>;\\n<print \\\"another legal name with a literal 123=\\\" <another legal name with a literal 123>>;\\n\";  }\n"

        << "function setExampleBuiltInTypeCode()\n"        
        << "{ document.getElementById(\"code\").value=\"variable x:code;\\n\"; }\n"

        << "function setExampleSimpleCodeAssignments()\n"        
        << "{ document.getElementById(\"code\").value=\"variable x:code;\\nlet x=value code { <print \\\"Hello\\\">;};\\n\"; }\n"

        << "function setExampleExecutingCodeStoredInAVariable()\n"        
        << "{ document.getElementById(\"code\").value=\"variable x:code;\\nlet x=value code { <print \\\"Hello\\\">;};\\n<x>;\\n\"; }\n"

        << "function setExampleExecutingCodeLiteral()\n"        
        << "{ document.getElementById(\"code\").value=\"value code { <print \\\"Hello\\\">;};\\n\"; }\n"

        << "function setExampleDeclaringVariablesWithASinglePlaceholder()\n"        
        << "{ document.getElementById(\"code\").value=\"variable some function of (X:boolean):boolean;\\n\"; }\n"
        
        << "function setExampleAssigningVariablesWithASinglePlaceholder()\n"        
        << "{ document.getElementById(\"code\").value=\"variable some function of (X:boolean):boolean;\\nlet some function of (X:boolean)=value boolean true;\\n<print \\\"some function of true=\\\" <some function of true>>;\\n<print \\\"some function of false=\\\" <some function of false>>;\\nlet some function of (X:boolean)=value boolean false;\\n<print \\\"some function of true=\\\" <some function of true>>;\\n<print \\\"some function of false=\\\" <some function of false>>;\\n\"; }\n"

        << "function setExamplePrintingWithImplicitLoopWithASinglePlaceholder()\n"        
        << "{ document.getElementById(\"code\").value=\"variable some function of (X:boolean):boolean;\\nlet some function of (X:boolean)=value boolean true;\\n<print \\\"some function of \\\" (A:boolean) \\\"=\\\" <some function of (A:boolean)>>;\\nlet some function of (X:boolean)=value boolean false;\\n<print \\\"some function of \\\" (B:boolean) \\\"=\\\" <some function of (B:boolean)>>;\\n\"; }\n"        
        
        << "function setExampleDeclaringVariablesWithMultiplePlaceholders()\n"        
        << "{ document.getElementById(\"code\").value=\"variable some function of (X:boolean) and (Y:boolean):boolean;\\n\"; }\n"

        << "function setExampleAssigningVariableWithMultiplePlaceholders()\n"        
        << "{ document.getElementById(\"code\").value=\"variable some function of (X:boolean) and (Y:boolean):boolean;\\nlet some function of (A:boolean) and (B:boolean)=value boolean false;\\n\"; }\n"

        << "function setExamplePrintingWithImplicitLoopWithMultiplePlaceholders()\n"        
        << "{ document.getElementById(\"code\").value=\"variable some function of (X:boolean) and (Y:boolean):boolean;\\nlet some function of (A:boolean) and (B:boolean)=value boolean false;\\n<print \\\"some function of\\\" (S:boolean) \\\"and\\\" (T:boolean) \\\"=\\\" <some function of (S:boolean) and (T:boolean)>>;\\n\"; }\n"


        << "function setExampleSimpleEnumerations()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir};\\nexpand(1);\\n\"; }\n"

        << "function setExampleDeclaringVariablesOfEnumerationType()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir};\\nexpand(1);\\nvariable somebody:person;\\n\"; }\n"

        << "function setExampleAssigningVariablesOfEnumerationType()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir};\\nexpand(1);\\nvariable somebody:person;\\nlet somebody=value person Gwaigilion;\"; }\n"
        
        << "function setExamplePrintingWithImplicitLoopForASingleEnumeration()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir};\\nexpand(1);\\n<print (X:person)>;\\n\"; }\n"
        
        << "function setExampleEnumerationWithPlaceholders()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir}, place={Krakow, Warszawa, Wroclaw, Poznan, Gdansk};\\ntype information={(X:person) likes (Y:place)};\\nexpand(2);\\n\"; }\n"        

        << "function setExamplePrintingEnumerationWithPlaceholders()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir}, place={Krakow, Warszawa, Wroclaw, Poznan, Gdansk};\\ntype information={(X:person) likes (Y:place)};\\nexpand(2);\\n<print (I:information)>;\\n\"; }\n"        

        << "function setExampleRecursiveEnumerations()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir}, place={Krakow, Warszawa, Wroclaw, Poznan, Gdansk};\\ntype information={(X:person) likes (Y:place), not (I:information)};\\nexpand(3);\\n<print (I:information)>;\\n\"; }\n"        

        << "function setExampleCyclicRecursiveEnumerations()\n"        
        << "{ document.getElementById(\"code\").value=\"type person={Gotrek, Gwaigilion, Gerrudir}, place={Krakow, Warszawa, Wroclaw, Poznan, Gdansk};\\ntype information={(X:person) is in (Y:place), (X:person) has told (Y:person) that (I:information), (X:person) has done (A:action)};\\ntype action={tell (X:person) that (I:information), go to (X:place)};\\nexpand(3);\\n<print (I:information)>;\\n\"; }\n"        
        
        << "function setExamplePrintCompareResult()\n"        
        << "{ document.getElementById(\"code\").value=\"<print (X:compare_result)>;\\n\"; }\n"        

        << "function setExampleComparingIntegers()\n"        
        << "{ document.getElementById(\"code\").value=\"variable do something depending on (X:compare_result):code;\\nlet do something depending on lower=value code { <print \\\"it is lower\\\">;};\\nlet do something depending on equal=value code { <print \\\"it is equal\\\">;};\\nlet do something depending on greater=value code { <print \\\"it is greater\\\">;};\\n<compare \\\"integer\\\" 1 2>;\\n<do something depending on <the compare result>>;\\n\"; }\n"        

        << "function setExampleComparingStrings()\n"        
        << "{ document.getElementById(\"code\").value=\"variable do something depending on (X:compare_result):code;\\nlet do something depending on lower=value code { <print \\\"it is lower\\\">;};\\nlet do something depending on equal=value code { <print \\\"it is equal\\\">;};\\nlet do something depending on greater=value code { <print \\\"it is greater\\\">;};\\n<compare \\\"string\\\" \\\"alpha\\\" \\\"alpha\\\">;\\n<do something depending on <the compare result>>;\\n\"; }\n"        

        << "function setExampleAddingIntegers()\n"        
        << "{ document.getElementById(\"code\").value=\"<add \\\"integer\\\" 2 2>;\\n<print <the add result \\\"integer\\\">>;\"; }\n"        

        << "function setExampleSubstractingIntegers()\n"        
        << "{ document.getElementById(\"code\").value=\"<substract \\\"integer\\\" 2 2>;\\n<print <the add result \\\"integer\\\">>;\"; }\n"        

    
        << "function setExampleCreatingAStringStream()\n"        
        << "{ document.getElementById(\"code\").value=\"<create new output stringstream>;\\nvariable my stream index:integer;\\nlet my stream index=<the created stream index>;\\nlet the print target stream index =  <my stream index>;\\nlet the print end of line = value string \\\"\\\";\\nlet the get from stream stream index = <the created stream index>;\\nexecute <print \\\"alpha\\\" \\\"beta\\\" \\\".\\\">;\\nlet the print target stream index=value integer 0;\\nlet the print end of line=value string \\\"\\\\n\\\";\\nlet the get from stream stream index = <my stream index>;\\n<get from stream>;\\n<print <the get from stream result>>;\\n\"; }\n"        
        
        << "</script>";
        
    add_html_navbar(message_stream);

    message_stream
            
        << "<div class=\"container\">\n"
        

        // main
        << "<div class=\"row\">\n"
        << "<div class=\"col-1\">\n"        
        << "</div>\n"
        
        << "<div class=\"col-10\">\n"        
        << "<form>"
        << "<textarea cols=\"80\" rows=\"30\" name=\"code\" id=\"code\">";
        
    add_html_default_chomik_code(message_stream);
        
        message_stream << "</textarea>"        
        << "<button class=\"btn btn-danger\" type=\"submit\">Run</button><br/>"
        << "</form>";
        
        message_stream << "</div>\n"
        << "<div class=\"col-1\">\n"
        << "</div>\n"
        << "</div>\n"
        << "</div>\n";
        
        
        //<< "<a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a>";
}


void fancy_http_chomik::server::add_html_body_for_incorrect_uri(std::ostream & message_stream)
{
    message_stream
        << "<nav class=\"navbar navbar-expand-lg navbar-dark bg-primary\">\n"
        << "<div class=\"container-fluid\">\n"
        << "<a class=\"navbar-brand\" href=\"/\"><img src=\"chomik.png\" alt=""></a>\n"
            
        << "</div>\n"
        << "</nav>\n";        
    
    message_stream
        << "<div class=\"container\">\n";        

    message_stream

        // main
        << "<div class=\"row\">\n"
        << "<div class=\"col-1\">\n"        
        << "</div>\n"
        
        << "<div class=\"col-10\">\n";
        
    message_stream << "Sorry, fancy_http_chomik does not understand this URI";
    
    message_stream << "</div>\n"
        << "<div class=\"col-1\">\n"
        << "<button class=\"btn btn-primary\" onclick=\"history.back()\">Go Back</button>"
        << "</div>\n"
        << "</div>\n"
        << "</div>\n";
        
}

void fancy_http_chomik::server::add_html_body_for_code(std::ostream & message_stream, const std::string & decoded_code, std::stringstream & error_stream, std::stringstream & error_stream2)
{
    add_html_common_scripts(message_stream);

    message_stream
        << "<nav class=\"navbar navbar-expand-lg navbar-dark bg-primary\">\n"
        << "<div class=\"container-fluid\">\n"
        << "<a class=\"navbar-brand\" href=\"/\"><img src=\"chomik.png\" alt=""></a>\n"
            
        << "</div>\n"
        << "</nav>\n";        
    
    message_stream
        << "<div class=\"container\">\n";        

    message_stream

        // main
        << "<div class=\"row\">\n"
        << "<div class=\"col-1\">\n"        
        << "</div>\n"
        
        << "<div class=\"col-10\">\n";
    
    if (the_parser.parse_string(decoded_code, error_stream)==0)
    {
        std::unique_ptr<http_chomik::machine> my_machine;
        
        create_a_new_machine(my_machine); // you can redefine this method to use different machines        
        
        chomik::machine::current_runtime_warning_stream = &error_stream2;
        
        my_machine->create_predefined_types();
        my_machine->create_predefined_variables();
        my_machine->create_predefined_streams();
        
        the_program.execute(*my_machine);   

        std::stringstream x;
        x << my_machine->get_stream(0).get_result() << error_stream2.str();
        
        x.seekg(0);
        std::string x3;
        while (std::getline(x, x3))
        {
            message_stream << "<div class=\"text-start\">" << x3 << "</div>";
        }
        
        
    }
    else
    {
        message_stream << error_stream.str();
    }    
    message_stream << "</div>\n"
        << "<div class=\"col-1\">\n"
        << "<button class=\"btn btn-primary\" onclick=\"history.back()\">Go Back</button>"
        << "</div>\n"
        << "</div>\n"
        << "</div>\n";
        
    
    
    
    
    // "<br/><br/><a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a>";    
}



int main(int argc, char * argv[])
{
    std::cout << "fancy_http_chomik, copyright by Pawel Biernacki\n";
	std::cout << "Vantaa 2022\n";
	std::cout << PACKAGE_URL << "\n";
    
    
	for (int i=1; i<argc; i++)
	{
		std::string option(argv[i]);
		
		if (option=="-p" || option=="--port")
		{
			if (i == argc-1)
			{
				std::cerr << "option -p or --port require a port number\n";
				return -1;
			}
			std::string port(argv[i+1]);
			i++;
			my_port = std::stoi(port);
			my_port_known = true;
		}
		else
		if (option == "-h" || option == "--help")
		{
			std::cout << "usage: " << argv[0] 
			<< " [-p|--port <port>] [-h|--help]\n";
			return 0;
		}
	}

    if (!my_port_known)
	{
		std::cerr << "unknown port\n";
		return -1;
	}

#ifdef FANCY_HTTP_CHOMIK_DONT_FORK_FOR_TEST_PURPOSES	
    std::cerr << "warning: this version is not suitable for the production!\n";
#else	
    HTTP_CHOMIK_LOG_NOTICE("http_chomik started.");
    pid_t f, pid;
    
    f = fork();
    if (f < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (setsid() < 0)
        exit(EXIT_FAILURE);
	
	signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
	
	if (pid < 0)
        exit(EXIT_FAILURE);
    
    if (pid > 0)
        exit(EXIT_SUCCESS);
	
	umask(0);
    
    chdir("/root");
	
	int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
        
    openlog("chomik", LOG_PID|LOG_CONS, LOG_USER);
            
    syslog(LOG_INFO, PACKAGE_STRING " daemon is running");
#endif    
        
    http_chomik::wrapper<fancy_http_chomik::server> my_wrapper;        
    http_chomik::server::do_processing(my_port, my_wrapper);
    
    return EXIT_SUCCESS;	
}
 
