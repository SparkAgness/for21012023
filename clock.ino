#include <microDS3231.h>
MicroDS3231 rtc

uint8_t sec, minute, hour, week_day, day, month;
uint16_t year;

uint8_t menu_switcher() { //switch to setup clock menu
    uint8_t menu_switcher = 0;
    if (!increm && !decrem && millis() - time_enter > anti_bounce) {
	menu_switcher += 1;
        time_enter = millis();
        if (menu_switcher == 6) {
            menu_switcher = 0;
	}
    }
    return menu_switcher;
}
