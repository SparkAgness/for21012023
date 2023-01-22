#include <forcedClimate.h>
#include <microDS3231.h>
#include <RGBmatrixPanel.h>

#define alarm_pin 3
#define increm_pin 7
#define decrem_pin 5
#define clock_clim 1000
#define anti_bounce 500
#define switch_num_max 11

#define CLK 11 // these 7 defines are for RGB-matrix 32x64
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3

MicroDS3231 rtc;
ForcedClimate climateSensor = ForcedClimate();

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 64); //declare object for matrix, twice-buffer on(true)

uint8_t menu_switcher(); //declaration of func (see below)

bool increm, decrem, cc_flag, alarm_flag;
bool holiday_flag;
uint16_t clock;
uint32_t time_enter; //always will be in global-value!!!
uint32_t timer, timer2;
uint8_t sec, minute, hour, week_day, day, month; // for setting clock
uint8_t alarm_minute, alarm_hour; //for setting alarm
uint16_t year;
uint8_t m_switcher;
enum {dur_clim_display = 2000, dur_clock_display = 4000}; //durations of matrix_print_climate() and matrix_print_clock()

struct val_flag {    //will be used with switch_helper 
    uint8_t val;     //will return value of increm/decrem/menu_switcher mode
    bool flag;       //will return value of switcher's flag
} switch_help;

struct val_flag *pSwitch_help = &switch_help;

void clock_var_val() {
    sec = rtc.getSeconds();
    minute = rtc.getMinutes();
    hour = rtc.getHours();
    week_day = rtc.getDay();
    day = rtc.getDate();
    month = rtc.getMonth();
    year = 2023; //must be edit
}

struct val_flag switch_helper(bool increm, bool decrem, struct val_flag *pSwitch_help) {
//helps to avoid effect of changing clock's values during menu_switcher() using
    
    if (!increm && decrem && millis() - time_enter > anti_bounce) {
        pSwitch_help->val = 1;
        pSwitch_help->flag = 1;
        time_enter = millis();
    }
    if (increm && !decrem && millis() - time_enter > anti_bounce) {
        pSwitch_help->val = 2;
        pSwitch_help->flag = 1;
	time_enter = millis();
    }
    if (!increm && !decrem && millis() - time_enter > anti_bounce) {
        pSwitch_help->val = 3;
        pSwitch_help->flag = 1;
        time_enter = millis();
    }
    if (increm && decrem && millis() - time_enter > anti_bounce) {
        pSwitch_help->flag = 0;
        time_enter = millis();
    }

    return *pSwitch_help;
}

uint8_t menu_switcher(uint8_t *m_switcher, struct val_flag *pSwitch_help) { 
//switches to setup clock menu
    
    if (pSwitch_help->val == 3 && pSwitch_help->flag == 0) {
        *m_switcher += 1;
        pSwitch_help->val = 0; // so that *m_switcher++ will be once
        if (*m_switcher == switch_num_max) {
            *m_switcher = 0;
	}
    }
    return *m_switcher;
}

void print_alarm_status(bool flag) {
  uint8_t indentX, indentY, rect_hight, rect_width;
  indentX = 1;
  indentY = 1;
  rect_hight = 3;
  rect_width = 3;
  if (flag) {
    if (hour <= 7 || hour >= 21)
      matrix.fillRect(indentX, indentY, rect_hight, rect_width, matrix.Color333(0, 0, 1));
    if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
      matrix.fillRect(indentX, indentY, rect_hight, rect_width, matrix.Color333(7, 3, 0));
    if (hour >= 12 && hour <= 17)
      matrix.fillRect(indentX, indentY, rect_hight, rect_width, matrix.Color333(7, 0, 0));
  }
}

uint16_t switcher(uint16_t *clck, struct val_flag *pSwitch_help) { 
  struct flags { // the structure is for comfortable work with flags and values of switch_help struct 
    bool inc = 0;
    bool dec = 0;
  } inc_dec;
    if (pSwitch_help->val == 2 && pSwitch_help->flag == 0) {
      inc_dec.dec = 1;
    }
    if (pSwitch_help->val == 1 && pSwitch_help->flag == 0) {
      inc_dec.inc = 1;       
    }
    clck = &clock; //it's necessary to clock be changed from this func
    if (inc_dec.dec) {           
        *clck -= 1;
        pSwitch_help->val = 0; //so that clock++ will be once
        Serial.println(*clck);
    }
    if (inc_dec.inc) { 
        *clck += 1;
        pSwitch_help->val = 0; //so that clock++ will be once
        Serial.println(*clck);
	}
  
    return *clck;
}

