#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    SDL_Window* m_window = SDL_CreateWindow("SDL Template", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        SDL_DestroyWindow(m_window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture* background = IMG_LoadTexture(m_renderer, "assets/images/upper_background.jpg");
    SDL_Rect destRect;

    if (background != nullptr) {
        // Get the width and height of the background texture
        int bgWidth, bgHeight;
        SDL_QueryTexture(background, nullptr, nullptr, &bgWidth, &bgHeight);

        // Calculate the scale factor to fit the window, preserving aspect ratio
        float scaleX = 800 / (float)bgWidth;
        float scaleY = 600 / (float)bgHeight;
        float scale = std::min(scaleX, scaleY);

        // Calculate the destination rectangle
        destRect.w = (int)(bgWidth * scale);
        destRect.h = (int)(bgHeight * scale);
        destRect.x = (800 - destRect.w) / 2; // Center the image horizontally
        destRect.y = (600 - destRect.h) / 2; // Center the image vertically
    }
    else
    {
        std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
    }


    TTF_Font* font = TTF_OpenFont("assets/fonts/RobotoThin.ttf", 64);
    if (font == nullptr) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Color textColor = { 255, 255, 255 }; // White color
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, "SDL Template", textColor);
    SDL_Texture* message = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);

    SDL_Rect message_rect; //create a rect
    message_rect.x = 350;  //controls the rect's x coordinate 
    message_rect.y = 280; // controls the rect's y coordinate
    message_rect.w = surfaceMessage->w; // controls the width of the rect
    message_rect.h = surfaceMessage->h; // controls the height of the rect

    Mix_Music* music = Mix_LoadMUS("assets/audio/PunchDeckICantStop.ogg");
    if (music == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        // If there is an error loading the music, we can continue without music.
    }

    if (music != nullptr && Mix_PlayMusic(music, -1) == -1) {
        std::cerr << "Mix_PlayMusic Error: " << Mix_GetError() << std::endl;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        if (background != nullptr) {
            SDL_RenderCopy(m_renderer, background, nullptr, &destRect); // Render the background texture
        } else {
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);  // Set the drawing color to black
            SDL_RenderClear(m_renderer);  // Clear the entire screen with the drawing color.
        }
        SDL_RenderCopy(m_renderer, message, nullptr, &message_rect); // render the message texture
        SDL_RenderPresent(m_renderer);  // Make the changes appear on screen
        SDL_Delay(1);  // Delay to reduce CPU usage
    }

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
    if (background != nullptr) {
        SDL_DestroyTexture(background);
    }
    TTF_CloseFont(font);
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
    Mix_CloseAudio();

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
