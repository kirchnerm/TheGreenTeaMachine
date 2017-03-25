#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 6 
#define BUTTON 2 // interrupt 0
#define BUZZER 5

int LEDPins[] = { 8, 9, 10 };
int temps[] = { 70, 80, 90 };
int availableTemps = 3;

int targetTemp = 1; // 80 Degree default

unsigned int previousTime = 0, prellTime = 500;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, LOW); 
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonPressed, HIGH);

  for(int i=0; i < availableTemps; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }

  setTempLeds();

  Serial.begin(9600);
  Serial.println("TeaTime!");

  pinMode(BUZZER, OUTPUT);
  tone(BUZZER, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(BUZZER);     // Stop sound...
  delay(1000);        // ...for 1sec

   // Start up the library 
 sensors.begin(); 
}

void loop() {
 Serial.print(" Requesting targetTemperatures..."); 
 sensors.requestTemperatures(); // Send the command to get targetTemperature readings 
 Serial.println("DONE"); 
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
 delay(1000); 
}

void buttonPressed() {
  Serial.println(previousTime);  
  int buttonState = digitalRead(BUTTON);
  if( buttonState == 1 && (millis() - previousTime) > prellTime ) {
    previousTime = millis();
    targetTemp++;
    if(targetTemp > availableTemps - 1) {
      targetTemp = 0;
    }
    setTempLeds();
  }
}

void setTempLeds() {   
     for(int i=0; i<availableTemps; i++) {
      if(i==targetTemp) {
        digitalWrite(LEDPins[i],HIGH);
      } else {
        digitalWrite(LEDPins[i],LOW);
      }
    }
}

