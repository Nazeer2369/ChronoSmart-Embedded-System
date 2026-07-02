#include<lpc21xx.h>
#include"keypad.h"
#include "lcd.h"
#define ROW 16
#define COL 20
//keypad.h

#define OP 16 
int row_no=0,col_no=0;
unsigned int key1=0;
#define READNIBBLE(WORD,DATA) ((WORD>>DATA)&(0XF))
int KEYLUT[4][4]={1,2,3,13,
									4,5,6,12,
				          7,8,9,11,
		             14,0,15,16};
int ROWLUT[4]={14,13,11,7};

void keypad_init()
{
IODIR1 |=0XF<<ROW;
IODIR1 |=0X0<<COL;

}
int col_scan()
{
	if((READNIBBLE(IOPIN1,COL))<15)
	{
		return 0;
	}
	return 1;
	

}

int find_row()
{
	int i;//row;
	for(i=0;i<4;i++)
	{
		IOPIN1 =(IOPIN1&~(0XF<<ROW))|ROWLUT[i]<<ROW;	
		if(col_scan()==0)
		{
			
			break;
		//	return i;		
		}	
	}
	IOPIN1 = (IOPIN1&~(0XF<<ROW))|(0X0<<ROW);
	
	return i;
}
int find_col()
{	
int i;
	for(i=0;i<4;i++)
	{
		if(((IOPIN1>>(COL+i))&1)==0)
		{
			break;
			//return i;
		}
	}
	return i;
}

int key_scan(int *key)
{
	
	while(col_scan()==0)
			{
				row_no=find_row();
				
				col_no=find_col();
				
				while(col_scan()==0);
				*key=KEYLUT[row_no][col_no];
				return 0;
			}
			return 1;
}
