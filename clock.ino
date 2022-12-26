#include <microDS3231.h>
MicroDS3231 rtc

uint8_t sec, minute, hour, week_day, day, month;
uint16_t year;

uint8_t menu_switcher() {
    uint8_t menu_switcher = 0;
    if (!increm && !decrem && millis() - time_enter > anti_bounce) {
        time_enter = millis();
    }
}
