// Dat Vuong and Joshua Kim :)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	return Engine::Instance().Run();
}