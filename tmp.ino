#define increm_pin 7
#define decrem_pin 5
#define anti_bounce_switch_helper 1000
#define anti_bounce 500
#define switch_num_max 7

#include <microDS3231.h>
#include <forcedClimate.h>

MicroDS3231 rtc;
ForcedClimate climateSensor = ForcedClimate();

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
  uint16_t *clck;
  clck = &clock;  
    switch (m_switcher) {
        case 1: 
            clock = minute; //here & below 1st string is for comfortable setting clock variables
            minute = switcher(&clock, &switch_help);
            if (!decrem && minute == 0) {
              minute = 59;
              }
            if (!increm && minute == 59) {
              minute = 0;
            }
                                
            break;
        case 2:
            clock = hour;
            hour = switcher(&clock, &switch_help);
            break;
        case 3:
            clock = week_day;
            week_day = switcher(&clock, &switch_help);
            break;
        case 4:
            clock = day;
            day = switcher(&clock, &switch_help);
            break;
        case 5:
            clock = month;
            month = switcher(&clock, &switch_help);
            break;
        case 6:
            rtc.setTime(sec, minute, hour, day, month, year); 
	    break;
    }
}

void setup() {
Serial.begin(9600);
pinMode(increm_pin, INPUT_PULLUP);
pinMode(decrem_pin, INPUT_PULLUP);
climateSensor.begin();
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
    Serial.print(m_switcher);
    Serial.print(" clock: ");
    Serial.println(clock);
    Serial.print(" The minutes is: ");
    Serial.println(minute);
    Serial.print(" The hours is: ");
    Serial.println(hour);
    Serial.print(" The date is: ");
    Serial.println(day);
    Serial.print(" The month is: ");
    Serial.println(month);    
    }
if (!m_switcher) {
Serial.println(rtc.getTimeString());
Serial.println(rtc.getDateString());
clock_var_val();
Serial.println(climateSensor.getTemperatureCelcius());
Serial.println(climateSensor.getRelativeHumidity());
 Serial.println(0.75*climateSensor.getPressure());
 delay(1000);
}
}
