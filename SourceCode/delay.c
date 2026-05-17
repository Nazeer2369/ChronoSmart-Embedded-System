#include "types.h"

// Global delay counter variable
u32 i;


/*
 Function : delay_us()
 Purpose  : Generate delay in microseconds
*/
void delay_us(u32 s)
{
	// Loop count adjusted for microsecond delay
	for(i=s*12;i>0;i--);
}


/*
 Function : delay_ms()
 Purpose  : Generate delay in milliseconds
*/
void delay_ms(u32 s)
{
	// Loop count adjusted for millisecond delay
	for(i=s*12000;i>0;i--);
}


/*
 Function : delay_s()
 Purpose  : Generate delay in seconds
*/
void delay_s(u32 s)
{
	// Loop count adjusted for second delay
	for(i=s*12000000;i>0;i--);
}
