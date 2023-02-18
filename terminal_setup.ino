
void setup() {
    Serial.begin(9600);
    Serial.setTimeout(100);
    
    uint8_t date, hour, minute, month;

void loop() {
    if (Serial.available()) {
       String data = Serial.readString(); 
       if data = "date" {
           date = Serial.parseInt();
       }
    }
    Serial.println(date);
}
