#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nuklear.h>
#include <nuklear_glfw_gl3.h>
#include <stdio.h>

#include <mcal.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 10240

int gui_runner() {
  struct nk_glfw glfw = {0};
  static GLFWwindow *win;
  struct nk_context *ctx;
  struct nk_colorf bg = {0.10f, 0.18f, 0.24f, 1.0f};


  /* Inicializa GLFW */
  if (!glfwInit()) {
    fprintf(stderr, "Erro: Falha ao inicializar GLFW!\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "REB GUI", NULL, NULL);
  if (!win) {
    fprintf(stderr, "Erro: Falha ao criar janela!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(win);
  glewExperimental = 1;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Erro: Falha ao inicializar GLEW!\n");
    return -1;
  }

  /* Inicializa Nuklear */
  ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
  struct nk_font_atlas *atlas;
  nk_glfw3_font_stash_begin(&glfw, &atlas);
  struct nk_font *font =
      nk_font_atlas_add_default(atlas, 13, NULL); // Carregar uma fonte padrão
  nk_glfw3_font_stash_end(&glfw);

  nk_style_set_font(ctx, &font->handle);

  static int button_state[10]; // Estado inicial de cada botão
  for (int i = 0; i < 10; i++) {
    button_state[i] = 0;
  }
  while (!glfwWindowShouldClose(win)) {
    glfwPollEvents();
    nk_glfw3_new_frame(&glfw);

    char buton_label[20];

    /* Interface */
    if (nk_begin(ctx, "INPUT", nk_rect(50, 50, 230, 500),
                 NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR)) {

      nk_layout_row_dynamic(ctx, 30, 1);

      for (int i = 0; i < 10; i++) {
        sprintf(buton_label, "PIN %d", i);
        if (nk_button_label(ctx, buton_label)) {
          button_state[i] = !button_state[i]; // Alterna estado
          set_pin_status(button_state[i], i);
        }
      }
    }
    nk_end(ctx);

    if (nk_begin(ctx, "OUTPUT", nk_rect(300, 50, 230, 500),
                 NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR)) {
      nk_layout_row_dynamic(ctx, 30, 1);

      uint8_t *status = 0;
      for (int i = 0; i < 10; i++) {
        read_pin_status(&status, i);
        sprintf(buton_label, "PIN %d - %s", i, status ? "ON" : "OFF");
        nk_label(ctx, buton_label, NK_TEXT_CENTERED);
      }
    }
    nk_end(ctx);

    /* Renderização */
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                    MAX_ELEMENT_BUFFER);
    glfwSwapBuffers(win);
  }

  /* Limpeza */
  if (ctx) {
    nk_glfw3_shutdown(&glfw);
  }

  if (win) {

    glfwDestroyWindow(win);
  }
  glfwTerminate();
  return 0;
}
