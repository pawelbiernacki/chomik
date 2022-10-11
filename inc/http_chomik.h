#ifndef HTTP_CHOMIK_H
#define HTTP_CHOMIK_H

#include "chomik.h"
#include <regex>

namespace http_chomik
{    
    class machine: public chomik::machine
    {
    public:    
        virtual void create_predefined_streams() override;
        
        virtual bool get_can_create_files() const override { return false; }        
    };    
    
    
    class server
    {
    private:
        int sock;
    public:
        server(int s): sock{s} {}        
        ~server();        
        void read_data(std::ostream & input_buffer_stream);
        void handle_main(std::ostream & message_stream);
        void handle_incorrect_uri(std::ostream & message_stream);
        void handle_code(std::ostream & message_stream, std::smatch & m);
        
        void send_message(const std::string & message);
    };
}

#define HTTP_CHOMIK_PRODUCTION

#ifdef HTTP_CHOMIK_PRODUCTION
#define HTTP_CHOMIK_LOG_ERROR(X)    \
{                                   \
    std::stringstream s;            \
    s << X;                         \
    syslog (LOG_PERROR, s.str().c_str());   \
}
#define HTTP_CHOMIK_LOG_NOTICE(X)   \
{                                   \
    std::stringstream s;            \
    s << X;                         \
    syslog (LOG_NOTICE, s.str().c_str());   \
}
#else
#define HTTP_CHOMIK_LOG_ERROR(X)    \
{                                   \
    std::stringstream s;            \
    s << X;                         \
    std::cerr << s.str() << "\n";   \
}
#define HTTP_CHOMIK_LOG_NOTICE(X)   \
{                                   \
    std::stringstream s;            \
    s << X;                         \
    std::cout << s.str() << "\n";   \
}
#endif


#endif
 
