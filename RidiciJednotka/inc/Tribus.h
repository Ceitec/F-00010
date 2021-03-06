#ifndef TRIBUS_H
#define TRIBUS_H

#define TRI_LEN (9) 

// ttype defines
struct TB_IO {
  byte b0 : 1;
  byte b1 : 1;
  byte b2 : 1;
  byte b3 : 1;
  byte b4 : 1;
  byte b5 : 1;
  byte b6 : 1;
  byte b7 : 1;
};

struct TB_PARAM {
  unsigned long target_position;
  unsigned long actual_position;
  unsigned long speed;
  unsigned long acceleration;
  unsigned long current;
  unsigned long current_hold;
//  unsigned word position reached : 1; // 8
  byte resolution; // 140
  unsigned long rfs_distance; // 196 (length output)
};

struct TB_GBPARAM {
  byte eemagic;
  byte baud;
  byte address;
  byte telegram_pause_time;
  byte host_address;

};

// shared variables
extern volatile struct TB_PARAM TB_param;
extern volatile struct TB_GBPARAM TB_gbparam;
extern volatile struct TB_IO TB_inp;
extern volatile struct TB_IO TB_out;

extern byte TB_bufOut[9];
extern byte TB_bufIn[9];
extern byte TB_AddrReply;
extern byte TB_AddrModule;
extern long int TB_Value;
extern byte TB_send_flag;
//extern void (*TB_Callback_TX)(); // callback for sending data
extern void (*TB_Callback_setBaud)(byte); // callback for switch uart speed


void TB_Init(void * setting_in_eeprom);

byte TB_calcSum_Bus(byte * Buffer);

// master mode
void TB_SetParam(byte addr, byte command, byte type, byte motor, long int value);

// slave mode
void TB_SendAck(byte status, long int value);
void TB_SendAckOK(void);
void TB_SendData(byte Address, byte Command, byte Type, byte Motor, long int value);
byte TB_Read(void);
byte TB_Decode(void);

#endif
