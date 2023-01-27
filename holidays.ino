enum events {WeddingDay, SashaEfremovBD, IlyaEfremovBD, YouryEfremovBD, KravtsovBD, PetrovichBD, BiberinBD, DumanskiBD, ElenaEfremovaBD, NewYear, Christmas, DefDay, VictoryDay, BastiliaDay, preNewYear, FrontierGuard, FoolDay} holidays; //for holiday_label() switch

bool holiday_flag;

struct holiday_date {
    enum events holidays;
    uint8_t date;
    uint8_t month;
};

holiday_date wedding_day, Sasha_Efremov_BD, Ilya_Efremov_BD, Youry_Efremov_BD, Kravtsov_BD, Petrovich_BD, Biberin_BD, Dumanski_BD, Elena_Efremova_BD, New_Year, Christmas_, Def_Day, Victory_Day, Bastilia_Day, pre_NewYear, Frontier_Guard, Fool_Day;

holiday_date dates[17]; //if holiday_dates will be added, inevitably increase index [] value

//That must be placed in void setup()
wedding_day.holidays = WeddingDay;
wedding_day.date = 8;
wedding_day.month = 3;
dates[0] = wedding_day;
Sasha_Efremov_BD.holidays = SashaEfremovBD;
Sasha_Efremov_BD.date = 20;
Sasha_Efremov_BD.month = 10;
dates[1] = Sasha_Efremov_BD;
Ilya_Efremov_BD.holidays = IlyaEfremovBD;
Ilya_Efremov_BD.date = 7;
Ilya_Efremov_BD.month = 12;
dates[2] = Ilya_Efremov_BD;
Youry_Efremov_BD.holidays = YouryEfremovBD;
Youry_Efremov_BD.date = 1;
Youry_Efremov_BD.month = 6;
dates[3] = Youry_Efremov_BD;
Kravtsov_BD.holidays = KravtsovBD;
Kravtsov_BD.date = 24;
Kravtsov_BD.month = 12;
dates[4] = Kravtsov_BD;
Petrovich_BD.holidays = PetrovichBD;
Petrovich_BD.date = 20;
Petrovich_BD.month = 3;
dates[5] = Petrovich_BD;
Biberin_BD.holidays = BiberinBD;
Biberin_BD.date = 28;
Biberin_BD.month = 3;
dates[6] = Biberin_BD;
Dumanski_BD.holidays = DumanskiBD;
Dumanski_BD.date = 23;
Dumanski_BD.month = 5;
dates[7] = Dumanski_BD;
Elena_Efremova_BD.holidays = ElenaEfremovaBD;
Elena_Efremova_BD.date = 21;
Elena_Efremova_BD.month = 1;
dates[8] = Elena_Efremova_BD;
New_Year.holidays = NewYear;
New_Year.date = 1;
New_Year.month = 1;
dates[9] = New_Year;
Christmas_.holidays = Christmas;
Christmas.date = 7;
Christmas.month = 1;
dates[10] = Christmas_;
Def_Day.holidays = DefDay;
Def_Day.date = 23;
Def_Day.month = 2;
dates[11] = Def_Day;
Victory_Day.holidays = VictoryDay;
Victory_Day.date = 9;
Victory_Day.month = 5;
dates[12] = Victory_Day;
Bastilia_Day.holidays = BastiliaDay;
Bastilia_Day.date = 14;
Bastilia_Day.month = 7;
dates[13] = Bastilia_Day;
pre_NewYear.holidays = pre_NewYear;
pre_NewYear.date = 31;
pre_NewYear.month = 12;
dates[14] = pre_NewYear;
Frontier_Guard.holidays = FrontierGuard;
Frontier_Guard.date = 28;
Frontier_Guard.month = 5;
dates[15] = Frontier_Guard;
Fool_Day.holidays = FoolDay;
Fool_Day.date = 1;
Fool_Day.month = 4;
dates[16] = Fool_Day;

void idiotism(enum FD jokes) { //for FoolDay
    enum FD {buratino, kolobok, spina, posmotri} jokes; //replace to global partition
    switch(jokes) {
        case buratino:
            indentX = 7;
	    matrix.setCursor(indentX, indentY);
            matrix.print("BURATINO");
            indentX = 13;
            indentY += hight;
            matrix.setCursor(indentX, inentY);
            matrix.print("UTONUL");
            break;
        case kolobok:
            indentX = 10;
	    matrix.setCursor(indentX, indentY);
            matrix.print("KOLOBOK");
            indentY += hight;
            indentX = 4;
            matrix.setCursor(indentX, indentY);
            matrix.print("POVESILSA");
        break;
        case spina:
            indentX = 1;
	    matrix.setCursor(indentX, indentY);
            matrix.print("VASHA SPINA");
            indentX = 10;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("BELAYA");
        break;
        case posmotri:
            indentX = 1;
            matrix.setCursor(indentX, indentY);
            matrix.print("POSMOTRI");
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("POD NOGI");
        break;
    }
} 

