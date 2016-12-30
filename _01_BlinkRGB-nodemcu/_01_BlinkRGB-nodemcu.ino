/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
//15 red, 12 green ,13 blue.
int redpin = 15;
int greenpin = 12;
int bluepin = 13;

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 2; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)


void setup() {
  pinMode(redpin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    pinMode(greenpin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    pinMode(bluepin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
      Serial.begin(115200);


}

// the loop function runs over and over again forever
void loop() {

  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);

  
  digitalWrite(redpin, LOW);   // Turn the LED on (Note that LOW is the voltage level
     digitalWrite(greenpin, LOW);       
          digitalWrite(bluepin, LOW);                                   // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(500);                      // Wait for a second 
  digitalWrite(redpin, HIGH);   // Turn the LED on (Note that LOW is the voltage level
     digitalWrite(greenpin, LOW);       
          digitalWrite(bluepin, LOW);    
          delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
            digitalWrite(redpin, LOW);   // Turn the LED on (Note that LOW is the voltage level
     digitalWrite(greenpin, HIGH);       
          digitalWrite(bluepin, LOW);    
          delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
                      digitalWrite(redpin, LOW);   // Turn the LED on (Note that LOW is the voltage level
     digitalWrite(greenpin, LOW);       
          digitalWrite(bluepin, HIGH);    
          delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
}

