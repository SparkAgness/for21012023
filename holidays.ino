struct holiday_date {
    uint8_t date;
    uint8_t month;
};

holiday_date wedding_day, Sasha_Efremov_BD, Ilya_Efremov_BD, Youry_Efremov_BD, Kravtsov_BD, Petrovich_BD, Biberin_BD, Dumanski_BD;

holiday_date dates[] = {wedding_day, Sasha_Efremov_BD, Ilya_Efremov_BD, Youry_Efremov_BD, Kravtsov_BD, Petrovich_BD, Biberin_BD, Dumanski_BD};

//That must be placed in setup()
wedding_day.date = 8;
wedding_day.month = 3;
Sasha_Efremov_BD = 20;
Sasha_Efremov_BD = 10;
Ilya_Efremov_BD.date = 7;
Ilya_Efremov_BD.month = 12;
Youry_Efremov_BD.date = 1;
Youry_Efremov_BD.month = 6;
Kravtsov_BD.date = 24;
Kravtsov_BD.month = 12;
Petrovich_BD.date = 20;
Petrovich_BD.month = 3;
Biberin_BD.date = 28;
Biberin_BD.month = 3;
Dumanski_BD.date = 23;
Dumanski_BD.month = 5;


enum events {WeddingDay, SashaEfremovBD, IlyaEfremovBD, YouryEfremovBD, KravtsovBD, PetrovichBD, BiberinBD, DumanskiBD} holidays; //for holiday_label() switch

void holiday_label() { //outputs im LED-display congratulation label
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
    }
}

void matrix_holiday_print() { //outputs LED-display content whole
    matrix.fillScreen(0);
    matrix_color_switcher();
    holiday_label();  
    matrix_print_date();
    matrix.swapBuffers(false);
}

bool holiday_flagger(bool holiday_flag) {
    if ((day == wedding_day->date) && (month == wedding_day->month)) {
        holiday_flag = true; 
    }
}
