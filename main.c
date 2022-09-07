#include "Library/types.h"
#include "include/DIO_Interface.h"
#include "include/LCD_Interface.h"
#include "include/SRV_Interface.h"
#include "include/TMR_Interface.h"
#include "include/EXTI_interface.h"
#include "include/UART_Interface.h"
#include <avr/delay.h>


#define SMART_LOCK_PORT	DIO_PORTA
#define SMART_LOCK_PIN	DIO_PIN0

#define SERVO_PORT		DIO_PORTD
#define SERVO_PIN		DIO_PIN5

#define LED_PORT		DIO_PORTD
#define RED_LED_PIN		DIO_PIN7

#define GREEN_LED_PIN	DIO_PIN4

void ToggleRedLed(void);
void decimal_to_binary(u8 n);
void CLR_BUFFER(void);
void actions(void);


extern u8 RX_BUFFER[RX_BUFFER_SIZE];
extern u16 RX_WRITE_POS;

char admin_user[5]={'a','d','m','i','n'};
char admin_pass[5]={'a','d','m','i','n'};

char user_name[10][16]={{'a','h','m','e','d'},{'o','m','a','r'},{'y','o','u','s','s','e','f'}};
char user_pass[10][16]={{'1','2','3','4'},{'5','6','7','8'},{'9','9','8','8'}};

enum user
{
	false,
	true
};

enum user name_check=false;
enum user pass_check=false;
enum user admin_check=false;

u8 user_index;
u8 user_id;
u8 return_status;
u8 binary_num[16]={0};
u8 binary_num1[16]={0};

u8 redLEDflag=0;
u8 yellowLEDflag=0;
u8 greenLEDflag=0;
u8 doorFLAG=0;

u8 hundreds_red;
u8 tens_red;
u8 ones_red;
u8 duty_red;

u8 hundreds_yellow;
u8 tens_yellow;
u8 ones_yellow;
u8 duty_yellow;

u16 hundreds_green;
u16 tens_green;
u16 ones_green;
u16 duty_green;


