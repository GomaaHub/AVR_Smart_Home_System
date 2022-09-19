#include "Library/types.h"
#include "include/DIO_Interface.h"
#include "include/LCD_Interface.h"
#include "include/SRV_Interface.h"
#include "include/TMR_Interface.h"
#include "include/EXTI_interface.h"
#include "include/UART_Interface.h"
#include <avr/delay.h>

#define BUZZER_PORT		DIO_PORTC
#define BUZZER_PIN		DIO_PIN1

#define SMART_LOCK_PORT	DIO_PORTA
#define SMART_LOCK_PIN	DIO_PIN0

#define SERVO_PORT		DIO_PORTD
#define SERVO_PIN		DIO_PIN5

#define LED_PORT		DIO_PORTD
#define RED_LED_PIN		DIO_PIN7
#define GREEN_LED_PIN	DIO_PIN4

void ToggleRedLed(void);
void ToggleGreenLed(void);
void ToggleSmartLock(void);
void decimal_to_binary(u8 n);
void CLR_BUFFER(void);
u8 Login (void);
u8 SignUp(void);



extern u8 RX_BUFFER[RX_BUFFER_SIZE];
extern u16 RX_WRITE_POS;

// These are the needed credentials if the Admin wants to register a new user.

char admin_user[5]={'a','d','m','i','n'};
char admin_pass[5]={'a','d','m','i','n'};

// Initialize the system with 3 existing users with their user names and passwords.

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

u8 status;
u8 user_index;
u8 user_id;
u8 return_status;			// If this variable is set to 1, it means that the user application will return to the Home Page.
u8 binary_num[16]={0};
u8 binary_num1[16]={0};

u8 redLEDflag=0;
u8 doorflag=0;
u8 greenLEDflag=0;
u8 doorFLAG=0;

u8 hundreds_red;
u8 tens_red;
u8 ones_red;
u8 duty_red;

u8 hundreds_servo;
u8 tens_servo;
u8 ones_servo;
u8 duty_servo;

u16 hundreds_green;
u16 tens_green;
u16 ones_green;
u16 duty_green;


