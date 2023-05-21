#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Wire.h>
#include "api.h"
#include "encrypt.h"

// Update these with values suitable for your network.
#define KEY_LEN 16
#define IV_LEN 12
#define MAX_MSG_LEN 1024

const char* ssid = "Rapael gimang";
const char* password = "katasandi";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  unsigned char key[KEY_LEN] = "mysecretkey1234";
  unsigned char iv[IV_LEN] = "myniceiv78";
  unsigned char ciphertext[MAX_MSG_LEN] = {0};

  unsigned long long ciphertext_len = 0;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] :");
  
  // Mengisi array ciphertext dengan data dari payload dengan handling
  for (int i = 0; i < length; i++) {
    ciphertext[i] = payload[i];
    ciphertext_len++;
  }

  for (int i = 0; i < length; i++) {
    Serial.write(payload[i]);
  }
  Serial.println();

  // Dekripsi ciphertext dan mencetak decrypted_plaintext sebagai string
  unsigned char decrypted_plaintext[MAX_MSG_LEN] = {0};
  unsigned long long decrypted_plaintext_len = MAX_MSG_LEN;
  int decrypted = crypto_aead_decrypt(decrypted_plaintext, &decrypted_plaintext_len, NULL, ciphertext, ciphertext_len, NULL, 0, iv, key);

  if (decrypted == 0) {
    Serial.print("Decrypted plaintext: ");
    Serial.println((char*)decrypted_plaintext);
  } else {
    Serial.println("Decryption failed!");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("kelompok11", "hello world");
      // ... and resubscribe
      client.subscribe("kelompok11");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}