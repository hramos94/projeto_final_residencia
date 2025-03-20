#ifndef H_IPC
#define H_IPC

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>


typedef struct {
    SDL_Rect rect;
    int clicked;
    const char* label;
} Button;

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);
void draw_image(SDL_Renderer* renderer, const char* image_path, int x, int y, int width, int height); 
void draw_rectangle(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color);
void draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void draw_button(SDL_Renderer* renderer, Button* button, SDL_Color color, TTF_Font* font);
void handle_button_click(Button* button, int mouseX, int mouseY);
int ipc_runner();

#endif
