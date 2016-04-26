/*
 * RidiciJednotka.c
 *
 * Created: 19.4.2016 16:37:57
 * Author : atom2

 * Main control board
 *
 * USART0 - USB (PC)
 * USART1 - RS485
 * I2C - ATmega8
 *
 */

#include "inc/common_defs.h"
#include "inc/defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "inc/uart_types.h"
#include "inc/uart_tri_1.h"
#include "inc/uart_tri_0.h"

#include "inc/adc.h"
#include "inc/timer.h"
#include "inc/Tribus_types.h"
#include "inc/Tribus.h"
#include "inc/packet_processor.h"
#include "inc/graphic.h"
#include "inc/t6963c.h"


volatile byte timer0_flag = 0; // T = 10ms

byte Dtime = 0;
Ttripac CheckModules;  // buffer
uint8_t Back=0;
extern uint8_t Pocet;

//----------------------------------------------------------
ISR(TIMER0_COMPA_vect) {
  // T = 10ms
  timer0_flag = true;
}

ISR(TIMER0_COMPB_vect) {
  // T = 10ms
  timer0_flag = true;
}

ISR(TIMER0_OVF_vect) {
  // T = 10ms
  timer0_flag = true;
}


uint8_t try_receive_data(void)
{
	byte i;
	byte *ptr;
	//TB_SendAck(100, uart1_flags.data_received);
	/*if (!uart1_flags.data_received)
	{
		Pocet = 4;
		ptr = uart1_get_data_begin();
		for (i=0; i<9; i++)
		{
			Pocet = 5;
			TB_bufIn[i] = *ptr;
			ptr++;
		}
		uart1_get_data_end();
		uart1_flags.data_received = FALSE;
		if (TB_Read() == 0)
		{
			Pocet = 6;
			switch (TB_Decode())
			{
				Pocet = 7;
				case TB_CMD_GGP:
				switch (TB_bufIn[TB_BUF_TYPE])
				{
					return 1;
				}
				break;
			}
		}
	}*/
	return 0;
}

//----------------------------------------------------------
void process_timer_100Hz(void)
{
  if (timer0_flag)
  { // T = 10ms
    timer0_flag = false;
    uart0_ISR_timer();
    uart1_ISR_timer();
    //uart2_ISR_timer();
  }
}

//----------------------------------------------------------
void init(void)
{

  //DDRB = BV(PB0);
  DDRD = BV(PD1) | BV(PD3) | BV(PD4);

  uart0_init(); // PC
  uart1_init(); // internal
  adc_init();
  timer_init();
  TB_Callback_setBaud = uart1_set_baud;
  TB_Init((void*) 0x10); // addr in eeprom with settings
  pp_init();
  sei();
}


//----------------------------------------------------------
int main(void)
{
	init();
	
	GLCD_Initalize(); // Initalize LCD
	
	GLCD_ClearText(); // Clear text area
	GLCD_ClearCG(); // Clear character generator area
	GLCD_ClearGraphic(); // Clear graphic area

	GLCD_TextGoTo(5,T6963_CURSOR_1_LINE);// set text coordinates
	GLCD_WriteString("AtomTrace - Sci-Trace"); // write text
	/*
	CheckModules.n.addr = 0x09;
	CheckModules.n.cmd = 0x0A;
	CheckModules.n.motor = 0x00;
	CheckModules.n.type = TB_GBPARAM_ADDRESS;
	CheckModules.n.val.dw = 0;
	CheckModules.n.sum = TB_calcSum_Bus((byte *) &CheckModules);
	
	//byte * data_ptr;
	for (uint8_t cnt=1; cnt < 255; cnt++)
	{
		Pocet = 0;
		CheckModules.n.addr = cnt;
		uart1_put_data((byte *) &CheckModules);
		if (timer0_flag)
		{ // T = 10ms
			timer0_flag = false;
			uart1_ISR_timer();
		}
		CheckModules.n.addr++;
		CheckModules.n.sum = TB_calcSum_Bus((byte *) &CheckModules);
	}
	*/
	/*
	GLCD_TextGoTo(0,T6963_CURSOR_2_LINE);// set text coordinates
	GLCD_WriteString("Number of modules: "); // write text
	GLCD_WriteChar(Pocet & 0x30); // write text
	*/
	uint16_t Hodnota=0;
	uint8_t MSB, LSB;
	char Vystup[12];
	float Convert;
	
	GLCD_TextGoTo(0,T6963_CURSOR_3_LINE);// set text coordinates
	GLCD_WriteString("Bus-A:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_4_LINE);// set text coordinates
	GLCD_WriteString("Bus-B:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_5_LINE);// set text coordinates
	GLCD_WriteString("USB:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_6_LINE);// set text coordinates
	GLCD_WriteString("+5V:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_7_LINE);// set text coordinates
	GLCD_WriteString("+12V:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_8_LINE);// set text coordinates
	GLCD_WriteString("-12V:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_9_LINE);// set text coordinates
	GLCD_WriteString("+24V-BP:"); // write text
	
	GLCD_TextGoTo(0,T6963_CURSOR_10_LINE);// set text coordinates
	GLCD_WriteString("+24V-RJ:"); // write text
	
	
	
	
	
// 	GLCD_WriteString((char *)adc_read(4)); // write text
// 	GLCD_TextGoTo(10, T6963_CURSOR_7_LINE);// set text coordinates
// 	GLCD_WriteString((char *)adc_read(2)); // write text
// 	GLCD_TextGoTo(10, T6963_CURSOR_8_LINE);// set text coordinates
// 	GLCD_WriteString((char *)adc_read(3)); // write text
// 	GLCD_TextGoTo(10, T6963_CURSOR_9_LINE);// set text coordinates
// 	GLCD_WriteString((char *)adc_read(1)); // write text
// 	GLCD_TextGoTo(10, T6963_CURSOR_10_LINE);// set text coordinates
// 	GLCD_WriteString((char *)adc_read(0)); // write text

	
	//PRIu16
	uint8_t MereniADC=0;
	while(1)
	{ // mail loop
		pp_loop();
		process_timer_100Hz();
		uart0_process();
		uart1_process();
		if(timer0_flag)
		{
			// Každých 500 ms se provede ètení a zápis do Displeje
			if (MereniADC < 51)
			{
				MereniADC++;
			} 
			else
			{
				Hodnota = adc_read(4);
				//realne = (float) 0,795;
				//Hodnota = (float) 0,795 * Hodnota;
				//sprintf(Vystup,"%f", realne);
				sprintf(Vystup,"%i", Hodnota);
				//itoa(Hodnota, Vystup, 12);
				GLCD_TextGoTo(0,T6963_CURSOR_4_LINE);// set text coordinates
				GLCD_WriteString(Vystup);
				Hodnota = Hodnota * 0.795;
				sprintf(Vystup,"%i", Hodnota);
				GLCD_TextGoTo(0,T6963_CURSOR_5_LINE);// set text coordinates
				GLCD_WriteString(Vystup);
				
				MereniADC = 0;
			}
		}
	}
}

