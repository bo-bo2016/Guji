#include <stdio.h>
#include "renderer.h"
#include "SDL.h"
#include "SDL_vulkan.h"
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("hello world",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    Init(window);
    printf("init sucess!\n");
    int quit = 0;
    SDL_Event event;
    while (quit <= 0)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
    }
    Quit();
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("quit sucess!\n");
    return 0;
}