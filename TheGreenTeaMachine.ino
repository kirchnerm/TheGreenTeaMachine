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
int state;

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
  playSound(1000, 500);
  sensors.begin();
  
  // start with waiting for threshold temperature
  state = WAITING_FOR_THRESHOLD;
}

void loop() {
  sensors.requestTemperatures();
  float actualTemp = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(actualTemp);

  // activate on passing threshold
  if (state == WAITING_FOR_THRESHOLD && actualTemp > temps[targetTemp]) {
    state = WAITING_FOR_TARGET_TEMP;
  }

  // blink while waiting for target temperature
  if (state == WAITING_FOR_TARGET_TEMP ) {
    digitalRead(LEDPins[targetTemp]) == HIGH ? digitalWrite(LEDPins[targetTemp], LOW) : digitalWrite(LEDPins[targetTemp], HIGH);
    delay(500);
  } else {
    delay(1000);
  }

  // target temp reached
  if (state == WAITING_FOR_TARGET_TEMP && actualTemp <= temps[targetTemp]) {
    state = WAITING_FOR_THRESHOLD;
    do {
      for(int i=500; i<=1000; i=i+100) {
        playSound(i, 100);
      }
      delay(5000);
  } while(sensors.getTempCByIndex(0) > (temps[targetTemp] - 3) );
    setTempLeds();
  }  
}

void buttonPressed() {
  int buttonState = digitalRead(BUTTON);
  if ( buttonState == 1 && (millis() - previousTime) > prellTime ) {
    previousTime = millis();
    targetTemp++;
    if (targetTemp > availableTemps - 1) {
      targetTemp = 0;
    }
    Serial.print("New Target Temperature is: ");
    Serial.println(temps[targetTemp]);
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

void playSound(int herz, int duration) {
  tone(BUZZER, herz);
  delay(duration);
  noTone(BUZZER);
}
