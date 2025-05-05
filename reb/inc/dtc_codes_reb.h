#ifndef DTC_CODES_REB_H
#define DTC_CODES_REB_H

#define DTC_MCAL_INIT_FAIL 0x03200001u          // reb/app
#define DTC_CAN_INIT_FAIL 0x03200002u           // reb/app
#define DTC_READ_INPUT_FAIL 0x03200003u         // reb/app
#define DTC_CAN_RESPONSE_FAIL 0x02200001u       // reb/app
#define DTC_MONITOR_READ_CAN_FAIL 0x02200002u   // reb/app
#define DTC_REB_CAN_IPC_CANCEL_FAIL 0x04200001u // reb/app
#define DTC_REB_CAN_ECU_CANCEL_FAIL 0x04200002u // reb/app
#define DTC_REB_CAN_IPC_START_FAIL 0x04200003u  // reb/app
#define DTC_REB_CANCELLED_TIMEOUT 0x04200004u   // reb/app
#define DTC_TCU_CANCEL_REB_FAIL 0x04200005u     // reb/ecu_reb
#define DTC_TCU_START_REB_FAIL 0x04200006u      // reb/ecu_reb
#define DTC_RTE_INIT_FAIL 0x05200001u           // reb/rte

#endif