#include "Library/types.h"
#include "TMR_Interface.h"
#include "SRV_Interface.h"
#include "avr/delay.h"

void SRV_SetDegree(u8 degree, u8 max_degree)
{
	f32 set = (f32)degree/(f32)max_degree;

	set = (((f32)SERVO_MAX - (f32)SERVO_MIN) * set) + (f32)SERVO_MIN;

	u16 y=(u16)set;

	TMR1_OCR1ASet(y);

}

void SRV_Test(void)
{
	for(u16 i=SERVO_MIN ; i < SERVO_MAX; i+=10)
	{
		TMR1_OCR1ASet(i);
		_delay_ms(1);
	}

	for(u16 i=SERVO_MAX ; i > SERVO_MIN; i-=10)
	{
		TMR1_OCR1ASet(i);
		_delay_ms(1);
	}

	_delay_ms(1000);
}
