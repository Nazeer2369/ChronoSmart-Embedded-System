// Header file for LPC21xx microcontroller register definitions
#include <lpc21xx.h>
// Header file for LCD related functions
#include "lcd.h"

// Header file for keypad related functions
#include "keypad.h"

// Header file for RTC (Real Time Clock) functions
#include "rtc.h"

// Header file for device control functions
#include "devices.h"

// External interrupt channel number
#define CH_NO_EXT 14
// Header file for delay functions
#include "delay.h"
// #include "main.h"
//  Function prototype for PIN checking
int check_PIN(int, int);

// Temperature threshold default value
int temp_threshold = 800;

// Device-1 ON time variables
extern int D1_ON_HH, D1_ON_MM, D1_ON_SS;

// Device-2 ON time variables
extern int D2_ON_HH, D2_ON_MM, D2_ON_SS;

// Device-1 OFF time variables
extern int D1_OFF_HH, D1_OFF_MM, D1_OFF_SS;

// Device-2 OFF time variables
extern int D2_OFF_HH, D2_OFF_MM, D2_OFF_SS;

// Array storing names of week days
extern char Day[7][4];

// Variable storing current day index
extern int day1;

// Variable used to store function return results
int res = 0;

// Flags used in different menu operations
int temp_flag = 0, device_flag = 0, pass_flag = 0, date_flag = 0, time_flag = 0, done_flag = 0;

// extern int i;
// External interrupt/status flag
extern int flag;

// RTC current time and date variables
extern int hour1, min1, sec1, Date, Month, Year;

// Local variables used for temporary editing of time/date
int hour11, min11, sec11, date11, month11, year11, day;

// Function prototype to get PIN from keypad
int get_PIN(void);

// Main menu strings
char menu1[][11] =
{
  "0.Done",
  "1.time",
  "2.date",
  "3.Device",
  "4.PIN",
  "5.temp",
  "6.modified"

};

// String displayed while asking for PIN
char pass_str[] = "enter PIN";

// Done message string
char done[] = "Done";

// Wrong PIN message string
char w_pass[] = "invalid PIN";

// Device menu strings
char Device[][11] = 
{
  "0.Exit",
  "1.Device-1",
  "2.Device-2",
};

// Device ON/OFF timing menu strings

char Device_ON_OFF[][11] = 
{
  "0.Exit",
  "1.ON TIME",
  "2.OFF TIME"
};

// Default system PIN
int pass = 1111;

// Array containing day names
char days[][4] =
{
  "SUN",
  "MON",
  "TUE",
  "WED",
  "THU",
  "FRI",
  "SAT"
};

// char u_pass[5];
// char device[]="1.ON TIME 2.OFF TIME";
// Variable storing keypad key value
int key;

// Temporary variable for storing changed PIN
int temp_pass;

// General temporary variables
int temp1, pos = 0;

// General purpose variable
int abc;

// Function to perform backspace operation while entering numbers
// p	-> pointer to current index position
// var  -> double pointer to the variable being edited
void back_space(int* p, int** var)
{
  // Check whether cursor position is greater than 0
  if (*p > 0) {
    // Move cursor one position back
    --(*p);

    // Move LCD cursor to previous position
    cmd_lcd(0xc0 + (*p));

    // Clear previous digit from LCD
    display_char(' ');

    // Again place cursor at same position
    cmd_lcd(0xc0 + (*p));

    // Remove last digit from variable
    **var = **var / 10;

    // Update temporary variable
    temp1 = **var;

    /*
    Equivalent logic:
    *data = (*data)/10;
    temp1 = *data;
    */
  }
}

void device_display()
{
  int cnt = 0, pos = 0, break_flag = 0;

  // infinite loop for device menu
  while (1) {
    // clear LCD
    cmd_lcd(0x01);

    // move cursor to first line
    cmd_lcd(0x80);

    // display selection symbol
    display_char('-');

    // display current menu option
    cmd_lcd(0x84);
    display_string(Device_ON_OFF[pos]);

    // display next symbol
    cmd_lcd(0x8f);
    display_char('+');

    // move cursor to second line
    cmd_lcd(0xc0);

    // clear second line
    display_string("				");

    // display next menu item
    cmd_lcd(0xc4);
    display_string(Device_ON_OFF[(pos + 1) % 3]);
    display_string("  ");

    // wait for key press
    while (key_scan(&key)) {
      cnt++;

      // timeout condition
      if (cnt > 10000 * 1000) {
        cnt = 0;
        return;
      }
    }

    // reset counter
    cnt = 0;

    // if selected menu item is pressed
    if (key == pos || key == ((pos + 1) % 3)) {
      switch (key) {

      // ---------------- DEVICE 1 TIMINGS ----------------
      case 1: {
        while (1) {
          // display Device-1 ON time
          cmd_lcd(0x01);
          cmd_lcd(0x80);

          display_int(D1_ON_HH);
          display_char(':');

          display_int(D1_ON_MM / 10);
          display_int(D1_ON_MM % 10);

          display_char(':');

          display_int(D1_ON_SS / 10);
          display_int(D1_ON_SS % 10);

          // display Device-1 OFF time
          cmd_lcd(0xC0);

          display_int(D1_OFF_HH);
          display_char(':');

          display_int(D1_OFF_MM / 10);
          display_int(D1_OFF_MM % 10);

          display_char(':');

          display_int(D1_OFF_SS / 10);
          display_int(D1_OFF_SS % 10);

          // wait for key press
          while (key_scan(&key)) {
            cnt++;

            // timeout condition
            if (cnt > 10000 * 1000) {
              cnt = 0;
              return;
            }
          }

          cnt = 0;

          // exit on ENTER key
          if (key == 16) {
            break_flag = 1;
            break;
          }
        }

        // break from device display loop
        if (break_flag == 1) {
          break_flag = 0;
          break;
        }
      }

      // ---------------- DEVICE 2 TIMINGS ----------------
      case 2: {
        while (1) {
          // display Device-2 ON time
          cmd_lcd(0x01);
          cmd_lcd(0x80);

          display_int(D2_ON_HH);
          display_char(':');

          display_int(D2_ON_MM);
          display_char(':');

          display_int(D2_ON_SS);

          // display Device-2 OFF time
          cmd_lcd(0xC0);

          display_int(D2_OFF_HH);
          display_char(':');

          display_int(D2_OFF_MM);
          display_char(':');

          display_int(D2_OFF_SS);

          // wait for key press
          while (key_scan(&key)) {
            cnt++;

            // timeout condition
            if (cnt > 10000 * 1000) {
              cnt = 0;
              return;
            }
          }

          // exit on ENTER key
          if (key == 16) {
            break_flag = 1;
            break;
          }
        }

        // break from loop
        if (break_flag == 1) {
          break_flag = 0;
          break;
        }
      }

      // ---------------- EXIT ----------------
      case 0: {
        return;
      }
      }
    }

    // ---------------- MENU INCREMENT ----------------
    if (key == 11) {
      // move to next menu item
      pos = (pos + 1 + 3) % 3;
    }

    // ---------------- MENU DECREMENT ----------------
    if (key == 12) {
      // move to previous menu item
      pos = (pos - 1 + 3) % 3;
    }
  }

  // exit function
  return;
}

