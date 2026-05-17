#include<lpc21xx.h>
#include "types.h"
#include "delay.h"

// LCD control pins
#define RS 16
#define RW 18
#define EN 17

// LCD data pins starting position
#define DATA 8

//char space[17]="                ";

// Function prototype
void write_lcd(int );


/*
 Function : cmd_lcd()
 Purpose  : Send command to LCD
*/
void cmd_lcd(int data)
{
	// RS = 0 --> Instruction register selected
	// RS:0/1
	// 0 --> Instruction register
	// 1 --> Data register

	IOCLR0=1<<RS;

	// Send command to LCD
	write_lcd(data);
}


/*
 Function : lcd_init()
 Purpose  : Initialize LCD in 8-bit mode
*/
void lcd_init()
{
	// Configure LCD control pins as output
	IODIR0 |=1<<RS;
	IODIR0 |=1<<RW;
	IODIR0 |=1<<EN;

	// Configure LCD data pins as output
	IODIR0 |=0XFF<<DATA;
	
	// LCD power ON delay
	delay_ms(15);

	// Initialize LCD sequence
	cmd_lcd(0x30);
	
	delay_ms(5);
	cmd_lcd(0x30);
	
	delay_ms(5);
	cmd_lcd(0x30);

	/*
	 Function set command
	 
	 000D NF--

	 D : 1 --> 8-bit mode
	     0 --> 4-bit mode

	 N : 1 --> 2 lines
	     0 --> 1 line

	 F : 1 --> 5x10 font
	     0 --> 5x8 font

	 0x38 = 8-bit, 2-line, 5x8 font
	*/
	cmd_lcd(0x38);


	/*
	 Display ON/OFF control
	 
	 0000 1DCB
	 0000 1111

	 D : Display ON/OFF
	 C : Cursor ON/OFF
	 B : Blink ON/OFF

	 0x0C = Display ON, Cursor OFF, Blink OFF
	*/
	cmd_lcd(0x0c);


	// Clear LCD display
	cmd_lcd(0x01);
	

	/*
	 Entry mode set
	 
	 0000 01ID S

	 ID : 
	      0 --> Decrement cursor
	      1 --> Increment cursor

	 S  :
	      0 --> No display shift
	      1 --> Display shift

	 0x06 = Increment cursor, no shift
	*/

	//cmd_lcd(0x0f);
	cmd_lcd(0x06);
	
}


/*
 Function : write_lcd()
 Purpose  : Write data/command to LCD
*/
void write_lcd(int data)
{
	// RW = 0 --> Write operation
	// RW:0/1
	// 0 --> WRITE
	// 1 --> READ
	
	IOCLR0=1<<RW;

	// Clear old data bits and place new data
	IOPIN0 = (IOPIN0&~(0XFF<<DATA))| (data<<DATA);
	
	/*
	 Enable pulse:
	 For write operation EN should go HIGH to LOW
	*/

	// EN = 1
	IOSET0 =1<<EN;

	// Small delay for LCD processing
	delay_ms(1);

	// EN = 0
	IOCLR0 =1<<EN;
}


/*
 Function : display_char()
 Purpose  : Display single character on LCD
*/
void display_char(int data)
{
	// RS = 1 --> Data register selected
	IOSET0 = 1<<RS;

	// Send character to LCD
	write_lcd(data);
}


/*
 Function : display_string()
 Purpose  : Display string on LCD
*/
void display_string(char *str)
{
	int count=0,l_cnt=1,i;

	// Loop until NULL character
	while(*str)
	{   
		// If first line completed
		if(count==16 && l_cnt==1)
		{
			l_cnt++;

			// Move cursor to second line
			cmd_lcd(0xc0);

			// Clear second line
			for(i=0;i<17;i++)
			{
				display_char(32);
			}

			///delay_ms(2);

			count=0;

			// Again move cursor to second line start
			cmd_lcd(0xc0);
		}

		// If second line completed
		if(count==16 && l_cnt==2)
		{	
			l_cnt++;

			// Move cursor to first line
			cmd_lcd(0x80);

			// Clear first line
			for(i=0;i<17;i++)
			{
				display_char(32);
			}

			count=0;

			// Move cursor to second line
			cmd_lcd(0xc0);

			//delay_ms(2);

			// Clear display
			cmd_lcd(0x01);

			l_cnt=1;
		}

		// Increment character count
		count++;

		// Display character
		display_char(*str);

		//delay_ms(2);

		// Move to next character
		str++;
	}
	
}


/*
 Function : displayTime()
 Purpose  : Display time in HH:MM:SS format
*/
void displayTime(int hour1,int min1,int sec1)
{
	// Move cursor to first line
	cmd_lcd(0x80);

	// Display hours
	display_char(((hour1)/10)+48);
	display_char(((hour1)%10)+48);

	display_char(':');

	// Display minutes
	display_char((min1/10)+48);
	display_char((min1%10)+48);

	display_char(':');

	// Display seconds
	display_char((sec1/10)+48);
	display_char((sec1%10)+48);
}


/*
 Function : displayDate()
 Purpose  : Display date in DD/MM/YYYY format
*/
void displayDate(int Date,int Month,int Year)
{
	// Move cursor to second line
	cmd_lcd(0xC0);
			
	// Display date
	display_char((Date/10)+48);
	display_char((Date%10)+48);

	display_char('/');

	// Display month
	display_char((Month/10)+48);
	display_char((Month%10)+48);

	display_char('/');

	// Display year
	display_char((Year/1000)+48);
	display_char(((Year/100)%10)+48);
	display_char(((Year/10)%10)+48);
	display_char((Year%10)+48);

	//delay_ms(1);
}


/*
 Function : display_int()
 Purpose  : Display integer value on LCD
*/
void display_int(int n)
{
	u8 a[10];
	s32 i=0;

	// Special case for 0
	if(n==0)
	{
		display_char('0');
	}
	else
	{
		// Extract digits one by one
		while(n)
		{
			a[i++]=(n%10)+48;
			n/=10;
		}

		// Display digits in reverse order
		for(--i; i>=0; i--)
		{
			display_char(a[i]);
		}
	}
}


/*
 Function : display_float()
 Purpose  : Display floating point number on LCD
*/
void display_float(f32 f, u8 nDP)
{
	u32 n,i;

	// Handle negative numbers
	if(f<0)
	{
		display_char('-');
		f=-f;
	}

	// Extract integer part
	n=f;

	// Display integer part
	display_int(n);

	// Display decimal point
	display_char('.');

	// Display digits after decimal point
	for(i=0; i<nDP; i++)
	{
		f=(f-n)*10;

		n=f;

		display_char(n+'0');
	}
}
