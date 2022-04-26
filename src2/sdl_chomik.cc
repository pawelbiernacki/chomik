#include "sdl_chomik.h"
#include "config.h"


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
        exit(0);
    }
}

sdl_chomik::image::~image() 
{ 
    SDL_DestroyTexture(tex);
}

sdl_chomik::machine::machine()
{
    win = SDL_CreateWindow("sdl_chomik " VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
            
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
            bool close=false;
            
            while (!close) 
            {
                SDL_Event event;
 
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
 
                    case SDL_QUIT:
                        close = true;
                        break;
                        
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.scancode) 
                        {                            
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
    }
}



chomik::program the_program;
chomik::parser the_parser{the_program};

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <filename>\n";
    }
    else
    {
        if (the_parser.parse(argv[1]) == 0)
        {            
            if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
            {
                std::cerr << SDL_GetError() << "\n";
            }
            sdl_chomik::machine m;
            m.create_predefined_types();
            m.create_predefined_variables();
            m.create_predefined_streams();
                        
            the_program.execute(m);

            SDL_Quit();            
        }
    }
        
    return 0;
} 