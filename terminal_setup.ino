uint16_t year;
uint16_t hour, minute, day, month;
uint32_t time, time2;
String tmp;


uint8_t fill_from_Serial(uint16_t *data) {  //fills datas from serial port to *variables
    if (Serial.available()) {
        uint16_t tmp = Serial.parseInt();
        if (tmp > 0) {
            *data = tmp;
            return -1;
        } 
    } 
}

void setting_terminal() {    
    if (Serial.available()) {
        tmp = Serial.readString();
    }
    if (tmp == "hour\n") {
        while (!Serial.available()) {
            Serial.println("Input hours");
        }
        if (fill_from_Serial(&hour) < 0) {
            tmp.remove(0);
        }
    }
    if (tmp == "minute\n") { //terminal setup minutes
	while (!Serial.available()) {
            Serial.println("Input minutes");
	}
        if (fill_from_Serial(&minute) < 0) {
            tmp.remove(0);
        }
    }
    if (tmp == "day\n") { // terminal setup days
        while(!Serial.available()) {
            Serial.println("Input day");
        }
        if (fill_from_Serial(&day) < 0) {
            tmp.remove(0);
        }
    }
    if (tmp == "month\n") { // terminal setup months
        while(!Serial.available()) {
            Serial.println("Input month");
        }
        if (fill_from_Serial(&month) < 0) {
            tmp.remove(0);
        }
    }
    if (tmp == "year\n") { //terminal setup years
        while(!Serial.available()) {
            Serial.println("Input year");
        }
        if (fill_from_Serial(&year) < 0) {
            tmp.remove(0);
        }
    }
    
    if (millis() - time2 > 3000) { // debugging writing 
        Serial.print("Time: " + String(hour) + ":" + String(minute) + "\t");
        Serial.println("Date: " + String(day) + "." + String(month) + "." + String(year));
        time2 = millis();
    }
}
