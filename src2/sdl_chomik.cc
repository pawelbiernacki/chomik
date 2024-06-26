#include "sdl_chomik.h"
#include "config.h"


sdl_chomik::font::font(machine & m, const std::string & file_path, int font_size)
{
    my_font=TTF_OpenFont(file_path.c_str(), font_size);
    
    if (!my_font)
    {
        std::cerr << "failed to load the font from file " << file_path << "\n";
        SDL_Quit();
        TTF_Quit();
        exit(1);
    }
}

sdl_chomik::font::~font()
{
    TTF_CloseFont(my_font);    
    my_font = nullptr;
}

void sdl_chomik::font::render(machine & m, const std::string & t, int x, int y)
{
    SDL_Color color={0,0,0};
    SDL_Surface *text_surface;
    
    if (!(text_surface=TTF_RenderText_Solid(my_font,t.c_str(),color))) {    
        std::cerr << "TTF error - " << TTF_GetError() << "\n";
    } 
    else 
    {
        SDL_Rect dest;
        SDL_Texture * tex = SDL_CreateTextureFromSurface(m.get_renderer(), text_surface);
        if (!tex)
        {
            std::cerr << "TTF error - " << TTF_GetError() << "\n";
            SDL_FreeSurface(text_surface);
        }
        else
        {        
            SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
            dest.x = x;
            dest.y = y;
            SDL_RenderCopy(m.get_renderer(), tex, NULL, &dest);
            SDL_FreeSurface(text_surface);
            SDL_DestroyTexture(tex);
        }
    }
}



sdl_chomik::image::image(machine & m, const std::string & file_path)
{
    SDL_Surface* surface;
    surface = IMG_Load(file_path.c_str());
    if (surface)
    {
        tex = SDL_CreateTextureFromSurface(m.get_renderer(), surface);
        SDL_FreeSurface(surface); 
    }
    else
    {
        std::cerr << "failed to load the image from file " << file_path << "\n";
        SDL_Quit();
        exit(1);
    }
}

sdl_chomik::image::~image() 
{ 
    SDL_DestroyTexture(tex);
}

