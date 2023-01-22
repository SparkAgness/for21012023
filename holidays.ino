enum events {WeddingDay, SashaEfremovBD, IlyaEfremovBD, YouryEfremovBD, KravtsovBD, PetrovichBD, BiberinBD, DumanskiBD, ElenaEfremovaBD} holidays; //for holiday_label() switch
bool holiday_flag;

struct holiday_date {
    enum events holidays;
    uint8_t date;
    uint8_t month;
};

holiday_date wedding_day, Sasha_Efremov_BD, Ilya_Efremov_BD, Youry_Efremov_BD, Kravtsov_BD, Petrovich_BD, Biberin_BD, Dumanski_BD, Elena_Efremova_BD;

holiday_date dates[9]; //if holiday_dates will be added, inevitably increase index [] value

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

void holiday_label(holiday_date active) { //outputs im LED-display congratulation label
    uint8_t indentX, indentY, width, hight; //add condition to outputing label every hour
    indentX = 3;
    indentY = 3;
    width = 5;
    hight = 9;
    matrix.setTextSize(1);
    switch(enum events holidays) {
        case WeddingDay:
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
        case KravtsovBD:
            break;
        case PetrovichBD:
            break;
        case BiberinBD:
            break;
        case DumanskiBD:
            break;
        case ElenaEfremovaBD:
            break;
    }
}

void matrix_holiday_print() { //outputs LED-display content whole
    matrix.fillScreen(0);
    matrix_color_switcher();
    holiday_label();  
    matrix_print_date();
    matrix.swapBuffers(false);
}

holiday_date coincidence(holiday_date *m, int n) { //checks coincidences of dates with holidays, n = sizeof(dates)/sizeof(holiday_date)
    for(int i = 0; i < n; i++) {
        if (m[i].date == 20 && m[i].month == 3) {
            holiday_flag = true;           
            return m[i];
        }    
    }
}

bool holiday_flagger(bool holiday_flag) {
    if ((day == wedding_day->date) && (month == wedding_day->month)) {
        holiday_flag = true; 
    }
}
