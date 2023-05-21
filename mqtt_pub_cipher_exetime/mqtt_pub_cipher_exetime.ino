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

const char* ssid = "2Bro_3";
const char* password = "abad21beda";
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

  // Serial.println("Payload length: " + String(length)); // Menampilkan panjang payload
  // char receivedPayload[length + 1];
  // memcpy(receivedPayload, payload, length);
  // receivedPayload[length] = '\0';

  // Serial.println(strcmp(receivedPayload, "on"));
  // Serial.println(strcmp(receivedPayload, "off"));

  // // Mengendalikan LED berdasarkan pesan yang diterima
  // if (strcmp(receivedPayload, "on") == 0) {
  //   digitalWrite(BUILTIN_LED, LOW); // Menyalakan LED
  // } else if (strcmp(receivedPayload, "off") == 0) {
  //   digitalWrite(BUILTIN_LED, HIGH); // Mematikan LED
  // }
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
  static int messageCounter = 1;  // Counter untuk menambah angka pada pesan

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

  // Membuat pesan dengan format "test_x" dengan nilai x pada messageCounter
  snprintf((char*)plaintext, MAX_MSG_LEN, "altitude_%d;speed_%d;orientation_%d", messageCounter, messageCounter, messageCounter);
  plaintext_len = strlen((char*)plaintext);

  unsigned long startTime = millis(); // Waktu awal

  int encrypted = crypto_aead_encrypt(ciphertext, &ciphertext_len, plaintext, plaintext_len, NULL, 0, NULL, iv, key);
  if (encrypted == 0) {
    Serial.print("Ciphertext : ");
    for (unsigned long long i = 0; i < ciphertext_len; i++) {
      Serial.printf("%02X", ciphertext[i]);
    }
    Serial.println("");
    // Convert ciphertext to hexadecimal string
    char hex_ciphertext[ciphertext_len * 2 + 1];
    for (unsigned long long i = 0; i < ciphertext_len; i++) {
      sprintf(&hex_ciphertext[i * 2], "%02X", ciphertext[i]);
    }
    hex_ciphertext[ciphertext_len * 2] = '\0';
    // Publish encrypted message to "kelompok11" topic
    if (client.publish("kelompok11", hex_ciphertext)) {
      Serial.println("Encrypted message published");
    } else {
      Serial.println("Failed to publish encrypted message");
    }
  } else {
    Serial.println("Encryption failed!");
  }

  unsigned long endTime = millis(); // Waktu akhir
  unsigned long executionTime = endTime - startTime; // Selisih waktu eksekusi

  Serial.print("Encryption execution time: ");
  Serial.print(executionTime);
  Serial.println(" ms");

  messageCounter++;  // Menambah counter setelah setiap pengiriman pesan
  delay(2000);       // Menunda selama 1 detik
}