sdl_chomik::machine::machine(int w, int h): window_width{w}, window_height{h}
{
    win = SDL_CreateWindow("sdl_chomik " VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
            
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    my_renderer = SDL_CreateRenderer(win, -1, render_flags);
}

sdl_chomik::machine::~machine()
{
    SDL_DestroyRenderer(my_renderer);
 
    SDL_DestroyWindow(win);
}


void sdl_chomik::machine::clear_renderer()
{
    SDL_RenderClear(my_renderer);
}

void sdl_chomik::machine::present_renderer()
{
    SDL_RenderPresent(my_renderer);        
}

void sdl_chomik::machine::create_predefined_variables()
{
    chomik::machine::create_predefined_variables();
    
    chomik::generic_name gn;
    gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("created"));
    gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("image"));
    gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("index"));
    std::shared_ptr<chomik::signature> the_created_image_index=std::make_shared<chomik::signature>(gn);
    add_variable_with_value(std::make_shared<chomik::simple_variable_with_value_integer>(std::move(the_created_image_index), 0));    
    
    chomik::generic_name gn2;
    gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("sdl"));
    gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("loop"));
    gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("body"));
    std::shared_ptr<chomik::signature> sdl_loop_body=std::make_shared<chomik::signature>(gn2);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(sdl_loop_body), std::make_unique<chomik::code>()));    

    chomik::generic_name gn3;
    gn3.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn3.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn3.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("left"));
    std::shared_ptr<chomik::signature> on_key_left=std::make_shared<chomik::signature>(gn3);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_left), std::make_unique<chomik::code>()));    

    chomik::generic_name gn4;
    gn4.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn4.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn4.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("right"));
    std::shared_ptr<chomik::signature> on_key_right=std::make_shared<chomik::signature>(gn4);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_right), std::make_unique<chomik::code>()));    

    chomik::generic_name gn5;
    gn5.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn5.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn5.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("down"));
    std::shared_ptr<chomik::signature> on_key_down=std::make_shared<chomik::signature>(gn5);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_down), std::make_unique<chomik::code>()));    

    chomik::generic_name gn6;
    gn6.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn6.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn6.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("up"));
    std::shared_ptr<chomik::signature> on_key_up=std::make_shared<chomik::signature>(gn6);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_up), std::make_unique<chomik::code>()));    

    chomik::generic_name gn7;
    gn7.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn7.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("created"));
    gn7.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("font"));
    gn7.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("index"));
    std::shared_ptr<chomik::signature> the_created_font_index=std::make_shared<chomik::signature>(gn7);
    add_variable_with_value(std::make_shared<chomik::simple_variable_with_value_integer>(std::move(the_created_font_index), 0));    

    chomik::generic_name gn8;
    gn8.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn8.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn8.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("return"));
    std::shared_ptr<chomik::signature> on_key_return=std::make_shared<chomik::signature>(gn8);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_return), std::make_unique<chomik::code>()));    

    chomik::generic_name gn9;
    gn9.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn9.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn9.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("backspace"));
    std::shared_ptr<chomik::signature> on_key_backspace=std::make_shared<chomik::signature>(gn9);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_backspace), std::make_unique<chomik::code>()));    

    chomik::generic_name gn10;
    gn10.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn10.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn10.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("escape"));
    std::shared_ptr<chomik::signature> on_key_escape=std::make_shared<chomik::signature>(gn10);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_escape), std::make_unique<chomik::code>()));    

    chomik::generic_name gn11;
    gn11.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn11.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
    gn11.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("space"));
    std::shared_ptr<chomik::signature> on_key_space=std::make_shared<chomik::signature>(gn11);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_key_space), std::make_unique<chomik::code>()));        

    chomik::generic_name gn12;
    gn12.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn12.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("sdl"));
    gn12.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("window"));
    gn12.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("width"));
    std::shared_ptr<chomik::signature> the_sdl_window_width=std::make_shared<chomik::signature>(gn12);
    add_variable_with_value(std::make_shared<chomik::simple_variable_with_value_integer>(std::move(the_sdl_window_width), window_width));

    chomik::generic_name gn13;
    gn13.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn13.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("sdl"));
    gn13.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("window"));
    gn13.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("height"));
    std::shared_ptr<chomik::signature> the_sdl_window_height=std::make_shared<chomik::signature>(gn13);
    add_variable_with_value(std::make_shared<chomik::simple_variable_with_value_integer>(std::move(the_sdl_window_height), window_height));


    chomik::generic_name gn14;
    gn14.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn14.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
    gn14.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("x"));
    std::shared_ptr<chomik::signature> the_mouse_x=std::make_shared<chomik::signature>(gn14);
    add_variable_with_value(std::make_shared<chomik::simple_variable_with_value_integer>(std::move(the_mouse_x), 0));

    chomik::generic_name gn15;
    gn15.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
    gn15.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
    gn15.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("y"));
    std::shared_ptr<chomik::signature> the_mouse_y=std::make_shared<chomik::signature>(gn15);
    add_variable_with_value(std::make_shared<chomik::simple_variable_with_value_integer>(std::move(the_mouse_y), 0));

    chomik::generic_name gn16;
    gn16.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn16.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
    gn16.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("motion"));
    std::shared_ptr<chomik::signature> on_mouse_motion=std::make_shared<chomik::signature>(gn16);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_mouse_motion), std::make_unique<chomik::code>()));


    chomik::generic_name gn17;
    gn17.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn17.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
    gn17.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("button"));
    gn17.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("left"));
    std::shared_ptr<chomik::signature> on_mouse_button_left=std::make_shared<chomik::signature>(gn17);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_mouse_button_left), std::make_unique<chomik::code>()));

    chomik::generic_name gn18;
    gn18.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
    gn18.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
    gn18.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("button"));
    gn18.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("right"));
    std::shared_ptr<chomik::signature> on_mouse_button_right=std::make_shared<chomik::signature>(gn18);
    add_variable_with_value(std::make_shared<chomik::variable_with_value_code>(std::move(on_mouse_button_right), std::make_unique<chomik::code>()));

}

