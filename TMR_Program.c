#include "Library/types.h"
#include "Library/Bit_math.h"
#include "TMR_Private.h"
#include "TMR_Interface.h"
#include "TMR_Config.h"
#include "avr/interrupt.h"



u8 count;
u16 ON_TIME;
u16 OFF_TIME;
u16 PERIOD;


void TMR0_Init()
{

	switch(TMR0_MODE)
	{
		case 0:
		{
			TCCR0_REG &= ~(1 << WGM00);
			TCCR0_REG &= ~(1 << WGM01);
			break;
		}
		case 1:
		{
			TCCR0_REG |=  (1 << WGM00);
			TCCR0_REG &= ~(1 << WGM01);
			break;
		}
		case 2:
		{
			TCCR0_REG &= ~(1 << WGM00);
			TCCR0_REG |=  (1 << WGM01);
			break;
		}
		case 3:
		{
			TCCR0_REG |= (1 << WGM00);
			TCCR0_REG |= (1 << WGM01);
			break;
		}
	}

		TCCR0_REG &= TMR0_CLEAR_OC_BITS;
		TCCR0_REG |= (TMR0_OC_MODE << COM00);

		TCCR0_REG &= TMR0_CLEAR_PRESCALE_BITS;
		TCCR0_REG |= TMR0_PRESCALE;

		TIMSK_REG &= TMR0_CLEAR_INTERRUPT_BITS;

#if(TMR0_OVF_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << TOIE0);

#elif(TMR0_OC_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << OCIE0);

#endif

}

void TMR0_TCNTSet(u8 start)
{
	TCNT0_REG = start;
}

void TMR0_OCRSet(u8 ocr0)
{
	OCR0_REG = ocr0;
}

/********************************************TIMER 1 DRIVERS********************************************/