int main (void)
{
	DIO_SetPortDirection(DIO_PORTB,0xFF);		// Set the LCD PORT as an output port.
	DIO_SetPortDirection(DIO_PORTC,0xFF);		// Set the Buzzer Port as an output
	DIO_SetPortDirection(DIO_PORTA,0xFF);		// Smart Lock Port

	DIO_SetPinDirection(SERVO_PORT,SERVO_PIN,DIO_OUTPUT);	// Setting the Servo Motor Pin as an Output pin.


	GLOBAL_INTERRUPT_ENABLE();			// Enabling the Global Interrupts.

	LCD_Init();							// Initializing the LCD Peripheral.
	LCD_CLEAR();						// Clearing the LCD in case there any existing characters from any previous application.

	LCD_DISPLAY_CONTROL(1,0,0);			// Turn of the Display, but turn off the cursor and the blinking cursor.

	UART_Init();						// Initializing the UART Peripheral.

	TMR2_Init();						// Initializing TIMER2 Peripheral.		(Generates the PWM Wave for the Red LED)

	TMR1_Init();						/* Initializing TIMER1 Peripheral.		(Generates the PWM Wave for the SERVO MOTOR and the Green LED)
										   TIMER1 has 2 output compare match values (OCR1A and OCR1B).*/

	TMR2_OCRSet(0);						// Setting the Output compare value of TIMER2 to zero.	(Red LED in initially turned off)

	TMR1_OCR1BSet(0);					// Setting the Output compare value of TIMER1 to zero.	(Green LED in initially turned off)

	SRV_SetDegree(0,204);				// Setting the Servo Motor Angle to Zero at the start of the application. (The Servo Motor is controlled using OCR1A)
										// The function takes 2 arguments (The desired angle, the maximum angle that the Servo can go to)

	LCD_GoToXY(0,0);					// Going to the Top Left Position of the LCD.




	LCD_Print_String("WELCOME TO OUR  SMART HOME");			// Greeting the User.
	_delay_ms(2000);										// Wait 2 Seconds after the greeting.

	while(1)
	{

			CLR_BUFFER();				// Clear the UART Buffer in case these are any garbage values.
			LCD_CLEAR();

			//Display 2 options for the user: Either to Login using an existing account or Register a new account.

			LCD_GoToXY(0,0);
			LCD_Print_String("1. LOGIN");		// If the byte sent from the mobile application is 'M', the application will jump into the Login while loop.
			LCD_GoToXY(0,1);
			LCD_Print_String("2. REGISTER");	// If the byte sent from the mobile application 'N', the application will jump into the Register while loop.

			while(1)
			{
				if(RX_BUFFER[0]=='M')			// Check if the byte received from the mobile application is equal to 'M'.
				{
					status=Login();					// Go to the Login function.
				}
				else if(RX_BUFFER[0]=='N')		// Check if the byte received from the mobile application is equal to 'M'.
				{
					status=SignUp();					// Go to the Register function.
				}

				if(status==1)			// If it is equal to 1, then go back to the Application Home Page.
				{
					status=0;
					break;
				}
			}

			}

	return 0;
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


void ToggleRedLed(void)
{
	if(redLEDflag==1)
	{
		DIO_SetPinValue(LED_PORT,RED_LED_PIN,DIO_LOW);
		redLEDflag=0;

	}
	else if(redLEDflag==0)
	{
		DIO_SetPinValue(LED_PORT,RED_LED_PIN,DIO_HIGH);
		redLEDflag=1;
	}

	CLR_BUFFER();
}

void ToggleSmartLock(void)
{
	if(doorflag==1)
	{
		DIO_SetPinValue(SMART_LOCK_PORT,SMART_LOCK_PIN,DIO_LOW);
		doorflag=0;

	}
	else if(doorflag==0)
	{
		DIO_SetPinValue(SMART_LOCK_PORT,SMART_LOCK_PIN,DIO_HIGH);
		doorflag=1;
	}

	CLR_BUFFER();
}

void ToggleGreenLed(void)
{
	if(greenLEDflag==1)
	{
		DIO_SetPinValue(LED_PORT,GREEN_LED_PIN,DIO_LOW);
		greenLEDflag=0;

	}
	else if(greenLEDflag==0)
	{
		DIO_SetPinValue(LED_PORT,GREEN_LED_PIN,DIO_HIGH);
		greenLEDflag=1;
	}

	CLR_BUFFER();
}

u8 Login (void)
{
	u8 attempts=0;

	while(1)
	{
		CLR_BUFFER();
		LCD_CLEAR();
		LCD_GoToXY(0,0);
		LCD_Print_String("ENTER USERNAME");

		while(1)
		{

			if(RX_BUFFER[0]!=0)						//Check if a byte has been received.
			{
				LCD_GoToXY(0,1);					// Go the the 2nd line on the LCD.
				LCD_Print_String_UART(RX_BUFFER);	// Print the Data received on the LCD.

				_delay_ms(1000);


				// This Loop checks if the name entered exists in the user name array

				for(u8 id=0;id<10;id++)
				{
					name_check=true;		// Resets the name_check boolean.

					for(u8 name=0;name<16;name++)
					{
						if(user_name[id][name]!=RX_BUFFER[name])	/* If the characters do not correspond to the ones in the array, the next iteration starts.*/
						{
							name_check=false;
							break;
						}
					}

					if(name_check==true)
					{
						user_index=id;		// assigns the user_index to the array index so that it uses the same index to check for the password.
						break;
					}

				}

				if(name_check==false)		// If the loop checks all the names and still the name does not exist, the application will jump to the home page.
				{
					LCD_CLEAR();
					LCD_Print_String("INVALID USERNAME");
					_delay_ms(2000);
					return_status=1;	// When this variable is set to 1, the application must jump to the home page.
					return 1;
				}
				else
				{
					break;
				}


			}

		}



		CLR_BUFFER();
		LCD_CLEAR();
		LCD_GoToXY(0,0);
		LCD_Print_String("ENTER PASSWORD");

		pass_check=true;

		while(1)
		{

			if(RX_BUFFER[0]!=0)			//Check if a byte has been received.
			{

				LCD_GoToXY(0,1);
				LCD_Print_String_UART_PASSWORD(RX_BUFFER);	/* This function prints the password using the '*' sign.
															If the user entered 5 characters, then the output will be "*****"   */

				_delay_ms(1000);

				// This loop checks the password. If the User failed to enter the correct password 3 times, the Buzzer will be triggered.

				for(attempts=0;attempts<3;attempts++)
				{
					pass_check=true;

					for(u8 pass=0;pass<16;pass++)
					{
						if(user_pass[user_index][pass]!=RX_BUFFER[pass])	/* If the characters do not correspond to the ones in the array, the next iteration starts.*/
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
					while(RX_BUFFER[0]==0);				// Wait for the user to enter the password again.
					LCD_GoToXY(0,1);
					LCD_Print_String_UART_PASSWORD(RX_BUFFER);
					_delay_ms(1000);
					}

				}

				if(pass_check==false)
				{
					return_status=1;
					DIO_SetPinValue(BUZZER_PORT,BUZZER_PIN,DIO_HIGH);	// Switch on the Buzzer.
					LCD_CLEAR();
					LCD_GoToXY(0,0);
					LCD_Print_String("3 FAILED        ATTEMPTS");
					_delay_ms(5000);								// Switch On the Buzzer for 5 seconds.
					DIO_SetPinValue(BUZZER_PORT,BUZZER_PIN,DIO_LOW);	// Switch Off the Buzzer.
					return 1;
				}
				else
				{
					break;
				}
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

		// If the User manages to login successfully, The application will enter the actions loop.

		while(1)
		{
				if(RX_BUFFER[0]=='3')		// If '3' is received, then toggle the Red LED.
				{
					ToggleRedLed();
				}

				else if(RX_BUFFER[0]=='5')		// If '5' is received, then toggle the Green LED.
				{
					ToggleGreenLed();
				}
				else if(RX_BUFFER[0]=='6')		// If '6' is received, then either open or close the door lock.
				{
					ToggleSmartLock();
				}
				else if(RX_BUFFER[0]=='Y')		/* If 'Y' has been received, it means that the user wants to set the Red LED Intensity
												using the Seek Bar from the mobile application.*/
				{
					CLR_BUFFER();

					while(1)
					{
						if(RX_BUFFER[0]!=0)		/*Check if data has been received. (As soon the user sets the Seek Bar,
												the mobile application will send the corresponding bytes)
												e.g if the user sets the seek bar at 75%, the mobile will send '7' and then '5'    */
						{

							_delay_ms(100);

							if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0 && RX_BUFFER[2]!=0)	// Check if the number received is of 3 digits.
							{
								// Convert the bytes to actual numbers.

								hundreds_red=RX_BUFFER[0];
								tens_red=RX_BUFFER[1];
								ones_red=RX_BUFFER[2];

								duty_red=((RX_BUFFER[0]-'0')*100 + (RX_BUFFER[1]-'0')*10 + RX_BUFFER[2]-'0');
							}
							else if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0)		// Check if the number received is of 2 digits.
							{
								// Convert the bytes to actual numbers.

								tens_red=RX_BUFFER[0];
								ones_red=RX_BUFFER[1];

								duty_red=((RX_BUFFER[0]-'0')*10 + RX_BUFFER[1]-'0');
							}
							else	// Check if the number received is of 1 digit.
							{
								// Convert the bytes to actual numbers.

								ones_red=RX_BUFFER[0];

								duty_red=(RX_BUFFER[0]-'0');
							}




							TMR2_OCRSet(duty_red);

							CLR_BUFFER();

							break;

						}
					}


				}
				else if(RX_BUFFER[0]=='X')		/* If 'X' has been received, it means that the user wants to set the Servo Motor Angle
												using the Seek Bar from the mobile application.*/
				{
					CLR_BUFFER();
					while(1)
					{

						if(RX_BUFFER[0]!=0)		/*Check if data has been received. (As soon the user sets the Seek Bar,
												the mobile application will send the corresponding bytes)
												e.g if the user sets the seek bar at 75%, the mobile will send '7' and then '5'    */
						{

							_delay_ms(100);		// Wait 100 ms to allow time for the data to be correctly sent. (Can be reduced).

							if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0 && RX_BUFFER[2]!=0)	// Check if the number received is of 3 digits.
							{
								// Convert the bytes to actual numbers.

								hundreds_servo=RX_BUFFER[0];
								tens_servo=RX_BUFFER[1];
								ones_servo=RX_BUFFER[2];

								duty_servo=((RX_BUFFER[0]-'0')*100 + (RX_BUFFER[1]-'0')*10 + RX_BUFFER[2]-'0');
							}
							else if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0) // Check if the number received is of 2 digits.
							{
								// Convert the bytes to actual numbers.

								tens_servo=RX_BUFFER[0];
								ones_servo=RX_BUFFER[1];

								duty_servo=((RX_BUFFER[0]-'0')*10 + RX_BUFFER[1]-'0');
							}
							else // Check if the number received is of 1 digit. (The mobile application will never send more than 100%).
							{
								// Convert the bytes to actual numbers.

								ones_servo=RX_BUFFER[0];

								duty_servo=(RX_BUFFER[0]-'0');
							}

							SRV_SetDegree(duty_servo,204);	// Finally set the Servo Motor Angle accordingly.

							CLR_BUFFER();

							break;

						}

					}
				}
			    else if(RX_BUFFER[0]=='Z')		/* If 'Z' has been received, it means that the user wants to set the Green LED Intensity
												using the Seek Bar from the mobile application.*/
				{
			    	CLR_BUFFER();
					while(1)
					{

						if(RX_BUFFER[0]!=0)		/*Check if data has been received. (As soon the user sets the Seek Bar,
												the mobile application will send the corresponding bytes)
												e.g if the user sets the seek bar at 75%, the mobile will send '7' and then '5'    */
						{

							_delay_ms(100);

							if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0 && RX_BUFFER[2]!=0)	// Check if the number received is of 3 digits.
							{
								// Convert the bytes to actual numbers.

								hundreds_green=RX_BUFFER[0];
								tens_green=RX_BUFFER[1];
								ones_green=RX_BUFFER[2];

								duty_green=((RX_BUFFER[0]-'0')*100 + (RX_BUFFER[1]-'0')*10 + RX_BUFFER[2]-'0');
							}
							else if(RX_BUFFER[0]!=0 && RX_BUFFER[1]!=0)	// Check if the number received is of 2 digits.
							{
								// Convert the bytes to actual numbers.

								tens_green=RX_BUFFER[0];
								ones_green=RX_BUFFER[1];

								duty_green=((RX_BUFFER[0]-'0')*10 + RX_BUFFER[1]-'0');
							}
							else		// Check if the number received is of 1 digit.
							{
								// Convert the bytes to actual numbers.

								ones_green=RX_BUFFER[0];

								duty_green=(RX_BUFFER[0]-'0');
							}

							duty_green*=20;		// Here we are multiplying the duy_green value by 20 because we are using an ICR value of 20,000.


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
					return 1;
				}
		}

		if(return_status==1)
		{
			return 1;
		}

		}


		if(return_status==1)
		{
			return 1;
		}

	}
}

u8 SignUp(void)
{

	// This function behaves similar to the Log in Function except that it doesn't check the password.

	static int j;
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
		return 1;
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
				return 1;
			}
			else
			{
				break;
			}

		}

	}

	CLR_BUFFER();

	LCD_CLEAR();
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
			return 1;
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
	{
		return 1;
	}

	CLR_BUFFER();
	LCD_CLEAR();
	LCD_GoToXY(0,0);
	LCD_Print_String("SUCCESSFULLY    REGISTERED");
	_delay_ms(2000);


	user_id++;
	return 0;

}
