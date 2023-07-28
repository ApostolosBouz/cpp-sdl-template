#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Function to load an image as an SDL_Texture
SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Error loading image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

// Function to load a font and render text as an SDL_Texture
SDL_Texture* loadFont(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, const std::string& text, SDL_Color textColor) {
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    return texture;
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        // You can still continue the program if font initialization fails, but font rendering won't work
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cerr << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
        // You can still continue the program if audio initialization fails, but music won't play
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        // You can still continue the program if window creation fails, but the window won't show up
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        // You can still continue the program if renderer creation fails, but graphics won't be displayed
    }

    // Load background textures
    bool texturesLoaded = true;
    SDL_Texture* upperTexture = loadTexture(renderer, "upper_background.png");
    SDL_Texture* lowerTexture = loadTexture(renderer, "lower_background.png");
    if (!upperTexture || !lowerTexture) {
        std::cerr << "Failed to load background textures." << std::endl;
        texturesLoaded = false;
        // You can still continue the program if background texture loading fails, but the textures won't be displayed
    }

    // Load font and render "SDL Template" text
    bool fontLoaded = true;
    TTF_Font* font = TTF_OpenFont("Roboto-Regular.ttf", 36);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        fontLoaded = false;
        // You can still continue the program if font loading fails, but the text won't be displayed
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; // White color for text
    SDL_Texture* textTexture = loadFont(renderer, "Roboto-Regular.ttf", 36, "SDL Template", textColor);
    if (!textTexture) {
        std::cerr << "Failed to render text." << std::endl;
        fontLoaded = false;
        // You can still continue the program if text rendering fails, but the text won't be displayed
    }

    // Set the blend mode to enable transparency
    SDL_SetTextureBlendMode(upperTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(lowerTexture, SDL_BLENDMODE_BLEND);

    // Clear the screen with black color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Get the dimensions of the text texture
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);

    // Calculate the position to center the text
    int textX = (SCREEN_WIDTH - textWidth) / 2;
    int textY = (SCREEN_HEIGHT - textHeight) / 2;

    // Load music
    Mix_Music* music = Mix_LoadMUS("music.mp3");
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        // You can still continue the program if music loading fails, but music won't play
    } else {
        // Play music if loaded successfully
        Mix_PlayMusic(music, -1); // The second parameter (-1) means loop indefinitely
    }

    // Fade out effect (reduce alpha from 255 to 0)
    if (texturesLoaded && fontLoaded) {
        for (int alpha = 255; alpha >= 0; alpha -= 5) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
            SDL_RenderClear(renderer);

            // Render upper and lower background textures
            SDL_RenderCopy(renderer, upperTexture, nullptr, nullptr);
            SDL_RenderCopy(renderer, lowerTexture, nullptr, nullptr);

            // Render the text in the middle of the window
            SDL_SetTextureAlphaMod(textTexture, alpha);
            SDL_Rect textRect = { textX, textY, textWidth, textHeight };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_RenderPresent(renderer);
            SDL_Delay(50); // Delay to control fade speed (adjust as needed)
        }
    }

    // Release resources
    if (music) {
        Mix_FreeMusic(music);
    }
    TTF_CloseFont(font);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(upperTexture);
    SDL_DestroyTexture(lowerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL_mixer
    Mix_CloseAudio();

    // Quit SDL_ttf
    TTF_Quit();

    // Quit SDL
    SDL_Quit();
	
	return 0;
}