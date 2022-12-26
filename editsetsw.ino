#define increm_pin 7
#define decrem_pin 5
#define anti_bounce 500

#include <microDS3231.h>
MicroDS3231 rtc;

uint8_t menu_switcher();

bool increm, decrem;
uint16_t clock;
uint32_t time_enter; //always will be in global-value!!!
uint8_t sec, minute, hour, week_day, day, month;
uint16_t year;
uint8_t m_switcher;

uint8_t menu_switcher(uint8_t *m_switcher) { //switch to setup clock menu
    
    if (!increm && !decrem && millis() - time_enter > anti_bounce) {
	      *m_switcher += 1;
        time_enter = millis();
        if (*m_switcher == 6) {
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

void setup() {
Serial.begin(9600);
pinMode(increm_pin, INPUT_PULLUP);
pinMode(decrem_pin, INPUT_PULLUP);
}

void loop() {
increm = digitalRead(increm_pin);
decrem = digitalRead(decrem_pin);
 
//switcher(&clock);
menu_switcher(&m_switcher);
Serial.println(m_switcher);
if (!m_switcher) {
  Serial.println("False!");
}
delay(1000);
}
