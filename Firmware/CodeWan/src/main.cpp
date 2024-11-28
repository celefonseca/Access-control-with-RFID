//Lybraries
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <cJSON.h>


#define SS_PIN 5
#define RST_PIN 0
#define buzzer 2
#define ServoPin 14
#define LEDRed 33
#define LEDGreen 32

//Variables

byte lock = 0;
String ID= ""; 
int angle = 0;


const char* SSID = "X-cele"; // rede wifi
const char* PASSWORD = "deumaoito"; // senha da rede wifi

//const char* BROKER_MQTT = "broker.hivemq.com"; // ip/host do broker
const char* BROKER_MQTT = "192.168.212.123"; // ip/host do broker
int BROKER_PORT = 1883; // porta do broker
const char *mqtt_username = "";
const char *mqtt_password = "";

char extracted_data[32]; // Ajuste o tamanho conforme necessário

const char* topic_publish = "codewan/RFID-project/accessRequest";
const char* topic_subscribe = "codewan/RFID-project/accessResponse";

// Functions
void callback(char* topic_subscribe, byte* payload, unsigned int length);
void reconnectMQTT();
void recconectWiFi();
void rfidProcess();
void Acess(String message);
void active_motor();


// Objects
WiFiClient espClientCodewan;
PubSubClient client_MQTT(espClientCodewan); // instancia o mqtt
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
  client_MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  client_MQTT.setCallback(callback);

  //Start RFID  
  SPI.begin();
  RFID.PCD_Init();
  Serial.println("Bring your card closer to the reader...");
  Serial.println();

  //Start Servo Motor
  ServoMotor.attach(ServoPin);
  ServoMotor.write(angle);

  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);

}

void loop() {

  if (!client_MQTT.connected()) {
    reconnectMQTT();
  }
  recconectWiFi();
  client_MQTT.loop();

  if ( ! RFID.PICC_IsNewCardPresent()) {
    delay(500);
    return;
  }

  if ( ! RFID.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  rfidProcess();
  ID="";

}



//Função que recebe as mensagens publicadas
void callback(char* topic_subscribe, byte* payload, unsigned int length) {

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.println(message);
  /*for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }*/
  Serial.println("Topic => " + String(topic_subscribe) + " | Value => " + String(message));

  Serial.flush();

  // Parsing JSON
  cJSON *json = cJSON_Parse(message);
  if (json == NULL) {
    Serial.println("Failed to parse JSON");
    return;
  }

  cJSON *data_item = cJSON_GetObjectItem(json, "data");
  if (data_item == NULL) {
    Serial.println("No 'data' item in JSON");
  } else {
    if (cJSON_IsString(data_item)) {
      strcpy(extracted_data, data_item->valuestring);
      //Serial.print("Extracted data: ");
      //Serial.println(extracted_data);

      // Aqui você pode usar a variável 'extracted_data' como uma string
    } else {
      Serial.println("'data' item is not a string");
    }
  }

  // Liberando a memória do JSON
  cJSON_Delete(json);
  Acess(extracted_data);
}

void reconnectMQTT() {
  while (!client_MQTT.connected()) {
    Serial.println("Trying to connect to the MQTT Broker: " + String(BROKER_MQTT));
    if (client_MQTT.connect("CodeWan")) {
      Serial.println("Connected");
      client_MQTT.subscribe("codewan/RFID-project/accessResponse");

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
  byte letter;

  for (byte i = 0; i < RFID.uid.size; i++){
    ID.concat(String(RFID.uid.uidByte[i] < 0x10 ? "0" : ""));
    ID.concat(String(RFID.uid.uidByte[i], HEX));
  }

  char UUID[10];
  ID.toCharArray(UUID, 9);
  client_MQTT.publish(topic_publish, UUID);
  ID.toUpperCase();
  Serial.print("ID Substring:");
  Serial.println(ID.substring(0));
  delay(500);
  return;
}


void Acess(String extracted_data){

  // Return FALSE is given whenever the card id does not exist on the server
  // or some validation failure has occurred
  if (extracted_data == "1" && lock == 0){
    Serial.println("Welcome");
    delay(50);
    Serial.println(ID.substring(0));
    delay(50);

    for (int i = 0; i < 5; i++) {
        digitalWrite(LEDGreen, HIGH);  // Liga o LED
        delay(50);                  // Espera 100 milissegundos
        digitalWrite(LEDGreen, LOW);   // Desliga o LED
        delay(50);                  // Espera 100 milissegundos
    }
    Serial.println("Opening");
    active_motor();
    lock = 1;
  } else if (extracted_data == "1" && lock == 1){
      Serial.println("Thanks");
      delay(50);
      Serial.println(ID.substring(0));
      delay(50);

      for (int i = 0; i < 5; i++) {
        digitalWrite(LEDGreen, HIGH);  // Liga o LED
        delay(100);                  // Espera 100 milissegundos
        digitalWrite(LEDGreen, LOW);   // Desliga o LED
        delay(100);                  // Espera 100 milissegundos
        }
      Serial.println("Closing");
      active_motor();
      lock = 0;
    } else if (extracted_data == "0"){
        Serial.println("Access denied!");
        delay(50); 
        for (int i = 0; i < 5; i++) {
          digitalWrite(LEDRed, HIGH);  // Liga o LED
          digitalWrite(LEDGreen, HIGH);  // Liga o LED
          delay(100);                  // Espera 100 milissegundos
          digitalWrite(LEDRed, LOW);   // Desliga o LED
          digitalWrite(LEDGreen, LOW);  // Liga o LED
          delay(100);                  // Espera 100 milissegundos
        }
      }  
  delay(1000);
  Serial.println("");
  Serial.println("Swipe your card!");
  return;
}

void active_motor(){
  if (angle == 0){
    angle = 180;
  } else //if(angle == 180)
      angle = 0;
      // control servo motor arccoding to the angle
  ServoMotor.write(angle);
}