#include "chomik.h"
#include "config.h"
#include <cstring>
#include <algorithm>
#include <string>

typedef void* yyscan_t;

extern "C"
{
	#include "chomik_parser.tab.h"
}

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

#define CHOMIK_PARSER_STDERR(X) *chomik::parser::current_error_stream << X

extern "C" void chomik_open_file(const char * f, yyscan_t scanner, void *extra_chomik_parameter);
extern "C" void chomik_close_file(yyscan_t scanner, void *extra_chomik_parameter);
extern "C" int chomik_scannerlex_init(yyscan_t * scanner);
extern "C" int chomik_scannerlex_destroy(yyscan_t scanner);
extern "C" int chomik_get_current_yylineno(void * yyscanner, void * extra_chomik_parameter);

extern "C" int chomik_standard_yyerror_on;
extern "C" bool chomik_scanner_can_open_files;
extern "C" void chomik_read_from_string(const char * b, int len, yyscan_t scanner);


int chomik::parser::get_line_number()
{
	return chomik_get_current_yylineno(my_scanner, this);
}


int chomik::parser::parse(const char * filename)
{
	parser::current_error_stream=&std::cerr;

	chomik_standard_yyerror_on = 1;

	yyscan_t myscanner;

	if (chomik_scannerlex_init(&myscanner) != 0)
	{
		*parser::current_error_stream << "failed to initialize the scanner\n";
		return -1;
	}

	remember_the_scanner(myscanner);

	chomik_scanner_can_open_files = true;
	chomik_open_file(filename, myscanner, this);

	int i = 0;
	try
	{
		i = chomik_parserparse(myscanner, this);
	}
	catch (std::runtime_error & e)
	{
		*current_error_stream << e.what() << "\n";
		return -1;
	}

	if (chomik_scannerlex_destroy(myscanner) != 0)
	{
		*parser::current_error_stream << "failed to deinitialize the scanner\n";
		forget_the_scanner();
		return -1;
	}

	if (i != 0)
		*parser::current_error_stream << "error" << "\n";

	//chomik_close_file(); no need to close it, it is going to be closed by the scanner

	forget_the_scanner();

	return i;
}


int chomik::parser::parse_string(const std::string & new_code)
{
	parser::current_error_stream=&std::cerr;
	chomik_standard_yyerror_on = 1;

	yyscan_t myscanner;

	if (chomik_scannerlex_init(&myscanner) != 0)
	{
		*current_error_stream << "failed to initialize the scanner\n";
		return -1;
	}

	remember_the_scanner(myscanner);
	chomik_scanner_can_open_files = true;

	chomik_read_from_string(new_code.c_str(), new_code.length(), myscanner);

	int i = 0;
	try
	{
		i = chomik_parserparse(myscanner, this);
	}
	catch (std::runtime_error & e)
	{
		*current_error_stream << e.what() << "\n";
		return -1;
	}

	if (chomik_scannerlex_destroy(myscanner) != 0)
	{
		*current_error_stream << "failed to deinitialize the scanner\n";
		forget_the_scanner();
		return -1;
	}

	if (i != 0)
		*current_error_stream << "error" << "\n";

	forget_the_scanner();

	return i;
}


int chomik::parser::parse_string(const std::string & new_code, std::ostream * error_stream)
{
	parser::current_error_stream=error_stream;
	chomik_standard_yyerror_on = 0;

	yyscan_t myscanner;

	if (chomik_scannerlex_init(&myscanner) != 0)
	{
		*current_error_stream << "failed to initialize the scanner\n";
		return -1;
	}

	remember_the_scanner(myscanner);
	chomik_scanner_can_open_files = false;

	chomik_read_from_string(new_code.c_str(), new_code.length(), myscanner);

	int i = 0;
	try
	{
		i = chomik_parserparse(myscanner, this);
	}
	catch (std::runtime_error & e)
	{
		*current_error_stream << e.what() << "\n";
		return -1;
	}

	if (chomik_scannerlex_destroy(myscanner) != 0)
	{
		*current_error_stream << "failed to deinitialize the scanner\n";
		forget_the_scanner();
		return -1;
	}

	if (i != 0)
		*current_error_stream << "error" << "\n";

	forget_the_scanner();

	return i;
}


extern "C" void chomik_nonstandard_yyerror(unsigned line_number, const char * message)
{
	// we talk to parser here!, not to the machine
	*chomik::parser::current_error_stream << "line " << line_number << ": " << message << "\n";
}


extern "C" void chomik_reset_vector_of_buffer_data();
extern "C" void chomik_reset_parser();
extern "C" void chomik_reset_scanner();

void chomik::parser::reset()
{
	chomik_reset_vector_of_buffer_data();
	chomik_reset_parser();
	chomik_reset_scanner();
}


