#include "http_chomik.h"
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


//#define HTTP_CHOMIK_DONT_FORK_FOR_TEST_PURPOSES


chomik::program the_program;
chomik::parser the_parser{the_program};



int main(int argc, char * argv[])
{
    chomik::parser::register_parser(&the_parser);

    std::cout << "http_chomik, copyright by Pawel Biernacki\n";
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

#ifdef HTTP_CHOMIK_DONT_FORK_FOR_TEST_PURPOSES	
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
        
    http_chomik::wrapper<http_chomik::server> my_wrapper;        
    http_chomik::server::do_processing(my_port, my_wrapper);
    
    return EXIT_SUCCESS;	
}
