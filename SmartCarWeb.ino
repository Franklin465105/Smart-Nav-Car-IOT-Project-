#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_INA219.h"   // INA219 library

Adafruit_MMA8451 mma;
Adafruit_INA219 ina219;        // INA219 object

#define PIN_TRIG 16
#define PIN_ECHO 17
#define BUZZER_PIN 25

const char* ssid = "FranklinIphone";
const char* password = "123456789";

WebServer server(80);

int readUltra() {
  digitalWrite(PIN_TRIG, LOW);      
  delayMicroseconds(2);             
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH, 30000); // timeout 
  if (duration == 0) return 999;      // no echo = treat as far away
  
  int distanceCM = duration / 58;
  delay(50);
  return distanceCM;
}

String getUltra() {
  int d = readUltra();
  return String(d);
}

String getMotion()
{
  mma.read();

  float ax = mma.x / 4096.0 * 9.81;
  float ay = mma.y / 4096.0 * 9.81;
  float az = mma.z / 4096.0 * 9.81;

  float totalAccel = sqrt(ax * ax + ay * ay + az * az);
  if (abs(totalAccel - 9.81) < 0.5) {
    return "Stationary";
  } else {
    return "Moving";
  }
}

// INA219 helper functions
String getVoltage() {
  float voltage = ina219.getBusVoltage_V();
  return String(voltage, 2); // 2 decimal places
}

String getCurrent() {
  float current_mA = ina219.getCurrent_mA();
  return String(current_mA, 2); // 2 decimal places
}

String getPower() {
  float voltage = ina219.getBusVoltage_V();
  float current_A = ina219.getCurrent_mA() / 1000.0;
  float power = voltage * current_A;
  return String(power, 3); // 3 decimal places
}
void handleBuzzer(int distance) {
  if (distance <= 0 || distance > 10) {  // above 15cm ilence
    noTone(BUZZER_PIN);
    return;
  }
  // Only reaches here if distance is between 1–15cm
  tone(BUZZER_PIN, 2000);
  delay(100);
  noTone(BUZZER_PIN);
  delay(100);
}

void handleRoot() {
  // Updated for your homepage
  String message = homePagePart1 + getUltra() + homePagePart2 + getMotion() + homePagePart3;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) 
{
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

 Wire.begin(21, 22);
if (!mma.begin()) {
  Serial.println("Failed to find MMA8451!");
} mma.setRange(MMA8451_RANGE_2_G);

if (!ina219.begin(&Wire)) { // use same Wire instance
  Serial.println("Failed to find INA219!");
}

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  // Server routes
  server.on("/", handleRoot);
  server.on("/ultra", []() { server.send(200, "text/plain", getUltra()); });
  server.on("/motion", []() { server.send(200, "text/plain", getMotion()); });
  server.on("/voltage", []() { server.send(200, "text/plain", getVoltage()); });
  server.on("/current", []() { server.send(200, "text/plain", getCurrent()); });
  server.on("/power", []() { server.send(200, "text/plain", getPower()); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  int distance = readUltra();
  handleBuzzer(distance); 
  delay(50); 
}

