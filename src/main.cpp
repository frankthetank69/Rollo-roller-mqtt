#include <MQTT.h>
#include <WiFi.h>
#include <WiFiClient.h>

const char ssid[] = "Liwest2C91";
const char pass[] = "TQEptUFHt787";

MQTTClient client;
WiFiClient net;
IPAddress brokerIP(192,168,0,206);

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("!");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("brokerl",nullptr,nullptr)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  client.begin(brokerIP, 1883, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
}