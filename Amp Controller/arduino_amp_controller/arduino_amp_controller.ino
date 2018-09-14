/* 
*  Switches amp between RX and TX
*
*  Author: Tane Tatum
*  Updated: 9-13-18
*/

#define switchPin 13  // Pin number for switchPin

void setup() {
  digitalWrite(switchPin, HIGH);  // Preset switchpin to RX
  pinMode(switchPin, OUTPUT);   // Set switchPin to output
  
  Serial.begin(9600);
  Serial.println("Arduino Conncected");
}

void loop() {
  while(Serial.available()){
    int input = Serial.read();
    input = input - 48;
    switch (input) {
      case 0:
        // Set to mode to TX
        digitalWrite(switchPin, LOW);
        Serial.println("Amp set to TX");
      break;
      case 1:
        // Set mode to RX
        digitalWrite(switchPin, HIGH);
        Serial.println("Amp set to RX");
      break;
      default:
        Serial.println("Invalid input");
      break;
    } 
  }
  delay(1);
}
