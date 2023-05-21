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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] :");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  Serial.println("Payload length: " + String(length)); // Menampilkan panjang payload
  char receivedPayload[length + 1];
  memcpy(receivedPayload, payload, length);
  receivedPayload[length] = '\0';

  Serial.println(strcmp(receivedPayload, "on"));
  Serial.println(strcmp(receivedPayload, "off"));

  // Mengendalikan LED berdasarkan pesan yang diterima
  if (strcmp(receivedPayload, "on") == 0) {
    digitalWrite(BUILTIN_LED, LOW); // Menyalakan LED
  } else if (strcmp(receivedPayload, "off") == 0) {
    digitalWrite(BUILTIN_LED, HIGH); // Mematikan LED
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
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
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

  unsigned char key[KEY_LEN] = "mysecretkey1234";
  unsigned char iv[IV_LEN] = "myniceiv78";
  unsigned char plaintext[MAX_MSG_LEN] = {0};
  unsigned char ciphertext[MAX_MSG_LEN] = {0};

  unsigned long long ciphertext_len = 0;
  unsigned long long plaintext_len = 0;

  Serial.println("Enter plaintext (max 1023 bytes): ");
  while (Serial.available() == 0) {
    ; // tunggu input pengguna
  }
  plaintext_len = Serial.readBytesUntil('\n', plaintext, MAX_MSG_LEN);

  // Tambahkan null-terminator ke plaintext
  plaintext[plaintext_len] = '\0';

  int encrypted = crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, plaintext_len, NULL, 0, NULL, iv, key);
  if (encrypted == 0) {
    Serial.print("Ciphertext : ");
    for (unsigned long long i = 0; i < ciphertext_len; i++) {
      Serial.write(ciphertext[i]);  // Send ciphertext byte by byte
    }
    Serial.println("");
    // Publish encrypted message to "kelompok11" topic
    if (client.publish("kelompok11", ciphertext, ciphertext_len)) {
      Serial.println("Encrypted message published");
    } else {
      Serial.println("Failed to publish encrypted message");
    }
  } else {
    Serial.println("Encryption failed!");
  }
}