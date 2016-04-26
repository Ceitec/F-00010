


/***** P O Z O R  ! !  kan�ly 4, 5 maj� u ATmega8 pouze 8 bitovou p�esnost *****/
#define ENABLE_BIT_DEFINITIONS  	/* Povoluje definici jmen pro p��stup k bit�m SFR registr� */
#include <avr/io.h>               	/* Deklarace adres a registr� pro procesor ATMega8 */
#include  "Adc_MiraN.h"

/* --------------------------------- Glob�ln� prom�nn� ------------------------------- */

/* Softvare n�v�t� indikuj�c� ukon�en� ADC p�evodu */
volatile unsigned char ADCUkoncenNavesti;

/* Obsluha p�eru�en� ukon�en� p�evodu AD p�evodn�ku */
#pragma vector=ADC_vect;
void preruseni_ukonceni_prevodu_ADC(void)
{/* Nastaven� n�v�t� ADC p�evodu indikuj�c� ukon�en� p�evodu */
  ADCUkoncenNavesti = 1;
}


/* ---------------------------- Funkce ---------------------------------------- */

/* Inicializace AD p�evodn�ku */
void ADCInit(void)
{	ADCSRA |= (1 << ADEN);		/* Zapnut� ADC (zapnut� nap�jen� ADC ) */
	ADCSRA &= ~(1 << ADFR);		/* Standardn� zapnut m�d pro jeden p�evod */
	ADCNastPreddelice(ADC_PREDDELIC);	/* Nastaven� standardn�ho d�l�c�ho pom�ru p�edd�li�e */
	ADCNastReference(ADC_REFERENCE);	/* Nastaven� standarn�ho zdroje referen�n�ho nap�t� */
	ADMUX &= ~(1 << ADLAR);		/* Zarovn�n� bit� v�sledku doprava */

	ADCSRA |= (1 << ADIE);		/* Povolen� p�eru�en� od ukon�en� p�evodu */

	ADCUkoncenNavesti = 0;		/* Vynulov�n� softvare n�v�t� indikuj�c� ukon�en� ADC p�evodu */
}

/* Vypnut� AD p�evodn�ku */
void ADCVyp(void)
{	ADCSRA &= ~(1 << ADIE);		/* Z�kaz p�eru�en� od ukon�en� p�evodu */
	ADCSRA &= ~(1 <<  ADEN);	              /* Vypnut� ADC (vypnut� nap�jen� ADC ) */
}

/* Nastaven� d�l�c�ho pom�ru pro hodiny A/D p�evodn�ku */
void ADCNastPreddelice(unsigned char preddelic)
{	ADMUX &= ~ADC_PREDDELIC_MASKA;	/* Vynulov�n� bit� 0, 1, 2 pro nastaven� p�edd�li�e */
	ADMUX |= preddelic;		/* Nastaven� bit�  volby p�edd�li�e */
}

/* V�b�r nap�ov� reference pro A/D p�evodn�k */
void ADCNastReference(unsigned char ref)
{	ADMUX &= ~ADC_REFERENCE_MASKA;	/* Vynulov�n� bit� 6, 7 pro nastaven� refren�n�ho nap�t� */
	ADMUX |= (ref<<6);			/* Nastaven� bit�  volby refren�n�ho nap�t� */
}

/* Nastaven� ADC vstupn�ho kan�lu */
void ADCNastKanal(unsigned char Kan)
{	ADMUX &= ~ADC_MUX_MASKA;		/* Vynulov�n� bit� 0, 1, 2, 3, 4 pro nastaven� vstupn�ho kan�lu */
	ADMUX |= (Kan & ADC_MUX_MASKA);	/* Nastaven� bit� pro nastaven� vstupn�ho kan�lu */
}

/* Start p�evodu na p�edvolen�m vstupn�m kan�lu */
void ADCStartPrevodu(void)
{	ADCSRA |= (1 << ADIF);		/* Vymaz�n� hardware n�v�t� "p�evod je kompletn�"   */
	ADCSRA |= (1 << ADSC);		/* Start p�evodu */
}	

/* P�e�ten� hodnoty z AD p�evodn�ku */
unsigned int ADCPrectiHodnotu(void)
{ return (ADCL | (ADCH<<8));		/* Vra� ADC (pln�ch 10 bit�); */
}


/* Vrac� TRUE kdy� je p�evod ukon�en */
unsigned char ADCJeUkoncen(void)
{	return (ADCSRA & (1 << ADSC));
}

/* P�evod s p�esnost� 10-bit  */
/* Start p�evodu, �ek�n� na konec p�evodu, vr�cen� v�sledku */
unsigned int ADCPrevod10bit(unsigned char Kan)
{	ADCUkoncenNavesti = 0;			/*  Vymaz�n� n�v�t� ADC p�evodu indikuj�c� ukon�en� p�evodu */
	ADMUX &= ~ADC_MUX_MASKA;		/* Vynulov�n� bit� 0, 1, 2, 3, 4 pro nastaven� vstupn�ho kan�lu */
	ADMUX |= (Kan & ADC_MUX_MASKA);	/* Nastaven� bit� pro nastaven� vstupn�ho kan�lu */
	ADCSRA |= (1 << ADIF);			/* Vymaz�n� hardware "p�evod je kompletn�" n�v�t�  */
	ADCSRA |= (1 << ADSC);			/* Start p�evodu */
	//while(!ADCUkoncenNavesti);		/* �ekej a� je p�evod ukon�en */
	//while(!(ADCSRA & (1 << ADIF)) );		/* �ekej a� je p�evod ukon�en */
	while( ADCSRA & (1 << ADSC) );		/* �ekej a� je p�evod ukon�en */

	/* POZOR: MUS� SE ��ST ADCL P�ED ADCH  !!! */
	return (ADCL | (ADCH<<8));			/* Vra� ADC (pln�ch 10 bit�); */
}

/* P�evod s p�esnost� 8-bit  */
/* Start p�evodu, �ek�n� na konec p�evodu, vr�cen� v�sledku */
unsigned char ADCPrevod8bit(unsigned char Kan)
{	/* prove� 10-bitov� p�evod a vra� nejvy���ch 8 bit� */
	return (ADCPrevod10bit(Kan)>>2);		/* Vrac� ADC vy��� byte */
}



