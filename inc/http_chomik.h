#ifndef HTTP_CHOMIK_H
#define HTTP_CHOMIK_H

#include "chomik.h"
#include <regex>

namespace http_chomik
{    
    /**
     * An extension of the chomik::machine, it must not create file streams.
     */
    class machine: public chomik::machine
    {
    public:    
        virtual void create_predefined_streams() override;
        
        virtual bool get_can_create_files() const override final { return false; }        
    };    

    class server;
    
    class wrapper_base
    {
    protected:
        virtual void create_a_new_server(std::unique_ptr<server> & target, int sock) = 0;
                
        /**
         * You may override this method in your own wrappers to handle multiple URIs.
         * Please call also the original version (i.e. wrapper_base::process).
         */
        virtual void process(const std::string & uri, std::ostream & message_stream, std::string & response_type, std::unique_ptr<http_chomik::server> & my_server);
        
    public:
        static void do_processing_client(int sock, wrapper_base & my_wrapper);
    };
    
    /**
     * You may inherit this class and redefine some virtual functions to tune the server's behaviour.
     */
    class server
    {
    private:
        int sock;
        
    protected:
        /**
         * Feel free to override this function if you wish another code in the HTML head.
         * Consider invoking the original version.
         */
        virtual void add_html_head_content(std::ostream & message_stream);
        
        /**
         * Remember to pack the code properly as HTML.
         */
        virtual void add_html_default_chomik_code(std::ostream & message_stream);
        
        /**
         * You may also change the whole body HTML by redefining this method.
         */
        virtual void add_html_body(std::ostream & message_stream);
        
        /**
         * This method can be used to redefine the whole document body when handling incorrect URI.
         */
        virtual void add_html_body_for_incorrect_uri(std::ostream & message_stream);
                
        /**
         * You may redefine it to handle the code processing.
         */
        virtual void add_html_body_for_code(std::ostream & message_stream, const std::string & decoded_code, std::stringstream & error_stream, 
                                            std::stringstream & error_stream2);
        
        
        virtual void create_a_new_machine(std::unique_ptr<machine> & target);
        
        
    public:
        server(int s): sock{s} {}        
        ~server();        
        void read_data(std::ostream & input_buffer_stream);
        void handle_main(std::ostream & message_stream);
        void handle_incorrect_uri(std::ostream & message_stream);
        void handle_code(std::ostream & message_stream, std::smatch & m);
        void handle_chomik_image(std::ostream & message_stream);
        void handle_stylesheet(std::ostream & message_stream);
        
        void send_message(const std::string & message);
        
        static void do_processing(int my_port, wrapper_base & my_wrapper);
    };
    
    /**
     * You may use this template for any server class provided it takes only the socket number in its constructor.
     * You may also inherit the wrapper_base and provide your own implementation of the wrapper_base::create_a_new_server.
     */
    template <typename SERVER_CLASS> class wrapper: public wrapper_base
    {
    protected:
        virtual void create_a_new_server(std::unique_ptr<server> & target, int sock) override
        {
            target = std::move(std::make_unique<SERVER_CLASS>(sock));
        }
    public:
        
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
 
