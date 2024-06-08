#ifndef SDL_CHOMIK_H
#define SDL_CHOMIK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h> 
#include <SDL2/SDL_ttf.h>

#include "chomik.h"

namespace sdl_chomik
{
    class image;
    class font;
    
    class machine: public chomik::machine
    {
    private:
        SDL_Window* win;
        SDL_Renderer * my_renderer;
        
        std::vector<std::unique_ptr<image>> vector_of_images;
        
        std::vector<std::unique_ptr<font>> vector_of_fonts;

        int window_width, window_height;

        bool close = false;
        
    public:
        machine(int w, int h);
        virtual ~machine();
        
        void clear_renderer();        
        void present_renderer();
        
        SDL_Renderer * get_renderer() { return my_renderer; }
        
        void add_image(std::unique_ptr<image> && i)
        {
            vector_of_images.push_back(std::move(i));
        }
        
        void add_font(std::unique_ptr<font> && f)
        {
            vector_of_fonts.push_back(std::move(f));
        }

        
        virtual void create_predefined_variables() override;
        
        virtual bool get_is_user_defined_executable(const chomik::signature & s) const override;
        
        virtual void execute_user_defined_executable(const chomik::signature & s) override;
        
        int get_last_created_image_index() const { return vector_of_images.size()-1; }
        
        int get_last_created_font_index() const { return vector_of_fonts.size()-1; }
    };

    class image
    {
    private:
        SDL_Texture* tex;
    public:
        image(machine & m, const std::string & file_path);
        virtual ~image();
                
        void render(machine & m, int x, int y);
    };
    
    class font
    {
    private:
        TTF_Font * my_font;
    public:
        font(machine & m, const std::string & file_path, int font_size);
        virtual ~font();
        void render(machine & m, const std::string & t, int x, int y);
    };
    
}


#endif
