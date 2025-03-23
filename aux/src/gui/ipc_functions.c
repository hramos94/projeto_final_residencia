
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

//this makes all cleanup when program is exiting
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




// Function to draw text, receiving a string and coordinates (x, y) for centering.
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int16_t x, int16_t y, SDL_Color textColor) 
{
    // Create surface with text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Erro ao renderizar texto: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from surfacce
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Liberar a superfície após criar a textura

    if (!textTexture) {
        printf("Erro ao criar textura do texto: %s\n", SDL_GetError());
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




// Função para desenhar uma imagem, recebendo o caminho do arquivo de imagem e as coordenadas (x, y)
void draw_image(SDL_Renderer* renderer, const char* image_path, int16_t x, int16_t y, int16_t width, int16_t height) 
{
    // Carregar a imagem
    SDL_Surface* imageSurface = IMG_Load(image_path);
    if (!imageSurface) {
        printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
        return;
    }

    // Criar textura a partir da superfície da imagem
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);  // Liberar a superfície após criar a textura

    if (!imageTexture) {
        printf("Erro ao criar a textura da imagem: %s\n", SDL_GetError());
        return;
    }

    // Definir o retângulo de destino para a imagem (posição e tamanho)
    SDL_Rect destRect = { x, y, width, height };

    // Renderizar a imagem
    SDL_RenderCopy(renderer, imageTexture, NULL, &destRect);

    // Liberar a textura
    SDL_DestroyTexture(imageTexture);
}

void draw_rectangle(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t width, int16_t height, SDL_Color color) 
{   
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Rect rect = {x, y, width, height};

    SDL_RenderFillRect(renderer, &rect);  // Retângulo preenchido

}

void draw_line(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, SDL_Color color) 
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

// Função para desenhar um botão
void draw_button(SDL_Renderer* renderer, Button* button, SDL_Color color, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_Color textColor = {255, 255, 255, 255};
    draw_text(renderer, font, button->label, button->rect.x + button->rect.w / 2, button->rect.y + button->rect.h / 2, textColor);
}

void draw_image_button(SDL_Renderer* renderer, Button* button, const char* image_path) {
    SDL_Surface* imageSurface = IMG_Load(image_path);
    if (!imageSurface) {
        printf("Erro ao carregar a imagem do botão: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);  

    if (!imageTexture) {
        printf("Erro ao criar a textura do botão: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderCopy(renderer, imageTexture, NULL, &button->rect);
    SDL_DestroyTexture(imageTexture);
}


// Função para verificar se um botão foi clicado
void handle_button_click(Button* button, int32_t mouseX, int32_t mouseY) {
    if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
        mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h) {
        button->clicked = !button->clicked;
    }
}


