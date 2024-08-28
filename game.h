#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_pixels.h>
#include <string>

enum class GameState { PLAY, EXIT };

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game {
public:
    Game();
    ~Game();
    void Run();
    void HandleEvents();
private:
    void GameLoop();
    void Cleanup();
    void DrawText(std::string str);
    void RenderCharacter(char c, int x, int y);

    GameState gameState;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* fontTexture; // Texture for bitmap font
};
