void print_climate_values() {
    uint8_t indentX, indentY, letter_width, temperature;
    temperature = (uint8_t)climateSensor.getTemperatureCelcius();
    indentX = 3;
    indentY = 7;
    letter_width = 10;
    matrix.fillScreen(0);
    matrix.setTextSize(2); //size of text output
    matrix.setTextColor(matrix.Color333(7, 3, 0));
    input_with_first_zero(indentX, indentY, letter_width, temperature);

}

void print_celcius() { //prints C
    
}
