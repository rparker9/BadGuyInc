#include "game.h"

Game::Game() : window(nullptr), renderer(nullptr), fontTexture(nullptr), gameState(GameState::PLAY)
{
	// Exit if initialization fails
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		gameState = GameState::EXIT;
		return;
	}

	// Create window and renderer
	window = SDL_CreateWindow("BadGuyInc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Exit if window or renderer creation fails
	if (window == nullptr || renderer == nullptr)
	{
		std::cerr << "Window or Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		gameState = GameState::EXIT;
		return;
	}

	// Load bitmap font surface
	SDL_Surface* fontSurface = SDL_LoadBMP("font.bmp");

	// Exit if surface creation fails
	if (fontSurface == nullptr) 
	{
		std::cerr << "Unable to load bitmap font! SDL_Error: " << SDL_GetError() << std::endl;
		gameState = GameState::EXIT;
		return;
	}

	// Set the color key (transparent color) — replace (255, 0, 255) with your background color
	SDL_SetColorKey(fontSurface, SDL_TRUE, SDL_MapRGB(fontSurface->format, 255, 0, 255));

	// Convert the surface to a texture
	fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);

	// Exit if texture creation fails
	if (fontTexture == nullptr) 
	{
		std::cerr << "Unable to create texture from bitmap font! SDL_Error: " << SDL_GetError() << std::endl;
		gameState = GameState::EXIT;
		return;
	}

	// Set texture blend mode to blend
	SDL_SetTextureBlendMode(fontTexture, SDL_BLENDMODE_BLEND);
}

Game::~Game()
{
	Cleanup();
}

void Game::Run()
{
	if (gameState == GameState::PLAY)
		GameLoop();
}

void Game::GameLoop()
{
	const int FPS_CAP = 60;
	const int FRAME_DELAY = 1000 / FPS_CAP;

	while (gameState != GameState::EXIT)
	{
		// Record the start time of the frame
		Uint32 startTicks = SDL_GetTicks();

		// HANDLE ALL INCOMING EVENTS
		HandleEvents();

		// Clear the renderer with a black color
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// RUN GAME CODE
		// -----------------
		
		// Calculate the elapsed time for this frame
		Uint32 endTicks = SDL_GetTicks();
		Uint32 frameTicks = endTicks - startTicks;

		// Calculate and render FPS
		float fps = frameTicks > 0 ? (1000.0f / frameTicks) : 0.0f;
		//DrawText("FPS: " + std::to_string(static_cast<int>(fps)));

		DrawText("Hello World!");

		// Present the renderer
		SDL_RenderPresent(renderer);

		// Cap the frame rate to 60 FPS
		if (frameTicks < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY - frameTicks);
		}
	}
}

/// <summary>
/// Polls for events to check if GameState::EXIT has been triggered
/// </summary>
void Game::HandleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			gameState = GameState::EXIT;
		}
	}
}

void Game::RenderCharacter(char c, int x, int y)
{
	if (!fontTexture)
		return;

	int charWidth = 12;  // Width of each character in the bitmap
	int charHeight = 12; // Height of each character in the bitmap
	int numCols = 16;    // Number of columns in the bitmap font image

	int ascii = static_cast<int>(c); // Get ASCII value of the character
	int col = ascii % numCols;       // Column in the image
	int row = ascii / numCols;       // Row in the image

	SDL_Rect srcRect = { col * charWidth, row * charHeight, charWidth, charHeight };
	SDL_Rect dstRect = { x, y, charWidth, charHeight };

	SDL_RenderCopy(renderer, fontTexture, &srcRect, &dstRect);
}

void Game::DrawText(std::string str)
{
	int x = 10; // Starting x position for the text
	int y = 10; // Starting y position for the text
	int charWidth = 16; // Width of each character

	for (char c : str)
	{
		RenderCharacter(c, x, y);
		x += charWidth; // Move to the next character position
	}
}

void Game::Cleanup()
{
	if (fontTexture)
	{
		SDL_DestroyTexture(fontTexture);
		fontTexture = nullptr;
	}
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Quit();
}