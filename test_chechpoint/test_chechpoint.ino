#include <ezButton.h>
const int stepPinIzq = 3;
const int dirPinIzq = 2;
const int enPinIzq = 4;

volatile int contador = 0;
volatile boolean findLimitA = true;
volatile boolean findLimitB = true;
volatile boolean findMediumLimit = true;

ezButton limitSwitch1(12);  // create ezButton object that attach to pin 7;
ezButton limitSwitch2(11); 

void setup() {
  Serial.begin(9600);
  limitSwitch1.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2.setDebounceTime(50);
  pinMode(stepPinIzq, OUTPUT);
  pinMode(dirPinIzq, OUTPUT);
  pinMode(enPinIzq, OUTPUT);
  digitalWrite(enPinIzq, LOW);
}

void loop() {
  limitSwitch1.loop(); // MUST call the loop() function first
  limitSwitch2.loop();
  
  if (findLimitA == true) { // Encontramos limite frontal
    findLimitA_Function(stepPinIzq, dirPinIzq, 700, HIGH);
  }
  else if (findLimitB == true) { // Encontramos limite trasero
    findLimitB_Function(stepPinIzq, dirPinIzq, 700, LOW); // setPin, pinDireccion, velocity, direccion
    contador++;
  }
  else if (findMediumLimit == true) {
    digitalWrite(enPinIzq, LOW);
    for (int i = 0; i < contador / 2; i++) {
      moveMotor(stepPinIzq, dirPinIzq, 700, HIGH);
    }
    findMediumLimit = false;
    sprintf(("Se avanzo %3d pasos de %3d/2"),contador/2,contador);
  }
  else {
    digitalWrite(enPinIzq, HIGH);
  }
}

void findLimitA_Function(int localStepPin, int localDirectionPin, int velocity, int dir) {
  int state = limitSwitch1.getState();
  if (state == HIGH) {
    digitalWrite(enPinIzq, LOW);
    moveMotor(localStepPin, localDirectionPin, velocity, dir);
  }
  else {
    Serial.println("The limit switch1: TOUCHED");
    digitalWrite(enPinIzq, HIGH);
    findLimitA = false;
    delay(1000);
  }
}
void findLimitB_Function(int localStepPin, int localDirectionPin, int velocity, int dir) {
  int state = limitSwitch2.getState();
  if (state == HIGH) {
    digitalWrite(enPinIzq, LOW);
    moveMotor(localStepPin, localDirectionPin, velocity, dir);
  }
  else {
    Serial.println("The limit switch2: TOUCHED");
    digitalWrite(enPinIzq, HIGH);
    findLimitB = false;
    delay(1000);
  }
}
void moveMotor(int localStepPin, int localDirectionPin, int velocity, int dir) { // setPin, pinDireccion, velocity, direccion
  digitalWrite(localDirectionPin, dir);
  digitalWrite(localStepPin, HIGH);
  delayMicroseconds(velocity);
  digitalWrite(localStepPin, LOW);
  delayMicroseconds(velocity);
  return;
}
