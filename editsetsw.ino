#define increm_pin 7
#define decrem_pin 5
#define anti_bounce 500
#define switch_num_max 7

#include <microDS3231.h>
MicroDS3231 rtc;

uint8_t menu_switcher(); //declaration of func (see below)

bool increm, decrem, switch_flag;
uint16_t clock;
uint32_t time_enter; //always will be in global-value!!!
uint8_t sec, minute, hour, week_day, day, month;
uint16_t year;
uint8_t m_switcher;

uint8_t menu_switcher(uint8_t *m_switcher) { //switch to setup clock menu
    
    if (!increm && !decrem && millis() - time_enter > anti_bounce) {
	    *m_switcher += 1;
        time_enter = millis();
        if (*m_switcher == switch_num_max) {
            *m_switcher = 0;
	}
    }
    return *m_switcher;
}


uint16_t switcher(uint16_t *clck) { 
    clck = &clock; //it's necessary to clock be changed from this func
    if (!decrem && millis() - time_enter > anti_bounce) {           
        *clck -= 1;
	    Serial.println(*clck);
        time_enter = millis();
	    
    }
    if (!increm && millis() - time_enter > anti_bounce) { 
        *clck += 1;
        Serial.println(*clck);
        time_enter = millis();      
	}
  
    return *clck;
}

void setuper() {
    switch (m_switcher) {
        case 1: 
            clock = minute; //here & below 1st string is for comfortable setting clock variables
            minute = switcher(&clock);
            break;
        case 2:
            clock = hour;
            hour = switcher(&clock);
            break;
        case 3:
            clock = year;
            week_day = switcher(&clock);
            break;
        case 4:
            clock = day;
            day = switcher(&clock);
            break;
        case 5:
            clock = month;
            month = switcher(&clock);
            break;
        case 6:
	        rtc.setTime(sec, minute, hour, day, month, year); 
    }
}

void setup() {
Serial.begin(9600);
pinMode(increm_pin, INPUT_PULLUP);
pinMode(decrem_pin, INPUT_PULLUP);
}

void loop() {
increm = digitalRead(increm_pin);
decrem = digitalRead(decrem_pin);
 
menu_switcher(&m_switcher);
    if (m_switcher) {
	Serial.println("Attention, please! Setting true time is going...");
        switcher(&clock);
    }
}
