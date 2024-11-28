/*Door lock system with Arduino Nano
   https://srituhobby.com
*/

// #include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0
#define buzzer 2
#define servoPin 3

String UID = "41 EC 56 A3"; // Enter your card ID
byte lock = 0;
String ID= ""; 

// Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

/*
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
*/

void setup(){
  Serial.begin(115200);
  // servo.write(70);
  // servo.attach(servoPin);
  SPI.begin();
  rfid.PCD_Init();
  Serial.print("Welcome!");
  Serial.print("Put your card");
  pinMode(buzzer, OUTPUT);
}

void loop(){
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;
  Serial.print("Scanning");
  Serial.print("n_UID tag is :");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }

  ID.toUpperCase();
  Serial.print("ID Substring");
  Serial.println(ID.substring(1));
  delay(1000);

  if (ID.substring(1) == UID && lock == 0){
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    // servo.write(50);
    delay(100);
    Serial.print("Locked");
    delay(1500);
    lock = 1;
  } else if (ID.substring(1) == UID && lock == 1){
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      // servo.write(110);
      delay(100);
      Serial.print("Unlocked");
      delay(1500);
      lock = 0;
    } else{
      Serial.print("Wrong card!");
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      delay(1500);
    
    }
    ID = "";
}
