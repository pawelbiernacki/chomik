#include "http_chomik.h"
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

extern chomik::parser the_parser; 
extern chomik::program the_program;

http_chomik::server::~server()
{
}

void http_chomik::server::send_message(const std::string & message)
{
	int n = write(sock, message.c_str(), message.size());
	if (n < 0)
	{
		HTTP_CHOMIK_LOG_ERROR("http_chomik error writing");
	}
}


void http_chomik::server::read_data(std::ostream & input_buffer_stream)
{
	char buffer[1024];
	ssize_t n;
	do
	{
        bzero(buffer,1024);
		n = read(sock,buffer,1023);
   
		if (n < 0) 
		{
            HTTP_CHOMIK_LOG_ERROR("http_chomik error reading");
            break;
        }
					
        input_buffer_stream.write(buffer, strlen(buffer));			
        
		if (n < 1023)
            break;        
    }
	while (true);    
}


void http_chomik::server::add_html_head_content(std::ostream & message_stream)
{
    message_stream << "<meta charset=\"UTF-8\"><link rel=\"stylesheet\" href=\"chomik.css\">";
}

void http_chomik::server::add_html_default_chomik_code(std::ostream & message_stream)
{
    message_stream << "&lt;print \"hello world\"&gt;;";
}

void http_chomik::server::add_html_body(std::ostream & message_stream)
{
    message_stream << "<image src=\"chomik.png\"/><h1>Welcome to our http_chomik server</h1><h3>by Paweł Biernacki</h3><form>"
        << "<textarea cols=\"80\" rows=\"40\" name=\"code\" id=\"code\">";
        
    add_html_default_chomik_code(message_stream);
        
    message_stream << "</textarea>"
        << "<button type=\"submit\">run</button></form><br/>"
        << "<a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a>";
}


void http_chomik::server::handle_main(std::ostream & message_stream)
{
    message_stream 
        << "<html>"    
        << "<head>";
        
    add_html_head_content(message_stream);
    
    message_stream << "</head>"
        << "<body>"; 
        
    add_html_body(message_stream);
    
    message_stream << "</body>"
        << "</html>";
}



void http_chomik::server::add_html_body_for_incorrect_uri(std::ostream & message_stream)
{
    message_stream << "<h1>Sorry, http_chomik does not understand this URI</h1><br/><button onclick=\"history.back()\">go back</button><br/><br/><a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a>";
}


void http_chomik::server::create_a_new_machine(std::unique_ptr<machine> & target)
{
    target = std::move(std::make_unique<http_chomik::machine>());
}


void http_chomik::server::add_html_body_for_code(std::ostream & message_stream, const std::string & decoded_code, std::stringstream & error_stream, std::stringstream & error_stream2)
{
    message_stream << "<image src=\"chomik.png\"/><h3>Result:</h3><pre>";
    
    if (the_parser.parse_string(decoded_code, error_stream)==0)
    {
        std::unique_ptr<http_chomik::machine> my_machine;
        
        create_a_new_machine(my_machine); // you can redefine this method to use different machines        
        
        chomik::machine::current_runtime_warning_stream = &error_stream2;
        
        my_machine->create_predefined_types();
        my_machine->create_predefined_variables();
        my_machine->create_predefined_streams();
        
        the_program.execute(*my_machine);           
        
        message_stream << my_machine->get_stream(0).get_result() << error_stream2.str();
    }
    else
    {
        message_stream << error_stream.str();
    }
    
    message_stream << "</pre><button onclick=\"history.back()\">go back</button><br/><br/><a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a>";    
}

void http_chomik::server::handle_incorrect_uri(std::ostream & message_stream)
{
    message_stream 
        << "<html>"    
        << "<head>";
        
    add_html_head_content(message_stream);
    
    message_stream << "</head>"
        << "<body>";
        
    add_html_body_for_incorrect_uri(message_stream);
        
    message_stream << "</body>"
        << "</html>";
}


void http_chomik::server::handle_chomik_image(std::ostream & message_stream)
{
    std::stringstream chomik_image_location_stream;
    
    chomik_image_location_stream << DATADIR << "/http_chomik/" << "chomik.png";
    std::ifstream chomik_image_stream(chomik_image_location_stream.str(), std::ios_base::binary);
    char buffer[1];
    
    while (chomik_image_stream.read(buffer, 1))
    {
        message_stream << buffer[0];        
    }
}


void http_chomik::server::handle_stylesheet(std::ostream & message_stream)
{
    std::stringstream chomik_css_location_stream;
    
    chomik_css_location_stream << DATADIR << "/http_chomik/" << "chomik.css";
    std::ifstream chomik_image_stream(chomik_css_location_stream.str());
    char buffer[1];

    while (chomik_image_stream.read(buffer, 1))
    {
        message_stream << buffer[0];        
    }    
}
 
void http_chomik::server::handle_code(std::ostream & message_stream, std::smatch & m)
{
    std::stringstream code(m[1]);
    std::stringstream decoded_code_stream, 
        error_stream, // this is for compilation errors
        error_stream2;  // this is for runtime errors
    std::string c,h;
    
    while (code >> std::setw(1) >> c)
    {
        if (c == "+")
        {
            decoded_code_stream << " ";
        }
        else
        if (c == "%")
        {
            code >> std::setw(2) >> h;
            unsigned int x = std::stoul(h, nullptr, 16);
            decoded_code_stream << static_cast<char>(x);
        }
        else
        {
            decoded_code_stream << c;
        }
    }    
    
    
    
    message_stream 
        << "<html>"    
        << "<head>";
        
    add_html_head_content(message_stream);
                
    message_stream 
        << "</head>"
        << "<body>";
                
    add_html_body_for_code(message_stream, decoded_code_stream.str(), error_stream, error_stream2);
        
    message_stream << "</body>" << "</html>";    
}



void http_chomik::server::do_processing(int my_port, http_chomik::wrapper_base & my_wrapper)
{
    int portno, sockfd,pid;
    int newsockfd;
    unsigned clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int opt = 1;
	
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (sockfd < 0) {
        HTTP_CHOMIK_LOG_ERROR("http_chomik failed to open socket.");
        exit(EXIT_FAILURE);
    }
    
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        HTTP_CHOMIK_LOG_ERROR("http_chomik failed to set the socket options.");
        exit(EXIT_FAILURE); 
    } 
    
    int a=65535;
	if ( (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &a ,sizeof(int)) ) < 0 )
	{
		HTTP_CHOMIK_LOG_ERROR("http_chomik options");
		exit(EXIT_FAILURE);
	}


    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = my_port;

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        HTTP_CHOMIK_LOG_ERROR("http_chomik error on binding.");
		close(sockfd);
        exit(EXIT_FAILURE);
    }

	listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    while (true) 
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
        if (newsockfd < 0) 
        {
			HTTP_CHOMIK_LOG_ERROR("http_chomik error on accept.");
			exit(EXIT_FAILURE);
        }
      
        /* Create child process */
        pid = fork();
		
        if (pid < 0) {
			HTTP_CHOMIK_LOG_ERROR("http_chomik error on fork.");
            exit(EXIT_FAILURE);
        }
      
        if (pid == 0) {
         /* This is the client process */
            close(sockfd);
            http_chomik::wrapper_base::do_processing_client(newsockfd, my_wrapper);
			HTTP_CHOMIK_LOG_NOTICE("http_chomik client process terminated.");
            
            close(newsockfd);
            exit(EXIT_SUCCESS);
        }
        else 
        {
            close(newsockfd);
        }		
    }

}
