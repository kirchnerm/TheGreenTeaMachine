#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 6
#define BUTTON 2
#define BUZZER 5

#define WAITING_FOR_THRESHOLD 0
#define WAITING_FOR_TARGET_TEMP 1

int LEDPins[] = { 8, 9, 10 };
int temps[] = { 70, 80, 90 };
int availableTemps = 3;

int targetTemp = 1; // 80 Degree default
int state = WAITING_FOR_THRESHOLD;

unsigned int previousTime = 0, prellTime = 500;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, LOW);
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressed, HIGH);

  for (int i = 0; i < availableTemps; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }

  setTempLeds();

  Serial.begin(9600);
  Serial.println("TeaTime!");

  pinMode(BUZZER, OUTPUT);
  playSound();
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float actualTemp = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(actualTemp);

  if (state == WAITING_FOR_THRESHOLD && actualTemp >= temps[targetTemp]) {
    state = WAITING_FOR_TARGET_TEMP;
  }

  if (state == WAITING_FOR_TARGET_TEMP && actualTemp <= temps[targetTemp]) {
    state = WAITING_FOR_THRESHOLD;
    // buzzer !!!!
    playSound();
  }
  delay(1000);
}

void buttonPressed() {
  Serial.println(previousTime);
  int buttonState = digitalRead(BUTTON);
  if ( buttonState == 1 && (millis() - previousTime) > prellTime ) {
    previousTime = millis();
    targetTemp++;
    if (targetTemp > availableTemps - 1) {
      targetTemp = 0;
    }
    setTempLeds();
  }
}

void setTempLeds() {
  for (int i = 0; i < availableTemps; i++) {
    if (i == targetTemp) {
      digitalWrite(LEDPins[i], HIGH);
    } else {
      digitalWrite(LEDPins[i], LOW);
    }
  }
}

void playSound() {
  tone(BUZZER, 1000);
  delay(1000);
  noTone(BUZZER);
  delay(1000);
}
