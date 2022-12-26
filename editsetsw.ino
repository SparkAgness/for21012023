#define increm_pin 7
#define decrem_pin 5
#define anti_bounce 500

bool increm, decrem;
uint16_t clock;
uint32_t time_enter; //always will be in global-value!!!


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
 
switcher(&clock);
}
