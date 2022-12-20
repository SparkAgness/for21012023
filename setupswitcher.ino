#define increm PINx
#define decrem PINy

const uint8_t item_quant = 6; // the quantity of setup menu's items
uint8_t item = 0; //item's counter
uint16_t year, mm, dd, hh, minut, sec;

uint16_t switcher(uint16_t clock) { //item's value switcher
    println(clock);
    if (!decrem) {
        clock -= 1;
	println(clock);
    }
    if (!increm) {
        clock += 1;
	println(clock);
    }
    return clock;
}

//call(true) or call-back(false) setup menu: 
bool setup_menu(uint8_t item) {
    if ((item >= 0) && (item <= item_quant)) && ((!decrem) && (!increm))
        return true;
    if (item > item_quant) && ((!increm) && (!decrem))
        return false;
}

uint16_t setup_input(uint16_t clock, *func()) { // to learn how to refer func to func
    
}
