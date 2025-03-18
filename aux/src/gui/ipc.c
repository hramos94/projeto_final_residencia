#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int reb_fault_warning =0;
int reb_imobilize_procedure =0;


// Função para desenhar o texto, recebendo uma string e coordenadas (x, y) para centralização
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, int color) 
{
    SDL_Color textColor = {255, 255, 255};  // Cor do texto (branco)

    if(color==2)
    {
            textColor.r = 255;  // componente vermelho
            textColor.g = 192;  // componente verde
            textColor.b = 0;    // componente azul
    }

    if(color==3)
    {
            textColor.r = 255;  // componente vermelho
            textColor.g = 0;  // componente verde
            textColor.b = 0;    // componente azul
    }
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
    int textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Calcular a posição para centralizar o texto em relação à coordenada (x, y)
    int textX = x - textWidth / 2;
    int textY = y - textHeight / 2;

    SDL_Rect textRect = {textX, textY, textWidth, textHeight};// Definir o retângulo do texto (posição e tamanho)
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Renderizar o texto
    SDL_DestroyTexture(textTexture); // Liberar a textura do texto
}

// Função para desenhar uma imagem, recebendo o caminho do arquivo de imagem e as coordenadas (x, y)
void draw_image(SDL_Renderer* renderer, const char* image_path, int x, int y, int width, int height) 
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

void draw_rectangle(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color) 
{   
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Rect rect = {x, y, width, height};

    SDL_RenderFillRect(renderer, &rect);  // Retângulo preenchido

}

void draw_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, SDL_Color color) 
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}






int ipc_runner() 
{
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("Erro ao inicializar o SDL: %s\n", SDL_GetError());
        return -1;
    }

    // Inicializar SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) 
    {
        printf("Erro ao inicializar o SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // Inicializar SDL_ttf
    if (TTF_Init() == -1) 
    {
        printf("Erro ao inicializar o SDL_ttf: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Criar janela
    SDL_Window* window = SDL_CreateWindow("Instrument Cluster Panel",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Erro ao criar a janela: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Criar renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro ao criar o renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Carregar a imagem
    SDL_Surface* surface = IMG_Load("./aux/img/ipc_background.png");
    if (!surface) {
        printf("Falha ao carregar a imagem: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Obter a largura e altura da imagem original
    int img_width = surface->w;
    int img_height = surface->h;

    // Calcular o aspect ratio
    float aspect_ratio = (float)img_width / (float)img_height;

    // Ajustar a imagem para caber na janela, mantendo o aspect ratio
    int render_width = WINDOW_WIDTH;
    int render_height = WINDOW_HEIGHT;

    if (render_width / aspect_ratio <= render_height) {
        render_height = render_width / aspect_ratio;
    } else {
        render_width = render_height * aspect_ratio;
    }

    // Criar textura a partir da superfície
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // Liberar a superfície após criar a textura
    if (!texture) {
        printf("Falha ao criar a textura: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Carregar fonte
    TTF_Font* font = TTF_OpenFont("./aux/img/aptos.ttf", 64);  // Você pode substituir "arial.ttf" pelo caminho para a fonte
    TTF_Font* font2 = TTF_OpenFont("./aux/img/aptos.ttf", 16);
    TTF_Font* font3 = TTF_OpenFont("./aux/img/aptos.ttf",24);    
    if (!font) {
        printf("Erro ao carregar a fonte: %s\n", TTF_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Loop principal
    SDL_Event e;
    int quit = 0;
    int counter = 1;
    int counter2 =-30;

    while (!quit) {
        // Lidar com eventos
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Limpar a tela
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // Definir a posição e o tamanho da imagem para preservar o aspect ratio
        SDL_Rect dest_rect = { (WINDOW_WIDTH - render_width) / 2, (WINDOW_HEIGHT - render_height) / 2, render_width, render_height };

        // Renderizar a imagem
        SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
        SDL_Color red = {255, 0, 0, 255};  // Cor vermelha
        //draw_rectangle(renderer, 580, 300, 100, 2, red);
        
        #define M_PI 3.1415
        int angle=counter2;
        double radians = angle * M_PI / 180.0;
        int Radius=80;
        int centerX=680;
        int centerY=300;
        int finalX = centerX - (int)(Radius * cos(radians));
        int finalY = centerY - (int)(Radius * sin(radians));
        draw_line(renderer,centerX, centerY, finalX, finalY,  red);


        if(reb_fault_warning==1)
        {
            char msg4[50];
            snprintf(msg4, sizeof(msg4), "REB", counter);
            draw_text(renderer, font3, msg4, 330,202,3);
            //texto da velocidade
            char velocidade[50];
            snprintf(velocidade, sizeof(velocidade), "REB Fault", counter);
            draw_text(renderer, font2, velocidade, 420,300,1);
            SDL_RenderPresent(renderer);

            
            

        }
        else if (reb_imobilize_procedure==1)
        {
            char msg[50];
            snprintf(msg, sizeof(msg), "Engine Will be Gradually", counter);
            draw_text(renderer, font2, msg, 420,300,1);

            char msg2[50];
            snprintf(msg2, sizeof(msg2), "Deactivated in 5 minutes", counter);
            draw_text(renderer, font2, msg2, 420,320,1);
            SDL_RenderPresent(renderer);
        }
        


        else
        {
            if(counter%50<25)
            {
                draw_image(renderer, "./aux/img/left_sign.png", 250,180,40,40);
                draw_image(renderer, "./aux/img/right_sign.png", 542,180,40,40);
            }
            draw_image(renderer, "./aux/img/hazard_warning.png", 490,185,35,35);

            char msg4[50];
            snprintf(msg4, sizeof(msg4), "REB", counter);
            draw_text(renderer, font3, msg4, 330,202,2);

            //texto da velocidade
            char velocidade[50];
            snprintf(velocidade, sizeof(velocidade), "%d", counter);
            draw_text(renderer, font, velocidade, 420,300,1);
            //SDL_RenderPresent(renderer);

            //texto da velocidade
            char km_indicator[50];
            snprintf(velocidade, sizeof(velocidade), "km/h", counter,1);
            draw_text(renderer, font2, velocidade, 420,340,1);
            SDL_RenderPresent(renderer);
        }

        if (counter ==200)
       {
            
       }
        
       if (counter >200 && counter<400)
       {
            reb_fault_warning =1;

       }
       else if(counter >400 && counter<600)
       {
            reb_fault_warning =0;
            reb_imobilize_procedure =1;
       }
       else
       {
            reb_imobilize_procedure =0;
       }    
        counter = (counter % 600) + 1; // Contador de 1 a 100
        counter2 = (counter2 % 210) + 1; // Contador de 1 a 100

        // Atualizar a tela

        
        SDL_Delay(5); // Define o FPS 100
    }

    // Liberar recursos
    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}