

#ifndef ADC_MIRAN_H
#define ADC_MIRAN_H

/*----------------------------- Definice ------------------------------*/

/*------- ADC hodinov� p�edd�li� ----------*/
/* V�b�r d�litele hodinov� frekvence CPU - vytvo�en� ADC hodinov� frekvence */
/* men�� d�litel znamen� rychlej�� p�evod */
/* vy��� d�litel znamen� p�esn�j�� p�evod */
#define ADC_PREDDELIC_2		0x00	/* 0x01,0x00 -> CPU clk/2 */
#define ADC_PREDDELIC_4		0x02	/* 0x02 -> CPU clk/4 */
#define ADC_PREDDELIC_8		0x03	/* 0x03 -> CPU clk/8 */
#define ADC_PREDDELIC_16	0x04	/* 0x04 -> CPU clk/16 */
#define ADC_PREDDELIC_32	0x05	/* 0x05 -> CPU clk/32 */
#define ADC_PREDDELIC_64	0x06	/* 0x06 -> CPU clk/64 */
#define ADC_PREDDELIC_128	0x07	/* 0x07 -> CPU clk/128 */
/* Standardn� hodnota */
#define ADC_PREDDELIC		ADC_PREDDELIC_128	
/* N�m�� masku p�edd�li�e !!!!!!!!  */
#define ADC_PREDDELIC_MASKA	0x07

/*----------- ADC v�b�r nap�ov� reference ------*/
/* ur�uje co je pou�ito pro pln� rozsah p�evodu ADC */
#define ADC_REFERENCE_AREF	0x00	/* 0x00 -> AREF pin, intern� VREF vypnuta */
#define ADC_REFERENCE_AVCC	0x01	/* 0x01 -> AVCC pin, intern� VREF vypnuta */
#define ADC_REFERENCE_RSVD	0x02	/* 0x02 -> Reservov�no */
#define ADC_REFERENCE_256V	0x03	/* 0x03 -> Intern� 2.56V VREF */
/* Standardn� hodnota */
#define ADC_REFERENCE		ADC_REFERENCE_AREF
/* N�m�� masku pro referenci !!!!!!!!!  */
#define ADC_REFERENCE_MASKA	0xC0

/*------- Bitov� maska pro ADC kan�lov� multipkexer ---------*/
#define ADC_MUX_MASKA		0x1F

/*-------- Definice kan�l� ---------*/
/* Tyto kan�ly podporuj� v�echny AVR s ADC */
#define ADC_K_ADC0			0x00
#define ADC_K_ADC1			0x01
#define ADC_K_ADC2			0x02
#define ADC_K_ADC3			0x03
#define ADC_K_ADC4			0x04
#define ADC_K_ADC5			0x05
#define ADC_K_ADC6			0x06
#define ADC_K_ADC7			0x07
#define ADC_K_122V			0x1E	/* 1.22V nap�ov� reference */
#define ADC_K_AGND			0x1F	/* AGND */
/* Tyto kan�ly podporuje jen ATmega128 */
/* Diferen�n� se zesilova�em */
#define ADC_K_0_0_DIFF10X		0x08
#define ADC_K_1_0_DIFF10X		0x09
#define ADC_K_0_0_DIFF200X		0x0A
#define ADC_K_1_0_DIFF200X		0x0B
#define ADC_K_2_2_DIFF10X		0x0C
#define ADC_K_3_2_DIFF10X		0x0D
#define ADC_K_2_2_DIFF200X		0x0E
#define ADC_K_3_2_DIFF200X		0x0F
/* Diferen�n� */
#define ADC_K_0_1_DIFF1X		0x10
#define ADC_K_1_1_DIFF1X		0x11
#define ADC_K_2_1_DIFF1X		0x12
#define ADC_K_3_1_DIFF1X		0x13
#define ADC_K_4_1_DIFF1X		0x14
#define ADC_K_5_1_DIFF1X		0x15
#define ADC_K_6_1_DIFF1X		0x16
#define ADC_K_7_1_DIFF1X		0x17

#define ADC_K_0_2_DIFF1X		0x18
#define ADC_K_1_2_DIFF1X		0x19
#define ADC_K_2_2_DIFF1X		0x1A
#define ADC_K_3_2_DIFF1X		0x1B
#define ADC_K_4_2_DIFF1X		0x1C
#define ADC_K_5_2_DIFF1X		0x1D

/* Kompatibilita pro nov� MEGA procesory */
#ifdef ADATE
	#define ADFR	ADATE
#endif

/*------------------- Prototypy funkc� -------------------------*/

/* Inicializace A/D p�evodn�ku */
/* (zapne ADC a p�iprav� ho pro pou�it�) */
void ADCInit(void);

/* Vypnut� A/D p�evodn�ku */
void ADCVyp(void);

/* Nastaven� d�l�c�ho pom�ru pro hodiny A/D konvertoru */
/* tato funkce je automaticky vol�na z ADCInit() se standardn� hodnotou */
void ADCNastPreddelice(unsigned char preddelic);

/* V�b�r nap�ov� reference pro A/D konvertor */
/* tato funkce je automaticky vol�na z ADCInit() se standardn� hodnotou */
void ADCNastReference(unsigned char ref);

/* Nastaven� ADC vstupn�ho kan�lu */
void ADCNastKanal(unsigned char Kan);

/* Start p�evodu na p�edvolen�m vstupn�m kan�lu */
void ADCStartPrevodu(void);

/* P�e�ten� hodnoty z AD p�evodn�ku */
unsigned int ADCPrectiHodnotu(void);

/* Vrac� TRUE kdy� je p�evod ukon�en */
unsigned char ADCJeUkoncen(void);

/* Start p�evodu na vstupn�m kan�lu Kan */
/* vrac� 10-bitovou hodnotu po ukon�en� p�evodu */
unsigned int ADCPrevod10bit(unsigned char Kan);

/* Start p�evodu na vstupn�m kan�lu Kan */
/* vrac� 8-bitovou hodnotu po ukon�en� p�evodu */
unsigned char ADCPrevod8bit(unsigned char Kan);

/*----------------------------------------------- Uk�zka pou�it� ----------------------------------------------
 unsigned int Prevod;
 ADCInit();
 ADCNastPreddelice(ADC_PREDDELIC_32);		Chci jin� p�edd�li� ne� standardn�ch 64
 ADCNastReference(ADC_REFERENCE_AVCC);	P�ipojen�m AVCC na 5V dostanem pln� rozsah = 5V 
 						tato volba je ji� v ADCInit()
 Prevod = ADCPrevod10bit(ADC_K_ADC2)		P�e�ten� 10 bitov� hodnoty nap�t� na kan�lu 2
------------------------------------------------------------------------------------------------------------------*/

#endif
