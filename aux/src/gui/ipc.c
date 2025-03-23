#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <mcal.h>
#include <pins.h>
#include <ipc_functions.h>
#include <ipc.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000



#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

uint8_t reb_fault_warning = 0;
uint8_t reb_activate = 0;
uint8_t reb_deactivate = 0;
uint8_t reb_imobilize_procedure = 0;
uint8_t reb_vehicle_imobilized = 0;


int16_t ipc_runner() 
{
    //SDL Inititalization
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (ipc_render_init(&window, &renderer,WINDOW_WIDTH,WINDOW_HEIGHT) == FAIL) 
    {
        return -1;
    }
    
    // Carregar fonte
    TTF_Font* font = TTF_OpenFont("./aux/img/aptos.ttf", 64); 
    TTF_Font* font2 = TTF_OpenFont("./aux/img/aptos.ttf", 16);
    TTF_Font* font3 = TTF_OpenFont("./aux/img/aptos.ttf",24);    
    if (!font || !font2 || !font3) 
    {
        printf("Erro ao carregar a fonte: %s\n", TTF_GetError());
        ipc_render_cleanup(&window, &renderer);
        return -1;
    }

    // Loop principal -- Responsável por fazer o movimento das coisas
    SDL_Event e;
    int16_t quit = 0;
    int16_t frame_counter = 1;
    int16_t counter2 =0;
    uint8_t hazard_lights_state=0;
    uint8_t hazard_light=0;

    //Vehicle Control Buttons
    int16_t button1_x0 = 60;
    int16_t button1_y0 = 725;
    int16_t button1_height=50;
    int16_t button1_width=175;
    int16_t button1_v_space=25;
    int16_t button1_h_space=20;
    #define NUM_BUTTONS1 6
    #define NUM_ROWS1 2
    #define NUM_BUTTONS_PER_ROW1 3
    const char* labels1[NUM_BUTTONS1] = {"Ignition On", "REB On", "REB Off","Scenario 1","Scenario 2","Scenario 3"};
    Button buttons1[NUM_BUTTONS1];

    int8_t rowcounter1=0, linecounter1=0; 
    for (int16_t i = 0; i < NUM_BUTTONS1; i++) 
    {
        buttons1[i].rect.x = button1_x0 + (rowcounter1 * (button1_width + button1_h_space));
        buttons1[i].rect.y = button1_y0 + (linecounter1 * (button1_height + button1_v_space));

        buttons1[i].rect.w = button1_width;
        buttons1[i].rect.h = button1_height;
        buttons1[i].clicked = 0;
        buttons1[i].label = labels1[i];

        if((linecounter1+1) == NUM_BUTTONS_PER_ROW1)
        {
            rowcounter1++;
            linecounter1=0;
        }
        else
        {
            linecounter1++;
        }
    }

    //Vehicle Fautl Simulation Buttons
    int16_t button2_x0 = 765;
    int16_t button2_y0 = 735;
    int16_t button2_height=50;
    int16_t button2_width=175;
    int16_t button2_v_space=25;
    int16_t button2_h_space=20;
    #define NUM_BUTTONS2 6
    #define NUM_ROWS2 2
    #define NUM_BUTTONS_PER_ROW2 3
    const char* labels2[NUM_BUTTONS2] = {"Can Fault"," ","Server Fault"," "," "," "};
    Button buttons2[NUM_BUTTONS2];

    int8_t rowcounter2=0, linecounter2=0; 
    for (int16_t i = 0; i < NUM_BUTTONS2; i++) 
    {
        buttons2[i].rect.x = button2_x0 + (rowcounter2 * (button2_width + button2_h_space));
        buttons2[i].rect.y = button2_y0 + (linecounter2 * (button2_height + button2_v_space));

        buttons2[i].rect.w = button2_width;
        buttons2[i].rect.h = button2_height;
        buttons2[i].clicked = 0;
        buttons2[i].label = labels2[i];

        if((linecounter2+1) == NUM_BUTTONS_PER_ROW2)
        {
            rowcounter2++;
            linecounter2=0;
        }
        else
        {
            linecounter2++;
        }
    }

    //Accelerator and Brake buttons
    Button accelarator = {{625, 750, 199*0.25, 519*0.25}, 0, " "};// 199 × 519
    Button brake = {{530, 788, 264*0.25, 364*0.25}, 0, " "};// 264 × 346 

    //color definition
    SDL_Color white = {255,255,255,255};
    SDL_Color background_rectangle = {45, 45, 45, 255};
    SDL_Color red = {255, 0, 0, 255};  // Cor vermelha
    SDL_Color button_clicked = {72, 143, 49, 255};
    SDL_Color button_not_clicked = {110, 110, 110, 110};

    uint8_t last_clicked=0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int32_t mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                for (int16_t i = 0; i < NUM_BUTTONS1; i++) 
                {
                    handle_button_click(&buttons1[i], mouseX, mouseY);
                }
                for (int16_t i = 0; i < NUM_BUTTONS2; i++) 
                {
                    handle_button_click(&buttons2[i], mouseX, mouseY);
                }
                handle_button_click(&accelarator, mouseX, mouseY);
                handle_button_click(&brake, mouseX, mouseY);
            }
        }

        SDL_SetRenderDrawColor(renderer, 26, 46, 61,255);
        SDL_RenderClear(renderer);// Limpar a tela
        
        //retangulos cinza e Imagem do velocimetro de fundo
        draw_rectangle(renderer,40,50,1120,550,background_rectangle);
        draw_rectangle(renderer,40,650,415,300,background_rectangle);
        draw_rectangle(renderer,475,650,250,300,background_rectangle);
        draw_rectangle(renderer,745,650,415,300,background_rectangle);
        SDL_Rect dest_rect = {120,100,1280*0.75,720*0.75};//Imagem SDL_Rect dest_rect = {120,100,960,540};
        draw_image(renderer, "./aux/img/ipc_background.png", 120,100,1280*0.75,720*0.75);
        //SDL_RenderCopy(renderer, texture, NULL, &dest_rect); // Renderizar a imagem

        //texto de títutlo
        char tit1[50];
        snprintf(tit1, sizeof(tit1), "Instrument Cluster Panel");
        draw_text(renderer, font3, tit1, 600,110,white);
        char tit2[50];
        snprintf(tit2, sizeof(tit2), "Vehicle Control");
        draw_text(renderer, font3, tit2, 257,680,white);
        char tit3[50];
        snprintf(tit3, sizeof(tit3), "Vehicle Control");
        draw_text(renderer, font3, tit3, 600,680,white);
        char tit4[50];
        snprintf(tit4, sizeof(tit4), "Fault Simulation");
        draw_text(renderer, font3, tit4, 952,680,white);



        //Speedometer
        int16_t vehicle_speed=counter2;
        if(reb_vehicle_imobilized == 1)
        {
            vehicle_speed=0;
        }
        double angle=-30+ 1*vehicle_speed;
        double radians = angle * M_PI / 180.0;
        int16_t Radius=107;
        int16_t centerX=888;
        int16_t centerY=408;
        int16_t finalX = centerX - (int16_t)(Radius * cos(radians));
        int16_t finalY = centerY - (int16_t)(Radius * sin(radians));
        draw_line(renderer,centerX, centerY, finalX, finalY,  red);

        //buttons1
        for (int16_t i = 0; i < NUM_BUTTONS1; i++) 
        {
            SDL_Color buttonColor = buttons1[i].clicked ? button_clicked : button_not_clicked;
            draw_button(renderer, &buttons1[i], buttonColor, font3);
        }

        //REB ON Button - if true write to pin

        if(buttons1[1].clicked==1 && last_clicked==0)
        {
            set_pin_status(1, REB_ACTIVATE_PIN);
            last_clicked=1;
           
        }
        else if(buttons1[1].clicked==0 && last_clicked==1)
        {
            set_pin_status(0, REB_ACTIVATE_PIN);
            last_clicked=0;
        }

        read_pin_status(&hazard_lights_state,HAZARD_BUTTON_PIN);
        read_pin_status(&hazard_light,HAZARD_LIGHTS_PIN);
        read_pin_status(&reb_imobilize_procedure,REB_IPC_WARNING);
        read_pin_status(&reb_fault_warning,REB_IPC_FAULT_PIN);
        read_pin_status(&reb_vehicle_imobilized,ENGINE_REB_MODE);

        //buttons2
        for (int16_t i = 0; i < NUM_BUTTONS2; i++) 
        {
            //SDL_Color buttonColor = buttons2[i].clicked ? button_clicked : button_not_clicked;
            SDL_Color buttonColor = accelarator.clicked ? button_clicked : button_not_clicked;
            draw_button(renderer, &buttons2[i], buttonColor, font3);
        }

        draw_image_button(renderer, &accelarator, "./aux/img/accelerator.png");
        draw_image_button(renderer, &brake, "./aux/img/brake.png");

        
        if(reb_fault_warning==1)
        {
            //alerta na parte superior
            draw_image(renderer, "./aux/img/reb_red.png", 480,284,408/7,227/7);

            char velocidade[50]; //alerta de fault no lugar da velocidade
            snprintf(velocidade, sizeof(velocidade), "REB Fault");
            draw_text(renderer, font2, velocidade, 600,420,white);
        }
        else if (reb_imobilize_procedure == 1 && reb_vehicle_imobilized == 0)
        {
            draw_image(renderer, "./aux/img/reb_yellow.png", 480,284,408/7,227/7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Engine Will be Gradually");
            draw_text(renderer, font2, msg, 600,400,white);

            char msg2[50];
            snprintf(msg2, sizeof(msg2), "Deactivated in 5 minutes");
            draw_text(renderer, font2, msg2, 600,420,white);
            

        }
        else if(reb_vehicle_imobilized == 1)
        {
            draw_image(renderer, "./aux/img/reb_green.png", 480,284,408/7,227/7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Engine is Stopping");
            draw_text(renderer, font2, msg, 600,410,white);

        }
        else
        {

            char velocidade[10]; //texto da velocidade
            snprintf(velocidade, sizeof(velocidade), "%d", vehicle_speed);
            draw_text(renderer, font, velocidade, 600,400,white);

            char km_indicator[10]; //texto da km
            snprintf(km_indicator, sizeof(km_indicator), "km/h");
            draw_text(renderer, font2,km_indicator, 600,440,white);
        
        }
    
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


        
        frame_counter = (frame_counter % 200) + 1; // Contador de 1 a 100
        counter2 = (counter2 % 200) + 1; // Contador de 1 a 100

        SDL_RenderPresent(renderer); // Atualizar a tela
        SDL_Delay(30); // Define o FPS 100
    }

    // Liberar recursos
    TTF_CloseFont(font);
    ipc_render_cleanup(&window, &renderer); 

    return 0;
}