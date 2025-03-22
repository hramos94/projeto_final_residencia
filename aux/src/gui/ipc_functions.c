
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <mcal.h>
#include <pins.h>
#include <ipc_functions.h>


// Função para desenhar o texto, recebendo uma string e coordenadas (x, y) para centralização
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int16_t x, int16_t y, SDL_Color textColor) 
{

    // Criar superfície com o texto
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Erro ao renderizar texto: %s\n", TTF_GetError());
        return;
    }

    // Criar textura a partir da superfície
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Liberar a superfície após criar a textura

    if (!textTexture) {
        printf("Erro ao criar textura do texto: %s\n", SDL_GetError());
        return;
    }

    // Obter a largura e altura do texto
    int32_t textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Calcular a posição para centralizar o texto em relação à coordenada (x, y)
    int16_t textX = x - textWidth / 2;
    int16_t textY = y - textHeight / 2;

    SDL_Rect textRect = {textX, textY, textWidth, textHeight};// Definir o retângulo do texto (posição e tamanho)
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Renderizar o texto
    SDL_DestroyTexture(textTexture); // Liberar a textura do texto
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


