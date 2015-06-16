//Using SDL and standard IO
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gDude = NULL;

//Startup image
SDL_Surface* gStart = NULL;

//The rectangle to load the dude into
SDL_Rect rDude;

//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gScratch = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
		//Initialize SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load images
	gDude = SDL_LoadBMP("dude.bmp");
	if (gDude == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "dude.bmp", SDL_GetError());
		success = false;
	}
	gStart = SDL_LoadBMP("b.bmp");
	if (gDude == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "dude.bmp", SDL_GetError());
		success = false;
	}
	rDude.w = 1920;
	rDude.h = 1080;

	//Load sound effects
	gScratch = Mix_LoadWAV("1.wav");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gDude);
	gDude = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Free the sound effects
	Mix_FreeChunk(gScratch);
	gScratch = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Display startup screen
			SDL_BlitSurface(gStart, &rDude, gScreenSurface, NULL);
			//Update the surface
			SDL_UpdateWindowSurface(gWindow);
			//Play a sound
			Mix_PlayChannel(-1, gScratch, 0);
			SDL_Delay(5000);
			SDL_BlitSurface(NULL, &rDude, gScreenSurface, NULL);
			//Update the surface
			SDL_UpdateWindowSurface(gWindow);

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
					{
						quit = true;
						Mix_HaltMusic();
					}
					else if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
							rDude.x -= 10;
						}
						if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
							rDude.x += 10;
						}
						if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
							rDude.y += 10;
						}
						if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
							rDude.y -= 10;
						}
					}
				}

				//Apply the image
				SDL_BlitSurface(gDude, &rDude, gScreenSurface, NULL);

				//Update the surface
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}