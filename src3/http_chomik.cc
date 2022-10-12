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


void http_chomik::machine::create_predefined_streams()
{
    // we do not call the original implementation here!
    // we want our streams to replace the original ones!
        
    add_stream(std::make_unique<chomik::generic_stream_stringstream>());
    add_stream(std::make_unique<chomik::generic_stream_stringstream>());
    add_stream(std::make_unique<chomik::generic_stream_stringstream>());        
}


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


void http_chomik::server::handle_main(std::ostream & message_stream)
{
    message_stream 
        << "<html>"    
        << "<head><meta charset=\"UTF-8\"><link rel=\"stylesheet\" href=\"chomik.css\"></head>"
        << "<body><image src=\"chomik.png\"/><h1>Welcome to our http_chomik server</h1><h3>by Paweł Biernacki</h3><form>"
        << "<textarea cols=\"80\" rows=\"40\" name=\"code\">&lt;print \"hello world\"&gt;;</textarea>"
        << "<button type=\"submit\">run</button></form><br/>"
        << "<a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a></body>"
        << "</html>";
}


void http_chomik::server::handle_incorrect_uri(std::ostream & message_stream)
{
    message_stream 
        << "<html>"    
        << "<head><meta charset=\"UTF-8\"><link rel=\"stylesheet\" href=\"chomik.css\"></head>"
        << "<body><h1>Sorry, http_chomik does not understand this URI</h1><br/><button onclick=\"history.back()\">go back</button><br/><br/><a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a></body>"
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


static void doprocessing_client(int sock) 
{
	http_chomik::server my_server{sock};
	
	HTTP_CHOMIK_LOG_NOTICE("http_chomik client processing.");
    
    std::stringstream input_buffer_stream;    
    my_server.read_data(input_buffer_stream);
    				
    std::cout << input_buffer_stream.str();		
        
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
    
    std::regex code_regex("/\\?code=(.+)");
    
    if (uri == "/")
    {
        my_server.handle_main(message_stream);        
    }
    else
    if (uri == "/chomik.png")
    {
        my_server.handle_chomik_image(message_stream);
        response_type="image/png";
    }
    else
    if (uri == "/chomik.css")
    {
        my_server.handle_stylesheet(message_stream);
        response_type="text/css";        
    }
    else
    if (std::smatch m; std::regex_search(uri, m, code_regex))
    {
        my_server.handle_code(message_stream, m);
    }
    else
    {
        my_server.handle_incorrect_uri(message_stream);
    }    
    
    std::stringstream response_stream;
    
    response_stream << 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: " << message_stream.str().length() << "\r\n"
        "Content-Type: " << response_type << "\r\n"
        "Server http_chomik 0.0.0\r\n"
        "Connection: Closed\r\n"
        "\r\n"
        << message_stream.str();
            
    my_server.send_message(response_stream.str());
    
    std::cout << response_stream.str() << "\n";
}

static void doprocessing()
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
            doprocessing_client(newsockfd);
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


chomik::program the_program;
chomik::parser the_parser{the_program};


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
        << "<head><meta charset=\"UTF-8\"><link rel=\"stylesheet\" href=\"chomik.css\"></head>"
        << "<body><image src=\"chomik.png\"/><h3>Result:</h3><pre>";
    
    if (the_parser.parse_string(decoded_code_stream.str(), error_stream)==0)
    {
        http_chomik::machine m;
        chomik::machine::current_runtime_warning_stream = &error_stream2;
        
        m.create_predefined_types();
        m.create_predefined_variables();
        m.create_predefined_streams();
        
        the_program.execute(m);           
        
        message_stream << m.get_stream(0).get_result() << error_stream2.str();
    }
    else
    {
        message_stream << error_stream.str();
    }
    
    message_stream << "</pre><button onclick=\"history.back()\">go back</button><br/><br/><a href=\"https://www.perkun.org/chomik\">https://www.perkun.org/chomik</a></body>"
        << "</html>";    
}


int main(int argc, char * argv[])
{
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
    
    doprocessing();
    
    return EXIT_SUCCESS;	
}