void TMR1_Init()
{

	switch(TMR1_MODE)
	{
		case 0:  TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG &= ~(1 << WGM10);break;
		case 1:  TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG |= (1 << WGM10); break;
		case 2:  TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG &= ~(1 << WGM10); break;
		case 3:  TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG |= (1 << WGM10);  break;
		case 4:  TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG &= ~(1 << WGM10); break;
		case 5:	 TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG |= (1 << WGM10);  break;
		case 6:	 TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG &= ~(1 << WGM10);  break;
		case 7:	 TCCR1B_REG &= ~(1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG |= (1 << WGM10);   break;
		case 8:	 TCCR1B_REG |= (1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG &= ~(1 << WGM10);  break;
		case 9:	 TCCR1B_REG |= (1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG |= (1 << WGM10);  break;
		case 10: TCCR1B_REG |= (1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG &= ~(1 << WGM10);  break;
		case 11: TCCR1B_REG |= (1 << WGM13); TCCR1B_REG &= ~(1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG |= (1 << WGM10);   break;
		case 12: TCCR1B_REG |= (1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG &= ~(1 << WGM10);  break;
		case 13: TCCR1B_REG |= (1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG &= ~(1 << WGM11); TCCR1A_REG |= (1 << WGM10);   break;
		case 14: TCCR1B_REG |= (1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG &= ~(1 << WGM10);   break;
		case 15: TCCR1B_REG |= (1 << WGM13); TCCR1B_REG |= (1 << WGM12); TCCR1A_REG |= (1 << WGM11); TCCR1A_REG |= (1 << WGM10);    break;

	}

#if(TMR1_CHANNEL_A==ENABLE && TMR1_CHANNEL_B==DISABLE)

			TCCR1A_REG &= TMR1_CLEAR_OCA_BITS;
			TCCR1A_REG |= (TMR1_OCA_MODE << COM1A0);

#elif(TMR1_CHANNEL_A==DISABLE && TMR1_CHANNEL_B==ENABLE)

			TCCR1A_REG &= TMR1_CLEAR_OCA_BITS;
			TCCR1A_REG |= (TMR1_OCA_MODE << COM1A0);

#elif(TMR1_CHANNEL_B==ENABLE && TMR1_CHANNEL_A==ENABLE)

			TCCR1A_REG &= TMR1_CLEAR_OCB_BITS;
			TCCR1A_REG |= (TMR1_OCA_MODE << COM1A0);
			TCCR1A_REG |= (TMR1_OCB_MODE << COM1B0);

#endif

		TCCR1B_REG &= TMR1_CLEAR_PRESCALE_BITS;
		TCCR1B_REG |= TMR1_PRESCALE;

		ICR1L_REG = TMR1_ICR_VALUE;


		TIMSK_REG &= TMR1_CLEAR_INTERRUPT_BITS;

#if(TMR1_OVF_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << TOIE1);

#elif(TMR1_OCA_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << OCIE1A);

#elif(TMR1_OCB_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << OCIE1B);

#elif(TMR1_IC_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << TICIE1);
#endif
}

void TMR1_TCNT1Set(u16 tcnt1)
{
	TCNT1L_REG = (tcnt1 & 0x00FF);
	TCNT1H_REG = (tcnt1 & 0xFF00) >> 8;
}

void TMR1_OCR1ASet(u16 ocr1a)
{
	OCR1AL_REG = ocr1a;
}

void TMR1_OCR1BSet(u16 ocr1b)
{
	OCR1BL_REG = (ocr1b);
}

ISR(TIMER1_OVF_vect)
{

}

ISR(TIMER1_COMPA_vect)
{

}

ISR(TIMER1_CAPT_vect)
{
	switch(count)
	{
		case 0:TMR1_ICU_SetTCNT1(0);TCCR1B_REG &= ~(1 << ICES1); count=1; break;
		case 1:ON_TIME=TMR1_ICU_GetTCNT1();TCCR1B_REG |= (1 << ICES1); count=2; break;
		case 2:PERIOD=TMR1_ICU_GetTCNT1();count=3; break;
	}
}


//static void (*Callback) (void) = NULL;

/*void TMR1_Set_ICU_CallBackFunction(void(*Call_Back_Function))
{
	if(Call_Back_Function!=NULL)
	{
		Callback=Call_Back_Function;
	}
}

void vector_6 (void) __attribute__ ((signal));
void vector_6 (void)
{
	if(Callback != NULL)
	{
		Callback();
	}
}*/
/********************************************TIMER 2 DRIVERS********************************************/

void TMR2_Init()
{

	switch(TMR2_MODE)
	{
		case 0:
		{
			TCCR2_REG &= ~(1 << WGM20);
			TCCR2_REG &= ~(1 << WGM21);
			break;
		}
		case 1:
		{
			TCCR2_REG |=  (1 << WGM20);
			TCCR2_REG &= ~(1 << WGM21);
			break;
		}
		case 2:
		{
			TCCR2_REG &= ~(1 << WGM20);
			TCCR2_REG |=  (1 << WGM21);
			break;
		}
		case 3:
		{
			TCCR2_REG |= (1 << WGM20);
			TCCR2_REG |= (1 << WGM21);
			break;
		}
	}

		TCCR2_REG &= TMR2_CLEAR_OC_BITS;
		TCCR2_REG |= (TMR2_OC_MODE << COM20);

		TCCR2_REG &= TMR2_CLEAR_PRESCALE_BITS;
		TCCR2_REG |= TMR2_PRESCALE;

		TIMSK_REG &= TMR2_CLEAR_INTERRUPT_BITS;

#if(TMR2_OVF_INTERRUPT==ENABLE && TMR2_OC_INTERRUPT==DISABLE)

		TIMSK_REG |= (1 << TOIE2);
		TIMSK_REG &= ~(1 << OCIE2);

#elif(TMR2_OVF_INTERRUPT==DISABLE && TMR2_OC_INTERRUPT==ENABLE)

		TIMSK_REG |= (1 << OCIE2);
		TIMSK_REG &= ~(1 << TOIE2);

#elif(TMR2_OVF_INTERRUPT==DISBALE && TMR2_OVF_INTERRUPT==DISABLE)

		TIMSK_REG &= ~(1 << TOIE2);
		TIMSK_REG &= ~(1 << OCIE2);
#endif

}

void TMR2_TCNTSet(u8 start)
{
	TCNT2_REG = start;
}

void TMR2_OCRSet(u8 ocr2)
{
	OCR2_REG = ocr2;
}

/*************************************WATCH DOG TIMER**********************************/

void WDT_Enable(void)
{
	WDTCR_REG |= (1 << WDE);
}

void WDT_Sleep(u8 sleep_time)
{
	WDTCR_REG &= 0xF8;
	WDTCR_REG |= sleep_time;
}

void WDT_TurnOff(void)
{
	SREG_REG &= ~(1 << 7);			// Disable Global interrupt (because below is a critical section)

	WDTCR_REG = ((1 << WDTOE) | (1 << WDE));
	WDTCR_REG = 0;

	SREG_REG |= (1 << 7);			// Enable again the global interrupt
}


void TMR1_ICU_Enable(void)
{
	TCCR1B_REG |= (1 << ICES1);		// Set Input capture edge select to Rising Edge
	TCCR1B_REG |= (1 << ICNC1);		// Input capture noise canceler
}

void TMR1_ICU_Disable(void)
{
	TIMSK_REG &= ~(1 << TICIE1);
}

u16 TMR1_ICU_GetTCNT1(void)
{
	return ICR1L_REG;
}

void TMR1_ICU_SetTCNT1(u16 new_tcnt1)
{
	TCNT1L_REG = new_tcnt1;
}
