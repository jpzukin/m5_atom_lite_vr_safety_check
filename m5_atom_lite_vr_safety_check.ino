#include <M5Atom.h>

const int triggerPins[] = {21, 22};
const int echoPins[] = {25, 19};
const int vibratorPin = 23;

const int rightSensor = 0;
const int leftSensor = 1;

const float dangerousDistance = 80.0;   // 80cm

void setup()
{
  M5.begin(true, false, true);

  pinMode(triggerPins[rightSensor], OUTPUT);
  pinMode(triggerPins[leftSensor],  OUTPUT);
  pinMode(echoPins[rightSensor], INPUT);
  pinMode(echoPins[leftSensor],  INPUT);
  pinMode(vibratorPin, OUTPUT);

  Serial.println();
  Serial.println("=== Start ===");
  M5.dis.drawpix(0, 0xf00000);
}

float measureDistance(int currentSensor)
{
  int triggerPin = triggerPins[currentSensor];
  int echoPin = echoPins[currentSensor];

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(11);
  digitalWrite(triggerPin, LOW);

  int duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.017;
  Serial.print("sensor:");    Serial.print(currentSensor);
  Serial.print(",distance:"); Serial.print(distance);
  Serial.println();

  return distance;
}

int safetyCheck()
{
  float rightDistance = measureDistance(rightSensor);
  float leftDistance = measureDistance(leftSensor);

  int warning = 0;
  if ((0 < rightDistance) && (rightDistance <= dangerousDistance))
  {
    warning += 1;
  }

  if ((0 < leftDistance) && (leftDistance <= dangerousDistance))
  {
    warning += 2;
  }

  return warning;
}

void loop()
{
  int warning = safetyCheck();
  for (int c=0; c<warning; c++)
  {
    M5.dis.drawpix(0, 0x00f000);
    digitalWrite(vibratorPin, HIGH);
    delay(100);
    M5.dis.drawpix(0, 0xf00000);
    digitalWrite(vibratorPin, LOW);
    delay(100);
  }

  delay(1000);
  M5.update();
}