void modified()
{
  int cnt = 0, pos = 0, break_flag = 0;

  // infinite loop for modified menu
  while (1) {
    // clear LCD
    cmd_lcd(0x01);

    // move cursor to first line
    cmd_lcd(0x80);

    // show selection symbol
    display_char('-');

    // display current menu item
    cmd_lcd(0x84);
    display_string(menu1[pos]);

    // show next symbol
    cmd_lcd(0x8f);
    display_char('+');

    // move to second line
    cmd_lcd(0xc0);

    // clear second line
    display_string("				");

    // display next menu item
    cmd_lcd(0xc4);
    display_string(menu1[(pos + 1) % 6]);
    display_string("  ");

    // wait for key press
    while (key_scan(&key)) {
      cnt++;

      // timeout condition
      if (cnt > 10000 * 1000) {
        return;
      }
    }

    // reset counter
    cnt = 0;

    // if selected menu item is pressed
    if ((key == ((pos + 1) % 6)) || (key == (pos))) {
      switch (key) {
      // ---------------- DISPLAY TIME ----------------
      case 1:

        while (1) {
          // clear LCD
          cmd_lcd(0x01);

          // move cursor to first line
          cmd_lcd(0x80);

          // display modified time
          displayTime(hour11, min11, sec11);

          delay_ms(100);

          // wait for key
          while (key_scan(&key))
            ;

          // exit on ENTER key
          if (key == 16) {
            break_flag = 1;
            break;
          }
        }

        // exit inner loop
        if (break_flag == 1) {
          break_flag = 0;
          break;
        }

      // ---------------- DISPLAY DATE ----------------
      case 2:

        while (1) {
          // clear LCD
          cmd_lcd(0x01);

          // move cursor
          cmd_lcd(0x80);

          // display modified date
          displayDate(date11, month11, year11);

          // display day name
          display_char(' ');
          getDay(&day1);
          display_string(Day[day1]);

          // wait for key press
          while (key_scan(&key))
            ;

          // exit on ENTER key
          if (key == 16) {
            break_flag = 1;
            break;
          }
        }

        // exit inner loop
        if (break_flag == 1) {
          break_flag = 0;
          break;
        }

      // ---------------- DISPLAY DEVICE DETAILS ----------------
      case 3:

        // call device display function
        device_display();

        break;

      // ---------------- DISPLAY PASSWORD ----------------
      case 4:

        while (1) {
          // clear LCD
          cmd_lcd(0x01);

          // move cursor
          cmd_lcd(0x80);

          // display current password
          display_int(pass);

          // wait for key press
          while (key_scan(&key))
            ;

          // exit on ENTER key
          if (key == 16) {
            break_flag = 1;
            break;
          }
        }

        // exit inner loop
        if (break_flag == 1) {
          break_flag = 0;
          break;
        }

      // ---------------- DISPLAY TEMPERATURE THRESHOLD ----------------
      case 5:

        while (1) {
          // clear LCD
          cmd_lcd(0x01);

          // move cursor
          cmd_lcd(0x80);

          // display threshold heading
          display_string("temp thres");

          // display temperature threshold value
          display_int(temp_threshold);

          // wait for key press
          while (key_scan(&key))
            ;

          // exit on ENTER key
          if (key == 16) {
            break_flag = 1;
            break;
          }
        }

        // exit inner loop
        if (break_flag == 1) {
          break_flag = 0;
          break;
        }

      // ---------------- EXIT ----------------
      case 0:

        return;
      }
    }

    // ---------------- MENU INCREMENT ----------------
    if (key == 11) {
      // move to next menu item
      pos = (pos + 1 + 6) % 6;
    }

    // ---------------- MENU DECREMENT ----------------
    if (key == 12) {
      // move to previous menu item
      pos = (pos - 1 + 6) % 6;
    }
  }
}

int year_fun(int* key_val, int* index)
{
  // year11=0;

  // check whether pressed key is a numeric digit
  if ((*key_val) >= 0 && (*key_val) <= 9) {
    // move cursor to required position
    cmd_lcd(0xc0 + (*index));

    // display entered digit
    display_int(*key_val);

    // small delay
    delay_ms(20);

    // form complete year value
    // example: 2 -> 20 -> 202 -> 2026
    year11 = (year11 * 10) + (*key_val);

    // valid digit entered
    return 1;
  }

  // check for backspace key
  else if ((*key_val) == 11) {
    // allow backspace only if index > 0
    if ((*index) > 0) {
      // move one position back
      (*index)--;

      // move cursor to previous position
      cmd_lcd(0xc0 + (*index));

      // erase digit from LCD
      display_char(' ');

      // reposition cursor
      cmd_lcd(0xc0 + (*index));

      // remove last digit from year value
      year11 = year11 / 10;
    }

    // stay at same index
    return 0;
  }

  // invalid key pressed
  return 0;
}
void invalid(char* data)
{

  // clear LCD display

  cmd_lcd(0x01);

  // move cursor to first line first position

  cmd_lcd(0x80);

  // display "invalid"

  display_string("invalid");

  // display additional message passed to function

  // example: "hours", "Minutes", "DATE"

  display_string(data);

  // delay so user can read message

  delay_ms(100);

  // clear LCD again

  cmd_lcd(0x01);

  // set cursor back to first line

  cmd_lcd(0x80);
}

