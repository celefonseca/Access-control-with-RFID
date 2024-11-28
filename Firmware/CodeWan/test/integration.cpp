/*
  CodeWan
*/

#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 0
#define buzzer 2
#define ServoPin 14
#define LEDRed 27
#define LEDGreen 26



String UID = "41 EC 56 A3"; // Enter your card ID
byte lock = 0;
String ID= ""; 

int angle = 0;
Servo ServoMotor;
MFRC522 RFID(SS_PIN, RST_PIN);

/*
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
*/

void ShowReaderDetails() {
  // Get the MFRC522 software version
  byte v = RFID.PCD_ReadRegister(RFID.VersionReg);
  Serial.print(F("Versao do software MFRC522: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (desconhecido),provavelmente um clone chines?"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("ALERTA: Falha na comunicacao, o modulo MFRC522 esta conectado corretamente?"));
    Serial.println(F("SISTEMA ABORTADO: Verifique as conexoes."));
    while (true); // do not go further
  }
}
void setup(){
  Serial.begin(115200);
  SPI.begin();

  ServoMotor.attach(ServoPin);
  ServoMotor.write(angle);
  
  
  RFID.PCD_Init();
  Serial.print("Welcome!");
  ShowReaderDetails();
  Serial.print("Put your card");

  pinMode(buzzer, OUTPUT);
  pinMode(LEDRed, OUTPUT);
  pinMode(LEDGreen, OUTPUT);

  digitalWrite(LEDRed, LOW);
  digitalWrite(LEDGreen, LOW);
}

void loop(){
  if (!RFID.PICC_IsNewCardPresent())
    return;
  if (!RFID.PICC_ReadCardSerial())
    return;
  Serial.print("Scanning");
  Serial.print("n_UID tag is :");
  for (byte i = 0; i < RFID.uid.size; i++) {
    Serial.print(".");
    ID.concat(String(RFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(RFID.uid.uidByte[i], HEX));
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
    
    // change angle of servo motor
    if (angle == 0)
      angle = 90;
    else //if(angle == 90)
      angle = 0;

    // control servo motor arccoding to the angle
    ServoMotor.write(angle);
    Serial.print("Rotate Servo Motor to ");
    Serial.print(angle);
    Serial.println("°");

    delay(100);
    Serial.print("Locked");
    delay(1500);
    lock = 1;

  } else if (ID.substring(1) == UID && lock == 1){
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);

      // change angle of servo motor
      if (angle == 0)
        angle = 90;
      else //if(angle == 90)
        angle = 0;

      // control servo motor arccoding to the angle
      ServoMotor.write(angle);
      Serial.print("Rotate Servo Motor to ");
      Serial.print(angle);
      Serial.println("°");
      

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
