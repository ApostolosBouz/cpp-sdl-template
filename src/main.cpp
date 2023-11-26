#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG)
    {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    SDL_Window* m_window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        SDL_DestroyWindow(m_window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex_upper_bg = IMG_LoadTexture(m_renderer, "assets/images/upper_background.jpg");
    SDL_Texture *tex_lower_bg = IMG_LoadTexture(m_renderer, "assets/images/lower_background.jpg");

    SDL_Rect rect_upper_bg, rect_lower_bg;

    int window_width, window_height;
    SDL_GetWindowSize(m_window, &window_width, &window_height);

    if (tex_upper_bg != nullptr)
    {
        // Upper background (3840 x 730), scaled by 2/3 the height of the window
        rect_upper_bg.w = 3840 * (2.0f / 3.0f) * (window_height / 730.0f);
        rect_upper_bg.h = window_height * 2 / 3;
        rect_upper_bg.x = 0;
        rect_upper_bg.y = 0;
    }
    else
    {
        std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
    }

    if (tex_lower_bg != nullptr)
    {
        // Lower background (3840 x 350), scaled by 1/3 the height of the window
        rect_lower_bg.w = 3840 * (1.0f / 3.0f) * (window_height / 350.0f);
        rect_lower_bg.h = window_height / 3;
        rect_lower_bg.x = 0;
        rect_lower_bg.y = rect_upper_bg.h; // Positioned right below the first one
    }
    else
    {
        std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
    }

    int scroll_speed_upper_bg = 1;
    int scroll_speed_lower_bg = 2;

    TTF_Font *font = TTF_OpenFont("assets/fonts/RobotoThin.ttf", 64);
    if (font == nullptr)
    {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Color textColor = {255, 255, 255}; // White color
    SDL_Surface *surfaceMessage = TTF_RenderText_Blended(font, "SDL Template", textColor);
    SDL_Texture *message = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);

    SDL_Rect message_rect;              // create a rect
    message_rect.x = 350;               // controls the rect's x coordinate
    message_rect.y = 280;               // controls the rect's y coordinate
    message_rect.w = surfaceMessage->w; // controls the width of the rect
    message_rect.h = surfaceMessage->h; // controls the height of the rect

    Mix_Music *music = Mix_LoadMUS("assets/audio/PunchDeckICantStop.ogg");
    if (music == nullptr)
    {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        // If there is an error loading the music, we can continue without music.
    }

    if (music != nullptr && Mix_PlayMusic(music, -1) == -1)
    {
        std::cerr << "Mix_PlayMusic Error: " << Mix_GetError() << std::endl;
    }

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Update positions for scrolling
        rect_upper_bg.x -= scroll_speed_upper_bg;
        rect_lower_bg.x -= scroll_speed_lower_bg;

        // Check if images need to be reset
        if (rect_upper_bg.x + rect_upper_bg.w <= window_width)
        {
            rect_upper_bg.x = 0;
        }
        if (rect_lower_bg.x + rect_lower_bg.w <= window_width)
        {
            rect_lower_bg.x = 0;
        }

        // Rendering
        SDL_RenderClear(m_renderer);

        // Render backgrounds
        SDL_RenderCopy(m_renderer, tex_upper_bg, nullptr, &rect_upper_bg);
        SDL_RenderCopy(m_renderer, tex_lower_bg, nullptr, &rect_lower_bg);

        // Other rendering (e.g., message)
        SDL_RenderCopy(m_renderer, message, nullptr, &message_rect);

        SDL_RenderPresent(m_renderer);
        SDL_Delay(1);
    }

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
    if (tex_upper_bg != nullptr)
    {
        SDL_DestroyTexture(tex_upper_bg);
    }
    TTF_CloseFont(font);
    if (music != nullptr)
    {
        Mix_FreeMusic(music);
    }
    Mix_CloseAudio();

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
