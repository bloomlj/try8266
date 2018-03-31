/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <MFRC522.h>
#include <SPI.h>
#include <Hash.h>
#include <Servo.h> 
#include <ESP8266WiFi.h>

const char* ssid     = "40637";
const char* password = "4063740637";

const char* host = "192.168.1.113";
const int httpPort = 3000;
const char* streamId   = "....................";
const char* privateKey = "southwestjiaotonguniversityswjtumaker";
const char* lock = "403b";


//RC522 RFID Shield
#define RST_PIN 0  //RST
#define SS_PIN  2  //SDA
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
//Min length of UID to be considered secure
#define AUTHORIZED_LENGTH 4
//Salt to prepend and append to UID before hashing
#define SALT1 "RANDOM1"
#define SALT2 "RANDOM2"
//Servo
#define SERVO_PIN 4 // Signal pin for Servo
Servo myservo;  // create servo object to control a servo 

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array_hex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; ++i) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

// Helper routine to dump a byte array to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; ++i) {
    Serial.print(buffer[i] < 100 ? buffer[i] < 10 ? " 00" : " 0" : " ");
    Serial.print(buffer[i]);
  }
}

// UID string
String uid_string(byte *buffer, byte bufferSize) {
  String uid = "";
  
  for (byte i = 0; i < bufferSize; ++i) {
    uid += buffer[i];
  }

  return uid;
}

// UID hashing
String uid_hash(byte *buffer, byte bufferSize) {
  if(bufferSize < AUTHORIZED_LENGTH)
    return String("The UID is too short!");
  
  return sha1(SALT1 + uid_string(buffer, bufferSize) + SALT2);
}

boolean check_response(String line){
  if(line == "{\"" + String(lock) + "\":true}")    //Check the server response
  {
    Serial.println("UID is authorized");
    return true;
  }
  else
  {
    Serial.println("UID is not authorized");
    return false;
  }
 }

 void openlock(){
  //pinMode(4, OUTPUT);
  int pos;
    for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    } 
    for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    } 
  //pinMode(4, INPUT);
 }

void setup() {
  Serial.begin(115200);
  delay(10);

  SPI.begin();                          // Init SPI bus
  delay(100);
  
  mfrc522.PCD_Init();                   // Init MFRC522
  delay(1000);

  Serial.println("Getting Antenna Gain");
  Serial.println(String(mfrc522.PCD_GetAntennaGain()));
  delay(100);
  Serial.println("Setting Max Antenna Gain");
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  delay(100);
  Serial.println("Getting Antenna Gain");
  Serial.println(String(mfrc522.PCD_GetAntennaGain()));
  delay(100);
  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to server:");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  // Look for new cards
  if(!mfrc522.PICC_IsNewCardPresent())
  {
    delay(50);
    return;
  }

  // Select one of the cards
  if(!mfrc522.PICC_ReadCardSerial())
  {
    Serial.println("Selection failed");
    delay(50);
    return;
  }

  //for debug
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  delay(10);

  Serial.println("UID SHA1 Hash:");
  String uid_hashed=uid_hash(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(uid_hashed);

  
  // We now create a URI for the request
  String url =  "/api/v1/access?lock=";
  url += lock;
  url += "&card=";
  url += uid_hashed;
  url += "&private_key=";
  url += privateKey;

  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\n');
    Serial.print(line);
    if(check_response(line)){
      openlock();
      Serial.println("now the door should be opend");
      
   }
  
  }
  
  Serial.println();
  Serial.println("closing connection");
}

