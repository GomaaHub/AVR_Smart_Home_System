/*
 * TMR_Interface.h
 *
 *  Created on: May 2, 2022
 *      Author: Ahmad
 */

#ifndef TMR_INTERFACE_H_
#define TMR_INTERFACE_H_



void TMR0_Init();
void TMR0_TCNTSet(u8 start);
void TMR0_OCRSet(u8 ocr0);
void TMR2_Init();
void TMR2_TCNTSet(u8 start);
void TMR2_OCRSet(u8 ocr2);
void TMR1_Init();
void TMR1_TCNT1Set(u16 tcnt1);
void TMR1_OCR1ASet(u16 ocr1a);
void TMR1_OCR1BSet(u16 ocr1b);

void WDT_Enable(void);
void WDT_Sleep(u8 sleep_time);
void WDT_TurnOff(void);


void TMR1_ICU_Enable(void);
void TMR1_ICU_Disable(void);
u16 TMR1_ICU_GetTCNT1(void);
void TMR1_ICU_SetTCNT1(u16 new_tcnt1);



#define ENABLE				1
#define DISABLE				0

#define CHANNEL_A			0
#define CHANNEL_B			1

/***********************************************TIMER 1 MODE MACROS****************************************************/
#define TMR1_NORMAL			0
#define TMR1_PWM_PC_8BIT	1
#define TMR1_PWM_PC_9BIT	2
#define TMR1_PWM_PC_10BIT	3
#define TMR1_CTC_OCR_TOP	4
#define TMR1_FAST_PWM_8BIT	5
#define TMR1_FAST_PWM_9BIT	6
#define TMR1_FAST_PWM_10BIT	7
#define TMR1_PC_FC_ICR		8
#define TMR1_PC_FC_OCR		9
#define TMR1_PC_ICR			10
#define TMR1_PC_OCR			11
#define TMR1_CTC_ICR_TOP	12
#define TMR1_FAST_PWM_ICR	14
#define TMR1_FAST_PWM_OCR	15


/**************************************************TIMER 0 and 2 MODE MACROS***********************************************/


#define NORMAL				0
#define PWM_PHASE_CORRECT	1
#define CTC					2
#define FAST_PWM			3

/**************************************************************************************************************************/

#define	OC_DISCONNECT		0
#define	TOGGLE_OC			1
#define	CLEAR_OC			2
#define	SET_OC				3

#define NO_CLOCK			0
#define CLOCK_NO_PRESCALE	1

#define TMR0_CLOCK_8			2
#define TMR0_CLOCK_64			3
#define TMR0_CLOCK_256			4
#define TMR0_CLOCK_1024			5
#define TMR0_EXT_CLOCK_FALL		6
#define TMR0_EXT_CLOCK_RISE		7

#define TMR1_CLOCK_8			2
#define TMR1_CLOCK_64			3
#define TMR1_CLOCK_256			4
#define TMR1_CLOCK_1024			5
#define TMR1_EXT_CLOCK_FALL		6
#define TMR1_EXT_CLOCK_RISE		7


#define TMR2_CLOCK_8			2
#define TMR2_CLOCK_32			3
#define TMR2_CLOCK_64			4
#define TMR2_CLOCK_128			5
#define TMR2_CLOCK_256			6
#define TMR2_CLOCK_1024			7



/***************************************TIMER0 REGISTER MASKING**************************************/

#define TMR0_MODE_MASK				0x03
#define TMR0_OC_MODE_MASK			0x03
#define TMR0_PRESCALE_MASK			0x07

#define TMR0_CLEAR_OC_BITS			0xCF
#define TMR0_CLEAR_PRESCALE_BITS	0xF8
#define TMR0_CLEAR_INTERRUPT_BITS	0xFC

/***************************************TIMER1 REGISTER MASKING**************************************/

#define TMR1_MODE_MASK				0x0F
#define TMR1_OC_MODE_MASK			0x03
#define TMR1_PRESCALE_MASK			0x07

#define TMR1_CLEAR_OCA_BITS			0b00111111
#define TMR1_CLEAR_OCB_BITS			0b11001111
#define TMR1_CLEAR_PRESCALE_BITS	0b11111000
#define TMR1_CLEAR_INTERRUPT_BITS	0b11000011

/***************************************TIMER2 REGISTER MASKING**************************************/

#define TMR2_MODE_MASK				0x03
#define TMR2_OC_MODE_MASK			0x03
#define TMR2_PRESCALE_MASK			0x07

#define TMR2_CLEAR_OC_BITS			0xCF
#define TMR2_CLEAR_PRESCALE_BITS	0xF8
#define TMR2_CLEAR_INTERRUPT_BITS	0x3F

/***************************************************************************************************/
#endif /* TMR_INTERFACE_H_ */