void main (void)
{
	DIO_SetPortDirection(DIO_PORTB,0xFF);		// LCD PORT
	DIO_SetPortDirection(DIO_PORTC,0xFF);
	DIO_SetPortDirection(DIO_PORTA,0xFF);

	DIO_SetPinDirection(SERVO_PORT,SERVO_PIN,1);





	GLOBAL_INTERRUPT_ENABLE();

	LCD_Init();
	LCD_CLEAR();

	LCD_DISPLAY_CONTROL(1,0,0);

	UART_Init();

	TMR2_Init();
	TMR1_Init();

	TMR2_OCRSet(0);
	//TMR1_OCR1ASet(0);
	TMR1_OCR1BSet(0);

	SRV_SetDegree(0,204);

	LCD_GoToXY(0,0);

	u8 j=0;


	//LCD_Print_String_UART(RX_BUFFER);
	LCD_GoToXY(0,0);
	LCD_Print_String("WELCOME TO OUR  SMART HOME");
	_delay_ms(2000);

	while(1)
	{

			CLR_BUFFER();
			LCD_CLEAR();
			LCD_GoToXY(0,0);
			LCD_Print_String("1. LOGIN");
			LCD_GoToXY(0,1);
			LCD_Print_String("2. REGISTER");

			while(1)
			{
				if(RX_BUFFER[0]=='M')
				{

					while(1)
					{
						CLR_BUFFER();
						LCD_CLEAR();
						LCD_GoToXY(0,0);
						LCD_Print_String("ENTER USERNAME");


						while(1)
						{

							if(RX_BUFFER[0]!=0)
							{
								LCD_GoToXY(0,1);
								LCD_Print_String_UART(RX_BUFFER);

								_delay_ms(1000);



								for(u8 id=0;id<10;id++)
								{
									name_check=true;

									for(u8 name=0;name<16;name++)
									{
										if(user_name[id][name]!=RX_BUFFER[name])
										{
											name_check=false;
											break;
										}

									}

									if(name_check==true)
									{
										user_index=id;
										break;
									}

								}

								if(name_check==false)
								{
									LCD_CLEAR();
									LCD_Print_String("INVALID USERNAME");
									_delay_ms(2000);
									return_status=1;
									break;
								}
								else
								{
									break;
								}


							}

						}

						if(return_status==1)
						{
							break;
						}

						CLR_BUFFER();
						LCD_CLEAR();
						LCD_GoToXY(0,0);
						LCD_Print_String("ENTER PASSWORD");

						pass_check=true;

						while(1)
						{
							u8 attempts=0;
							if(RX_BUFFER[0]!=0)
							{

								LCD_GoToXY(0,1);
								LCD_Print_String_UART_PASSWORD(RX_BUFFER);


								_delay_ms(1000);



								for(attempts=0;attempts<3;attempts++)
								{
									pass_check=true;

									for(u8 pass=0;pass<16;pass++)
									{
										if(user_pass[user_index][pass]!=RX_BUFFER[pass])
										{
											pass_check=false;
											break;
										}

									}

									if(pass_check==true)
									{
										break;
									}
									else if(pass_check==false && attempts<2)
									{
										LCD_CLEAR();
										LCD_GoToXY(0,0);
										LCD_Print_String("INVALID PASSWORD");
										LCD_Print_Char((2-attempts) +'0');
										if(attempts==1)
										{
											LCD_Print_String(" ATTEMPT LEFT");
										}
										else
										{
											LCD_Print_String(" ATTEMPTS LEFT");
										}


										_delay_ms(2000);
									}


									if (attempts<2)
									{
									CLR_BUFFER();
									LCD_CLEAR();
									LCD_GoToXY(0,0);
									LCD_Print_String("ENTER PASSWORD");
									while(RX_BUFFER[0]==0);
									LCD_GoToXY(0,1);
									LCD_Print_String_UART_PASSWORD(RX_BUFFER);
									_delay_ms(1000);
									}

								}

								if(pass_check==false)
								{
									return_status=1;
									DIO_SetPinValue(DIO_PORTC,DIO_PIN1,DIO_HIGH);
									LCD_CLEAR();
									LCD_GoToXY(0,0);
									LCD_Print_String("3 FAILED        ATTEMPTS");
									_delay_ms(5000);
									DIO_SetPinValue(DIO_PORTC,DIO_PIN1,DIO_LOW);
									break;
								}
								else
								{
									break;
								}
							}

							if(return_status==1)
							{

								break;
							}
						}


						if(pass_check==true)
						{


						CLR_BUFFER();
						LCD_CLEAR();
						LCD_GoToXY(0,0);
						LCD_Print_String("LOGIN SUCCESS");
						_delay_ms(3000);


						LCD_CLEAR();
						LCD_GoToXY(0,0);
						LCD_Print_String("WHAT WOULD YOU  LIKE TO DO?");

						while(1)
						{



								if(RX_BUFFER[0]=='3')
								{
									if(redLEDflag==1)
									{
										DIO_SetPinDirection(LED_PORT,RED_LED_PIN,0);
										redLEDflag=0;

									}
									else if(redLEDflag==0)
									{
										DIO_SetPinDirection(LED_PORT,RED_LED_PIN,1);
										redLEDflag=1;

									}

									CLR_BUFFER();


								}

								else if(RX_BUFFER[0]=='5')
								{
									if(greenLEDflag==1)
									{
										DIO_SetPinDirection(LED_PORT,GREEN_LED_PIN,0);
										greenLEDflag=0;

									}
									else if(greenLEDflag==0)
									{
										DIO_SetPinDirection(LED_PORT,GREEN_LED_PIN,1);
										greenLEDflag=1;

									}

									CLR_BUFFER();

								}
								else if(RX_BUFFER[0]=='6')
								{
									if(doorFLAG==1)
									{
										DIO_SetPinValue(SMART_LOCK_PORT,SMART_LOCK_PIN,0);
										doorFLAG=0;

									}
									else if(doorFLAG==0)
									{
										DIO_SetPinValue(SMART_LOCK_PORT,SMART_LOCK_PIN,1);
										doorFLAG=1;

									}

									CLR_BUFFER();

								}
								else if(RX_BUFFER[0]=='Y')
								{
									CLR_BUFFER();

									while(1)
									{
										if(RX_BUFFER[0]!=0)
										{

											_delay_ms(100);

											if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0 && RX_BUFFER[2]!=0)
											{
												hundreds_red=RX_BUFFER[0];
												tens_red=RX_BUFFER[1];
												ones_red=RX_BUFFER[2];

												duty_red=((RX_BUFFER[0]-'0')*100 + (RX_BUFFER[1]-'0')*10 + RX_BUFFER[2]-'0');
											}
											else if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0)
											{
												tens_red=RX_BUFFER[0];
												ones_red=RX_BUFFER[1];

												duty_red=((RX_BUFFER[0]-'0')*10 + RX_BUFFER[1]-'0');
											}
											else
											{
												ones_red=RX_BUFFER[0];

												duty_red=(RX_BUFFER[0]-'0');
											}




											TMR2_OCRSet(duty_red);

											CLR_BUFFER();

											break;

										}
									}


								}
								else if(RX_BUFFER[0]=='X')
								{
									CLR_BUFFER();
									while(1)
									{

										if(RX_BUFFER[0]!=0)
										{

											_delay_ms(100);

											if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0 && RX_BUFFER[2]!=0)
											{
												hundreds_yellow=RX_BUFFER[0];
												tens_yellow=RX_BUFFER[1];
												ones_yellow=RX_BUFFER[2];

												duty_yellow=((RX_BUFFER[0]-'0')*100 + (RX_BUFFER[1]-'0')*10 + RX_BUFFER[2]-'0');
											}
											else if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0)
											{
												tens_yellow=RX_BUFFER[0];
												ones_yellow=RX_BUFFER[1];

												duty_yellow=((RX_BUFFER[0]-'0')*10 + RX_BUFFER[1]-'0');
											}
											else
											{
												ones_yellow=RX_BUFFER[0];

												duty_yellow=(RX_BUFFER[0]-'0');
											}



											SRV_SetDegree(duty_yellow,204);


											CLR_BUFFER();

											break;

										}

									}
								}
							    else if(RX_BUFFER[0]=='Z')
								{
							    	CLR_BUFFER();
									while(1)
									{

										if(RX_BUFFER[0]!=0)
										{

											_delay_ms(100);

											if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0 && RX_BUFFER[2]!=0)
											{
												hundreds_green=RX_BUFFER[0];
												tens_green=RX_BUFFER[1];
												ones_green=RX_BUFFER[2];

												duty_green=((RX_BUFFER[0]-'0')*100 + (RX_BUFFER[1]-'0')*10 + RX_BUFFER[2]-'0');
											}
											else if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0)
											{
												tens_green=RX_BUFFER[0];
												ones_green=RX_BUFFER[1];

												duty_green=((RX_BUFFER[0]-'0')*10 + RX_BUFFER[1]-'0');
											}
											else
											{
												ones_green=RX_BUFFER[0];

												duty_green=(RX_BUFFER[0]-'0');
											}

											duty_green*=20;


											TMR1_OCR1BSet(duty_green);

											CLR_BUFFER();

											break;

										}

									}
								}
							    else if(RX_BUFFER[0]=='L')
								{
									CLR_BUFFER();
									return_status=1;
									break;
								}
						}

						if(return_status==1)
						{
							break;
						}

						}


						if(return_status==1)
						{

							break;
						}
					}
				}
				else if(RX_BUFFER[0]=='N')
				{
					CLR_BUFFER();

					LCD_CLEAR();
					LCD_GoToXY(0,0);
					LCD_Print_String("ENTER ADMIN     CREDENTIALS");

					_delay_ms(2000);
					LCD_CLEAR();
					LCD_GoToXY(0,0);
					LCD_Print_String("ENTER USERNAME");



					while(1)
					{
						if(RX_BUFFER[0]!=0)
						{
							LCD_GoToXY(0,1);
							LCD_Print_String_UART(RX_BUFFER);

							_delay_ms(1000);

								admin_check=true;

								for(u8 name=0;name<5;name++)
								{
									if(admin_user[name]!=RX_BUFFER[name])
									{
										admin_check=false;
										break;
									}

								}


							if(admin_check==false)
							{
								LCD_CLEAR();
								LCD_Print_String("INVALID USERNAME");
								_delay_ms(2000);
								return_status=1;
								break;
							}
							else
							{
								break;
							}


						}

					}

					CLR_BUFFER();

					if(return_status==1)
					{
						break;
					}
					else
					{
						LCD_CLEAR();
						LCD_GoToXY(0,0);
						LCD_Print_String("ENTER PASSWORD");
					}


					while(1)
					{


						if(RX_BUFFER[0]!=0)
						{
							LCD_GoToXY(0,1);
							LCD_Print_String_UART_PASSWORD(RX_BUFFER);

							_delay_ms(1000);

								admin_check=true;

								for(u8 name=0;name<5;name++)
								{
									if(admin_pass[name]!=RX_BUFFER[name])
									{
										admin_check=false;
										break;
									}

								}


							if(admin_check==false)
							{
								LCD_CLEAR();
								LCD_Print_String("INVALID PASSWORD");
								_delay_ms(2000);
								return_status=1;
								break;
							}
							else
							{
								break;
							}


						}




					}

					CLR_BUFFER();
					if(return_status==1)
						{
							break;
						}

					else
					{LCD_CLEAR();
					LCD_GoToXY(0,0);
					LCD_Print_String("ADMIN LOGIN     SUCCESSFUL");
					_delay_ms(2000);


					for(u8 i=0;i<10;i++)
										{
											if(user_name[i][0]==0)
											{
												user_id=i;
												break;
											}
										}

											LCD_CLEAR();
											LCD_GoToXY(0,0);
											LCD_Print_String("ENTER USERNAME");
					}


					while(1)
					{



						if(RX_BUFFER[0]!=0)
						{
							LCD_GoToXY(0,1);
							LCD_Print_String_UART(RX_BUFFER);

							_delay_ms(1000);
							j=0;

							for(u8 name=0;name<16;name++)
							{
								user_name[user_id][name]=RX_BUFFER[name];
							}

							break;
						}



					}

					CLR_BUFFER();

					if(return_status==1)
						{
						break;
						}
					else
					{
						CLR_BUFFER();
											LCD_CLEAR();
											LCD_GoToXY(0,0);
											LCD_Print_String("ENTER PASSWORD");
					}




					while(1)
					{



						if(RX_BUFFER[0]!=0)
						{

							LCD_GoToXY(0,1);
							LCD_Print_String_UART_PASSWORD(RX_BUFFER);



							_delay_ms(1000);

							j=0;

							for(u8 pass=0;pass<16;pass++)
							{
								user_pass[user_id][pass]=RX_BUFFER[pass];

							}
							break;

						}

					}

					if(return_status==1)
						break;

					CLR_BUFFER();
					LCD_CLEAR();
					LCD_GoToXY(0,0);
					LCD_Print_String("SUCCESSFULLY    REGISTERED");
					_delay_ms(2000);


					user_id++;
					break;
				}

				if(return_status==1)
				{
					return_status=0;
					break;
				}
			}

			}

}


