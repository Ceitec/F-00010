


/***** P O Z O R  ! !  kanály 4, 5 mají u ATmega8 pouze 8 bitovou pøesnost *****/
#define ENABLE_BIT_DEFINITIONS  	/* Povoluje definici jmen pro pøístup k bitùm SFR registrù */
#include <avr/io.h>               	/* Deklarace adres a registrù pro procesor ATMega8 */
#include  "Adc_MiraN.h"

/* --------------------------------- Globální promìnné ------------------------------- */

/* Softvare návìští indikující ukonèení ADC pøevodu */
volatile unsigned char ADCUkoncenNavesti;

/* Obsluha pøerušení ukonèení pøevodu AD pøevodníku */
#pragma vector=ADC_vect;
void preruseni_ukonceni_prevodu_ADC(void)
{/* Nastavení návìští ADC pøevodu indikující ukonèení pøevodu */
  ADCUkoncenNavesti = 1;
}


/* ---------------------------- Funkce ---------------------------------------- */

/* Inicializace AD pøevodníku */
void ADCInit(void)
{	ADCSRA |= (1 << ADEN);		/* Zapnutí ADC (zapnutí napájení ADC ) */
	ADCSRA &= ~(1 << ADFR);		/* Standardnì zapnut mód pro jeden pøevod */
	ADCNastPreddelice(ADC_PREDDELIC);	/* Nastavení standardního dìlícího pomìru pøeddìlièe */
	ADCNastReference(ADC_REFERENCE);	/* Nastavení standarního zdroje referenèního napìtí */
	ADMUX &= ~(1 << ADLAR);		/* Zarovnání bitù výsledku doprava */

	ADCSRA |= (1 << ADIE);		/* Povolení pøerušení od ukonèení pøevodu */

	ADCUkoncenNavesti = 0;		/* Vynulování softvare návìští indikující ukonèení ADC pøevodu */
}

/* Vypnutí AD pøevodníku */
void ADCVyp(void)
{	ADCSRA &= ~(1 << ADIE);		/* Zákaz pøerušení od ukonèení pøevodu */
	ADCSRA &= ~(1 <<  ADEN);	              /* Vypnutí ADC (vypnutí napájení ADC ) */
}

/* Nastavení dìlícího pomìru pro hodiny A/D pøevodníku */
void ADCNastPreddelice(unsigned char preddelic)
{	ADMUX &= ~ADC_PREDDELIC_MASKA;	/* Vynulování bitù 0, 1, 2 pro nastavení pøeddìlièe */
	ADMUX |= preddelic;		/* Nastavení bitù  volby pøeddìlièe */
}

/* Výbìr napìové reference pro A/D pøevodník */
void ADCNastReference(unsigned char ref)
{	ADMUX &= ~ADC_REFERENCE_MASKA;	/* Vynulování bitù 6, 7 pro nastavení refrenèního napìtí */
	ADMUX |= (ref<<6);			/* Nastavení bitù  volby refrenèního napìtí */
}

/* Nastavení ADC vstupního kanálu */
void ADCNastKanal(unsigned char Kan)
{	ADMUX &= ~ADC_MUX_MASKA;		/* Vynulování bitù 0, 1, 2, 3, 4 pro nastavení vstupního kanálu */
	ADMUX |= (Kan & ADC_MUX_MASKA);	/* Nastavení bitù pro nastavení vstupního kanálu */
}

/* Start pøevodu na pøedvoleném vstupním kanálu */
void ADCStartPrevodu(void)
{	ADCSRA |= (1 << ADIF);		/* Vymazání hardware návìští "pøevod je kompletní"   */
	ADCSRA |= (1 << ADSC);		/* Start pøevodu */
}	

/* Pøeètení hodnoty z AD pøevodníku */
unsigned int ADCPrectiHodnotu(void)
{ return (ADCL | (ADCH<<8));		/* Vra ADC (plných 10 bitù); */
}


/* Vrací TRUE když je pøevod ukonèen */
unsigned char ADCJeUkoncen(void)
{	return (ADCSRA & (1 << ADSC));
}

/* Pøevod s pøesností 10-bit  */
/* Start pøevodu, èekání na konec pøevodu, vrácení výsledku */
unsigned int ADCPrevod10bit(unsigned char Kan)
{	ADCUkoncenNavesti = 0;			/*  Vymazání návìští ADC pøevodu indikující ukonèení pøevodu */
	ADMUX &= ~ADC_MUX_MASKA;		/* Vynulování bitù 0, 1, 2, 3, 4 pro nastavení vstupního kanálu */
	ADMUX |= (Kan & ADC_MUX_MASKA);	/* Nastavení bitù pro nastavení vstupního kanálu */
	ADCSRA |= (1 << ADIF);			/* Vymazání hardware "pøevod je kompletní" návìští  */
	ADCSRA |= (1 << ADSC);			/* Start pøevodu */
	//while(!ADCUkoncenNavesti);		/* Èekej až je pøevod ukonèen */
	//while(!(ADCSRA & (1 << ADIF)) );		/* Èekej až je pøevod ukonèen */
	while( ADCSRA & (1 << ADSC) );		/* Èekej až je pøevod ukonèen */

	/* POZOR: MUSÍ SE ÈÍST ADCL PØED ADCH  !!! */
	return (ADCL | (ADCH<<8));			/* Vra ADC (plných 10 bitù); */
}

/* Pøevod s pøesností 8-bit  */
/* Start pøevodu, èekání na konec pøevodu, vrácení výsledku */
unsigned char ADCPrevod8bit(unsigned char Kan)
{	/* proveï 10-bitový pøevod a vra nejvyšších 8 bitù */
	return (ADCPrevod10bit(Kan)>>2);		/* Vrací ADC vyšší byte */
}



