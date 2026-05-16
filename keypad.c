/*#include<lpc21xx.h>
#include"keypad.h"
#include "lcd.h"
#define READ_NIBBLE(WORD,POS) ((WORD>>POS)&0XF)
#define ROW 16
#define COL 20

int row_no=0,col_no=0;
//#define READ_BIT(WORD,POS) ((WORD>>POS)&1)
//port 1
//16 17 18 19



//20 21 22 23

unsigned int KEYLUT[4][4]={1 ,2 ,3 ,11,
													 4 ,5 ,6 ,12,
							             7 ,8 ,9 ,13,
							            14 ,0,15 ,16};
unsigned int arr[4]={14,13,11,7};
int col_scan()
{
	if((READ_NIBBLE(IOPIN1,COL))<15)
		return 1;
	return 0;
}
int find_row()
{
	int i;
	
	for(i=0;i<=3;i++)
	{
		IOPIN1=(IOPIN1&~(0XF<<ROW))|(arr[i]<<ROW);
		if(col_scan())
		{
			break;
		}
		
		
	}
		IOPIN1=(IOPIN1&~(0XF<<ROW))|(0X0<<ROW);

	return i;
}
int find_col()
{
	int i;
	for(i=0;i<=3;i++)
	{
		if(((IOPIN1>>(COL+i))&1)==0)
		{
			break;
		}
		
	}
	return i;
}
	
void init_keypad()
{
	IODIR1 |=0XF<<ROW;
IODIR1 |=0X0<<COL;
}
int key_scan()
{
	int row_no,col_no;
	while(col_scan())
			{
				row_no=find_row();
				
				col_no=find_col();
				
				while(col_scan());
				//IOPIN0=(IOPIN0&~(0XF<<8))|KEYLUT[row_no][col_no]<<8;
			}
			return KEYLUT[row_no][col_no];
}

int key;

int main()
{
	
	IODIR0|=0XFF<<8;
	lcd_init();
	init_keypad();
	while(1)
	{
		key=key_scan();
			display_int(key);
	}
	
	
}
*/
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
				//IOPIN0=(IOPIN0&~(0XF<<8))|KEYLUT[row_no][col_no]<<8;
				*key=KEYLUT[row_no][col_no];
				return 0;
			}
			return 1;
}