int Date_time(int* data, char* str)

{

  int k, cnt_flag = 0;

  // counter used for timeout checking
  long int cnt = 0;

  // temporary variable used for number formation
  temp1 = 0;

  // move cursor to second line
  cmd_lcd(0xc0);

  // clear second line
  display_string("			  ");

  // loop to take 2 digit input
  for (k = 0; k < 2;) {
    // display heading/message on first line
    cmd_lcd(0x80);
    display_string(str);

    // move cursor to second line
    cmd_lcd(0xc0);

    // wait until key is pressed
    while (key_scan(&key)) {
      cnt++;

      // timeout condition
      if (cnt > 10000 * 1000) {
        cnt_flag = 1;
      }
    }

    // if timeout occurs
    if (cnt_flag == 1) {
      cnt_flag = 0;
      cnt = 0;

      // return failure
      return 0;
    }

    // if numeric key is pressed
    if (key >= 0 && key <= 9) {
      // place cursor at current digit position
      cmd_lcd(0xc0 + k);

      // display entered digit
      display_int(key);

      delay_ms(100);

      // form complete number
      *data = (temp1 * 10) + key;

      // store latest value in temp variable
      temp1 = *data;

      // move to next digit position
      k++;
    }

    // backspace key pressed
    else if (key == 11) {
      /*
      if(k>0)
      {
          k--;

          cmd_lcd(0xc0+k);
          display_char(' ');

          cmd_lcd(0xc0+k);

          *data=(*data)/10;

          temp1=*data;
      }
      */

      // call backspace helper function
      back_space(&k, &data);
    }

    // invalid key pressed
    else if (key > 9 && key != 11 && key != 16) {
      // show invalid option message
      invalid(" option");

      delay_ms(50);
    }

    // enter/exit key pressed
    if (key == 16) {
      // return without saving further input
      return 0;
    }
  }

  // successful input
  return 1;

  // display_string("Minutes");
}
void goto_time()
{

  // reset temporary variable

  temp1 = 0;

  // clear LCD display

  cmd_lcd(0x01);

  // move cursor to first line

  cmd_lcd(0x80);

  // ---------------- HOURS ----------------

  // display hour input message

  display_string("Hours(0-23)");

  // display current hour value

  display_int(HOUR);

  // get hour input from user

  if (Date_time(&hour11, "Hours(0-23)") == 0) {

    // if user exits or timeout occurs

    // restore original hour value

    hour11 = HOUR;
  }

  // validate hour range

  while (hour11 > 23)

  {

    // show invalid message

    invalid("hours");

    // again ask for hour

    cmd_lcd(0x80);

    display_string("Hours(0-23)");

    // display original hour

    display_int(HOUR);

    // get hour again

    if ((Date_time(&hour11, "Hours(0-23)")) == 0) {
      // restore default value if cancelled
      hour11 = HOUR;
    }
  }

  // ---------------- MINUTES ----------------

  // clear LCD
  cmd_lcd(0x01);

  // move cursor to first line
  cmd_lcd(0x80);

  // display minute prompt
  display_string("Min(0-59)");

  // display current minute value
  display_int(min11);

  // get minute input
  if ((Date_time(&min11, "Min(0-59)")) == 0) {
    // restore previous minute if cancelled
    min11 = MIN;
  }

  // validate minute range
  while (min11 > 59) {
    // invalid minute message
    invalid("Minutes");

    // ask again
    cmd_lcd(0x80);
    display_string("Min(0-59)");

    display_int(min11);

    // get minute input again
    if ((Date_time(&min11, "Min(0-59)")) == 0) {
      // restore old minute value
      min11 = MIN;
    }
  }

  // ---------------- SECONDS ----------------

  // clear LCD
  cmd_lcd(0x01);

  // move cursor to first line
  cmd_lcd(0x80);

  // display seconds prompt
  display_string("Sec(0-59)");

  // display current seconds
  display_int(sec11);

  // get second input
  if ((Date_time(&sec11, "Sec(0-59)")) == 0) {
    // restore old second value
    sec11 = SEC;
  }

  // validate second range
  while (sec11 > 59) {
    // invalid seconds message
    invalid("seconds");

    // ask again
    cmd_lcd(0x80);
    display_string("Sec(0-59)");

    display_int(sec11);

    // get seconds input again
    if ((Date_time(&sec11, "Sec(0-59)")) == 0) {
      // restore old seconds
      sec11 = SEC;
    }
  }

  // ---------------- DISPLAY FINAL TIME ----------------

  // clear LCD
  cmd_lcd(0x01);

  // display updated time
  displayTime(hour11, min11, sec11);

  // small delay for visibility
  delay_ms(150);

  // clear LCD again
  cmd_lcd(0x01);

  // indicate time has been modified
  time_flag = 1;
}
void goto_date()
{
  int i, prev_date = date11;
  ; // store previous date value
  temp1 = 0;
  cmd_lcd(0x01); // clear LCD

  // year
t1:
  year11 = 0; // reset year value

  cmd_lcd(0x01);
  cmd_lcd(0x80); // first line

  // ask user to enter year
  display_string("YYYY:(2026-4095)");
  display_int(year11);

  temp1 = 0;

  // take 4 digit year input
  for (i = 0; i < 4;) {
    // wait for key press
    while (key_scan(&key))
      ;

    // validate and store year digit
    if (year_fun(&key, &i)) {
      i++;
    }
  }

  // validate year range
  if (year11 < 2026 || year11 > 4095) {
    invalid(" year"); // invalid year message
    goto t1; // re-enter year
  }

  cmd_lcd(0x01);
  temp1 = 0;

  // select day
  while (1) {
    day = temp1;

    cmd_lcd(0x01);

    // display current selected day
    display_char('-');

    for (i = 0; i < 6; i++) {
      display_char(32); // spaces
    }

    display_string(days[temp1]);

    for (i = 0; i < 5; i++) {
      display_char(32); // spaces
    }

    display_char('+');

    // display day index
    cmd_lcd(0xc0);
    display_int(temp1);

    // wait for key press
    while (key_scan(&key))
      ;

    // 11 -> increment
    // 12 -> decrement
    // 16 -> ok
    switch (key) {
    case 16:
      date_flag = 1; // confirm selection
      break;

    case 15:
      date_flag = 2; // exit selection
      break;

    case 11:
      temp1++; // next day
      display_char(32);
      display_int(temp1);

      if (temp1 > 6) {
        temp1 = 0; // wrap around
      }

      temp1 = temp1 % 7;
      break;

    case 12:
      --temp1; // previous day

      if (temp1 < 0) {
        temp1 = 6; // wrap around
      }

      temp1 = temp1 % 7;
      break;
    }

    // exit loop after selection
    if (date_flag == 2 || date_flag == 1) {
      break;
    }
  }

  // ---------------- MONTH ----------------

  cmd_lcd(0x01);

  display_string("Month(01-12)");
  display_int(month11);

  // get month value
  Date_time(&month11, "Month(01-12)");

  // validate month
  while (month11 > 12 || month11 < 1) {
    invalid(" Month");

    display_string("Month(01-12)");
    display_int(month11);

    Date_time(&month11, "Month(01-12)");
  }

  // ---------------- DATE ----------------

  cmd_lcd(0x01);

  // check for February
  if (month11 == 2) {
    // leap year condition
    if ((year11 % 400 == 0) || (year11 % 4 == 0 && year11 % 100 != 0)) {
      // February with 29 days
      display_string("DATE(01-29)");
      display_int(date11);

      Date_time(&date11, "DATE(01-29)");

      // validate date
      while (date11 > 29 || date11 < 1) {
        cmd_lcd(0x01);

        invalid(" DATE");

        display_string("DATE(01-29)");
        display_int(prev_date);

        Date_time(&date11, "DATE(01-29)");
      }
    }

    // non leap year
    else {
      display_string("DATE(01-28)");
      display_int(date11);

      Date_time(&date11, "DATE(01-28)");

      // validate date
      while (date11 > 28 || date11 < 1) {
        cmd_lcd(0x01);

        invalid(" DATE");

        display_string("DATE(01-28)");
        display_int(prev_date);

        Date_time(&date11, "DATE(01-28)");
      }
    }
  }

  // all other months
  else {
    display_string("DATE(01-31)");
    display_int(date11);

    Date_time(&date11, "DATE(01-31)");

    // validate date
    while (date11 > 31 || date11 < 1) {
      cmd_lcd(0x01);

      invalid(" DATE");

      display_string("DATE(01-31)");
      display_int(prev_date);

      Date_time(&date11, "DATE(01-31)");
    }
  }

  // date_flag=1;
}
/*---------------------------------------------------------
Function : change_PIN
Purpose  : Allows user to change system PIN.

Steps:
1. Verify old PIN
2. Enter new 4-digit PIN
3. Store new PIN temporarily
----------------------------------------------------------*/
void change_PIN()
{
  int i, p, user = 0;

  // Reset temporary variable
  temp1 = 0;

  // Clear LCD
  cmd_lcd(0x01);

  // Move cursor to first line first position
  cmd_lcd(0x80);

  // Display PIN prompt
  display_string(pass_str);

  // Read PIN
  user = get_PIN();

  // Timeout condition
  if (user == -1) {
    return;
  }

  /*if(count>6)
  {
      count=0;
      return 1;
  }*/

  // Verify entered PIN
  if (check_PIN(user, pass) == 1) {
    // PIN correct -> Change PIN

    cmd_lcd(0x01);

    // Read 4-digit new PIN
    for (p = 0; p < 4;) {
      // Display prompt
      cmd_lcd(0x80);

      display_string("New PIN(0-9)");

      delay_ms(10);

      // Wait for key press
      while (key_scan(&key))
        ;

      // If numeric key pressed
      if (key >= 0 && key <= 9) {
        // Store PIN digit
        temp_pass = (temp1 * 10) + key;

        // Display entered digit
        cmd_lcd(0xc0 + p);

        display_int(key);

        delay_ms(100);

        // Replace digit with '*'
        cmd_lcd(0xc0 + p);

        display_char('*');

        // Update temporary value
        temp1 = temp_pass;

        p++;
      }

      // Invalid key except backspace
      else if (key != 11) {
        // Display invalid option message
        invalid(" option");

        delay_ms(100);

        // Restore already entered '*'
        cmd_lcd(0xC0);

        for (i = 0; i < p; i++) {
          display_char('*');
        }
      }

      // Backspace operation
      else if (key == 11) {
        // Check valid position
        if (p > 0) {
          // Move back one position
          p--;

          // Clear previous character
          cmd_lcd(0xc0 + p);

          display_char(' ');

          // Move cursor back
          cmd_lcd(0xc0 + p);

          // Remove last digit
          temp_pass = temp_pass / 10;

          temp1 = temp_pass;
        }
      }

      // cmd_lcd(0x01);
      // delay_ms(50);
    }

    // Display success message
    cmd_lcd(0x01);

    cmd_lcd(0x80);

    display_string("PIN changed");

    delay_ms(200);

    // Clear LCD
    cmd_lcd(0x01);

    // Set PIN update flag
    pass_flag = 1;
  }

  // Wrong old PIN
  else {
    invalid(" PIN");
  }
}
/*---------------------------------------------------------
Function : goto_device_1
Purpose  : Configure Device-1 ON/OFF timings.

Return:
-1 -> Timeout occurred
 0 -> Device timing modified
 1 -> Exit from menu
----------------------------------------------------------*/
int goto_device_1()
{
  // Menu position and status flags
  int pos = 0, device_on_flag = 1, device_off_flag = 1, d_flag = 1;

  // Counter for timeout
  long int cnt = 0;

  // Main menu loop
  while (1) {
    // Clear LCD
    cmd_lcd(0x01);

    cmd_lcd(0x80);

    // Display current menu item
    display_char('-');

    cmd_lcd(0x84);

    display_string(Device_ON_OFF[pos]);

    // Display '+' symbol
    cmd_lcd(0x8f);

    display_char('+');

    // Clear second line
    cmd_lcd(0xc0);

    display_string("				");

    // Display next menu item
    cmd_lcd(0xc4);

    display_string(Device_ON_OFF[(pos + 1) % 3]);

    display_string("  ");

    /*
    Old display logic

    temp1=0;
    cmd_lcd(0x01);
    cmd_lcd(0x80);
    display_string("1.ON TIME");
    cmd_lcd(0xc0);
    display_string("2.OFF TIME");
    */

    // Wait for key press
    while (key_scan(&key)) {
      cnt++;

      // Timeout condition
      if (cnt > 10000 * 1000) {
        cnt = 0;

        return -1;
      }
    }

    // Reset counter
    cnt = 0;

    // Check selected menu option
    if (key == pos || key == ((pos + 1) % 3)) {
      /*---------------- DEVICE ON TIME ----------------*/
      if (key == 1) {
        temp1 = 0;

        cmd_lcd(0x01);

        cmd_lcd(0x80);

        // Read ON hour
        display_string("Hours");

        Date_time(&D1_ON_HH, "HOUR(00-23)");

        display_int(D1_ON_HH);

        // Validate hour
        if (D1_ON_HH > 23) {
          invalid(" hour");

          display_string("HOUR(00-23)");

          Date_time(&D1_ON_HH, "HOUR(00-23)");

          display_int(D1_ON_HH);
        }

        // Read ON minutes
        cmd_lcd(0x01);

        display_string("Minutes");

        Date_time(&D1_ON_MM, "Minutes(00-59)");

        display_int(D1_ON_MM);

        // Validate minutes
        while (D1_ON_MM > 59) {
          invalid(" Minutes");

          display_string("Minutes(00-59)");

          Date_time(&D1_ON_MM, "Minutes(00-59)");

          display_int(D1_ON_MM);
        }

        // Read ON seconds
        cmd_lcd(0x01);

        cmd_lcd(0x80);

        display_string("SECONDS");

        Date_time(&D1_ON_SS, "Seconds(0-59)");

        display_int(D1_ON_SS);

        // Validate seconds
        while (D1_ON_SS > 59) {
          // Greater than 59 is invalid
          invalid(" Seconds");

          display_string("Seconds(00-59)");

          // Read seconds again
          Date_time(&D1_ON_SS, "Seconds(00-59)");

          display_int(D1_ON_SS);
        }

        // ON time modified successfully
        device_on_flag = 0;

        break;
      }

      /*---------------- DEVICE OFF TIME ----------------*/
      if (key == 2) {
        temp1 = 0;

        cmd_lcd(0x01);

        cmd_lcd(0x80);

        // Read OFF hour
        display_string("Hour");

        Date_time(&D1_OFF_HH, "Hour(00-23)");

        display_int(D1_OFF_HH);

        // Validate hour
        while (D1_OFF_HH > 23) {
          invalid(" Hour");

          display_string("Hour(00-23)");

          Date_time(&D1_OFF_HH, "Hour(00-23)");

          display_int(D1_OFF_HH);
        }

        // Read OFF minutes
        cmd_lcd(0x01);

        display_string("Minutes");

        Date_time(&D1_OFF_MM, "Minutes(00-59)");

        display_int(D1_OFF_MM);

        // Validate minutes
        while (D1_OFF_MM > 59) {
          invalid(" Minutes");

          display_string("Minutes(00-59)");

          Date_time(&D1_OFF_MM, "Minutes(00-59)");

          display_int(D1_OFF_MM);
        }

        // Read OFF seconds
        cmd_lcd(0x01);

        cmd_lcd(0x80);

        display_string("Seconds");

        Date_time(&D1_OFF_SS, "Seconds(00-59)");

        display_int(D1_OFF_SS);

        // Validate seconds
        while (D1_OFF_SS > 59) {
          invalid(" seconds");

          display_string("SS:Seconds(0-59)");

          Date_time(&D1_OFF_SS, "SS:Seconds(0-59)");

          display_int(D1_OFF_SS);
        }

        // OFF time modified successfully
        device_off_flag = 0;

        break;
      }

      /*---------------- EXIT ----------------*/
      if (key == 0) {
        // If timings not modified
        if (device_off_flag == 1 || device_on_flag == 1) {
          d_flag = -1;

          break;
        }

        // Exit normally
        else
          d_flag = 1;

        break;
      }
    }

    // Increment menu position
    if (key == 11) {
      // pos++;
      // pos=pos%3;

      pos = (pos + 1 + 3) % 3;
    }

    // Decrement menu position
    if (key == 12) {
      // pos--;
      // pos=pos%3;

      pos = (pos - 1 + 3) % 3;
    }

    // Clear LCD
    cmd_lcd(0x01);

    delay_ms(20);

    // If timing modified successfully
    if (device_on_flag == 0 || device_off_flag == 0) {
      device_on_flag = 1;

      device_off_flag = 1;

      d_flag = 0;

      break;
    }
  }

  // Return status
  if (d_flag == 0)
    return 0;

  else if (d_flag == 1)
    return 1;

  else
    return -1;
}
/*---------------------------------------------------------
Function : goto_device_2
Purpose  : Configure Device-2 ON/OFF timings.

Return:
-1 -> Timeout occurred
 0 -> Device timing modified
 1 -> Exit from menu
----------------------------------------------------------*/
int goto_device_2()
{
  // Menu position and status flags
  int pos = 0, device_off_flag = 1, device_on_flag = 1, d_flag = 1;

  // Counter for timeout
  long int cnt = 0;

  // Clear LCD
  cmd_lcd(0x01);

  cmd_lcd(0x80);

  // Main menu loop
  while (1) {
    // Clear LCD
    cmd_lcd(0x01);

    cmd_lcd(0x80);

    // Display current menu item
    display_char('-');

    cmd_lcd(0x84);

    display_string(Device_ON_OFF[pos]);

    // Display '+' symbol
    cmd_lcd(0x8f);

    display_char('+');

    // Clear second line
    cmd_lcd(0xc0);

    display_string("				");

    // Display next menu item
    cmd_lcd(0xc4);

    display_string(Device_ON_OFF[(pos + 1) % 3]);

    display_string("  ");

    /*
    Old display logic

    display_string("1.ON TIME");
    cmd_lcd(0xc0);
    display_string("2.OFF TIME");
    */

    // Wait for key press
    while (key_scan(&key)) {
      cnt++;

      // Timeout condition
      if (cnt > 10000 * 1000) {
        return -1;
      }
    }

    // Reset counter
    cnt = 0;

    // Check selected menu
    if (key == pos || key == ((pos + 1) % 3)) {
      /*---------------- DEVICE ON TIME ----------------*/
      if (key == 1) {
        temp1 = 0;

        cmd_lcd(0x01);

        cmd_lcd(0x80);

        // Read ON hour
        display_string("Hours");

        Date_time(&D2_ON_HH, "HH:Hour(0-23)");

        // Validate hour
        while (D2_ON_HH > 23) {
          invalid(" Hour");

          display_string("HH:Hour(0-23)");

          Date_time(&D2_ON_HH, "HH:Hour(0-23)");
        }

        // Read ON minutes
        cmd_lcd(0x01);

        display_string("Minutes");

        Date_time(&D2_ON_MM, "MM:Minutes(0-59)");

        // Validate minutes
        while (D2_ON_MM > 59) {
          invalid(" Minutes");

          display_string("MM:Minutes(0-59)");

          Date_time(&D2_ON_MM, "MM:Minutes(0-59)");
        }

        // Read ON seconds
        cmd_lcd(0x01);

        cmd_lcd(0x80);

        display_string("SS");

        Date_time(&D2_ON_SS, "SS:Seconds(0-59)");

        // Validate seconds
        while (D2_ON_SS > 59) {
          invalid(" Seconds");

          display_string("SS:Seconds(0-59)");

          Date_time(&D2_ON_SS, "SS:Seconds(0-59)");
        }

        // ON time successfully modified
        device_on_flag = 0;

        break;
      }

      /*---------------- DEVICE OFF TIME ----------------*/
      if (key == 2) {
        temp1 = 0;

        cmd_lcd(0x01);

        cmd_lcd(0x80);

        // Read OFF hour
        display_string("Hours");

        Date_time(&D2_OFF_HH, "HH:Hour(0-23)");

        // Validate hour
        while (D2_OFF_HH > 23) {
          invalid(" Hour");

          display_string("HH:Hour(0-23)");

          Date_time(&D2_OFF_HH, "HH:Hour(0-23)");
        }

        // Read OFF minutes
        cmd_lcd(0x01);

        display_string("Minutes");

        Date_time(&D2_OFF_MM, "MM:Minutes(0-59)");

        // Validate minutes
        while (D2_OFF_MM > 59) {
          invalid(" Minutes");

          display_string("MM:Minutes(0-59)");

          Date_time(&D2_OFF_MM, "MM:Minutes(0-59)");
        }

        // Read OFF seconds
        cmd_lcd(0x01);

        cmd_lcd(0x80);

        display_string("SECONDS");

        Date_time(&D2_OFF_SS, "SS:Seconds(0-59)");

        // Validate seconds
        while (D2_OFF_SS > 59) {
          invalid(" Seconds");

          display_string("SS:Seconds(0-59)");

          Date_time(&D2_OFF_SS, "SS:Seconds(0-59)");
        }

        // OFF time successfully modified
        device_off_flag = 0;

        break;
      }

      /*---------------- EXIT ----------------*/
      if (key == 0) {
        // If no timings modified
        if (device_off_flag == 1 || device_on_flag == 1) {
          d_flag = -1;

          break;
        }

        // Exit normally
        else
          d_flag = 1;

        break;
      }
    }

    // Increment menu position
    if (key == 11) {
      pos = (pos + 1 + 3) % 3;

      // pos++;
      // pos=pos%3;
    }

    // Decrement menu position
    if (key == 12) {
      // pos--;

      pos = (pos - 1 + 3) % 3;

      // pos=pos%3;
    }

    // Clear LCD
    cmd_lcd(0x01);

    delay_ms(20);

    // If timing modified
    if (device_on_flag == 0 || device_off_flag == 0) {
      device_on_flag = 1;

      device_off_flag = 1;

      d_flag = 0;

      break;
    }
  }

  // Return status
  if (d_flag == 0)
    return 0;

  else if (d_flag == 1)
    return 1;

  else
    return -1;
}
/*---------------------------------------------------------
Function : devices_fun
Purpose  : Displays device menu and allows user to
        configure Device-1 and Device-2 timings.

Return:
-1 -> Timeout occurred
 0 -> Device timings modified
 1 -> Exit from menu
----------------------------------------------------------*/
int devices_fun()
{
  // Counter for timeout
  int cnt = 0;
  // Status flag for device operations
  int device_flag = 0;
  // Initial menu position
  int pos1 = 1;
  // Main device menu loop
  while (1) {
    // Clear LCD
    cmd_lcd(0x01);

    // First line first position
    cmd_lcd(0x80);

    // Display current menu item
    display_char('-');

    cmd_lcd(0x84);

    display_string(Device[pos1]);

    display_char(' ');

    // Display '+' symbol
    cmd_lcd(0x8f);

    display_char('+');

    // Clear second line
    cmd_lcd(0xc0);

    display_string("				");

    // Display next menu item
    cmd_lcd(0xc4);

    display_string(Device[(pos1 + 1) % 3]);

    display_string("   ");

    /*
    Old display method
    cmd_lcd(0x01);
    display_string("1.Device1 Time ");
    cmd_lcd(0xc0);
    display_string("2.Device2 Time");
    */

    // Wait for key press
    while (key_scan(&key)) {
      cnt++;

      // Timeout condition
      if (cnt > 10000 * 1000) {
        cnt = 0;

        return -1;
      }
    }

    // Reset counter
    cnt = 0;

    /*
    Old menu display
    cmd_lcd(0x01);
    display_string("1.Device1 Time ");
    cmd_lcd(0xc0);
    display_string("2.Device2 Time");
    */

    // Check if selected current or next menu option
    if ((key == ((pos + 1) % 3)) || (key == (pos1))) {
      switch (key) {
      /*---------------- DEVICE 1 ----------------*/
      case 1:

        // Configure Device-1 timings
        res = goto_device_1();

        // Timeout
        if (res == -1)
          device_flag = -1;

        // Timings modified
        else if (res == 0)
          device_flag = 0;

        // Exit
        else
          device_flag = 1;

        break;

      /*---------------- DEVICE 2 ----------------*/
      case 2:

        // Configure Device-2 timings
        res = goto_device_2();

        // Timeout
        if (res == -1)
          device_flag = -1;

        // Timings modified
        else if (res == 0)
          device_flag = 0;

        // Exit
        else
          device_flag = 1;

        break;

      /*---------------- EXIT ----------------*/
      case 0:

        device_flag = 1;

        break;
      }
    }

    /*
    Old logic retained for reference

    if(key==1)
    {
        if(goto_device_1()==-1)
        {
            return -1;
        }
        else if(goto_device_1()==0)
            return 0;
        else
            return 1;
    }

    if(key==2)
    {
        if(goto_device_2()==-1)
            return 1;
        else if (goto_device_2()==0)
            return 0;
        else
            return 1;
    }
    */

    // Increment menu position
    if (key == 11) {
      pos1 = (pos1 + 1 + 3) % 3;
    }

    // Decrement menu position
    if (key == 12) {
      pos1 = (pos1 - 1 + 3) % 3;
    }

    // Clear LCD
    cmd_lcd(0x01);

    delay_ms(20);

    // Exit loop
    if (device_flag == 1)
      break;
  }

  // Return status
  if (device_flag == -1)
    return -1;

  if (device_flag == 0)
    return 0;

  if (device_flag == 1)
    return 1;

  // return ;
}

