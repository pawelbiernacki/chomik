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



void http_chomik::wrapper_base::process(const std::string & uri, std::ostream & message_stream, std::string & response_type, std::unique_ptr<http_chomik::server> & my_server)
{
    std::regex code_regex("/\\?code=(.+)");
    
    if (uri == "/")
    {
        my_server->handle_main(message_stream);        
    }
    else
    if (uri == "/chomik.png")
    {
        my_server->handle_chomik_image(message_stream);
        response_type="image/png";
    }
    else
    if (uri == "/chomik.css")
    {
        my_server->handle_stylesheet(message_stream);
        response_type="text/css";        
    }
    else
    if (std::smatch m; std::regex_search(uri, m, code_regex))
    {
        my_server->handle_code(message_stream, m);
    }
    else
    {
        my_server->handle_incorrect_uri(message_stream);
    }        
}


void http_chomik::wrapper_base::do_processing_client(int sock, wrapper_base & my_wrapper) 
{
	std::unique_ptr<http_chomik::server> my_server;
    
    my_wrapper.create_a_new_server(my_server, sock);
        	
	HTTP_CHOMIK_LOG_NOTICE("http_chomik client processing.");
    
    std::stringstream input_buffer_stream;    
    my_server->read_data(input_buffer_stream);
    				
    //std::cout << input_buffer_stream.str();
        
    std::stringstream data_stream(input_buffer_stream.str());
    
    
    std::string line;
    std::vector<std::string> vector_of_header_lines;
    do
    {
        std::getline(data_stream, line);        
        if (line=="")
        {
            break;
        }
        std::cout << "HEADER " << line << "\n";        
        vector_of_header_lines.push_back(line);
    }
    while (true);
    
    std::stringstream first_line(vector_of_header_lines[0]);
    std::string method_name, uri, protocol_name;
        
    first_line >> method_name >> uri >> protocol_name;
    
    std::cout << "URI " << uri << "\n";
        
    std::stringstream message_stream;
    std::string response_type="text/html";
        
    my_wrapper.process(uri, message_stream, response_type, my_server);
        
    std::stringstream response_stream;
    
    response_stream << 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: " << message_stream.str().length() << "\r\n"
        "Content-Type: " << response_type << "\r\n"
        "Server http_chomik 0.0.0\r\n"
        "Connection: Closed\r\n"
        "\r\n"
        << message_stream.str();
            
    my_server->send_message(response_stream.str());
    
    std::cout << response_stream.str() << "\n";
}
 
