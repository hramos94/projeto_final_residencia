#include <app.h>
#include <ecu.h>
#include <mcal.h>

#include <ecu_reb.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (application_init() != SUCCESS)
    {
        show_error("application_init error\n");
    }

    pthread_t read_input_th = new_thread(read_input);
    pthread_t monitor_read_can_th = new_thread(monitor_read_can);
    pthread_t countdown_reb_th = new_thread(countdown_reb);

    pthread_join(read_input_th, NULL);
    pthread_join(monitor_read_can_th, NULL);
    pthread_join(countdown_reb_th, NULL);
}
