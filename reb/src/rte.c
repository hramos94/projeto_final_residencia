#include <app.h>
#include <ecu.h>
#include <mcal.h>

int main(int argc, char *argv[])
{

    if (application_init() != SUCCESS)
    {
        show_error("application_init error\n");
    }

    pthread_t read_input_th = new_thread(read_input);
    pthread_t monitor_read_can_th = new_thread(monitor_read_can);
    pthread_t respond_to_can_test_th = new_thread(respond_to_can_test);

    pthread_join(read_input_th, NULL);
    pthread_join(monitor_read_can_th, NULL);
    pthread_join(respond_to_can_test_th, NULL);
}
