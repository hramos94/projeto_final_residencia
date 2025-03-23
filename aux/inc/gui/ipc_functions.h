#ifndef H_IPC_FUNCTIONS
#define H_IPC_FUNCTIONS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>


typedef struct {
    SDL_Rect rect;
    int16_t last_click;
    int16_t clicked;
    const char* label;
} Button;

#define SUCCESS 0
#define FAIL 1

uint8_t ipc_render_init(SDL_Window **window, SDL_Renderer **renderer, uint16_t window_width,int16_t window_height);
void ipc_render_cleanup(SDL_Window **window, SDL_Renderer **renderer);
uint8_t initialize_font(TTF_Font **font, char* font_path, uint16_t font_size);
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int16_t x, int16_t y, SDL_Color textColor);
void draw_image(SDL_Renderer* renderer, const char* image_path, int16_t x, int16_t y, int16_t width, int16_t height); 
void draw_rectangle(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t width, int16_t height, SDL_Color color);
void draw_line(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, SDL_Color color);
void draw_button(SDL_Renderer* renderer, Button* button, SDL_Color color, TTF_Font* font);
void handle_button_click(Button* button, int32_t mouseX, int32_t mouseY);
void handle_pedal_press(Button* button, int32_t mouseX, int32_t mouseY, uint16_t *percentage, uint16_t increment);
void handle_pedal_release(Button* button, int32_t mouseX, int32_t mouseY, uint16_t *percentage, uint16_t increment);
void draw_image_button(SDL_Renderer* renderer, Button* button, const char* image_path);
float update_speed(float speed, int accelerator_percentage, int brake_percentage);
#endif
