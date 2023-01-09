void input_with_first_zero(uint8_t a, uint8_t b, uint8_t c, uint8_t time) {
    if (time <= 9) {
        matrix.setCursor(a, b);
        matrix.print("0");
	      matrix.setCursor(a = c + a + 2, b);
        matrix.print(time);
    }
    else {
        matrix.setCursor(a, b);
        matrix.print(time);
    }
}


void matrix_print_time() {
    uint8_t indentX, indentY, letter_width;
    indentX = 5;
    indentY = 1;
    letter_width = 10;
    matrix.fillScreen(0);
    matrix.setTextSize(2); //size of text output
    matrix.setTextColor(matrix.Color333(7, 3, 0));
    input_with_first_zero(indentX, indentY, letter_width, hour);
    indentX = 2*letter_width + 6;
    matrix.setCursor(indentX, indentY);    
    matrix.print(":");
    indentX += 9;
    input_with_first_zero(indentX, indentY, letter_width, minute);    
    matrix_print_date();
    matrix.swapBuffers(true); //got out data from buffer(end of matrix_print()
}

void matrix_print_date() {    
    uint8_t indentX, indentY, letter_width;
    indentX = 6;
    indentY = 22;    
    letter_width = 4;
    matrix.setTextSize(1);    
    matrix.setCursor(indentX, indentY);
    input_with_first_zero(indentX, indentY, letter_width, day);
    indentX = 2*letter_width + 14;
    matrix.setCursor(indentX, indentY);    
    print_month_name(month); //insert month_name();
    indentX += 20;
    matrix.setCursor(indentX, indentY);
    print_year(year);
    indentX = 5;
    indentY = 24;
    matrix.setCursor(indentX, indentY);  
    //print_week_day(week_day); if want, may to unlock

void print_week_day(uint8_t week_day) {
    switch(week_day) {
        case 1:
            matrix.print("-MONDAY-");
            break;
        case 2:
            matrix.print("-TUESDAY-");
            break;
        case 3:
            matrix.print("-WEDNESDAY-");
            break;
        case 4:
            matrix.print("-THURSDAY-");
            break;
        case 5:
            matrix.print("-FRIDAY-");
            break;
        case 6:
            matrix.print("-SATURDAY-");
            break;
        case 7:
            matrix.print("-SUNDAY-");
            break;
     }
}

void matrix_print_clock_setup(uint8_t m_switcher) {
    uint8_t indentX, indentY, letter_width;
    matrix.fillScreen(0);	        
    switch(m_switcher) {
        case 1: //set minutes	        
            matrix.fillScreen(0);
            matrix.setTextSize(2);
            letter_width = 10;
            indentX = 2*letter_width + 6;
            indentY = 3;       
            matrix.setCursor(indentX, indentY);    
            matrix.print(":");
            indentX += 9;                      
            input_with_first_zero(indentX, indentY, letter_width, minute);          
            matrix.swapBuffers(false);
            break;
	case 2: //set hours
            matrix.fillScreen(0);
            indentX = 5;
            indentY = 3;
            letter_width = 10;
            matrix.fillScreen(0);
            matrix.setTextSize(2); //size of text output    
            input_with_first_zero(indentX, indentY, letter_width, hour);
            matrix.swapBuffers(false);            
            break;
        case 3: //set week-day
            matrix.fillScreen(0);
            indentX = 5;
            indentY = 24;
            matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);  
            print_week_day(week_day);
            matrix.swapBuffers(false); 
            break;
	case 4: //set date-day
            matrix.fillScreen(0);
            indentX = 6;
            indentY = 22;    
            letter_width = 4;
            matrix.setTextSize(1);    
            matrix.setCursor(indentX, indentY);
            input_with_first_zero(indentX, indentY, letter_width, day);
            matrix.swapBuffers(false);  
            break;
        case 5: //set month
            matrix.fillScreen(0);
            letter_width = 4;
	    indentX = 2*letter_width + 14;
            indentY = 22;
            matrix.setTextSize(1);    
            matrix.setCursor(indentX, indentY);
            print_month_name(month);
            matrix.swapBuffers(false);
            break;
       case 6: //set year
            matrix.fillScreen(0);
            letter_width = 4;
	    indentX = 2*letter_width + 34;
            indentY = 22;
	    matrix.setTextSize(1);
            matrix.setCursor(indentX, indentY);
            print_year(year);
            matrix.swapBuffers(false);
            break;
        }

}

void matrix_color_switcher() {
    if (hour <= 7 || hour >= 21) {
        matrix.setTextColor(matrix.Color333(0, 0, 7));
    }
    if (hour > 7 && hour < 21) {
        matrix.setTextColor(matrix.Color333(7, 3, 0));
    }
}

void month_name(uint8_t month) {
    switch(month) {
        case 1: 
            matrix.print("jan");
            break;
        case 2:
            matrix.print("feb");
            break;
        case 3:
            matrix.print("mar");
            break;
        case 4:
            matrix.print("apr");
            break;
        case 5:
            matrix.print("may");
            break;
        case 6:
            matrix.print("jun");
            break;
        case 7:
            matrix.print("jul");
            break;
        case 8:
            matrix.print("aug");
            break;
        case 9:
            matrix.print("sep");
            break;
        case 10:
            matrix.print("oct");
            break;
        case 11:
            matrix.print("nov");
            break;
        case 12:
            matrix.print("dec");
            break;
    }
}

void print_year(uint16_t year) {
    switch(year) {
    case 2023:
        matrix.print("'23");
        break;
    case 2024:
        matrix.print("'24");
        break;
    case 2025:
        matrix.print("'25");
        break;
    case 2026:
        matrix.print("'26");
        break;
    case 2027:
        matrix.print("'27");
        break;
    case 2028:
        matrix.print("'28");
        break;
    case 2029:
        matrix.print("'29");
        break;
    case 2030:
        matrix.print("'30");
        break;
    case 2031:
        matrix.print("'31");
        break;
    case 2032:
        matrix.print("'32");
        break;
    case 2033:
        matrix.print("'33");
        break;
    default:
        matrix.print(month);
        break;
    }
}
