#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <mcal.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

uint8_t reb_fault_warning =0;
uint8_t reb_imobilize_procedure =0;



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


typedef struct {
    SDL_Rect rect;
    int16_t clicked;
    const char* label;
} Button;

// Função para desenhar um botão
void draw_button(SDL_Renderer* renderer, Button* button, SDL_Color color, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_Color textColor = {255, 255, 255, 255};
    draw_text(renderer, font, button->label, button->rect.x + button->rect.w / 2, button->rect.y + button->rect.h / 2, textColor);
}

// Função para verificar se um botão foi clicado
void handle_button_click(Button* button, int16_t mouseX, int16_t mouseY) {
    if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
        mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h) {
        button->clicked = !button->clicked;
    }
}


int16_t ipc_runner() 
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

    // Loop principal -- Responsável por fazer o movimento das coisas
    SDL_Event e;
    int16_t quit = 0;
    int16_t frame_counter = 1;
    int16_t counter2 =0;
    uint8_t hazard_lights_state=0;
    uint8_t hazard_light=0;

    // Criar botões
    int16_t button_x0 = 100;
    int16_t button_y0 = 725;
    int16_t button_height=50;
    int16_t button_width=150;
    int16_t button_v_space=25;
    //int16_t button_h_space=25;

  
    #define NUM_BUTTONS 3
    const char* labels[NUM_BUTTONS] = {
        "Ignition On", "REB Activate", "CAN Fault"
    };
    
    Button buttons[NUM_BUTTONS];
    
    for (int16_t i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].rect.x = button_x0;
        buttons[i].rect.y = button_y0 + (i * (button_height + button_v_space));
        buttons[i].rect.w = button_width;
        buttons[i].rect.h = button_height;
        buttons[i].clicked = 0;
        buttons[i].label = labels[i];
    }
    

    //color definition
    SDL_Color white = {255,255,255,255};
    SDL_Color background_rectangle = {45, 45, 45, 255};
    SDL_Color red = {255, 0, 0, 255};  // Cor vermelha
    SDL_Color button_clicked = {72, 143, 49, 255};
    SDL_Color button_not_clicked = {110, 110, 110, 110};


    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int32_t mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                for (int16_t i = 0; i < 3; i++) 
                {
                    handle_button_click(&buttons[i], mouseX, mouseY);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 26, 46, 61,255);
        SDL_RenderClear(renderer);// Limpar a tela
        
        //retangulos cinza e Imagem do velocimetro de fundo
        draw_rectangle(renderer,40,50,1120,550,background_rectangle);
        draw_rectangle(renderer,40,650,1120,300,background_rectangle);
        SDL_Rect dest_rect = {120,100,1280*0.75,720*0.75};//Imagem SDL_Rect dest_rect = {120,100,960,540};
        SDL_RenderCopy(renderer, texture, NULL, &dest_rect); // Renderizar a imagem

        //Speedometer
        int16_t vehicle_speed=counter2;
        double angle=-30+ 1*vehicle_speed;
        double radians = angle * M_PI / 180.0;
        int16_t Radius=107;
        int16_t centerX=888;
        int16_t centerY=408;
        int16_t finalX = centerX - (int16_t)(Radius * cos(radians));
        int16_t finalY = centerY - (int16_t)(Radius * sin(radians));
        draw_line(renderer,centerX, centerY, finalX, finalY,  red);

        //buttons
        for (int16_t i = 0; i < 3; i++) 
        {
            SDL_Color buttonColor = buttons[i].clicked ? button_clicked : button_not_clicked;
            draw_button(renderer, &buttons[i], buttonColor, font3);
        }
        


        read_pin_status(&hazard_lights_state,1);
        read_pin_status(&hazard_light,0);
        read_pin_status(&reb_fault_warning,6);

        if(reb_fault_warning==1)
        {
            //alerta na parte superior
            draw_image(renderer, "./aux/img/reb_red.png", 480,284,408/7,227/7);

            char velocidade[50]; //alerta de fault no lugar da velocidade
            snprintf(velocidade, sizeof(velocidade), "REB Fault");
            draw_text(renderer, font2, velocidade, 600,420,white);
        }
        else if (reb_imobilize_procedure==1)
        {
            draw_image(renderer, "./aux/img/reb_yellow.png", 480,284,408/7,227/7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Engine Will be Gradually");
            draw_text(renderer, font2, msg, 420,300,white);

            char msg2[50];
            snprintf(msg2, sizeof(msg2), "Deactivated in 5 minutes");
            draw_text(renderer, font2, msg2, 420,320,white);
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
            draw_text(renderer, font, velocidade, 600,400,white);

            char km_indicator[10]; //texto da km
            snprintf(km_indicator, sizeof(km_indicator), "km/h");
            draw_text(renderer, font2,km_indicator, 600,440,white);
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