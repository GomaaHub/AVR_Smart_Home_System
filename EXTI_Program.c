#include "Library/types.h"
#include "include/DIO_Interface.h"
#include "include/EXTI_Interface.h"
#include "include/EXTI_Private.h"
#include <avr/interrupt.h>
#include <avr/delay.h>



void GLOBAL_INTERRUPT_ENABLE(void)
{
	SREG_REG |= (1 << 7);
}

void GLOBAL_INTERRUPT_DISABLE(void)
{
	SREG_REG &= ~(1 << 7);
}

void EXTI_ENABLE(u8 EXTI_NUM)
{
	switch(EXTI_NUM)
	{
		case 0: GICR_REG |= (1 << INT0); break;
		case 1: GICR_REG |= (1 << INT1); break;
		case 2: GICR_REG |= (1 << INT2); break;
	}
}

void EXTI_SENSE_CONTROL(u8 EXTI_NUM,u8 MODE)
{
	switch(EXTI_NUM)
	{
		case 0: MCUCR_REG |= (MODE << ISC00); break;
		case 1: MCUCR_REG |= (MODE << ISC10); break;
		case 2:
		{
			if(MODE==0 || MODE==1)
			{
				MODE=0;
			}
			else if(MODE==2)
			{
				MODE=0;
			}
			else if(MODE==3)
			{
				MODE=1;
			}

			MCUCSR_REG |= (MODE << ISC2);

			break;
		}
	}
}

ISR(INT0_vect)
{

}

ISR(INT1_vect)
{

}

ISR(INT2_vect)
{

}
