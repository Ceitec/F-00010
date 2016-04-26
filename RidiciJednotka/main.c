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


const unsigned char gImage_Obr[330] = { /* 0X00,0X01,0X29,0X00,0X37,0X00, */
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
	0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,
	0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,
	0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,
	0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,
	0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,
	0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,
	0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,
	0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,
	0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,
	0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,
	0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,
	0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,0XFF,0XFF,
0XFF,0XFF,0XFF,0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0X80,};

const unsigned char gImage_Obr3[16] = { /* 0X00,0X01,0X0A,0X00,0X08,0X00, */
	0XC0,0XC0,0XC0,0XC0,0X30,0X00,0X30,0X00,0X0C,0X00,0X0C,0X00,0X03,0X00,0X03,0X00,
};
const uint8_t  NewBitmap [768] = {	
	// 0, 0, 96, 64,
	
	0x80 , 0x00 , 0x7F , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x08 , 0x40 , 0x3F , 0xFF , 0xFF, 
	0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x10 , 0x23 , 0xFF , 0xFF , 0xFF , 0xF0 , 0x00 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x00 , 0x20 , 0x3F , 0xFF , 0xFF , 0xFF , 0xFE , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x40, 
	0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x80 , 0x7F , 0xFF , 0xFF , 0xFF, 
	0xFF , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x00 , 0x7F , 0xFF , 0xFF , 0xFF , 0xFF , 0x80 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x02 , 0x00 , 0x7F , 0xFF , 0xFF , 0xFF , 0xFF , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x04 , 0x00,
	0x3F , 0xFF , 0xFF , 0xFC , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x08 , 0x00 , 0x3F , 0xFF , 0xFF , 0x80, 
	0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x10 , 0x00 , 0x1F , 0xFF , 0xFC , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x20 , 0x00 , 0x1F , 0xFF , 0xE0 , 0x00 , 0xFE , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x40 , 0x00, 
	0x1F , 0xFF , 0x80 , 0x1F , 0xFC , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x80 , 0x00 , 0x0F , 0xFE , 0x01 , 0xFF, 
	0xFC , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x00 , 0x00 , 0x0F , 0xFA , 0x07 , 0xFF , 0xFC , 0x00 , 0x00 , 0x00, 
	0x00 , 0x02 , 0x00 , 0x00 , 0x07 , 0xF1 , 0x3F , 0xFF , 0xF8 , 0x00 , 0x00 , 0x00 , 0x00 , 0x04 , 0x00 , 0x00, 
	0x07 , 0xC0 , 0xFF , 0xFF , 0xC0 , 0x0E , 0x00 , 0x00 , 0x00 , 0x08 , 0x00 , 0x00 , 0x07 , 0x81 , 0xFF , 0xFF, 
	0x00 , 0x0E , 0x00 , 0x00 , 0x00 , 0x10 , 0x00 , 0x00 , 0x03 , 0x07 , 0xFF , 0xFE , 0x00 , 0x08 , 0x00 , 0x00, 
	0x00 , 0x20 , 0x00 , 0x00 , 0x00 , 0x0F , 0xFF , 0xF8 , 0x00 , 0x08 , 0x03 , 0x00 , 0x00 , 0x40 , 0x00 , 0x00, 
	0x00 , 0x3F , 0xFF , 0xE0 , 0x20 , 0x08 , 0x03 , 0x00 , 0x00 , 0x80 , 0x00 , 0x00 , 0x00 , 0x7F , 0xFF , 0xC0, 
	0xE0 , 0x00 , 0x0F , 0x00 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0xFF , 0xFF , 0x83 , 0xC0 , 0x00 , 0x0F , 0x00, 
	0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0xFF , 0xFF , 0x07 , 0xC0 , 0x00 , 0x3C , 0x00 , 0x04 , 0x00 , 0x00 , 0x00,
	0x00 , 0xFF , 0xFC , 0x9F , 0x80 , 0x00 , 0x1C , 0x00 , 0xC8 , 0x00 , 0x00 , 0x00 , 0x00 , 0x7F , 0xF8 , 0x7F, 
	0x80 , 0x00 , 0x1C , 0x00 , 0xF0 , 0x00 , 0x00 , 0x00 , 0x00 , 0x7F , 0xF8 , 0x7F , 0x87 , 0x07 , 0xFF , 0xF0, 
	0xF0 , 0x60 , 0x0F , 0x00 , 0x00 , 0x7F , 0xF8 , 0xFF , 0x87 , 0x07 , 0xFF , 0xF0 , 0xF0 , 0x40 , 0x0F , 0x00, 
	0x00 , 0x3F , 0xF7 , 0xFF , 0x84 , 0x07 , 0xC0 , 0x03 , 0xC7 , 0xC0 , 0x7F , 0x00 , 0x00 , 0x3F , 0xA7 , 0xFF, 
	0x84 , 0x07 , 0xC0 , 0x01 , 0xC7 , 0xC0 , 0x7F , 0x00 , 0x00 , 0x1F , 0xFF , 0xFF , 0x1C , 0x01 , 0xC0 , 0x03, 
	0xCF , 0x60 , 0xF7 , 0x00 , 0x00 , 0x1F , 0xFF , 0xFF , 0x18 , 0x01 , 0x80 , 0x07 , 0x38 , 0x41 , 0x87 , 0x00, 
	0x00 , 0x0E , 0xFF , 0xFE , 0x98 , 0x03 , 0x80 , 0x0D , 0x38 , 0x41 , 0x87 , 0x02 , 0x00 , 0x0C , 0xFF , 0xFC, 
	0x78 , 0x07 , 0x80 , 0x17 , 0x30 , 0xCF , 0x06 , 0x07 , 0x00 , 0x0C , 0xFF , 0xFC , 0x78 , 0x07 , 0x80 , 0x27, 
	0x30 , 0xCF , 0x06 , 0x07 , 0x00 , 0x03 , 0xFF , 0xF8 , 0xF0 , 0x0E , 0x00 , 0x4C , 0xC0 , 0xBC , 0x06 , 0x07, 
	0x00 , 0x03 , 0xFF , 0xF0 , 0xE8 , 0x0E , 0x04 , 0x8D , 0xC0 , 0x9C , 0x0C , 0x0B , 0x00 , 0x03 , 0xFF , 0xF0, 
	0xE4 , 0x0E , 0x0D , 0x0F , 0x00 , 0xF0 , 0x0E , 0x0F , 0x00 , 0x03 , 0xFF , 0xF0 , 0xE2 , 0x0E , 0x72 , 0x0C, 
	0x00 , 0xC0 , 0x0F , 0x0F , 0x00 , 0x07 , 0xFF , 0xE3 , 0xA1 , 0x0F , 0xD4 , 0x28 , 0x00 , 0x80 , 0x07 , 0x0F, 
	0x00 , 0x07 , 0xFF , 0xE3 , 0x80 , 0xBF , 0xC8 , 0x78 , 0x00 , 0x80 , 0x07 , 0x1F , 0x00 , 0x07 , 0xFF , 0xC3, 
	0x80 , 0x7F , 0xD0 , 0x78 , 0x00 , 0x80 , 0x07 , 0x1F , 0x00 , 0x07 , 0xFF , 0xC3 , 0x80 , 0x2F , 0xA0 , 0x00, 
	0x00 , 0x00 , 0x00 , 0x3F , 0x00 , 0x00 , 0xFF , 0xC0 , 0x00 , 0x10 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 
	0x00 , 0x00 , 0xFF , 0x80 , 0x00 , 0x08 , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x7F , 0x80, 
	0x00 , 0x05 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x7F , 0x00 , 0x00 , 0x02 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x3F , 0x00 , 0x00 , 0x05 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x3F , 0x00 , 0x00 , 0x08 , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x3E , 0x00, 
	0x00 , 0x10 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x1E , 0x00 , 0x00 , 0x20 , 0x20 , 0x00, 
	0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x1C , 0x00 , 0x00 , 0x40 , 0x10 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x0C , 0x3F , 0x80 , 0x80 , 0x08 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x08 , 0xBF, 
	0x81 , 0x00 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x0D , 0xFF , 0xFF , 0x00 , 0x02 , 0x00, 
	0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x0F , 0xFF , 0xFF , 0x00 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 
	0x00 , 0x00 , 0x3F , 0x00 , 0xFF , 0xC0 , 0xFF , 0xF8 , 0x00 , 0x00 , 0x0F , 0xF0 , 0x00 , 0x00 , 0x7E , 0x00, 
	0x1F , 0xE0 , 0xFF , 0xF0 , 0x00 , 0x00 , 0x7F , 0xFE , 0x00 , 0x00 , 0xF8 , 0x00 , 0x3F , 0xF8 , 0xFF , 0xF0, 
	0x00 , 0x03 , 0xFF , 0xFF , 0x00 , 0x00 , 0xF8 , 0x00 , 0x5F , 0xF8 , 0xFF , 0xF8 , 0x00 , 0x03 , 0xFF , 0xFF, 
	0x00 , 0x03 , 0xF8 , 0x38 , 0x8F , 0xF8 , 0x1F , 0x88 , 0x00 , 0x01 , 0xF8 , 0x07 , 0x00 , 0x03 , 0xF8 , 0xFF, 
	0x8F , 0x3C , 0x1F , 0x84 , 0x00 , 0x0F , 0xC0 , 0x03 , 0x00 , 0x03 , 0xF8 , 0xFF , 0x8F , 0x7C , 0x1F , 0x82, 
	0x00 , 0x07 , 0xC0 , 0x03 , 0x00 , 0x07 , 0xFF , 0xFF , 0x8F , 0xDE , 0x1F , 0x81 , 0x00 , 0x0F , 0x80 , 0x00, 
};






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
				GLCD_WriteString("-12V:       ");
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