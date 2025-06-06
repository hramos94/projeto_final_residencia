#include <app.h>
#include <ecu.h>
#include <ipc.h>
#include <mcal.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (application_init() != SUCCESS)
    {
        show_error("application_init error\n");
    }

    pthread_t hazard_th = new_thread(hazard_lights_blink);
    pthread_t read_input_th = new_thread(read_input);
    pthread_t monitor_read_can_th = new_thread(monitor_read_can);
    pthread_t monitor_tcu_th = new_thread(monitor_tcu);
    pthread_t check_can_communication_th = new_thread(check_can_communication);

    // pthread_t gui_runner_th = new_thread(gui_runner);
    pthread_t ipc_runner_th = new_thread(ipc_runner);

    if (pthread_join(hazard_th, NULL) != 0)
    {
        show_error("ret_error\n");
    }
    if (pthread_join(read_input_th, NULL) != 0)
    {
        show_error("ret_error\n");
    }
    // pthread_join(gui_runner_th, NULL);
    if (pthread_join(ipc_runner_th, NULL) != 0)
    {
        show_error("ret_error\n");
    }
    if (pthread_join(monitor_read_can_th, NULL) != 0)
    {
        show_error("ret_error\n");
    }
    if (pthread_join(monitor_tcu_th, NULL) != 0)
    {
        show_error("ret_error\n");
    }
    if (pthread_join(check_can_communication_th, NULL) != 0)
    {
        show_error("ret_error\n");
    }
}
