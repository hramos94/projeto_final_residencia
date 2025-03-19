#ifndef H_IPC
#define H_IPC

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);
void draw_image(SDL_Renderer* renderer, const char* image_path, int x, int y, int width, int height); 
void draw_rectangle(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color);
void draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color);
int ipc_runner();

#endif
