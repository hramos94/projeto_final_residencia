
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <mcal.h>
#include <pins.h>
#include <ipc_functions.h>

//This makes all windows initialization for drawing graphics 
uint8_t ipc_render_init(SDL_Window **window, SDL_Renderer **renderer, uint16_t window_width,int16_t window_height) {
    // start SDL - Initialize Simple DiretMedia layer - Allows for Audio videos and events
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init Fail: %s\n", SDL_GetError());
        return FAIL;
    }

    // start SDL_image - allows to load images
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Erro ao inicializar o SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return FAIL;
    }

    // Inicializar SDL_ttf - allows to use letter fonts
    if (TTF_Init() == -1) {
        printf("Erro ao inicializar o SDL_ttf: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return FAIL;
    }

    // This creates a window
    *window = SDL_CreateWindow("Instrument Cluster Panel",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               window_width, window_height, SDL_WINDOW_RESIZABLE);
    if (!*window) {
        printf("Erro ao criar a janela: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return FAIL;
    }

    // Create renderer object - used for drawing 
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Ativa interpolação linear
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Erro ao criar o renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return FAIL;
    }
    return SUCCESS;
}

//This makes all cleanup when program is exiting
void ipc_render_cleanup(SDL_Window **window, SDL_Renderer **renderer) 
{
    if (*renderer) {
        SDL_DestroyRenderer(*renderer);
    }
    if (window) {
        SDL_DestroyWindow(*window);
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//Initialize font
uint8_t initialize_font(TTF_Font **font, char* font_path, uint16_t font_size)
{
   *font = TTF_OpenFont(font_path, font_size); 
   if (*font == 0) 
   {
       printf("Erro ao carregar a fonte: %s\n", TTF_GetError());
       return FAIL;
   }
   
   return SUCCESS;
}

//Function to draw text, receiving a string and coordinates (x, y) for centering.
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int16_t x, int16_t y, SDL_Color textColor) 
{
    // Create surface with text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Error rendering text: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from surfacce
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); 

    if (!textTexture) {
        printf("Error to create text texture: %s\n", SDL_GetError());
        return;
    }

    // Get the width and height of the text
    int32_t textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Calculate the position to center the text relative to the (x, y) coordinate
    int16_t textX = x - textWidth / 2;
    int16_t textY = y - textHeight / 2;

    // Define the text position, size, and render it to the screen, then destroy the texture
    SDL_Rect textRect = {textX, textY, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect); 
    SDL_DestroyTexture(textTexture);
}

//Function to draw an image, receiving the file path and the coordinates (x, y)
void draw_image(SDL_Renderer* renderer, const char* image_path, int16_t x, int16_t y, int16_t width, int16_t height) 
{
    // Load image
    SDL_Surface* imageSurface = IMG_Load(image_path);
    if (!imageSurface) {
        printf("Erro loading image %s\n", IMG_GetError());
        return;
    }

    // Create texture on image surface
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);  // Liberar a superfície após criar a textura

    if (!imageTexture) {
        printf("Erro ao criar a textura da imagem: %s\n", SDL_GetError());
        return;
    }

    // Define the destination rectangle, render the image, and free the texture
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(renderer, imageTexture, NULL, &destRect);
    SDL_DestroyTexture(imageTexture);
}

//Funtion to draw a rectangle
void draw_rectangle(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t width, int16_t height, SDL_Color color) 
{   
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);  
}

//Function to draw a line
void draw_line(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, SDL_Color color) 
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

//Function to draw a button - simple rectangle
void draw_button(SDL_Renderer* renderer, Button* button, SDL_Color color, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_Color textColor = {255, 255, 255, 255};
    draw_text(renderer, font, button->label, button->rect.x + button->rect.w / 2, button->rect.y + button->rect.h / 2, textColor);
}

//Funtion to drwaw a button with image
void draw_image_button(SDL_Renderer* renderer, Button* button, const char* image_path) {
    SDL_Surface* imageSurface = IMG_Load(image_path);
    if (!imageSurface) {
        printf("Error loading button: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);  

    if (!imageTexture) {
        printf("Erro creating button texture: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderCopy(renderer, imageTexture, NULL, &button->rect);
    SDL_DestroyTexture(imageTexture);
}

// Funton to verify if button is clicked - it change the state at every click
void handle_button_click(Button* button, int32_t mouseX, int32_t mouseY) {
    if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
        mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h) {
        button->clicked = !button->clicked;
    }
}

void handle_pedal_press(Button* button, int32_t mouseX, int32_t mouseY, uint16_t *percentage, uint16_t increment) {
    if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
        mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h) {
        button->clicked = 1;
        *percentage=*percentage+increment;
        if(*percentage>100)
        {
            *percentage=0;
        }
    }
}

void handle_pedal_release(Button* button, int32_t mouseX, int32_t mouseY, uint16_t *percentage, uint16_t increment) {
    if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
        mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h) {
        button->clicked = 0;
        *percentage=*percentage-increment;
    }
}


//Function to simulate vehicle speed
#define MAX_SPEED 200.0     // Vehicle maximum speed km/h
#define ACCELERATION 0.005  // Acceleration coeficient
#define BRAKE_POWER 0.020   // Brake coeficient
#define FRICTION 0.1        // Friction coeficient
float update_speed(float speed, int accelerator_percentage, int brake_percentage)
{

    //this is the maximum possible speed in current conditions (depends on accelerator percentage)
    float current_max_speed = MAX_SPEED * accelerator_percentage * 0.01;

    //this part will smooth out when achieving the maximum current speed
    float accel_smothout_coeficient = accelerator_percentage;
    if (speed > current_max_speed * 0.8 && current_max_speed!=0 ) 
    {  
        accel_smothout_coeficient *= (current_max_speed - speed) / (current_max_speed * 0.2);
    }

    //here we do increment and decrement of the speed based on accelerator and brake pedals percentage
    speed += accel_smothout_coeficient * ACCELERATION;
    speed -= (brake_percentage) * BRAKE_POWER;
    
    //this will kick in to simulate drag when accelarator and brake are not pressed
    if (accelerator_percentage == 0 && brake_percentage == 0 && speed > 0) {
        speed -= FRICTION;
    }

    //assure that speed limits are not passed
    if (speed > MAX_SPEED) speed = current_max_speed;
    if (speed < 0) speed = 0;

    return speed;
}


