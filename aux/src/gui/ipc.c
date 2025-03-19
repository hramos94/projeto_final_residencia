#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000

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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Ativa interpolação linear
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
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);// Criar textura a partir da superfície
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
    TTF_Font* font = TTF_OpenFont("./aux/img/aptos.ttf", 64); 
    TTF_Font* font2 = TTF_OpenFont("./aux/img/aptos.ttf", 16);
    TTF_Font* font3 = TTF_OpenFont("./aux/img/aptos.ttf",24);    
    if (!font || !font2 || !font3) 
    {
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
    int frame_counter = 1;
    int counter2 =0;
    int hazard_lights_state=0;
    int hazard_light=0;

    static int button_state[10]; // Estado inicial de cada botão
    for (int i = 0; i < 10; i++)
    {
        button_state[i] = 0;
    }

    while (!quit) 
    {
        // Lidar com eventos
        while (SDL_PollEvent(&e)) 
        {
            if (e.type == SDL_QUIT) 
            {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 26, 46, 61,255);
        SDL_RenderClear(renderer);// Limpar a tela
        
        SDL_Color grey = {45, 45, 45, 255};  
        draw_rectangle(renderer,40,50,1120,550,grey);
        draw_rectangle(renderer,40,650,1120,300,grey);
        SDL_Rect dest_rect = {120,100,1280*0.75,720*0.75};//Imagem SDL_Rect dest_rect = {120,100,960,540};
        SDL_RenderCopy(renderer, texture, NULL, &dest_rect); // Renderizar a imagem
        SDL_Color red = {255, 0, 0, 255};  // Cor vermelha
        
        //Speedometer
        int vehicle_speed=counter2;
        #define M_PI 3.1415
        double angle=-30+ 1*vehicle_speed;
        double radians = angle * M_PI / 180.0;
        int Radius=107;
        int centerX=888;
        int centerY=408;
        int finalX = centerX - (int)(Radius * cos(radians));
        int finalY = centerY - (int)(Radius * sin(radians));
        draw_line(renderer,centerX, centerY, finalX, finalY,  red);

        read_pin_status(&hazard_lights_state,1);
        read_pin_status(&hazard_light,0);
        read_pin_status(&reb_fault_warning,6);

        if(reb_fault_warning==1)
        {
            //alerta na parte superior
            draw_image(renderer, "./aux/img/reb_red.png", 480,284,408/7,227/7);

            char velocidade[50]; //alerta de fault no lugar da velocidade
            snprintf(velocidade, sizeof(velocidade), "REB Fault");
            draw_text(renderer, font2, velocidade, 600,420,1);
        }
        else if (reb_imobilize_procedure==1)
        {
            draw_image(renderer, "./aux/img/reb_yellow.png", 480,284,408/7,227/7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Engine Will be Gradually");
            draw_text(renderer, font2, msg, 420,300,1);

            char msg2[50];
            snprintf(msg2, sizeof(msg2), "Deactivated in 5 minutes");
            draw_text(renderer, font2, msg2, 420,320,1);
        }
        else
        {
            if(hazard_lights_state==1) //pisca alerta
            {
                draw_image(renderer, "./aux/img/hazard_warning.png", 685,284,92/2.5,85/2.5);
                if(hazard_light==1)
                {
                    draw_image(renderer, "./aux/img/left_sign.png", 428,278,39,47);
                    draw_image(renderer, "./aux/img/right_sign.png", 732,278,39,47);
                }
            }

            //draw_image(renderer, "./aux/img/reb_green.png", 480,284,408/7,227/7);

            char velocidade[10]; //texto da velocidade
            snprintf(velocidade, sizeof(velocidade), "%d", vehicle_speed);
            draw_text(renderer, font, velocidade, 600,400,1);

            char km_indicator[10]; //texto da km
            snprintf(km_indicator, sizeof(km_indicator), "km/h");
            draw_text(renderer, font2,km_indicator, 600,440,1);
        }

        
        frame_counter = (frame_counter % 200) + 1; // Contador de 1 a 100
        counter2 = (counter2 % 200) + 1; // Contador de 1 a 100

        SDL_RenderPresent(renderer); // Atualizar a tela
        SDL_Delay(30); // Define o FPS 100
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