#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <ipc.h>
#include <ipc_functions.h>
#include <math.h>
#include <mcal.h>
#include <pins.h>
#include <stdio.h>
#include <sys/time.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 820
const uint32_t FPS = 30;

#ifndef M_PI
#define M_PI 3.141592
#endif

char terminal_frame_str[6][150]; // Buffer para armazenar a string
char timestamp[6][11];

#define filtered_ID1 0x13
#define filtered_ID2 0x15

int16_t terminal_read_can()
{
    const char *interface = "vcan0";
    int vcan_socket;
    uint8_t can_status = 0;

    // write a space in array so we dont get zero text width error
    for (int i = 0; i <= 6; i++)
    {
        terminal_frame_str[i][0] = ' ';
        timestamp[i][0] = ' ';
    }

    // Can Socket and Interface Initialization for Reading VCAN0;
    if (can_socket_open(&vcan_socket) == FAIL)
    {
        perror("Can socket open Error: ");
        can_status = FAIL;
    }
    if (can_interface_status(&vcan_socket, interface) == FAIL)
    {
        perror("Can interface Error: ");
        can_status = FAIL;
    }
    if (can_bind_socket(&vcan_socket, interface) == FAIL)
    {
        perror("Can bind Error: ");
        can_status = FAIL;
    }

    // variable to keep track in wich line we are writing
    int line_counter = 0;

    // Write Can Initialization Status
    if (can_status == FAIL)
    {
        strcpy(terminal_frame_str[0], "CAN OFFLINE");
    }
    else
    {
        strcpy(terminal_frame_str[0], "CAN ONLINE");
    }

    while (1)
    {
        pthread_testcancel();
        struct can_frame frame;
        char last_frame[150];
        struct timeval tv;
        struct tm *tm_info;

        uint8_t ignore_frame = 0;
        uint8_t can_filter = 0;

        // read Can Bus and wait until a message arrive
        can_read(&vcan_socket, &frame);
        read_pin_status(&can_filter,CAN_FILTER_PIN);
    
        if(can_filter == 1)
        {
            if(frame.can_id == filtered_ID1 || frame.can_id == filtered_ID2)
            {
                ignore_frame = 1;
            }        
        }

        //check if filter is ON
        if(ignore_frame == 0)
        {
            // get the timestamp of when the message was read
            gettimeofday(&tv, NULL);
            tm_info = localtime(&tv.tv_sec);

            // Write Timestam and CAN ID in the string
            uint32_t offset =
                snprintf(last_frame, sizeof(last_frame), "%02d:%02d:%02d:    [0x%X]   ",
                        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, frame.can_id);

            // Write Can message in the string
            for (int i = 0; i < frame.can_dlc && offset < sizeof(last_frame) - 3; i++)
            {
                offset +=
                    snprintf(last_frame + offset, sizeof(last_frame) - offset, "%02X ", frame.data[i]);
            }

            // this will keep track in wich line we are writing and create a scrolling effect whe the
            // maximum of 6 messages is achieved
            if (line_counter < 6)
            {
                strcpy(terminal_frame_str[line_counter], last_frame);

                strncpy(timestamp[line_counter], terminal_frame_str[line_counter], 10);
                timestamp[line_counter][10] = '\0';
                line_counter++;
            }
            else
            {
                for (int i = 0; i < 5; i++)
                {
                    strcpy(terminal_frame_str[i], terminal_frame_str[i + 1]);

                    strncpy(timestamp[i], terminal_frame_str[i], 10);
                    timestamp[i][10] = '\0';
                }
                strcpy(terminal_frame_str[5], last_frame);
                strncpy(timestamp[5], terminal_frame_str[5], 10);
                timestamp[5][10] = '\0';
            }
        }
    }
}

