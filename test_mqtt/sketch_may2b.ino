#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

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
  Serial.print("] ");
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
      client.publish("arif", "hello world");
      // ... and resubscribe
      client.subscribe("arif");
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

  if (Serial.available()) { // Cek apakah ada input dari serial monitor
    String input = Serial.readStringUntil('\n'); // Baca input dari serial monitor
    Serial.print("Input: ");
    Serial.println(input);

    // Publish input pengguna ke topik "arif"
    if (client.publish("arif", input.c_str())) {
      Serial.println("Message published");
    } else {
      Serial.println("Failed to publish message");
    }
  }
}