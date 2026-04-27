#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// Fix macro clash between Dabble and WebServer
#undef HTTP_GET
#undef HTTP_POST
#undef HTTP_RESPONSE

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_INA219.h"

Adafruit_MMA8451 mma;
Adafruit_INA219 ina219;

// Motors
int M1 = 19, E1 = 15; // Right
int M2 = 4,  E2 = 23; // Left
#define MAX_MOTOR_SPEED 255
const int PWMFreq = 1000;
const int PWMResolution = 8;
const int channelA = 2;
const int channelB = 3;

#define PIN_TRIG 16
#define PIN_ECHO 17
#define BUZZER_PIN 25

const char* ssid = "FranklinIphone";
const char* password = "123456789";

WebServer server(80);

// ── Motor functions ──────────────────────────────────────
void stopMotors() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  ledcWrite(channelA, 0);
  ledcWrite(channelB, 0);
}

void controlMotors(int rightSpeed, int leftSpeed) {
  if (rightSpeed >= 0) digitalWrite(M1, HIGH); else digitalWrite(M1, LOW);
  if (leftSpeed  >= 0) digitalWrite(M2, LOW);  else digitalWrite(M2, HIGH);
  ledcWrite(channelA, abs(rightSpeed));
  ledcWrite(channelB, abs(leftSpeed));
}

// ── Sensor functions ─────────────────────────────────────
int readUltra() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH, 30000);
  if (duration == 0) return 999;
  int distanceCM = duration / 58;
  delay(50);
  return distanceCM;
}

String getUltra() { return String(readUltra()); }

String getMotion() {
  mma.read();
  float ax = mma.x / 4096.0 * 9.81;
  float ay = mma.y / 4096.0 * 9.81;
  float az = mma.z / 4096.0 * 9.81;
  float totalAccel = sqrt(ax*ax + ay*ay + az*az);
  return (abs(totalAccel - 9.81) < 0.5) ? "Stationary" : "Moving";
}

String getBatteryPercent() {
  // Average 10 readings to smooth out fluctuations
  float total = 0;
  for (int i = 0; i < 10; i++) {
    total += ina219.getBusVoltage_V();
    delay(5);
  }
  float voltage = total / 10.0;
  float percent = ((voltage - 6.0) / (9.0 - 6.0)) * 100.0;
  percent = constrain(percent, 0, 100);
  return String(percent, 1) + "%";
}

String getPowerConsumed() {
  float voltage = ina219.getBusVoltage_V();
  float current_A = ina219.getCurrent_mA() / 1000.0;
  float power = voltage * current_A;
  return String(power, 3) + " W";
}

void handleBuzzer(int distance) {
  if (distance <= 0 || distance > 10) { noTone(BUZZER_PIN); return; }
  tone(BUZZER_PIN, 2000, 100); // beeps for 100ms then stops automatically
}

// ── Web handlers ─────────────────────────────────────────
void handleRoot() {
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

// ── Setup ────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Motor setup
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  ledcSetup(channelA, PWMFreq, PWMResolution);
  ledcSetup(channelB, PWMFreq, PWMResolution);
  ledcAttachPin(E1, channelA);
  ledcAttachPin(E2, channelB);
  stopMotors();

  // Ultrasonic
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // I2C sensors
  Wire.begin(21, 22);
  if (!mma.begin())         Serial.println("Failed to find MMA8451!");
  mma.setRange(MMA8451_RANGE_2_G);
  if (!ina219.begin(&Wire)) Serial.println("Failed to find INA219!");

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) Serial.println("MDNS responder started");

  server.on("/",          handleRoot);
  server.on("/ultra",     []() { server.send(200, "text/plain", getUltra()); });
  server.on("/motion",    []() { server.send(200, "text/plain", getMotion()); });
  server.on("/battery",   []() { server.send(200, "text/plain", getBatteryPercent()); });
  server.on("/powerused", []() { server.send(200, "text/plain", getPowerConsumed()); });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  // Bluetooth
  Dabble.begin("SmartNavCar");
}

// ── Loop ─────────────────────────────────────────────────
void loop() {
  Dabble.processInput();
  int rightSpeed = 0, leftSpeed = 0;

  if      (GamePad.isUpPressed())    { rightSpeed = -MAX_MOTOR_SPEED; leftSpeed = -MAX_MOTOR_SPEED; }
  else if (GamePad.isDownPressed())  { rightSpeed =  MAX_MOTOR_SPEED; leftSpeed =  MAX_MOTOR_SPEED; }
  else if (GamePad.isLeftPressed())  { rightSpeed = -MAX_MOTOR_SPEED; leftSpeed =  MAX_MOTOR_SPEED; }
  else if (GamePad.isRightPressed()) { rightSpeed =  MAX_MOTOR_SPEED; leftSpeed = -MAX_MOTOR_SPEED; }

  controlMotors(rightSpeed, leftSpeed);

  server.handleClient();

  int distance = readUltra();
  handleBuzzer(distance);

  delay(50);
}