int16_t ipc_runner()
{
    // local variables
    uint8_t reb_fault_warning = 0;
    uint8_t reb_imobilize_procedure = 0;
    uint8_t reb_vehicle_imobilized = 0;
    uint16_t accelerator_percentage = 0;
    uint16_t brake_percentage = 0;
    uint8_t hazard_lights_state = 0;
    uint8_t hazard_light = 0;
    float vehicle_speed_float = 0;
    set_pin_status(0, CAN_FILTER_PIN);

    // SDL Inititalization
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (ipc_render_init(&window, &renderer, WINDOW_WIDTH, WINDOW_HEIGHT) == FAIL)
    {
        return FAIL;
    }

    // Font Initialization
    TTF_Font *font, *font2, *font3, *font4;
    if (initialize_font(&font, "./aux/img/aptos.ttf", 64) == FAIL)
    {
        printf("Font Error: %s\n", TTF_GetError());
        ipc_render_cleanup(&window, &renderer);
        return FAIL;
    }
    if (initialize_font(&font2, "./aux/img/aptos.ttf", 16) == FAIL)
    {
        printf("Font Error: %s\n", TTF_GetError());
        ipc_render_cleanup(&window, &renderer);
        return FAIL;
    }
    if (initialize_font(&font3, "./aux/img/aptos.ttf", 24) == FAIL)
    {
        printf("Font Error: %s\n", TTF_GetError());
        ipc_render_cleanup(&window, &renderer);
        return FAIL;
    }

    if (initialize_font(&font4, "./aux/img/aptos.ttf", 22) == FAIL)
    {
        printf("Font Error: %s\n", TTF_GetError());
        ipc_render_cleanup(&window, &renderer);
        return FAIL;
    }

    // Start thread for reading CAN for mini terminal
    pthread_t terminal_read_can_th = new_thread(terminal_read_can);

    // Vehicle Control Buttons
    const int16_t button1_x0 = 30;
    const int16_t button1_y0 = 585;
    const int16_t button1_height = 50;
    const int16_t button1_width = 175;
    const int16_t button1_v_space = 25;
    const int16_t button1_h_space = 20;
    #define NUM_BUTTONS1 3
    #define NUM_ROWS1 1
    #define NUM_BUTTONS_PER_ROW1 3
    const char *labels1[NUM_BUTTONS1] = {"REB On", "REB Off", "Filter CAN"};
    Button buttons1[NUM_BUTTONS1];

    int8_t rowcounter1 = 0, linecounter1 = 0;
    for (int16_t i = 0; i < NUM_BUTTONS1; i++)
    {
        buttons1[i].rect.x = button1_x0 + (rowcounter1 * (button1_width + button1_h_space));
        buttons1[i].rect.y = button1_y0 + (linecounter1 * (button1_height + button1_v_space));

        buttons1[i].rect.w = button1_width;
        buttons1[i].rect.h = button1_height;
        buttons1[i].last_click = 0;
        buttons1[i].clicked = 0;
        buttons1[i].label = labels1[i];

        if ((linecounter1 + 1) == NUM_BUTTONS_PER_ROW1)
        {
            rowcounter1++;
            linecounter1 = 0;
        }
        else
        {
            linecounter1++;
        }
    }

    // Accelerator and Brake buttons
    Button accelarator = {{385, 610, 199 * 0.25, 519 * 0.25}, 1, 0, " "}; // 199 × 519
    Button brake = {{290, 648, 264 * 0.25, 364 * 0.25}, 1, 0, " "};       // 264 × 346

    // color definition
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color background_rectangle = {45, 45, 45, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color yellow = {255, 165, 0, 255};
    SDL_Color button_clicked = {72, 143, 49, 255};
    SDL_Color button_not_clicked = {110, 110, 110, 110};
    SDL_Color terminal_color = {48, 10, 36, 255};

    SDL_Event e;
    int16_t quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int32_t mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                for (int16_t i = 0; i < NUM_BUTTONS1; i++)
                {
                    handle_button_click(&buttons1[i], mouseX, mouseY);
                }
                handle_pedal_press(&accelarator, mouseX, mouseY, &accelerator_percentage, 10);
                handle_pedal_press(&brake, mouseX, mouseY, &brake_percentage, 10);

                // REB ON Button
                if (buttons1[0].clicked == 1 && buttons1[0].last_click == 0)
                {
                    set_pin_status(1, REB_ACTIVATE_PIN);
                    buttons1[0].last_click = 1;
                }
                else if (buttons1[0].clicked == 0 && buttons1[0].last_click == 1)
                {
                    set_pin_status(0, REB_ACTIVATE_PIN);
                    buttons1[0].last_click = 0;
                }

                // REB OFF Button
                if (buttons1[1].clicked == 1 && buttons1[1].last_click == 0)
                {
                    set_pin_status(1, REB_DEACTIVATE);
                    buttons1[1].last_click = 1;
                }
                else if (buttons1[1].clicked == 0 && buttons1[1].last_click == 1)
                {
                    set_pin_status(0, REB_DEACTIVATE);
                    buttons1[1].last_click = 0;
                }

                // Filter CAN Button
                if (buttons1[2].clicked == 1 && buttons1[2].last_click == 0)
                {
                    set_pin_status(1, CAN_FILTER_PIN);
                    buttons1[2].last_click = 1;
                }
                else if (buttons1[2].clicked == 0 && buttons1[2].last_click == 1)
                {
                    set_pin_status(1, CAN_FILTER_PIN);
                    buttons1[2].last_click = 0;
                }
            }
        }

        // clear Window
        SDL_SetRenderDrawColor(renderer, 26, 46, 61, 255);
        SDL_RenderClear(renderer);

        // Gray Background Rectangles and Speedometer Image
        draw_rectangle(renderer, 10, 20, 980, 470, background_rectangle);
        draw_rectangle(renderer, 10, 510, 215, 300, background_rectangle);
        draw_rectangle(renderer, 235, 510, 250, 300, background_rectangle); // 475 650 -240 -140
        draw_rectangle(renderer, 495, 510, 495, 300, terminal_color);
        draw_rectangle(renderer, 495, 510, 495, 50, background_rectangle); // 745 650 -250 -140
        draw_image(renderer, "./aux/img/ipc_background.png", 20, -30, 1280 * 0.75,
                   720 * 0.75); //(120,100)

        // Rectangle Titles
        //  char tit1[50];
        //  snprintf(tit1, sizeof(tit1), "Instrument Cluster Panel");
        //  draw_text(renderer, font3, tit1, 600,110,white);
        char tit2[50];
        snprintf(tit2, sizeof(tit2), "Vehicle Control");
        draw_text(renderer, font3, tit2, 112, 540, white);
        char tit3[50];
        snprintf(tit3, sizeof(tit3), "Pedal Control");
        draw_text(renderer, font3, tit3, 360, 540, white);
        char tit4[50];
        snprintf(tit4, sizeof(tit4), "CAN BUS Monitor");
        draw_text(renderer, font3, tit4, 742, 540, white);

        // Speed simulation
        if (reb_vehicle_imobilized == 1)
        {
            accelerator_percentage = 0;
        }
        vehicle_speed_float =
            update_speed(vehicle_speed_float, accelerator_percentage, brake_percentage);
        uint16_t vehicle_speed = (int16_t)(vehicle_speed_float);

        // spedometer Graph
        double angle = -30 + 1 * vehicle_speed_float;
        double radians = angle * M_PI / 180.0;
        int16_t Radius = 107;
        int16_t centerX = 788;
        int16_t centerY = 278;
        int16_t finalX = centerX - (int16_t)(Radius * cos(radians));
        int16_t finalY = centerY - (int16_t)(Radius * sin(radians));
        draw_line(renderer, centerX, centerY, finalX, finalY, red);

        // accelerator
        draw_image_button(renderer, &accelarator, "./aux/img/accelerator.png");
        char accelerator_perc_text[50];
        snprintf(accelerator_perc_text, sizeof(accelerator_perc_text), "%d %%",
                 accelerator_percentage);
        draw_text(renderer, font3, accelerator_perc_text, 420, 760, white);

        // brake
        draw_image_button(renderer, &brake, "./aux/img/brake.png");
        char brake_perc_text[50];
        snprintf(brake_perc_text, sizeof(brake_perc_text), "%d %%", brake_percentage);
        draw_text(renderer, font3, brake_perc_text, 330, 760, white);

        // buttons of the first panel - vehicle controls
        for (int16_t i = 0; i < NUM_BUTTONS1; i++)
        {
            SDL_Color buttonColor = buttons1[i].clicked ? button_clicked : button_not_clicked;
            draw_button(renderer, &buttons1[i], buttonColor, font3);
        }

        // Pins Reading
        read_pin_status(&hazard_lights_state, HAZARD_BUTTON_PIN);
        read_pin_status(&hazard_light, HAZARD_LIGHTS_PIN);
        read_pin_status(&reb_imobilize_procedure, REB_IPC_WARNING);
        read_pin_status(&reb_fault_warning, REB_IPC_FAULT_PIN);
        read_pin_status(&reb_vehicle_imobilized, ENGINE_REB_MODE);

        if (reb_fault_warning == 1)
        {
            draw_image(renderer, "./aux/img/reb_red.png", 380, 154, 408 / 7, 227 / 7);

            char velocidade[50];
            snprintf(velocidade, sizeof(velocidade), "REB Fault");
            draw_text(renderer, font2, velocidade, 500, 280, white);
        }
        else if (reb_imobilize_procedure == 1 && reb_vehicle_imobilized == 0)
        {
            draw_image(renderer, "./aux/img/reb_yellow.png", 380, 154, 408 / 7, 227 / 7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Engine Will be Gradually");
            draw_text(renderer, font2, msg, 500, 270, white);

            char msg2[50];
            snprintf(msg2, sizeof(msg2), "Deactivated in 5 minutes");
            draw_text(renderer, font2, msg2, 500, 290, white);
        }
        else if (reb_vehicle_imobilized == 1 && vehicle_speed != 0)
        {
            draw_image(renderer, "./aux/img/reb_yellow.png", 380, 154, 408 / 7, 227 / 7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Engine is Stopping");
            draw_text(renderer, font2, msg, 500, 280, white);
        }
        else if (reb_vehicle_imobilized == 1 && vehicle_speed == 0)
        {
            draw_image(renderer, "./aux/img/reb_green.png", 380, 154, 408 / 7, 227 / 7);

            char msg[50];
            snprintf(msg, sizeof(msg), "Vehicle is Imobilized");
            draw_text(renderer, font2, msg, 500, 280, white);
        }
        else
        {
            char velocidade[10]; // speed text
            snprintf(velocidade, sizeof(velocidade), "%d", vehicle_speed);
            draw_text(renderer, font, velocidade, 500, 270, white);

            char km_indicator[10]; // km/h text
            snprintf(km_indicator, sizeof(km_indicator), "km/h");
            draw_text(renderer, font2, km_indicator, 500, 310, white);
        }

        if (hazard_lights_state == 1) // Hazard lights
        {
            draw_image(renderer, "./aux/img/hazard_warning.png", 585, 154, 92 / 2.5, 85 / 2.5);
            if (hazard_light == 1)
            {
                draw_image(renderer, "./aux/img/left_sign.png", 328, 148, 39, 47);
                draw_image(renderer, "./aux/img/right_sign.png", 632, 148, 39, 47);
            }
        }

        // terminal text update
        for (uint8_t i = 0; i < 6; i++)
        {
            // data in white
            draw_text_top_right(renderer, font4, terminal_frame_str[i], 500, 600 + 40 * i, white);

            // timestamp in yellow
            draw_text_top_right(renderer, font4, timestamp[i], 500, 600 + 40 * i, yellow);
        }

        SDL_RenderPresent(renderer); // Atualizar a tela
        SDL_Delay((1 / FPS) * 1000); // Define o FPS 100
    }

    // cleanup resorces
    pthread_cancel(terminal_read_can_th);
    TTF_CloseFont(font);
    ipc_render_cleanup(&window, &renderer);

    return 0;
}