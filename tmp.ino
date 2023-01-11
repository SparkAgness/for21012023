#include <forcedClimate.h>
#include <microDS3231.h>
#include <RGBmatrixPanel.h>

#define alarm_pin 3
#define increm_pin 7
#define decrem_pin 5
#define clock_clim 1000
#define anti_bounce 500
#define switch_num_max 8

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

bool increm, decrem, cc_flag;
uint16_t clock;
uint32_t time_enter; //always will be in global-value!!!
uint32_t timer, timer2;
uint8_t sec, minute, hour, week_day, day, month;
uint16_t year;
uint8_t m_switcher;
enum {dur_clim_display = 2000, dur_clock_display = 4000}; //durations of matrix_print_climate() and matrix_print_clock()

struct val_flag {    //will be used with switch_helper 
    uint8_t val;     //will return value of increm/decrem/menu_switcher mode
    bool flag;       //will return value of switcher's flag
} switch_help;

struct val_flag *pSwitch_help = &switch_help;

void clim_clock(bool increm, bool decrem) {  //don't work as it need's
    if(increm && decrem) {
    if (millis() - timer > dur_clock_display) {
      timer2 = millis();
      while(millis() - timer2 < dur_clim_display) {
        matrix_print_climate();        
      }
      matrix_print_clock();
      timer = millis();
    }
  }
}

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

void setuper() {    
    switch (m_switcher) {
        case 1: 
            clock = minute; //here & below 1st string is for comfortable setting clock variables
            minute = switcher(&clock, &switch_help);
            if (minute == 255 && decrem) { //here & bellow if var switches down and draws to 0
                minute = 59;              //means top a head
            }
	          if (minute == 60 && increm) { //here & bellow if var switches up and top a head
                minute = 0;                //draws to zero
            }
            break;
        case 2:
            clock = hour;
            hour = switcher(&clock, &switch_help);
	    if (hour == 24 && increm) {
                hour = 0;
            }
	    if (hour == 255 && decrem) {
                hour = 23;
            }
            break;
        case 3:
            clock = week_day;
            week_day = switcher(&clock, &switch_help);
	    if (week_day == 0 && decrem) {
                week_day = 7;
            }
	    if (week_day == 8 && increm) {
	        week_day = 1;
            }
            break;
        case 4:
            clock = day;
            day = switcher(&clock, &switch_help);
	    if (day == 0 && decrem) {
                day = 31;
            }
	    if (day == 32 && increm) {
                day = 1;
            }
            break;
        case 5:
            clock = month;
            month = switcher(&clock, &switch_help);
	    if (month == 0 && decrem) {
                month = 12;
            }
	    if (month == 13 && increm) {
                month = 1;
            }
            break;
        case 6:
            clock = year;
            if (year == 2022 && decrem) {
              year = 2033;      
            }
            if (year == 2034 && increm) {
              year = 2023;
            }
	    year = switcher(&clock, &switch_help);
	    break;
        case 7:
            rtc.setTime(sec, minute, hour, day, month, year); //implict type conversation maybe
	    break;
    }
}

void input_with_first_zero(uint8_t a, uint8_t b, uint8_t c, uint8_t time) {
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

void print_month_name(uint8_t month) {
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

void print_year(uint16_t year) {
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
        matrix.print(month);
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

void matrix_color_switcher() {
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
    
    matrix.swapBuffers(false); //got out data from buffer(end of matrix_print()
}

void matrix_print_date() {    
    uint8_t indentX, indentY, letter_width;
    indentX = 6;
    indentY = 22;    
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
        
        clim_clock(increm, decrem);
        
    }
}
