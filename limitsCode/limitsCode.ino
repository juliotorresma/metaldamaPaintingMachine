#include <ezButton.h>
const int stepPin = 5;
const int dirPin = 2;
const int enPin = 8;
ezButton limitSwitch1(12);  // create ezButton object that attach to pin 7;
ezButton limitSwitch2(11);
volatile boolean flag = true;

void setup() {
  Serial.begin(9600);
  limitSwitch1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2.setDebounceTime(50);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
}

void loop() {

  limitSwitch1.loop(); // MUST call the loop() function first
  limitSwitch2.loop();
  if(flag==true){
    int state = limitSwitch1.getState();
    if (state == LOW)
      flag = false;
    
    findLimitA();
  }
  else{
    digitalWrite(enPin, HIGH);
    }
  
}
void findLimitA(){
  moveMotor(stepPin, dirPin, 1000, HIGH);
  return;
  }

  
void moveMotor(int localStepPin, int localDirectionPin, int velocity, int dir) { // setPin, velocity
  digitalWrite(localDirectionPin, dir);
  digitalWrite(localStepPin, HIGH);
  delayMicroseconds(velocity);
  digitalWrite(localStepPin, LOW);
  delayMicroseconds(velocity);
  return;
}
