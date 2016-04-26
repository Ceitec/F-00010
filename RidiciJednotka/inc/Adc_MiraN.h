

#ifndef ADC_MIRAN_H
#define ADC_MIRAN_H

/*----------------------------- Definice ------------------------------*/

/*------- ADC hodinovı pøeddìliè ----------*/
/* Vıbìr dìlitele hodinové frekvence CPU - vytvoøení ADC hodinové frekvence */
/* menší dìlitel znamená rychlejší pøevod */
/* vyšší dìlitel znamená pøesnìjší pøevod */
#define ADC_PREDDELIC_2		0x00	/* 0x01,0x00 -> CPU clk/2 */
#define ADC_PREDDELIC_4		0x02	/* 0x02 -> CPU clk/4 */
#define ADC_PREDDELIC_8		0x03	/* 0x03 -> CPU clk/8 */
#define ADC_PREDDELIC_16	0x04	/* 0x04 -> CPU clk/16 */
#define ADC_PREDDELIC_32	0x05	/* 0x05 -> CPU clk/32 */
#define ADC_PREDDELIC_64	0x06	/* 0x06 -> CPU clk/64 */
#define ADC_PREDDELIC_128	0x07	/* 0x07 -> CPU clk/128 */
/* Standardní hodnota */
#define ADC_PREDDELIC		ADC_PREDDELIC_128	
/* Nìmìò masku pøeddìlièe !!!!!!!!  */
#define ADC_PREDDELIC_MASKA	0x07

/*----------- ADC vıbìr napìové reference ------*/
/* urèuje co je pouito pro plnı rozsah pøevodu ADC */
#define ADC_REFERENCE_AREF	0x00	/* 0x00 -> AREF pin, interní VREF vypnuta */
#define ADC_REFERENCE_AVCC	0x01	/* 0x01 -> AVCC pin, interní VREF vypnuta */
#define ADC_REFERENCE_RSVD	0x02	/* 0x02 -> Reservováno */
#define ADC_REFERENCE_256V	0x03	/* 0x03 -> Interní 2.56V VREF */
/* Standardní hodnota */
#define ADC_REFERENCE		ADC_REFERENCE_AREF
/* Nìmìò masku pro referenci !!!!!!!!!  */
#define ADC_REFERENCE_MASKA	0xC0

/*------- Bitová maska pro ADC kanálovı multipkexer ---------*/
#define ADC_MUX_MASKA		0x1F

/*-------- Definice kanálù ---------*/
/* Tyto kanály podporují všechny AVR s ADC */
#define ADC_K_ADC0			0x00
#define ADC_K_ADC1			0x01
#define ADC_K_ADC2			0x02
#define ADC_K_ADC3			0x03
#define ADC_K_ADC4			0x04
#define ADC_K_ADC5			0x05
#define ADC_K_ADC6			0x06
#define ADC_K_ADC7			0x07
#define ADC_K_122V			0x1E	/* 1.22V napìová reference */
#define ADC_K_AGND			0x1F	/* AGND */
/* Tyto kanály podporuje jen ATmega128 */
/* Diferenèní se zesilovaèem */
#define ADC_K_0_0_DIFF10X		0x08
#define ADC_K_1_0_DIFF10X		0x09
#define ADC_K_0_0_DIFF200X		0x0A
#define ADC_K_1_0_DIFF200X		0x0B
#define ADC_K_2_2_DIFF10X		0x0C
#define ADC_K_3_2_DIFF10X		0x0D
#define ADC_K_2_2_DIFF200X		0x0E
#define ADC_K_3_2_DIFF200X		0x0F
/* Diferenèní */
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

/* Kompatibilita pro nové MEGA procesory */
#ifdef ADATE
	#define ADFR	ADATE
#endif

/*------------------- Prototypy funkcí -------------------------*/

/* Inicializace A/D pøevodníku */
/* (zapne ADC a pøipraví ho pro pouití) */
void ADCInit(void);

/* Vypnutí A/D pøevodníku */
void ADCVyp(void);

/* Nastavení dìlícího pomìru pro hodiny A/D konvertoru */
/* tato funkce je automaticky volána z ADCInit() se standardní hodnotou */
void ADCNastPreddelice(unsigned char preddelic);

/* Vıbìr napìové reference pro A/D konvertor */
/* tato funkce je automaticky volána z ADCInit() se standardní hodnotou */
void ADCNastReference(unsigned char ref);

/* Nastavení ADC vstupního kanálu */
void ADCNastKanal(unsigned char Kan);

/* Start pøevodu na pøedvoleném vstupním kanálu */
void ADCStartPrevodu(void);

/* Pøeètení hodnoty z AD pøevodníku */
unsigned int ADCPrectiHodnotu(void);

/* Vrací TRUE kdy je pøevod ukonèen */
unsigned char ADCJeUkoncen(void);

/* Start pøevodu na vstupním kanálu Kan */
/* vrací 10-bitovou hodnotu po ukonèení pøevodu */
unsigned int ADCPrevod10bit(unsigned char Kan);

/* Start pøevodu na vstupním kanálu Kan */
/* vrací 8-bitovou hodnotu po ukonèení pøevodu */
unsigned char ADCPrevod8bit(unsigned char Kan);

/*----------------------------------------------- Ukázka pouití ----------------------------------------------
 unsigned int Prevod;
 ADCInit();
 ADCNastPreddelice(ADC_PREDDELIC_32);		Chci jinı pøeddìliè ne standardních 64
 ADCNastReference(ADC_REFERENCE_AVCC);	Pøipojením AVCC na 5V dostanem plnı rozsah = 5V 
 						tato volba je ji v ADCInit()
 Prevod = ADCPrevod10bit(ADC_K_ADC2)		Pøeètení 10 bitové hodnoty napìtí na kanálu 2
------------------------------------------------------------------------------------------------------------------*/

#endif
