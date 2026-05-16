#include<lpc21xx.h>

#include "lcd.h"
#include "rtc.h"
#include "temp.h"
#include "keypad.h"
#include "delay.h"
#include "types.h"
#include "defines.h"
#include "menu.h"

#include "main.h"
//#include "devices.h"
#include "INT_switch.h"

// Device-1 ON time variables
extern int D1_ON_HH,D1_ON_MM,D1_ON_SS;

// Device-2 ON time variables
extern int D2_ON_HH,D2_ON_MM,D2_ON_SS;

// Device-1 OFF time variables
extern int D1_OFF_HH,D1_OFF_MM,D1_OFF_SS;

// Device-2 OFF time variables
extern int D2_OFF_HH,D2_OFF_MM,D2_OFF_SS;

// Temperature threshold value
extern int temp_threshold;

// Interrupt/menu flag
// flag = 1 --> normal operation
// flag = 0 --> interrupt/menu mode
int flag=1;



//#define SW 25

//#define CH_NO 1 


/*
s32 hour1,min1,sec1,Date,Month,Year;
char Day[7][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
char str[5]="Temp";
char space[17]="                ";
char edit[20]="press 1 for edit";
u32 ADC_VAL;
f32 evr,temp;
s32 day1;
*/
//char menu[40]="1.edit time 2.edit date 3.go back 4.done";


/*
 Function : operation()
 Purpose  : 
 1. Read temperature from ADC
 2. Read RTC time/date
 3. Control devices based on ON/OFF timing
 4. Check temperature threshold
 5. Display time, date and temperature on LCD
*/
void operation()
{
	// Read ADC value and equivalent voltage
	READ_ADC(CH_NO ,&ADC_VAL,&evr);

	// Convert voltage into temperature
	temp=evr*100;

	// Read current time from RTC
	GetRTCTimeInfo(&hour1,&min1,&sec1);

	// Read current date from RTC
	getDate(&Date,&Month,&Year);

	//display_char('a');
			
	/*	
		Device-1 ON time  = 08:00:00
		Device-1 OFF time = 09:00:00
		Device-2 ON time  = 10:30:00
		Device-2 OFF time = 11:15:00 
	*/
			
	// Check whether temperature exceeds threshold
	if(temp>=temp_threshold)
	{
		// Clear LCD
		cmd_lcd(0x01);

		// Display warning message
		display_string("temp limit exceed");

	 	// Turn ON both devices
	 	IOSET0=1<<DEVICE1;
	 	IOSET0=1<<DEVICE2;

		// Small delay
		delay_ms(50);

		// Keep devices ON
		IOSET0=1<<DEVICE1;
		IOSET0=1<<DEVICE2;

		// Wait until temperature comes below threshold
		while((temp>temp_threshold)==1);
	}


	/**************** DEVICE-1 CONTROL ****************/

	// Turn ON Device-1 at configured ON time
	if((hour1==D1_ON_HH && min1==D1_ON_MM && sec1==D1_ON_SS )||
	   (hour1==D1_ON_HH && min1>D1_ON_MM ))
	{
		// Device ON (active LOW)
        IOCLR0=1<<DEVICE1;
	}

	// Turn OFF Device-1 at configured OFF time
	if((hour1==D1_OFF_HH && min1==D1_OFF_MM && sec1>D1_OFF_SS)
		||hour1>D1_OFF_HH 
		|| hour1<D1_ON_HH)
	{
		// Device OFF
		IOSET0=1<<DEVICE1;
	}


	/**************** DEVICE-2 CONTROL ****************/

	// Turn ON Device-2 at configured ON time
	if((hour1==D2_ON_HH && min1==D2_ON_MM && sec1==D2_ON_SS)
	||(hour1==D2_ON_HH && min1>D2_ON_MM ))
	{
		// Device ON (active LOW)
        IOCLR0=1<<DEVICE2;
	}

	// Turn OFF Device-2 at configured OFF time
	if((hour1==D2_OFF_HH && min1==D2_OFF_MM && sec1>D2_OFF_SS)
		||(hour1>D2_OFF_HH) 
		|| (hour1<D2_ON_HH))
	{
		// Device OFF
        IOSET0=1<<DEVICE2;
	}
					

	//cmd_lcd(0x01);
			

	/**************** DISPLAY TIME ****************/

	// Display current time on LCD
	displayTime(hour1,min1,sec1);
			

	/**************** DISPLAY TEMPERATURE ****************/

	// Space
	display_char(32);

	// Display "T:"
	display_char('T');
	display_char(':');

	// Display temperature value
	display_int(temp);
			
	// Display degree symbol
	// 0xDF --> degree symbol in LCD
	display_char(0XDF);

	// Display 'C'
	display_char('C');

	// Extra spaces
	display_char(' ');
	display_char(' ');
			

	//display_char((temp/10)+48);
	//display_char((temp%10)+48);
				

	/**************** DISPLAY DATE ****************/

	// Display date
	displayDate(Date,Month,Year);

	display_char(' ');

	// Read current day
	getDay(&day1);

	// Display day name
	display_string(Day[day1]);
				
			
}


/*
 Function : initialization()
 Purpose  : Initialize all peripherals
*/
void initialization()
{
	// Initialize keypad
	keypad_init();

	// Initialize LCD
	lcd_init();

	// Initialize RTC
	init_rtc();

	// Initialize ADC
	init_ADC();

	// Enable external interrupt EINT0
	enable_eint0();
}


/*
 Function : main()
 Purpose  : Program entry point
*/
int main()
{
 	// Initialize peripherals
	initialization();
	
	// Configure DEVICE1 and DEVICE2 pins as output
	IODIR0 |=5<<DEVICE1;
	
	
	// Infinite loop
	while(1)
	{
		// If interrupt/menu mode selected
		if(flag==0)
		{
			intrpt();

			//flag=1;
		}

		// Normal operation mode
		else
		{
			operation();
		}
	}
}