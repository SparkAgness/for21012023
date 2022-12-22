#define increm PINx
#define decrem PINy

const uint8_t item_quant = 6; // the quantity of setup menu's items
uint8_t item = 0; //item's counter (year, mm, dd etc...)
uint16_t year, week_day, month, dd, hh, minut, sec; //sync with values of  standart library

uint16_t switcher(uint16_t clock) { //item's value switcher
    println(clock);
    if (!decrem) {
        clock -= 1;
	Serial.println(clock);
    }
    if (!increm) {
        clock += 1;
	Serial.println(clock);
    }
    return clock;
}

//call(true) or call-back(false) setup menu: 
bool setup_menu(uint8_t item) {
    if ((item >= 0) && (item <= item_quant)) && ((!decrem) && (!increm))
        return true;
    if (item > item_quant) && ((!increm) && (!decrem)) {
	item = 0;
        return false;
	}
}

uint8_t item_switcher(uint8_t item) {
//switches item's value
    if ((!decrem) && (!decrem))
        item += 1;
    return item;

}
uint16_t setup_input(uint16_t clock, uint16_t (*func)(uint16_t)) { 
//the func of setup input 
//*func = switcher(clock)
    if (setup_menu(item)) {
        switch(item) {
            case 1: switcher(hh);
	    case 2: switcher(minut);
	    case 3: switcher(year);
            case 4: switcher(month);
	    case 5: switcher(week_day);
	default 
            exit(0);	    
	}
    }

}