bool bool_switcher(bool flag, struct val_flag *pSwitch_help) { //for flags switch - alarm_flag
    struct flags {
    bool inc = 0;
    bool dec = 0;
    } inc_dec;
    if (pSwitch_help->val == 2 && pSwitch_help->flag == 0) {
        inc_dec.dec = 1;     
    }
    if (pSwitch_help->val == 1 && pSwitch_help->flag == 0) {
        inc_dec.inc = 1;
        pSwitch_help->val = 0; //so that clock++ will be once

    }
    if (inc_dec.dec || inc_dec.inc) {
        flag = !flag;
        pSwitch_help->val = 0; //so that clock++ will be once

    }
    return flag;
}

void setuper() {    
    switch (m_switcher) {
        case 1: //set minutes
            clock = minute; //here & below 1st string is for comfortable setting clock variables
            minute = switcher(&clock, &switch_help);
            if (minute == 255 && decrem) { //here & bellow if var switches down and draws to 0
                minute = 59;              //means top a head
            }
	          if (minute == 60 && increm) { //here & bellow if var switches up and top a head
                minute = 0;                //draws to zero
            }
            break;
        case 2: //set hours
            clock = hour;
            hour = switcher(&clock, &switch_help);
	          if (hour == 24 && increm) {
                hour = 0;
            }
	          if (hour == 255 && decrem) {
                hour = 23;
            }
            break;
        case 3: //set week_days
            clock = week_day;
            week_day = switcher(&clock, &switch_help);
	          if (week_day == 0 && decrem) {
                week_day = 7;
            }
	          if (week_day == 8 && increm) {
	          week_day = 1;
            }
            break;
        case 4: //set date's days
            clock = day;
            day = switcher(&clock, &switch_help);
	          if (day == 0 && decrem) {
                day = 31;
            }
	          if (day == 32 && increm) {
                day = 1;
            }
            break;
        case 5: //set date's months
            clock = month;
            month = switcher(&clock, &switch_help);
	          if (month == 0 && decrem) {
                month = 12;
            }
	          if (month == 13 && increm) {
                month = 1;
            }
            break;
        case 6: //set date's year
            clock = year;
            if (year == 2022 && decrem) {
              year = 2033;      
            }
            if (year == 2034 && increm) {
              year = 2023;
            }
	          year = switcher(&clock, &switch_help);
	          break;
        case 7: //set alarm_minutes
            clock = alarm_minute;
            alarm_minute = switcher(&clock, &switch_help);
            if (alarm_minute == 255 && decrem) { //here & bellow if var switches down and draws to 0
                alarm_minute = 59;              //means top a head
            }
	          if (alarm_minute == 60 && increm) { //here & bellow if var switches up and top a head
                alarm_minute = 0;                //draws to zero
            }
            break;
        case 8: //set alarm_hours
            clock = alarm_hour;
            alarm_hour = switcher(&clock, &switch_help);
	          if (alarm_hour == 24 && increm) {
                alarm_hour = 0;
            }
	          if (alarm_hour == 255 && decrem) {
                alarm_hour = 23;
            }
            break;
        case 9: //set alarm_flag
            alarm_flag = bool_switcher(alarm_flag, &switch_help);
            break;
        case 10: //settings are sets
            rtc.setTime(sec, minute, hour, day, month, year); //implict type conversation maybe
	          break;
    }
}

void input_with_first_zero(uint8_t a, uint8_t b, uint8_t c, uint8_t time) { //fix up first zero with 1-number digits
    if (time <= 9) {        
        matrix.setCursor(a, b);
        matrix.print("0");
	      matrix.setCursor(a = c + a + 2, b);
        matrix.print(time);
    }
    else {
        matrix.setCursor(a, b);
        matrix.print(time);
    }
}

