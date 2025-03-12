#include <ecu.h>
#include <mcal.h>

uint8_t get_hazard_button_status(uint8_t *status){
  if(read_pin_status(status, 1) == FAIL){
    show_error("ECU.read_pin_status FAIL\n");
    return FAIL;
  }
  return SUCCESS;
} 

uint8_t set_hazard_light(uint8_t status){
  if(set_pin_status(status, 0)==FAIL){
    show_error("ECU.set_pin_status FAIL\n");
    return FAIL;
  }
  return SUCCESS;
}

uint8_t read_console(){
  uint8_t pin = 0, status = 0;
  if(read_pint_status(&pin, &status) == FAIL){
    show_error("read_pin_status FAIL\n");
    return FAIL;
  }

  if(set_pin_status(status, pin)== FAIL){
    show_error("read_console.set_pin_status FAIL\n");
    return FAIL;
  }

  return SUCCESS;
}
