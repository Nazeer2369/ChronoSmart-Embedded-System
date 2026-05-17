#include<lpc21xx.h>
#include "types.h"
#include "rtc.h"

// Peripheral clock = 15 MHz
#define PCLK 15000000

// CPU clock = 60 MHz
#define CCLK 60000000

// RTC prescaler values
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK -((PREINT_VAL+1)*32768))

// Global RTC variables
u32 hour,min,sec,date,month,year;


/*
 Function : init_rtc()
 Purpose  : Initialize RTC module
*/
void init_rtc()
{
	// Enable power for RTC peripheral
	PCONP |= (1 << 9); 

	// Reset RTC clock
	CCR =(1<<1);
	
	// Load integer prescaler value
	PREINT = PREINT_VAL;

	// Load fractional prescaler value
	PREFRAC=PREFRAC_VAL;
	
	// Enable RTC clock
	CCR=1<<0;
	
	// Set initial time
	set_time(10,10,50);
	
	// Set initial date
	setDate(25,04,2026);
	
	// Set initial day
	// 0-SUN 1-MON 2-TUE ...
	set_Day(6);
}


/*
 Function : setDate()
 Purpose  : Set RTC date
*/
void setDate(int date,int month,int year)
{
	// Set day of month
	DOM=date;

	// Set month
	MONTH=month;

	// Set year
	YEAR=year;
}


/*
 Function : set_time()
 Purpose  : Set RTC time
*/
void set_time(int hour,int min,int sec)
{
	// Set hours
	HOUR=hour;

	// Set minutes
	MIN=min;

	// Set seconds
	SEC=sec;
}


/*
 Function : set_Day()
 Purpose  : Set current day
*/
void set_Day(int index)
{
	// Set day of week
	DOW=index;
}


/*
 Function : GetRTCTimeInfo()
 Purpose  : Read current RTC time
*/
void GetRTCTimeInfo(int *H,int *M,int *S)
{
	// Read hours
	*H=HOUR;

	// Read minutes
	*M=MIN;

	// Read seconds
	*S=SEC;
}


/*
 Function : getDate()
 Purpose  : Read current RTC date
*/
void getDate(int *d,int*m,int*y)
{
	// Read date
	*d=DOM;

	// Read month
	*m=MONTH;

	// Read year
	*y=YEAR;
}


/*
 Function : getDay()
 Purpose  : Read current day
*/
void getDay(int*day)
{
	// Read day of week
	*day=DOW;
}
