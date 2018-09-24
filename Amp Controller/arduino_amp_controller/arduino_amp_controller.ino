/* 
*  Switches amp between RX and TX
*
*  Author: Tane Tatum
*  Updated: 9-13-18
*/

#define switchPin 13  // Pin number for TX/RX switch      (5V/Float = RX, 0V = TX) 
#define tempPin 12    // Pin number for temperature flag  (5V = Normal, 0V = Over Temp)

bool overTemp = true;

void setup() {
  digitalWrite(switchPin, HIGH);  // Preset switchpin to RX
  pinMode(switchPin, OUTPUT);     // Set switchPin to output
  pinMode(tempPin, INPUT_PULLUP); // Set tempPin to input
  
  Serial.begin(9600);
  Serial.println("Arduino Conncected");
}

void loop() {

  overTemp = !digitalRead(tempPin);   // Check for over temp flag

  // If overheated, lock into RX
  if(overTemp){
    digitalWrite(switchPin, HIGH);
    Serial.println("Amp Overheated! Locked in RX until cooldown.");
    delay(3000);
  }

  else{
    // Respond to serial commands
    while(Serial.available()){
      int input = Serial.read();
      input = input - 48;         // Switch input from ASCII to value
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
  }
  delay(1);
}
