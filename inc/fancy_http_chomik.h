#ifndef FANCY_HTTP_CHOMIK_H
#define FANCY_HTTP_CHOMIK_H

#include "http_chomik.h"

namespace fancy_http_chomik
{
    class server: public http_chomik::server
    {
    protected:
        virtual void add_html_head_content(std::ostream & message_stream) override;
        
        virtual void add_html_default_chomik_code(std::ostream & message_stream) override;
        
        virtual void add_html_body(std::ostream & message_stream) override;
        
        virtual void add_html_body_for_code(std::ostream & message_stream, const std::string & decoded_code, std::stringstream & error_stream, std::stringstream & error_stream2);

        
        void add_html_common_scripts(std::ostream & message_stream);
        
        void add_html_navbar(std::ostream & message_stream);
        
    public:
        server(int s): http_chomik::server{s} {}
    };
}

#endif