void print_month_name(uint8_t month) { //shows on LED-display month name
    switch(month) {
        case 1: 
            matrix.print("JAN");
            break;
        case 2:
            matrix.print("feb");
            break;
        case 3:
            matrix.print("mar");
            break;
        case 4:
            matrix.print("apr");
            break;
        case 5:
            matrix.print("may");
            break;
        case 6:
            matrix.print("JUN");
            break;
        case 7:
            matrix.print("JUL");
            break;
        case 8:
            matrix.print("aug");
            break;
        case 9:
            matrix.print("sep");
            break;
        case 10:
            matrix.print("oct");
            break;
        case 11:
            matrix.print("nov");
            break;
        case 12:
            matrix.print("dec");
            break;
    }
}

void print_year(uint16_t year) {//shows on LED-display year
    switch(year) {
    case 2023:
        matrix.print("'23");
        break;
    case 2024:
        matrix.print("'24");
        break;
    case 2025:
        matrix.print("'25");
        break;
    case 2026:
        matrix.print("'26");
        break;
    case 2027:
        matrix.print("'27");
        break;
    case 2028:
        matrix.print("'28");
        break;
    case 2029:
        matrix.print("'29");
        break;
    case 2030:
        matrix.print("'30");
        break;
    case 2031:
        matrix.print("'31");
        break;
    case 2032:
        matrix.print("'32");
        break;
    case 2033:
        matrix.print("'33");
        break;
    default:
        matrix.print(year);
        break;
    }
}

void print_week_day(uint8_t week_day) {
    switch(week_day) {
        case 1:
            matrix.print("-MONDAY-");
            break;
        case 2:
            matrix.print("-TUESDAY-");
            break;
        case 3:
            matrix.print("-WEDNESDAY-");
            break;
        case 4:
            matrix.print("-THURSDAY-");
            break;
        case 5:
            matrix.print("-FRIDAY-");
            break;
        case 6:
            matrix.print("-SATURDAY-");
            break;
        case 7:
            matrix.print("-SUNDAY-");
            break;
     }
}

void show_week_day() { //shows week-day circles (icons) on LED-display
    uint8_t indentX, indentY, radius, between;
    indentX = 3;
    indentY = 29;
    radius = 2;
    between = 9;
    switch(week_day) {  
        case 7: 
            indentX = between*6 + 3;           
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0));
        case 6:
            indentX = between*5 + 3;
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0));  
        case 5:
            indentX = between*4 + 3;   
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0));
        case 4:
            indentX = between*3 + 3;
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0));  
        case 3:
            indentX = between*2 + 3;
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0)); 
        case 2:
            indentX = 3 + between;
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0));      
        case 1:
            indentX = 3;
            if (hour <= 7 || hour >= 21)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(0, 0, 1));
            if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21))   
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 3, 0));
            if (hour >= 12 && hour <= 17)
              matrix.fillCircle(indentX, indentY, radius, matrix.Color333(7, 0, 0)); 
            
    }
}

void matrix_color_switcher() { //switches LED-color in appliance with time
    if (hour <= 7 || hour >= 21) {
        matrix.setTextColor(matrix.Color333(0, 0, 1));
	}
	if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21)) {
		matrix.setTextColor(matrix.Color333(7, 3, 0));
	}
  if (hour >= 12 && hour <= 17) {
    matrix.setTextColor(matrix.Color333(7, 0, 0));
  }
}

void matrix_print_clock() { 
    uint8_t indentX, indentY, letter_width;
    indentX = 5;
    indentY = 3;
    letter_width = 10;
    matrix.fillScreen(0);
    matrix.setTextSize(2); //size of text output    
    input_with_first_zero(indentX, indentY, letter_width, hour);
    indentX = 2*letter_width + 6;
    matrix.setCursor(indentX, indentY);    
    matrix.print(":");
    indentX += 9;
    input_with_first_zero(indentX, indentY, letter_width, minute);    
    matrix_print_date();
    print_alarm_status(alarm_flag);
    
    matrix.swapBuffers(false); //got out data from buffer(end of matrix_print()
}

