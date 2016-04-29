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
#include "inc/Adc_MiraN.h"
#include "inc/timer.h"
#include "inc/Tribus_types.h"
#include "inc/Tribus.h"
#include "inc/packet_processor.h"
#include "inc/graphic.h"
#include "inc/t6963c.h"
#include <string.h>
#include <math.h>
#include <stdio.h>


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
	//byte i;
	//byte *ptr;
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
  //ADCInit();
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
	
	char Vystup[20];
	uint8_t MereniADC=0;
	float Convert=0;
	uint8_t PosunX=0, PosunY=0;
	
	GLCD_Initalize(); // Initalize LCD
	
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
	
	GLCD_ClearText(); // Clear text area
	GLCD_ClearCG(); // Clear character generator area
	GLCD_ClearGraphic(); // Clear graphic area
	
	PosunX = 5;
	PosunY = 2;

	while(1)
	{ // mail loop
		pp_loop();
		process_timer_100Hz();
		uart0_process();
		uart1_process();
		
		if(timer0_flag)
		{
			
			
			// Ka�d�ch 1000 ms se provede �ten� a z�pis do Displeje
			if (MereniADC < 101)
			{
				MereniADC++;
			}
			// Ka�d�ch 5000ms se provede vymaz�n� displaye
			else
			{
				// ADC 7 - USB
				// ADC 6 - BUS-A
				// ADC 5 - BUS-B
				// ADC 4 - +5V
				// ADC 3 - -12V
				// ADC 2 - +12V
				// ADC 1 - +24V - BackPlane
				// ADC 0 - +24V - On Board
				// Display BUS-A - Line 3
				// Display BUS-B - Line 4
				// Display USB - Line 5
				// Display +5V - Line 6
				// Display +12V - Line 7
				// Display -12V - Line 8
				// Display +24V - BackPlane - Line 9
				// Display +24V - On Board - Line 10
				// 0.795
				// Nap�jec� nap�t� / (1024 / (Reference / Deli�ka nap�t�))
				//= 5 / (1024 / ( 2.54 / 1.56 ))
				
				//Vymazani Duch� z Displaye
				GLCD_ClearGraphic(); // Clear graphic area
				
				GLCD_TextGoTo(5,T6963_CURSOR_1_LINE);// set text coordinates
				GLCD_WriteString("AtomTrace - Sci-Trace"); // write text
				
				// Bus-A
				memset(Vystup, 0, 12);
				Convert = (adc_read(6) * 7.65f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_3_LINE + PosunY);
				GLCD_WriteString("Bus-A:      ");
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);
				
				// Bus-B
				memset(Vystup, 0, 12);
				Convert = (adc_read(5) * 7.65f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_4_LINE + PosunY);
				GLCD_WriteString("Bus-B:      ");
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);
				
				// USB
				memset(Vystup, 0, 12);
				Convert = (adc_read(7) * 7.65f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_5_LINE + PosunY);
				GLCD_WriteString("USB:        ");
				if (Convert > 4.5f)
				{
					GLCD_WriteString("Connect");
				} 
				else
				{
					GLCD_WriteString("  N.C. ");
				}
				
				// +5V
				memset(Vystup, 0, 12);
				Convert = (adc_read(4) * 7.65f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_6_LINE + PosunY);
				GLCD_WriteString("+5V:        ");
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);
				
				// +12V
				memset(Vystup, 0, 12);
				Convert = (adc_read(2) * 10.80f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_7_LINE + PosunY);
				GLCD_WriteString("+12V:       ");
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);
				
				// -12V - 2,01V
				memset(Vystup, 0, 12);
				Convert = (adc_read(3) * -12.93f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_8_LINE + PosunY);
				GLCD_WriteString("-12V:");
				GLCD_TextGoTo(12 + PosunX, T6963_CURSOR_8_LINE + PosunY);
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);

				// +24V - BP
				memset(Vystup, 0, 12);
				Convert = (adc_read(1) * 31.52f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_9_LINE + PosunY);
				GLCD_WriteString("+24V-BP:    ");
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);
				
				// +24V - RJ
				memset(Vystup, 0, 12);
				Convert = (adc_read(0) * 31.52f / 1000.0f);
				GLCD_TextGoTo(0 + PosunX, T6963_CURSOR_10_LINE + PosunY);
				GLCD_WriteString("+24V-RJ:    ");
				sprintf(Vystup,"%+6.2fV", Convert);
				GLCD_WriteString(Vystup);

// 				Hodnota = (adc_read(7) * 7.95);
// 				if (Hodnota > 4700)
// 				{
// 					
// 				}
//  				else
//  				{
// 					 GLCD_TextGoTo(10, T6963_CURSOR_5_LINE);// set text coordinates
// 					GLCD_WriteString("N.C.   ");
//  				}
				
				
				MereniADC = 0;
			}
		}
	}
}

/*
if (VymazaniDisplay > 5)
{
	GLCD_ClearText(); // Clear text area
	GLCD_ClearCG(); // Clear character generator area
	GLCD_ClearGraphic(); // Clear graphic area
	GLCD_TextGoTo(5,T6963_CURSOR_1_LINE);// set text coordinates
	GLCD_WriteString("AtomTrace - Sci-Trace"); // write text
	
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
	VymazaniDisplay = 0;
}*/
/*

GLCD_TextGoTo(5,T6963_CURSOR_1_LINE);// set text coordinates
GLCD_WriteString("AtomTrace - Sci-Trace"); // write text

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

*/