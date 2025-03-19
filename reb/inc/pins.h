#ifndef H_PINS
#define H_PINS

// pin 0 - hazard lights                 Pino da luz de pisca alerta
// pin 1 - Hazard lights toggle          Pino do botão do pisca alerta
// pin 2 - Server Lock Engine            Pino que envia envia bloqueio REB 
// pin 3 - Server Unlock Engine          Pino que envia envia desbloqueio REB
// pin 4 - REB driver warning status     Pino que envia sinal de REB ativado ao motorista

#define hazard_lights_pin 0     
#define hazard_toggle_pin 1         
#define reb_lock_eng_pin 2
#define reb_unlock_eng_pin 3
#define reb_driv_warn_pin 4

#endif