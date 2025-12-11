#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma;

void setup() {
  Serial.begin(9600);
  Wire.begin(16, 17); // SCL SDA

  if (!mma.begin()) {
    Serial.println("Failed to find MMA8451!");
    while (1);
  }

  mma.setRange(MMA8451_RANGE_2_G);
  Serial.println("MMA8451 Ready!");
}

void loop() {
  mma.read();

  float ax = mma.x / 4096.0 * 9.81;
  float ay = mma.y / 4096.0 * 9.81;
  float az = mma.z / 4096.0 * 9.81;

  float totalAccel = sqrt(ax * ax + ay * ay + az * az);

  if (abs(totalAccel - 9.81) < 0.5) {
    Serial.println("Stationary");
  } else {
    Serial.println("Moving");
  }

  delay(200);
}