void matrix_print_date() {    
    uint8_t indentX, indentY, letter_width;
    indentX = 6;
    indentY = 19;    
    letter_width = 4;
    matrix.setTextSize(1);    
	  matrix.setCursor(indentX, indentY);
    input_with_first_zero(indentX, indentY, letter_width, day);
    indentX = 2*letter_width + 14;
    matrix.setCursor(indentX, indentY);    
    print_month_name(month); //insert month_name();
    indentX += 20;
    matrix.setCursor(indentX, indentY);
    print_year(year);
    indentX = 5;
    indentY = 24;
    matrix.setCursor(indentX, indentY);
    show_week_day();  
    //print_week_day(week_day); if want, may to unlock

}

void print_pres_n_humid() {
  uint8_t indentX, indentY, letter_width;
  uint16_t pressure, humidity;
  pressure = 0.75*climateSensor.getPressure();
  humidity = climateSensor.getRelativeHumidity();
  indentX = 3;
  indentY = 21;
  letter_width = 5;  
  matrix.setCursor(indentX, indentY);
  matrix.setTextSize(1);
  matrix.print(pressure);  
  indentX = indentX + 3*letter_width + 4;
  matrix.setCursor(indentX, indentY);
  matrix.print("mm");
  indentX += 21;
  matrix.setCursor(indentX, indentY);
  matrix.print(humidity);
  indentX += 13;
  matrix.setCursor(indentX, indentY);
  matrix.print("%");
}

void matrix_print_climate() {
    uint8_t indentX, indentY, letter_width, temperature;
    temperature = (uint8_t)climateSensor.getTemperatureCelcius() - 3;
    indentX = 3;
    indentY = 3;
    letter_width = 10;
    matrix.fillScreen(0);
    matrix.setTextSize(1);
    matrix.setCursor(indentX, indentY + 7);
    matrix.print("t");    
    matrix.setTextSize(2); //size of text output
    indentX = indentX + letter_width + 6;
    matrix.setCursor(indentX, indentY);
    input_with_first_zero(indentX, indentY, letter_width, temperature);
    indentX = indentX + 2*letter_width + 9;
    matrix.setCursor(indentX, indentY);
    matrix.print("C");
    print_pres_n_humid();    
    matrix.swapBuffers(false); //got out data from buffer(end of matrix_print()

}

void matrix_print_clock_setup(uint8_t m_switcher) {
    uint8_t indentX, indentY, letter_width;
    matrix.fillScreen(0);	        
    switch(m_switcher) {
    case 1: //set minutes	    
          matrix.fillScreen(0);    
          matrix.setTextSize(2);
          letter_width = 10;
          indentX = 2*letter_width + 6;
          indentY = 3;       
          matrix.setCursor(indentX, indentY);    
          matrix.print(":");
          indentX += 9;                      
          input_with_first_zero(indentX, indentY, letter_width, minute);          
          matrix.swapBuffers(false);
          break;
	  case 2: //set hours
            indentX = 5;
            indentY = 3;
            letter_width = 10;
            matrix.fillScreen(0);
            matrix.setTextSize(2); //size of text output    
            input_with_first_zero(indentX, indentY, letter_width, hour);
            matrix.swapBuffers(false);            
	    break;
    case 3: //set week-day
      indentX = 5;
      indentY = 24;
      matrix.fillScreen(0);
      matrix.setTextSize(1);
      matrix.setCursor(indentX, indentY);  
      print_week_day(week_day);
      matrix.swapBuffers(false); 
      break;
    case 4: //set date-day
      matrix.fillScreen(0);
	    uint8_t indentX, indentY, letter_width;
            indentX = 6;
            indentY = 22;    
            letter_width = 4;
            matrix.setTextSize(1);    
            matrix.setCursor(indentX, indentY);
            input_with_first_zero(indentX, indentY, letter_width, day);
            matrix.swapBuffers(false);
            break;             
    case 5: //set month
            matrix.fillScreen(0);
            letter_width = 4;
	          indentX = 2*letter_width + 14;
            indentY = 22;
            matrix.setTextSize(1);    
            matrix.setCursor(indentX, indentY);
            print_month_name(month);
            matrix.swapBuffers(false);
            break;
    case 6: //set year
            matrix.fillScreen(0);
            letter_width = 4;
	          indentX = 2*letter_width + 34;
            indentY = 22;
	          matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);
            print_year(year);
            matrix.swapBuffers(false);
            break;
    case 7: //set alarm_minutes
            matrix.fillScreen(0);    
            matrix.setTextSize(2);
            letter_width = 10;
            indentX = 2*letter_width + 6;
            indentY = 3;       
            matrix.setCursor(indentX, indentY);    
            matrix.print(":");
            indentX += 9;                      
            input_with_first_zero(indentX, indentY, letter_width, alarm_minute);          
            indentX = 6;
	    indentY = 22;
	    matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);
            matrix.print("alarm");
            matrix.swapBuffers(false);
	    break;
    case 8: //set alarm_hours
            indentX = 5;
            indentY = 3;
            letter_width = 10;
            matrix.fillScreen(0);
            matrix.setTextSize(2); //size of text output    
            input_with_first_zero(indentX, indentY, letter_width, alarm_hour);
            indentX = 6;
	    indentY = 22;
	    matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);
            matrix.print("alarm");
            matrix.swapBuffers(false);
            break;
    case 9: //set alarm_flag
	    if (alarm_flag) {
            indentX = 6;
	        indentY = 22;
	        matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);
            matrix.print("alarm on");
            matrix.swapBuffers(false);
            }
	    else {
            indentX = 6;
	        indentY = 22;
	        matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);
            matrix.print("alarm off");
            matrix.swapBuffers(false);
            }
            break;
    case 10: //settings are available
	    indentX = 6;
	    indentY = 22;
	    matrix.setTextSize(1);
        matrix.setCursor(indentX, indentY);
        matrix.print("SETTING");
        matrix.swapBuffers(false);
        break;            
    }
}

