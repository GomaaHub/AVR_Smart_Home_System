/*
 * TMR_Private.h
 *
 *  Created on: May 2, 2022
 *      Author: Ahmad
 */

#ifndef TMR_PRIVATE_H_
#define TMR_PRIVATE_H_

/*********************************************STATUS REGISTER**************************************/

#define SREG_REG	*((volatile u8*)0x5F)

/***************************************WATCH DOG TIMER REGISTER***********************************/

#define WDTCR_REG	*((volatile u8*)0x41)

#define WDTOE	4
#define WDE		3
#define WDP2	2
#define WDP1	1
#define WDP0	0


/***************************************TIMER0 REGISTERS AND MACROS****************************************/

#define TCCR0_REG	*((volatile u8*)0x53)
#define TCNT0_REG 	*((volatile u8*)0x52)
#define OCR0_REG	*((volatile u8*)0x5C)


#define CS00		0
#define CS01		1
#define CS02		2
#define WGM01		3
#define	COM00		4
#define COM01		5
#define WGM00		6
#define FOC0		7


/**********************************************TIMER1 REGISTERS*******************************************/

#define TCCR1A_REG	*((volatile u8*)0x4F)
#define TCCR1B_REG	*((volatile u8*)0x4E)

#define TCNT1L_REG *((volatile u16*)0x4C)
#define TCNT1H_REG *((volatile u8*)0x4D)

#define OCR1AL_REG	*((volatile u16*)0x4A)
#define OCR1AH_REG	*((volatile u8*)0x4B)

#define OCR1BL_REG	*((volatile u16*)0x48)
#define OCR1BH_REG	*((volatile u8*)0x49)

#define ICR1L_REG	*((volatile u16*)0x46)
#define ICR1H_REG	*((volatile u8*)0x47)

#define COM1A1	7
#define COM1A0	6
#define COM1B1	5
#define COM1B0	4
#define FOC1A	3
#define FOC1B	2
#define WGM11	1
#define WGM10	0

#define ICNC1	7
#define ICES1	6
#define WGM13	4
#define WGM12	3
#define CS12	2
#define CS11	1
#define CS10	0

/**********************************************TIMER2 REGISTERS*******************************************/

#define TCCR2_REG	*((volatile u8*)0x45)
#define TCNT2_REG 	*((volatile u8*)0x44)
#define OCR2_REG	*((volatile u8*)0x43)


#define CS20		0
#define CS21		1
#define CS22		2
#define WGM21		3
#define	COM20		4
#define COM21		5
#define WGM20		6
#define FOC2		7


/*****************************************TIMSK REGISTER************************************************/

#define TIMSK_REG	*((volatile u8*)0x59)

#define OCIE2	7
#define TOIE2	6
#define TICIE1	5
#define OCIE1A	4
#define OCIE1B	3
#define TOIE1	2
#define OCIE0	1
#define TOIE0	0








#endif /* TMR_PRIVATE_H_ */
