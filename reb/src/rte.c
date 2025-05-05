#include <app.h>
#include <ecu.h>
#include <mcal.h>

#include "dtc_codes_reb.h"
#include "dtc_logger.h"
#include <ecu_reb.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (application_init() != SUCCESS)
    {
        REPORT_ERROR("application_init error\n", DTC_RTE_INIT_FAIL);
    }

    pthread_t read_input_th = new_thread(read_input);
    pthread_t monitor_read_can_th = new_thread(monitor_read_can);
    pthread_t countdown_reb_th = new_thread(countdown_reb);

    int ret = 0;

    ret = pthread_join(read_input_th, NULL);
    if (ret != 0)
    {
        REPORT_ERROR("Error to join the read_input_th thread\n", DTC_RTE_INIT_FAIL);
    }
    
    ret = pthread_join(monitor_read_can_th, NULL);
    if (ret != 0)
    {
        REPORT_ERROR("Error to join the monitor_read_can_th thread\n", DTC_RTE_INIT_FAIL);
    }

    ret = pthread_join(countdown_reb_th, NULL);
    if (ret != 0)
    {
        REPORT_ERROR("Error to join the countdown_reb_th thread\n", DTC_RTE_INIT_FAIL);
    }    
}