void holiday_label(holiday_date active_event) { //outputs LED-display congratulation label
    uint8_t indentX, indentY, width, hight; //add condition to outputing label every hour
    indentX = 3;
    indentY = 1;
    width = 5;
    hight = 9;
    matrix.setTextSize(1);
    switch(active_event.holidays) { //enum events holidays
        case WeddingDay: //TESTED
            matrix.setCursor(indentX, indentY);
            matrix.print("LENA +");
            indentX += width;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("SASHA!!!");
            break;
        case SashaEfremovBD:
            break;
        case IlyaEfremovBD:
            break;
        case YouryEfremovBD:
            break;
        case KravtsovBD: //TESTED
            matrix.setCursor(indentX, indentY);
            matrix.print("KRAVTSOV");
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("BIRTHDAY!");
	          break;
        case PetrovichBD: //TESTED
            matrix.setCursor(indentX, indentY);
            matrix.print("PETROVICH");
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("BIRTHDAY!");
            break;
        case BiberinBD: //TESTED
            indentX = 9;
            matrix.setCursor(indentX, indentY);
            matrix.print("BIBERIN");
            indentX = 3;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("BIRTHDAY!");
            break;
        case DumanskiBD: //TESTED
            matrix.setCursor(indentX, indentY);
            matrix.print("DUMANSKI");
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("BIRTHDAY!");
            break;
        case ElenaEfremovaBD:
            break;
        case NewYear: //TESTED
            indentX = 16; 
            matrix.setCursor(indentX, indentY);
            matrix.print("HAPPY");
            indentX = 9;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("NEW YEAR!!!");
            break;
        case Christmas: //TESTED
            indentX = 16; 
            matrix.setCursor(indentX, indentY);
            matrix.print("HAPPY");
            indentX = 3;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("CHRISTMAS!");
            break;
        case DefDay: //TESTED
            indentX = 3;
            matrix.setCursor(indentX, indentY);
            matrix.print("POZDRAV 23");
            indentY += hight;
            indentX = 6;
            matrix.setCursor(indentX, indentY);
            matrix.print("FEVRAL!!!");
            break;
        case VictoryDay: //TESTED
            indentX = 25;
            matrix.setCursor(indentX, indentY);
            matrix.print("DEN");
            indentY += hight;
            indentX = 7;
            matrix.setCursor(indentX, indentY);
            matrix.print("POBEDY!!!");
            break;
        case BastiliaDay: //TESTED
            indentX = 7;
            matrix.setCursor(indentX, indentY);
            matrix.print("BASTILIA");
            indentX = 3;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("FALLS DAY!");
            break;
        case preNewYear:
            indentX = 8;
            matrix.setCursor(indentX, indentY);
            matrix.print("TIME TO");
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("DRINK???");
            break;
        case FrontierGuard:
            indentX = 16;
            matrix.setCursor(indentX, indentY);
            matrix.print("POGRAN");
            indentX = 10;
            indentY += hight;
            matrix.setCursor(indentX, indentY);
            matrix.print("VOISKA!!!");
            break;
        case FoolDay:
            break;
    }
}

holiday_date coincindence(holiday_date *m, int n) { //checks coincidences of dates with holidays, n = sizeof(dates)/sizeof(holiday_date)
    for(int i = 0; i < n; i++) {
        if (m[i].date == day && m[i].month == month) {
            holiday_flag = true;           
            return m[i];
        }    
    }
}



void show_h(uint8_t x, uint8_t y, uint8_t R, uint8_t G, uint8_t B) { //draws "h" isn't used in during version
    uint8_t max_hight = 5;
    uint8_t max_width = 3;
    for(y; y <= max_hight; y++) {
        matrix.drawPixel(x, y, matrix.Color333(R, G, B));
    } 
    x += 1;
    y += 2;
    matrix.drawPixel(x, y, matrix.Color333(R, G, B));
    x += 1;
    y += 1;
    for (y; y <= max_hight - 3; y++) {
        matrix.drawPixel(x, y, matrix.Color333(R, G, B));
    }
    
}

void show_holiday_flag() { //shows "h" in right-up corner = isn't used in during version
    uint8_t indentX, indentY, R, G, B;
    indentX = 62;
    indentY = 1;
    if (hour <= 7 || hour >= 21) {
        R = 0;
        G = 0;
        B = 1;
    }
    if ((hour > 7 && hour < 12) || (hour > 17 && hour < 21)) {
        R = 7;
        G = 3;
        B = 0;
    }
    if (hour >= 12 && hour <= 17) {
        R = 7;
	G = 0;
        B = 0;
    }
    show_h(indentX, indentY, R, G, B);
}

void matrix_holiday_print(holiday_date active_event) { //outputs LED-display content whole
    matrix.fillScreen(0);
    matrix_color_switcher();
    holiday_label(active_event);  
    matrix_print_date();
    matrix.swapBuffers(false);
}

bool stop_flag(bool holiday_flag) { //tested!!!  
    uint32_t del;
    if (!increm || !decrem) {
        stp_flag = true;
        del = millis();        
    }
    if (stp_flag && millis() - del > 10000) { //because it, stp_flag is always false
        stp_flag = false;
        del = millis();                                  
    } 
  return stp_flag;  
}

void call_MHP() { //THE MAIN FUNCTION - calls matrix_holiday_print() in depend of date and time - TESTED
    holiday_date active_event;
    uint16_t duration = 60000; //time of matrix_holiday_print() on LED display
    uint32_t time;
    stp_flag = stop_flag(holiday_flag);     
    active_event = coincindence(dates, sizeof(dates)/sizeof(holiday_date)); 
    if (holiday_flag && (minute == 1 || minute == 31)) {
        time = millis();
        while (millis() - time <= duration && !stp_flag) {
            matrix_holiday_print(active_event);            
            if (!digitalRead(increm_pin) || !digitalRead(decrem_pin)) {
              stp_flag = true;
            }
          }
    }           
}


