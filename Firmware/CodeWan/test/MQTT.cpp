#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>

// WiFi
const char *ssid = "X-cele"; // Enter your Wi-Fi name
const char *password = "12345678";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic_subscribe = "input/name";
const char *topic_publish = "output/name";
const char *mqtt_username = "admin";
const char *mqtt_password = "hivemq";
const int mqtt_port = 1883;

WiFiClient espClientCodewan;
PubSubClient client(espClientCodewan);

void callback(char *topic_subscribe, byte *payload, unsigned int length);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public Hivemq MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.println(client.state());
            delay(2000);
        }
    }
    client.publish(topic_publish, "Ola ola ola ^^");
    client.subscribe(topic_subscribe);
}

void callback(char *topic_subscribe, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic_subscribe);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    // Publish and subscribe
    client.loop();
    delay(1000);
}