#include <app.h>
#include <mcal.h>
#include <ecu.h>

int main(int argc, char *argv[]){

  if(application_init() != SUCCESS){
    show_error("application_init error\n");
  }

  pthread_t hazard_th = new_thread(hazard_lights_blink);
  pthread_t read_input_th = new_thread(read_input);

  pthread_join(hazard_th, NULL);
  pthread_join(read_input_th, NULL);

}
