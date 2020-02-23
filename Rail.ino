#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library

#define SS_PIN 4  //D2
#define RST_PIN D4 //D1
#include <SPI.h>
#include <MFRC522.h>

#define FIREBASE_HOST "rail-rfid.firebaseapp.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "SELX4EPm3VpA2Ukg6kIXvolGewAlaLIjkaeLoDkR"            // the secret key generated from firebase

#define WIFI_SSID "Realme"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "12345678"                                    //password of wifi ssid
 
                                               
void setup() {
  Serial.begin(9600);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
        SPI.begin();      // Initiate  SPI bus
      mfrc522.PCD_Init();   // Initiate MFRC522
                                                                

}


void loop(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "69 CA 8B 98"){
    Serial.println(" Access Granted ");
    Serial.println();
    int statuss = 1;
    delay(2000);
  }
  
  else{
    Serial.println(" Access Denied ");
    delay(3000);
  }
  Serial.print("UID:");  
  //Serial.print(h);
 // String fireuid = String(mfrc522.uid.uidByte[i], HEX);                                         //convert integer humidity to string humidity 
  Firebase.pushString("/MFRC52210/UID", content);                                  //setup path and send readings
}