/*---------------------------------------------------------
Function : get_PIN
Purpose  : Reads 4-digit PIN from keypad.

Return:
PIN value -> if entered successfully
-1			 -> if timeout occurs
----------------------------------------------------------*/
int get_PIN()
{
  int value = 0, p;

  // Counter used for timeout
  long int cnt = 0;

  int i;

  // Read 4 digits
  for (i = 0; i < 4;) {
    // Display PIN prompt
    cmd_lcd(0x80);

    display_string(pass_str);

    // Move cursor to second line
    cmd_lcd(0xc0);

    // Wait for key press
    while (key_scan(&key)) {
      cnt++;

      // Timeout condition
      if (cnt > 10000 * 1000) {
        cnt = 0;

        return -1;
      }
    }

    // Reset counter
    cnt = 0;

    // If numeric key pressed
    if (key >= 0 && key <= 9) {
      // Move cursor position
      cmd_lcd(0xc0 + i);

      // Display entered digit temporarily
      display_int(key);

      delay_ms(20);

      // Replace digit with '*'
      cmd_lcd(0xc0 + i);

      display_char('*');

      // Store PIN digit
      value = (value * 10) + key;

      i++;
    }

    // Backspace operation
    else if (key == 11) {
      // Check valid position
      if (i >= 0) {
        // Move back one position
        i--;
        // Clear previous character
        cmd_lcd(0xc0 + i);
        display_char(' ');
        // Move cursor back
        cmd_lcd(0xc0 + i);
        // Remove last entered digit
        value = value / 10;
        temp1 = value;
        // Prevent negative index
        if (i < 0)
          i = 0;
      }
    }

    // Invalid key pressed
    else if (key > 9 && key != 11) {
      // Display invalid option message
      invalid(" option");

      delay_ms(100);

      // Restore '*' characters
      cmd_lcd(0xC0);

      for (p = 0; p < i; p++) {
        display_char('*');
      }
    }
  }

  delay_ms(20);

  // Return entered PIN
  return value;
}
/*---------------------------------------------------------
Function : check_PIN
Purpose  : Compares entered PIN with stored PIN.

Parameters:
p1 -> User entered PIN
p2 -> Stored PIN

Return:
1 -> PIN matched
0 -> PIN not matched
----------------------------------------------------------*/
int check_PIN(int p1, int p2)
{
  // int i;

  /*
  Old logic for string comparison

  for(i=0;i<4;i++)
  {
      if(str1[i]!=str2[i])
      {
          return 1;
          break;
      }
  }
  */

  // Compare both PINs
  if (p1 == p2) {
    // PIN correct
    return 1;
  }

  // PIN incorrect
  return 0;
}
/*---------------------------------------------------------
Function : goto_menu
Purpose  : Verifies PIN and displays main menu.
        Allows user to:
        1. Set Time
        2. Set Date
        3. Configure Devices
        4. Change PIN
        5. Set Temperature Threshold
        6. View Modified Values
----------------------------------------------------------*/
int goto_menu()
{
  int user = 0;
  // Counter used for timeout
  long int cnt = 0;
  // Copy current RTC values into temporary variables
  hour11 = hour1;
  min11 = min1;
  sec11 = sec1;
  year11 = Year;
  month11 = Month;
  date11 = Date;
  // Clear LCD
  cmd_lcd(0x01);
  // Cursor to first line first position
  cmd_lcd(0x80);
  // Display PIN prompt
  display_string(pass_str);
  // Read PIN from keypad
  user = get_PIN();
  // Timeout condition
  if (user == -1) {
    return -1;
  }

  /*if(count>6)
  {
      count=0;
      return 1;
  }*/
  // Compare entered PIN with stored PIN
  if (check_PIN(user, pass) == 1) {
    // Initialize menu position
    pos = 1;
    // Main menu loop
    while (1) {
      // Clear LCD
      cmd_lcd(0x01);
      // Display current menu option
      cmd_lcd(0x80);
      display_char('-');
      cmd_lcd(0x84);
      display_string(menu1[pos]);
      cmd_lcd(0x8f);
      display_char('+');
      // Display next menu option
      cmd_lcd(0xc0);
      display_string("				");
      cmd_lcd(0xc4);
      display_string(menu1[(pos + 1) % 7]);
      display_string("  ");
      // Wait for keypad input
      while (key_scan(&key)) {
        cnt++;
        // Timeout
        if (cnt > 10000 * 1000) {
          return -1;
        }
      }
      // If selected current or next menu
      if ((key == ((pos + 1) % 7)) || (key == (pos))) {
        switch (key) {
        /*---------------- TIME MENU ----------------*/
        case 1:
          // Go to time setting
          goto_time();
          temp1 = 0;
          time_flag = 1;
          break;
        /*---------------- DATE MENU ----------------*/
        case 2:
          // Go to date setting
          goto_date();
          temp1 = 0;
          date_flag = 1;
          break;
        /*---------------- DEVICE MENU ----------------*/
        case 3:
          // Configure devices
          res = devices_fun();
          // Timeout
          if (res == -1) {
            device_flag = 1;
            break;
          }
          // Exit
          else if (res == 1) {
            device_flag = 0;
            break;
          }
          // Success message
          cmd_lcd(0x01);
          display_string("device timings are set");
          delay_ms(50);
          break;
        /*---------------- PIN MENU ----------------*/
        case 4:
          // Change PIN
          change_PIN();
          pass_flag = 1;
          temp1 = 0;
          break;
        /*---------------- TEMPERATURE MENU ----------------*/
        case 5:
          // Clear LCD
          cmd_lcd(0x01);
          cmd_lcd(0x80);
          // Display message
          display_string("temp thresh");
          // Read threshold value
          Date_time(&temp_threshold, "temp thresh");
          temp_flag = 1;
          break;
        /*---------------- MODIFIED VALUES ----------------*/
        case 6:
          cmd_lcd(0x01);
          cmd_lcd(0x80);
          // Display modified settings
          modified();
          break;

        /*---------------- DONE / EXIT ----------------*/
        case 0:
          // Update RTC time
          if (time_flag == 1) {
            time_flag = 0;
            set_time(hour11, min11, sec11);
          }
          // Update RTC date
          if (date_flag == 1) {
            date_flag = -1;
            setDate(date11, month11, year11);
            set_Day(day);
          }

          // Update temperature settings
          if (temp_flag == 1) {
            temp_flag = 0;
            set_Day(temp1);
          }
          // Update PIN
          if (pass_flag) {
            pass_flag = 0;
            pass = temp_pass;
          }
          // Reset device flag
          if (device_flag == 1) {
            device_flag = 0;
          }
          // Done flag
          done_flag = 1;
          break;
        }
      }
      // Increment menu position
      if (key == 11) {
        pos = (pos + 1 + 7) % 7;
      }
      // Decrement menu position
      if (key == 12) {
        pos = (pos - 1 + 7) % 7;
      }
      /*
      Old menu handling code kept for reference
      */

      // Clear LCD
      cmd_lcd(0x01);
      delay_ms(50);
      // Exit menu loop
      if (done_flag == 1) {
        done_flag = 0;
        break;
      }
    }
    // Clear LCD before exit
    cmd_lcd(0x01);
    return 1;
  }
  // Wrong PIN
  else {
    cmd_lcd(0x80);
    display_string(w_pass);
    delay_ms(50);
    cmd_lcd(0x80);
  }

  // Invalid PIN return
  return 0;

  // cmd_lcd(0x01);
  // display_string(Done);
}
/*---------------------------------------------------------
Function : intrpt
Purpose  : Handles interrupt based menu access.
        Gives user 3 chances to enter correct PIN.
        If PIN is correct -> opens menu.
        If all attempts fail -> device gets locked temporarily.
----------------------------------------------------------*/
void intrpt()
{
  int cnt = 0, delay = 30;
  // Try entering menu maximum 3 times
  while (cnt < 3) {
    cnt++;
    // Open menu and verify PIN
    res = goto_menu();
    // If menu access successful then exit loop
    if (res) {
      break;
    }
    // Small delay between attempts
    delay_ms(100);
  }
  // If entered successfully OR timeout occurred
  if (cnt < 3 || res == -1) {
    // Clear LCD
    cmd_lcd(0x01);
    // Display DONE message
    display_string("DONE");
    delay_ms(70);
    // Clear LCD again
    cmd_lcd(0x01);
    // Reset counters and result
    cnt = 0;
    res = 0;
    // Set flag indicating operation complete

    flag = 1;

  }
  // If all 3 attempts failed
  else {
    // Clear LCD
    cmd_lcd(0x01);
    // Display lock message
    display_string("device locked!!");
    // Move cursor to second line
    cmd_lcd(0xc0);
    display_string("wait for:");
    // Countdown delay
    while (delay--) {
      // Display remaining seconds
      display_int(delay);
      display_string("sec");
      delay_ms(80);
      // Move cursor position
      cmd_lcd(0xc9);
      // Set flag
      flag = 1;
    }
  }
  // Finally clear LCD
  cmd_lcd(0x01);
  // flag=1;
}
void eint0_isr(void) __irq
{
  flag = 0;
  // cmd_lcd(0x01);
  EXTINT = 1 << 0;
  // wriing dummy address which clears the existing interrupt address
  VICVectAddr = 0;
  // Disablig interruput
  // or clearing the interrupt
}
void enable_eint0()
{
  // IODIR0 &= ~(1<<14);
  // IODIR0|=1<<0;
  // IODIR0|=1<<2;

  // Cfg po.1 as EINT0
  PINSEL0 |= 0x0000000C;

  // PINSEL1|=0x00000001;
  // Select EINT0 as IRQ
  VICIntSelect = 0 << CH_NO_EXT;

  // Enabling the interrupt
  VICIntEnable = 1 << CH_NO_EXT;

  // assiging the isr address
  VICVectAddr0 = (unsigned int)eint0_isr;

  // assign slot to that particular channel
  VICVectCntl0 = (1 << 5) | CH_NO_EXT;
  // select
  // 0:level or
  // 1:edge triggering
  // 31 ...............  4   3	 2	 1	   0
  //    --------------------------------------
  //    |   Reserved   | E3 |  E2 |  E1 |  E0 |
  //   --------------------------------------
  //
  EXTMODE = 1 << 0;

  // selected edge so possibilites are

  // 1:raising edge or

  // 0:falling edge
  // EXTPOLAR=~1<<0;//selecting raising edge
}
