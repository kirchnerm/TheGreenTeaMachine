#include <DallasTemperature.h>

#include <OneWire.h>

// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 6 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


#define BUTTON_PIN 2 // interrupt 0
#define BUZZER 5

int ledPins[] = { 8, 9, 10 };
int temps[] = { 70, 80, 90 };
int availableTemps = 3;

int temp = 1; // 80 Degree default

unsigned int previousTime = 0, prellTime = 500;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, LOW); // internal PullUp to 5V
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, HIGH);

  for(int i=0; i < availableTemps; i++) {
    pinMode(ledPins[i], OUTPUT);
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
 // call sensors.requestTemperatures() to issue a global temperature 
 // request to all devices on the bus 
/********************************************************************/
 Serial.print(" Requesting temperatures..."); 
 sensors.requestTemperatures(); // Send the command to get temperature readings 
 Serial.println("DONE"); 
/********************************************************************/
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   delay(1000); 
}

void buttonPressed() {
  Serial.println(previousTime);  
  int buttonState = digitalRead(BUTTON_PIN);
  if( buttonState == 1 && (millis() - previousTime) > prellTime ) {
    previousTime = millis();
    temp++;
    if(temp > availableTemps - 1) {
      temp = 0;
    }
    setTempLeds();
  }
}

void setTempLeds() {   
     for(int i=0; i<availableTemps; i++) {
      if(i==temp) {
        digitalWrite(ledPins[i],HIGH);
      } else {
        digitalWrite(ledPins[i],LOW);
      }
    }
}