bool sdl_chomik::machine::get_is_user_defined_executable(const chomik::signature & s) const
{
    if (s.get_vector_of_items().size() == 2)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("sdl") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("loop"))
        {
            return true;
        }
        if (s.get_vector_of_items()[0]->get_it_is_identifier("sdl")
            && s.get_vector_of_items()[1]->get_it_is_identifier("quit"))
        {
            return true;
        }
    }
    else
    if (s.get_vector_of_items().size() == 3)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("on")
            && s.get_vector_of_items()[1]->get_it_is_identifier("mouse")
            && s.get_vector_of_items()[2]->get_it_is_identifier("motion"))
        {
            return true;
        }
    }
    else
    if (s.get_vector_of_items().size() == 4)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("create") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("new")
            && s.get_vector_of_items()[2]->get_it_is_identifier("image")
            && s.get_vector_of_items()[3]->get_it_is_string())
        {
            return true;
        }
        if (s.get_vector_of_items()[0]->get_it_is_identifier("on")
            && s.get_vector_of_items()[1]->get_it_is_identifier("mouse")
            && s.get_vector_of_items()[2]->get_it_is_identifier("button")
            && s.get_vector_of_items()[2]->get_it_is_identifier("left"))
        {
            return true;
        }
        if (s.get_vector_of_items()[0]->get_it_is_identifier("on")
            && s.get_vector_of_items()[1]->get_it_is_identifier("mouse")
            && s.get_vector_of_items()[2]->get_it_is_identifier("button")
            && s.get_vector_of_items()[2]->get_it_is_identifier("right"))
        {
            return true;
        }
    }
    else
    if (s.get_vector_of_items().size() == 5)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("show") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("image")
            && s.get_vector_of_items()[2]->get_it_is_integer()
            && s.get_vector_of_items()[3]->get_it_is_integer()
            && s.get_vector_of_items()[4]->get_it_is_integer())
        {
            return true;
        }
        else
        if (s.get_vector_of_items()[0]->get_it_is_identifier("create") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("new")
            && s.get_vector_of_items()[2]->get_it_is_identifier("font")
            && s.get_vector_of_items()[3]->get_it_is_string()
            && s.get_vector_of_items()[4]->get_it_is_integer())
        {
            return true;
        }
    }
    else
    if (s.get_vector_of_items().size() == 6)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("show")
            && s.get_vector_of_items()[1]->get_it_is_identifier("text")
            && s.get_vector_of_items()[2]->get_it_is_integer()   // font index
            && s.get_vector_of_items()[3]->get_it_is_string()   
            && s.get_vector_of_items()[4]->get_it_is_integer()
            && s.get_vector_of_items()[5]->get_it_is_integer())
        {
            return true;
        }
    }
    
    return false;
}

void sdl_chomik::image::render(machine & m, int x, int y)
{
    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = x;
    dest.y = y;
    SDL_RenderCopy(m.get_renderer(), tex, NULL, &dest);
}
        
void sdl_chomik::machine::execute_user_defined_executable(const chomik::signature & s)
{
    if (s.get_vector_of_items().size() == 2)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("sdl") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("loop"))
        {
            while (!close) 
            {
                SDL_Event event;
 
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
 
                    case SDL_QUIT:
                        close = true;
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        switch (event.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("button"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("left"));

                                chomik::signature s0{gn};

                                chomik::code ci;
                                get_variable_value_code(s0, ci);
                                ci.execute(*this);
                            }
                            break;
                            case SDL_BUTTON_RIGHT:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("button"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("right"));

                                chomik::signature s0{gn};

                                chomik::code ci;
                                get_variable_value_code(s0, ci);
                                ci.execute(*this);
                            }
                            break;
                            default:

                            break;
                        }
                        break;

                    case SDL_MOUSEMOTION:
                        {
                            chomik::generic_name gn_x;
                            gn_x.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
                            gn_x.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
                            gn_x.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("x"));
                            chomik::signature s_x{gn_x};
                            get_variable_with_value(s_x).assign_value_integer(event.motion.x);

                            chomik::generic_name gn_y;
                            gn_y.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
                            gn_y.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
                            gn_y.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("y"));
                            chomik::signature s_y{gn_y};
                            get_variable_with_value(s_y).assign_value_integer(event.motion.y);

                            chomik::generic_name gn;
                            gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                            gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("mouse"));
                            gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("motion"));

                            chomik::signature s0{gn};

                            chomik::code ci;
                            get_variable_value_code(s0, ci);
                            ci.execute(*this);
                        }
                        break;
                        
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.scancode) 
                        {                            
                            case SDL_SCANCODE_RETURN:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("return"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }
                                break;
                            case SDL_SCANCODE_SPACE:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("space"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }
                                break;
                                
                            case SDL_SCANCODE_ESCAPE:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("escape"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }
                                break;
                                
                            case SDL_SCANCODE_BACKSPACE:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("backspace"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }
                                break;

                                
                            case SDL_SCANCODE_UP:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("up"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }
                                break;

                            case SDL_SCANCODE_LEFT:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("left"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }                    
                                break;

                            case SDL_SCANCODE_DOWN:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("down"));
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }                                
                                break;
                                
                            case SDL_SCANCODE_RIGHT:
                            {
                                chomik::generic_name gn;
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("on"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("key"));
                                gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("right"));
                                
                                chomik::signature s0{gn};
                
                                chomik::code ci;                
                                get_variable_value_code(s0, ci);                
                                ci.execute(*this);
                            }
                                break;
                                
                            default:
                                break;
                        }
                        break;
                    }
                }                                                
                
                clear_renderer();
                
                chomik::generic_name gn2;
                gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("sdl"));
                gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("loop"));
                gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("body"));
                chomik::signature s2{gn2};
                
                chomik::code ci;

                clear_renderer();
                get_variable_value_code(s2, ci);                
                ci.execute(*this);
                
                present_renderer();
                
                SDL_Delay(1000 / 60);
            }            
        }
        else
        if (s.get_vector_of_items()[0]->get_it_is_identifier("sdl")
            && s.get_vector_of_items()[1]->get_it_is_identifier("quit"))
        {
            close = true;
        }
    }
    else
    if (s.get_vector_of_items().size() == 4)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("create") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("new")
            && s.get_vector_of_items()[2]->get_it_is_identifier("image")
            && s.get_vector_of_items()[3]->get_it_is_string())
        {
            add_image(std::make_unique<image>(*this, s.get_vector_of_items()[3]->get_value_string()));
            
            chomik::generic_name gn2;
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("created"));
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("image"));
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("index"));
            chomik::signature the_created_image_index{gn2};            
            get_variable_with_value(the_created_image_index).assign_value_integer(get_last_created_image_index());
        }
    }
    else
    if (s.get_vector_of_items().size() == 5)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("show") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("image")
            && s.get_vector_of_items()[2]->get_it_is_integer()
            && s.get_vector_of_items()[3]->get_it_is_integer()
            && s.get_vector_of_items()[4]->get_it_is_integer())
        {
            int index = s.get_vector_of_items()[2]->get_value_integer();
            
            if (index >= 0 && index <vector_of_images.size())
            {
                vector_of_images[index]->render(*this, s.get_vector_of_items()[3]->get_value_integer(), s.get_vector_of_items()[4]->get_value_integer());
            }
        }
        else
        if (s.get_vector_of_items()[0]->get_it_is_identifier("create") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("new")
            && s.get_vector_of_items()[2]->get_it_is_identifier("font")
            && s.get_vector_of_items()[3]->get_it_is_string()
            && s.get_vector_of_items()[4]->get_it_is_integer())
        {
            add_font(std::make_unique<font>(*this, s.get_vector_of_items()[3]->get_value_string(), s.get_vector_of_items()[4]->get_value_integer()));

            chomik::generic_name gn2;
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("created"));
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("font"));
            gn2.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("index"));
            chomik::signature the_created_font_index{gn2};            
            get_variable_with_value(the_created_font_index).assign_value_integer(get_last_created_font_index());            
        }                
    }
    else
    if (s.get_vector_of_items().size() == 6)
    {
        if (s.get_vector_of_items()[0]->get_it_is_identifier("show") 
            && s.get_vector_of_items()[1]->get_it_is_identifier("text")
            && s.get_vector_of_items()[2]->get_it_is_integer()  // font index
            && s.get_vector_of_items()[3]->get_it_is_string()
            && s.get_vector_of_items()[4]->get_it_is_integer()
            && s.get_vector_of_items()[5]->get_it_is_integer())
        {
            int index = s.get_vector_of_items()[2]->get_value_integer();
            
            if (index >= 0 && index <vector_of_fonts.size())
            {
                vector_of_fonts[index]->render(*this, s.get_vector_of_items()[3]->get_value_string(), 
                                               s.get_vector_of_items()[4]->get_value_integer(), s.get_vector_of_items()[5]->get_value_integer());
            }
        }        
    }
}



