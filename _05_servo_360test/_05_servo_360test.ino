//测试8266和改造后可以连续转动的舵机的控制问题，发现只有设置pinMode 为input才能使舵机停止。

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
int inByte = 0;         // incoming serial byte

void setup() {
    //myservo.attach(4);  // attaches the servo on GIO2 to the servo object 
    
    digitalWrite(4, LOW);
    Serial.begin(9600);
       while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
   }
    Serial.print("Connecting to ");
}

 void openlock(){
  pinMode(4, OUTPUT);
  myservo.attach(4);
  myservo.write(120);
  delay(2000);
  //digitalWrite(4, LOW);  
 }
void loop() {
    inByte = Serial.read();
    if (inByte == 'A') {
       openlock();
    }
    if(inByte == 'B'){
      pinMode(4, INPUT);
      //digitalWrite(4, LOW);  
      }
  
}

