#define BUTTON_PIN 2 // interrupt 0

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
}

void loop() {
  // temperature scanning here ...
  //buttonPressed();
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

