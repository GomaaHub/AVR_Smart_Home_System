/*
 * TMR_Config.h
 *
 *  Created on: May 16, 2022
 *      Author: Ahmad
 */

#ifndef TMR_CONFIG_H_
#define TMR_CONFIG_H_

/******************************************TIMER 0 CONFIG****************************************/

#define TMR0_MODE			FAST_PWM
#define TMR0_OC_MODE		CLEAR_OC
#define TMR0_PRESCALE		TMR0_CLOCK_8
#define TMR0_OVF_INTERRUPT	DISABLE
#define TMR0_OC_INTERRUPT	DISABLE

/******************************************TIMER 1 CONFIG****************************************/

#define TMR1_MODE			TMR1_FAST_PWM_ICR
#define TMR1_OCA_MODE		CLEAR_OC
#define TMR1_OCB_MODE		CLEAR_OC
#define TMR1_PRESCALE		TMR1_CLOCK_8
#define TMR1_CHANNEL_A		ENABLE
#define TMR1_CHANNEL_B		ENABLE
#define TMR1_ICR_VALUE		20000
#define TMR1_OVF_INTERRUPT	DISABLE
#define TMR1_OCA_INTERRUPT	DISABLE
#define TMR1_OCB_INTERRUPT	DISABLE
#define TMR1_IC_INTERRUPT	DISABLE

/******************************************TIMER 2 CONFIG****************************************/

#define TMR2_MODE			FAST_PWM
#define TMR2_OC_MODE		CLEAR_OC
#define TMR2_PRESCALE		TMR2_CLOCK_8
#define TMR2_OVF_INTERRUPT	DISABLE
#define TMR2_OC_INTERRUPT	DISABLE

/************************************************************************************************/

#endif /* TMR_CONFIG_H_ */
