



enum {nothing = 0, minutes, hours, week_days, days, months, clocks, alarm_minutes, alarm_hours, alarm_flags, setups, ends} setvalue;
bool alarm_flag;
uint8_t alarm_hour, alarm_minute;
void alarm_tone() {
    uint8_t pause_time = 100;
    uint8_t ring_freq = 50;
    uint32_t ring_timer;
    uint32_t pause_timer = millis(); 
    if (millis() - pause_timer > tact_time) {
        ring_timer = millis();
        while(millis() - ring_timer <= ring_freq) {
            digitalWrite(alarm_pin, HIGH);
        }
        pause_timer = millis();
    }
}
void alarm() {
if (ring_minutes == minute && ring_hours == hours && ring_flag && increm && decrem) {
    while(increm && decrem) { 
        alarm_tone();
        }
	if(!decrem || !increm) {
		break; // if it isn't exit from alarm(), try to use goto
	}
    }
}