void alarm_tone() {	
    uint8_t pause_time = 50;
    uint8_t ring_freq = 25;
    uint32_t ring_timer;
    static uint32_t pause_timer = millis(); 	
    if (millis() - pause_timer > pause_time) {
        ring_timer = millis();        
        while (millis() - ring_timer <= ring_freq) {
            digitalWrite(alarm_pin, HIGH);
        }
        digitalWrite(alarm_pin, 0);
        pause_timer = millis();
    }

}

void alarm() { //don't work, hang
  bool start = 0;
  uint16_t tim;
  if (alarm_hour == hour && alarm_minute == minute && alarm_flag) {
    start = true;   
    tim = millis();      
    if (!decrem || !increm) {        
        start = false;        
     }   
    if (start) {
      alarm_tone();      
      }
    if (!start) {      
      alarm_flag = false;
  }     
 }
}

void clim_clock(bool increm, bool decrem) {  //"while" interrupts alarm()
    if(increm && decrem) {
    if (millis() - timer > dur_clock_display) {
      timer2 = millis();
      while (millis() - timer2 < dur_clim_display) {
        matrix_print_climate();        
      }
      matrix_print_clock();
      timer = millis();
    }
  }
}

void clim_clock2 () {  //alternative version without "while" - no interrupts for alarm()
  if (millis() - timer > dur_clim_display) {
    matrix_print_clock();    
    }    
  if(millis() - timer2 > dur_clock_display) {
    timer = millis();
    timer2 = millis();
    matrix_print_climate();    
    }
}

void setup() {
Serial.begin(9600);
pinMode(increm_pin, INPUT_PULLUP);
pinMode(decrem_pin, INPUT_PULLUP);
pinMode (alarm_pin, OUTPUT);
digitalWrite(alarm_pin, LOW);
climateSensor.begin();
matrix.begin(); //RGB-matrix is started
matrix.setTextWrap(false); //text hyphenation is off
}

void loop() {
increm = digitalRead(increm_pin);
decrem = digitalRead(decrem_pin);
 
switch_helper(increm, decrem, &switch_help);
menu_switcher(&m_switcher, &switch_help);

setuper();
  if (m_switcher) {    
	    switcher(&clock, &switch_help);            
      matrix_print_clock_setup(m_switcher);
      Serial.print(m_switcher);
      Serial.print("   ");    
      Serial.println(clock);
}
    if (!m_switcher) {
        
        clock_var_val();
        Serial.println(climateSensor.getTemperatureCelcius());
        Serial.println(climateSensor.getRelativeHumidity());
        Serial.println(0.75*climateSensor.getPressure());
        matrix_color_switcher(); //switches led-light after 21 o'clock 
        //matrix_print_climate();
        //matrix_print_clock();
        
        alarm();
        clim_clock2();
        
    }
}