void decimal_to_binary(u8 n)
{
	u8 i=0;
	u8 j=15;

	for(i=0;i<16;i++)
	{
		binary_num[i]=(n%2) + '0';
		n=(n/2);
	}

	for(i=0;i<16;i++)
	{
		binary_num1[i]=binary_num[j--];
	}
}


void CLR_BUFFER(void)
{

	for(u8 i=0;i<31;i++)
	{
		RX_BUFFER[i]=0;
	}

	RX_WRITE_POS=0;

}

void actions(void)
{
	u8 j;
	CLR_BUFFER();
							j=0;

							if(RX_BUFFER[j]=='1')
							{
								DIO_SetPinValue(DIO_PORTC,DIO_PIN0,DIO_HIGH);
								j++;
							}
							else if(RX_BUFFER[j]=='0')
							{
								DIO_SetPinValue(DIO_PORTC,DIO_PIN0,DIO_LOW);
								j++;
							}
							else if(RX_BUFFER[j]=='2')
							{
								DIO_SetPinValue(DIO_PORTA,DIO_PIN0,DIO_HIGH);
								j++;
							}
							else if(RX_BUFFER[j]=='3')
							{
								DIO_SetPinValue(DIO_PORTA,DIO_PIN0,DIO_LOW);
								j++;
							}

}

void ToggleRedLed(void)
{
	if(redLEDflag==1)
	{
		DIO_SetPinValue(LED_PORT,RED_LED_PIN,0);
		redLEDflag=0;

	}
	else if(redLEDflag==0)
	{
		DIO_SetPinValue(LED_PORT,RED_LED_PIN,1);
		redLEDflag=1;

	}
}
