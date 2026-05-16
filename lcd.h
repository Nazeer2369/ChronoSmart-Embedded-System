#include"types.h"
void lcd_init(void);
void write_lcd(int );
void cmd_lcd(int);
void display_char(int);
void display_string(char *);
void displayTime(int ,int ,int );
void displayDate(int ,int ,int );
void display_float(f32 f, u8 nDP);
void display_int(int);
	
