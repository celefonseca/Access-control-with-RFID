//Lybraries
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>


#define SS_PIN 5
#define RST_PIN 0
#define buzzer 2
#define ServoPin 14
#define LEDRed 35
#define LEDGreen 32

//Variables

byte lock = 0;
String content= ""; 
int angle = 0;


const char* SSID = "X-cele"; // rede wifi
const char* PASSWORD = "12345678"; // senha da rede wifi

const char* BROKER_MQTT = "broker.hivemq.com"; // ip/host do broker
int BROKER_PORT = 1883; // porta do broker
const char *mqtt_username = "admin";
const char *mqtt_password = "hivemq";

const char* topic_publish = "input/name";
const char* topic_subscribe = "output/name";

// Functions
void callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT();
void recconectWiFi();
void rfidProcess();
void Acess(String message);
void active_motor();


// Objects
WiFiClient espClient;
PubSubClient client(espClient); // instancia o mqtt
MFRC522 RFID(SS_PIN, RST_PIN); // instancia o rfid
Servo ServoMotor;


void setup() {
  //Start serial
  Serial.begin(115200);

  //Start Wifi
  delay(10);
  Serial.println("Connecting: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected  " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());

  //Connect Broker MQTT
  client.setServer(BROKER_MQTT, BROKER_PORT);
  client.setCallback(callback);

  //Start RFID  
  SPI.begin();
  RFID.PCD_Init();
  Serial.println("Bring your card closer to the reader...");
  Serial.println();

  //Start Servo Motor
  ServoMotor.attach(ServoPin);
  ServoMotor.write(angle);

}

void loop() {

  if (!client.connected()) {
    reconnectMQTT();
  }
  recconectWiFi();
  client.loop();

  if ( ! RFID.PICC_IsNewCardPresent()) {
    delay(500);
    return;
  }

  if ( ! RFID.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  rfidProcess();

}



//Função que recebe as mensagens publicadas
void callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("Topic => " + String(topic) + " | Value => " + String(message));

  Serial.flush();
  Acess(message);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Trying to connect to the MQTT Broker: " + String(BROKER_MQTT));
    if (client.connect("CodeWan")) {
      Serial.println("Connected");
      client.subscribe("input/name");

    } else {
      Serial.println("Reconnection failed");
      Serial.println("Trying to reconnect in 2 seconds");
      delay(2000);
    }
  }
}

void recconectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
}

void rfidProcess(){
  Serial.print("UID of tag : ");
  byte letter;
  for (byte i = 0; i < RFID.uid.size; i++)
  {
    content.concat(String(RFID.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(RFID.uid.uidByte[i], HEX));
  }
  char UUID[10];
  content.toCharArray(UUID, 9);
  Serial.println(content);
  client.publish(topic_subscribe, UUID);
  return;
}


void Acess(String message){
  // Structure 
  // input/Name
  // output/Name

  // Return FALSE is given whenever the card id does not exist on the server
  // or some validation failure has occurred

  if (content.substring(0,8) == "IN"){
    Serial.println("Welcome");
    delay(1000);
    Serial.println(content.substring(9));
    delay(1000);
    active_motor();
  } else if (content.substring(0,6) == "OUT"){
      Serial.println("Thanks");
      delay(1000);
      Serial.println(content.substring(7));
      delay(1000);
      active_motor();
    } else{
        Serial.println("Access denied!");
        delay(1000);
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
  delay(1000);
  Serial.println("Swipe your card!");
  return;
}

void active_motor(){
  if (angle == 0){
    angle = 90;
  } else //if(angle == 90)
      angle = 0;
      // control servo motor arccoding to the angle
  ServoMotor.write(angle);
}