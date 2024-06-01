#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define pinLED1 12 // led 1
#define pinLED2 13 // led 2
String SerialData;

// WiFi
const char *ssid = "MAKERINDO2";        // Enter your Wi-Fi name
const char *password = "makerindo2019"; // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic1 = "LED/tag1";
const char *topic2 = "LED/tag2";
const char *mqtt_username = "jes";      //"didii18";
const char *mqtt_password = "password"; //"testmqtt";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Serial.begin(9600);

  pinMode(pinLED1, OUTPUT);
  digitalWrite(pinLED1, LOW);

  pinMode(pinLED2, OUTPUT);
  digitalWrite(pinLED2, LOW);

  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");

  // connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Public EMQX MQTT broker connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(5000);
    }
  }
  // Publish and subscribe led 1
  client.publish(topic1, "Hi, I'm ESP32 ^^");
  client.subscribe(topic1);

  // Publish and subscribe led 1
  client.publish(topic2, "Hi, I'm ESP32 ^^");
  client.subscribe(topic2);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String data = "";
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }

  if (strcmp(topic, topic1) == 0)
  {
    if (data.equals("ON"))
    {
      digitalWrite(pinLED1, HIGH);
      Serial.println("LED 1 is ON");
    }
    else
    {
      digitalWrite(pinLED1, LOW);
      Serial.println("LED 1 is OFF");
    }
  }
  else if (strcmp(topic, topic2) == 0)
  {
    if (data.equals("ON"))
    {
      digitalWrite(pinLED2, HIGH);
      Serial.println("LED 2 is ON");
    }
    else
    {
      digitalWrite(pinLED2, LOW);
      Serial.println("LED 2 is OFF");
    }
  }

  Serial.println();
  Serial.println("-----------------------");
}

void loop()
{
  client.loop();
}
