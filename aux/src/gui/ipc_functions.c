
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <mcal.h>
#include <pins.h>
#include <ipc_functions.h>

/**
 * @brief Initializes all windows for drawing graphics.
 * 
 * This function sets up the window and renderer for graphic drawing using SDL.
 *
 * @param window Pointer to the SDL_Window to be initialized.
 * @param renderer Pointer to the SDL_Renderer to be configured.
 * @param window_width Width of the window in pixels.
 * @param window_height Height of the window in pixels.
 * 
 * @return Returns a uint8_t indicating the success or failure of the initialization.
 */
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


/**
 * @brief Performs all cleanup when the program is exiting.
 * 
 * This function cleans up resources related to the window and renderer when the program is shutting down.
 *
 * @param window Pointer to the SDL_Window that needs to be cleaned up.
 * @param renderer Pointer to the SDL_Renderer that needs to be cleaned up.
 */
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


/**
 * @brief Initializes the font.
 * 
 * This function loads a font from the specified path and sets the desired font size.
 *
 * @param font Pointer to the TTF_Font to be initialized.
 * @param font_path Path to the font file.
 * @param font_size Desired font size.
 * 
 * @return Returns a uint8_t indicating the success or failure of the font initialization.
 */
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


/**
 * @brief Draws text on the screen.
 * 
 * This function renders the specified text at the given coordinates (x, y), 
 * centering it on the screen based on the provided position.
 *
 * @param renderer The SDL_Renderer used for drawing the text.
 * @param font The TTF_Font used for rendering the text.
 * @param text The text string to be drawn.
 * @param x The x-coordinate of the center of the text.
 * @param y The y-coordinate of the center of the text.
 * @param textColor The color of the text.
 */
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


/**
 * @brief Draws text on the screen with top-right alignment.
 * 
 * This function renders the specified text at the given coordinates (x, y), 
 * aligning it to the top-right corner based on the provided position.
 *
 * @param renderer The SDL_Renderer used for drawing the text.
 * @param font The TTF_Font used for rendering the text.
 * @param text The text string to be drawn.
 * @param x The x-coordinate of the top-right corner of the text.
 * @param y The y-coordinate of the top-right corner of the text.
 * @param textColor The color of the text.
 */
void draw_text_top_right(SDL_Renderer* renderer, TTF_Font* font, const char* text, int16_t x, int16_t y, SDL_Color textColor) 
{
    // Create surface with text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface) {
        printf("Error rendering text: %s\n", TTF_GetError());
        return;
    }

    // Create a texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture) {
        printf("Error to create text texture: %s\n", SDL_GetError());
        return;
    }

    // Get the width and height of the text
    int32_t textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Calculate the position to align the text to the top-right of (x, y)
    int16_t textX = x;
    int16_t textY = y - textHeight;

    // Define the text position, size, and render it to the screen, then destroy the texture
    SDL_Rect textRect = {textX, textY, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}


/**
 * @brief Draws an image on the screen.
 * 
 * This function loads an image from the specified file path and renders it 
 * at the given coordinates (x, y) with the specified width and height.
 *
 * @param renderer The SDL_Renderer used for rendering the image.
 * @param image_path The file path to the image.
 * @param x The x-coordinate for the position of the image.
 * @param y The y-coordinate for the position of the image.
 * @param width The width of the image to be drawn.
 * @param height The height of the image to be drawn.
 */
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


/**
 * @brief Draws a rectangle on the screen.
 * 
 * This function renders a rectangle at the specified coordinates (x, y) with the given width, height, 
 * and color.
 *
 * @param renderer The SDL_Renderer used for drawing the rectangle.
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 */
void draw_rectangle(SDL_Renderer* renderer, int16_t x, int16_t y, int16_t width, int16_t height, SDL_Color color) 
{   
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);  
}


