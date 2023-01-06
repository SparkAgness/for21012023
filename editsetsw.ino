#include <forcedClimate.h>
#include <microDS3231.h>
#include <RGBmatrixPanel.h>

#define increm_pin 7
#define decrem_pin 5
#define anti_bounce_switch_helper 1000
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

bool increm, decrem;
uint16_t clock;
uint32_t time_enter; //always will be in global-value!!!
uint8_t sec, minute, hour, week_day, day, month;
uint16_t year;
uint8_t m_switcher;

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
    year = 2023;
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
    clck = &clock; //it's necessary to clock be changed from this func
    if (pSwitch_help->val == 2 && pSwitch_help->flag == 0) {           
        *clck -= 1;
        pSwitch_help->val = 0; //so that clock++ will be once
        Serial.println(*clck);
    }
    if (pSwitch_help->val == 1 && pSwitch_help->flag == 0) { 
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
            if (!decrem && minute == 0) { //here & bellow if var switches down and draws to 0
                minute = 59;              //means top a head
            }
	    if (!increm && minute == 59) { //here & bellow if var switches up and top a head
                minute = 0;                //draws to zero
            }
            break;
        case 2:
            clock = hour;
            hour = switcher(&clock, &switch_help);
	    if (!decrem && hour == 0) {
                hour = 12;
            }
	    if (!increm && hour == 12) {
                hour = 0;
            }
            break;
        case 3:
            clock = week_day;
            week_day = switcher(&clock, &switch_help);
	    if (!decrem && week_day == 1) {
                week_day = 7;
            }
	    if (!increm && week_day == 7) {
	        week_day = 1;
            }
            break;
        case 4:
            clock = day;
            day = switcher(&clock, &switch_help);
	    if (!decrem && day == 1) {
                day = 31;
            }
	    if (!increm && day == 31) {
                day = 1;
            }
            break;
        case 5:
            clock = month;
            month = switcher(&clock, &switch_help);
	    if (!decrem && month == 1) {
                month = 12;
            }
	    if (!increm && month == 12) {
                month = 1;
            }
            break;
        case 6:
            clock = year;
	    year = switcher(&clock, &switch_help);
	    break;
        case 7:
            rtc.setTime(sec, minute, hour, day, month, year); //implict type conversation maybe
	    break;
    }
}

void setup() {
Serial.begin(9600);
pinMode(increm_pin, INPUT_PULLUP);
pinMode(decrem_pin, INPUT_PULLUP);
climateSensor.begin();
matrix.begin(); //RGB-matrix is started
}

void loop() {
increm = digitalRead(increm_pin);
decrem = digitalRead(decrem_pin);
 
switch_helper(increm, decrem, &switch_help);
menu_switcher(&m_switcher, &switch_help);
setuper();
    if (m_switcher) {
	Serial.println("Attention, please! Setting true time is going...");
        switcher(&clock, &switch_help);
    }
    if (!m_switcher) {
        clock_var_val();
        Serial.println(climateSensor.getTemperatureCelcius());
        Serial.println(climateSensor.getRelativeHumidity());
        Serial.println(0.75*climateSensor.getPressure());
 
    }
}