chomik::program the_program;
chomik::parser the_parser{the_program};

int main(int argc, char * argv[])
{
    int width = 800, height = 600;
    std::string filename = "";

    chomik::parser::register_parser(&the_parser);
    int return_value = 0;


    for (int a=1; a < argc; a++)
    {
        if (!strcmp(argv[a], "-w") || !strcmp(argv[a], "--width"))
        {
            width = std::stoi(argv[a+1]);
            a++;
        }
        else
        if (!strcmp(argv[a], "-h") || !strcmp(argv[a], "--height"))
        {
            height = std::stoi(argv[a+1]);
            a++;
        }
        else
        {
            filename = argv[a];
        }
    }


    if (filename == "")
    {
        std::cerr << "usage: " << argv[0] << " [-w|--width <window width>] [-h|--height <window height>] <filename>\n";
        return_value = 1;
    }
    else
    {
        if (the_parser.parse(filename.c_str()) == 0)
        {
            if (TTF_Init()==-1) 
            {
                std::cerr << TTF_GetError() << "\n";
                exit(1);
            }
            
            if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
            {
                TTF_Quit();
                std::cerr << SDL_GetError() << "\n";
                exit(1);
            }
            sdl_chomik::machine m{width, height};
            m.create_predefined_types();
            m.create_predefined_variables();
            m.create_predefined_streams();
                        
            the_program.execute(m);      
            
            
            chomik::generic_name gn;
            gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("the"));
            gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("program"));
            gn.add_generic_name_item(std::make_shared<chomik::identifier_name_item>("return"));
            chomik::signature s0{gn};
                        
            return_value = m.get_variable_value_integer(s0);                        
        }        
        
        SDL_Quit();            
        TTF_Quit();
    }
        
    return return_value;
} 
