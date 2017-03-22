#define BUTTON_PIN 2 // interrupt 0

int ledPins[] = { 1, 2, 3 };
int temps[] = { 70, 80, 90 };
int availableTemps = 3;

int temp = 1; // 80 Degree default

unsigned int previousTime = 0, prellTime = 20;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // internal PullUp to 5V
  attachInterrupt(0, buttonPressed, LOW);

  for(int i=0; i < availableTemps; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

}

void loop() {
  // temperature scanning here ...

}

void buttonPressed() {
  if( (millis() - previousTime) > prellTime ) {
    temp++;
    if(temp > availableTemps - 1) {
      temp = 0;
    }
    for(int i=0; i<availableTemps; i++) {
      if(i==temp) {
        digitalWrite(ledPins[i],HIGH);
      } else {
        digitalWrite(ledPins[i],LOW);
      }
    }
  }
}

