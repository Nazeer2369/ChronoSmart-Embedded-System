int key_scan(int*);
int find_col(void);
int find_row(void);
int col_scan(void);
void keypad_init(void);

#define READNIBBLE(WORD,DATA) ((WORD>>DATA)&(0XF))

