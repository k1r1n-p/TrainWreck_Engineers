#include <WiFi.h>
#include <PubSubClient.h>

// wifi credentials
const char* ssid        = "wifi name";
const char* password    = "wifi password";
const char* mqtt_server = "mqtt broker ip";

// motor pins
#define IN1 26
#define IN2 27
#define ENA 25

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

// mqtt topics
#define COMMAND_TOPIC "train/command"
#define STATUS_TOPIC  "train/status"

WiFiClient espClient;
PubSubClient client(espClient);

String currentState = "STOPPED";

void stopMotor() {
  ledcWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  currentState = "STOPPED";
  client.publish(STATUS_TOPIC, "STOPPED");
  Serial.println(">> STOPPED");
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(ENA, 200);
  currentState = "FORWARD";
  client.publish(STATUS_TOPIC, "FORWARD");
  Serial.println(">> FORWARD");
}

void reverse() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(ENA, 200);
  currentState = "REVERSE";
  client.publish(STATUS_TOPIC, "REVERSE");
  Serial.println(">> REVERSE");
}

void onMessage(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();
  Serial.print(">> Received: ");
  Serial.println(msg);

  if (msg == "f") {
    forward();
  } else if (msg == "r") {
    reverse();
  } else if (msg == "s") {
    stopMotor();
  } else {
    Serial.println(">> Unknown command");
  }
}

void connectWifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Train")) {
      Serial.println("connected");
      client.subscribe(COMMAND_TOPIC);
      client.publish(STATUS_TOPIC, "ESP32 online");
    } else {
      Serial.print("failed rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION);

  stopMotor();
  connectWifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(onMessage);
  connectMQTT();

  Serial.println("Ready for commands");
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
} 