/**
 * @brief Draws a line on the screen.
 * 
 * This function renders a line from the point (x1, y1) to the point (x2, y2) with the specified color.
 *
 * @param renderer The SDL_Renderer used for drawing the line.
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 * @param color The color of the line.
 */
void draw_line(SDL_Renderer* renderer, int16_t x1, int16_t y1, int16_t x2, int16_t y2, SDL_Color color) 
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}


/**
 * @brief Draws a button as a simple rectangle.
 * 
 * This function renders a button as a rectangle with the specified color, using the provided 
 * font to display the button text.
 *
 * @param renderer The SDL_Renderer used for drawing the button.
 * @param button A pointer to the Button struct containing button properties such as position and text.
 * @param color The color of the button.
 * @param font The TTF_Font used to render the button's text.
 */
void draw_button(SDL_Renderer* renderer, Button* button, SDL_Color color, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_Color textColor = {255, 255, 255, 255};
    draw_text(renderer, font, button->label, button->rect.x + button->rect.w / 2, button->rect.y + button->rect.h / 2, textColor);
}


/**
 * @brief Draws a button with an image.
 * 
 * This function renders a button using an image from the specified file path. The button's position 
 * and size are determined by the properties in the Button struct.
 *
 * @param renderer The SDL_Renderer used for drawing the button.
 * @param button A pointer to the Button struct containing the button's position and size.
 * @param image_path The file path to the image to be used as the button's background.
 */
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

/**
 * @brief Verifies if a button is clicked and changes its state on every click.
 * 
 * This function checks if the mouse coordinates (mouseX, mouseY) are inside the button's area. 
 * If a click occurs, it toggles the button's state (e.g., from pressed to released or vice versa).
 *
 * @param button A pointer to the Button struct that represents the button to be checked.
 * @param mouseX The x-coordinate of the mouse pointer.
 * @param mouseY The y-coordinate of the mouse pointer.
 */
void handle_button_click(Button* button, int32_t mouseX, int32_t mouseY) {
    if (mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
        mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h) {
        button->clicked = !button->clicked;
    }
}


/**
 * @brief Handles the press of a pedal (button) and updates its percentage.
 * 
 * This function checks if the mouse coordinates (mouseX, mouseY) are within the button's area. 
 * If the button is pressed (clicked inside its area), the button's state is updated, 
 * and the percentage value is increased by the specified increment. If the percentage exceeds 100, it is reset to 0.
 *
 * @param button A pointer to the Button struct representing the pedal/button to be handled.
 * @param mouseX The x-coordinate of the mouse pointer.
 * @param mouseY The y-coordinate of the mouse pointer.
 * @param percentage A pointer to the percentage value that will be updated when the pedal is pressed.
 * @param increment The amount by which the percentage will be increased when the pedal is pressed.
 */
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


/**
 * @brief Handles the release of a pedal (button) and updates its percentage.
 * 
 * This function checks if the mouse coordinates (mouseX, mouseY) are within the button's area. 
 * If the button is released (clicked inside its area), the button's state is updated, 
 * and the percentage value is decreased by the specified increment.
 *
 * @param button A pointer to the Button struct representing the pedal/button to be handled.
 * @param mouseX The x-coordinate of the mouse pointer.
 * @param mouseY The y-coordinate of the mouse pointer.
 * @param percentage A pointer to the percentage value that will be updated when the pedal is released.
 * @param increment The amount by which the percentage will be decreased when the pedal is released.
 */
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

/**
 * @brief Simulates the vehicle speed update.
 * 
 * This function calculates and updates the vehicle's speed based on the accelerator and brake percentages. 
 * The speed is affected by acceleration, braking power, and friction.
 *
 * @param speed The current speed of the vehicle in km/h.
 * @param accelerator_percentage The percentage of accelerator pressed (0-100%).
 * @param brake_percentage The percentage of brake applied (0-100%).
 * 
 * @return The updated vehicle speed in km/h after applying acceleration, braking, and friction.
 */
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


