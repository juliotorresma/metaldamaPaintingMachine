#include <ezButton.h>
// POINTERS
const int enPinIzq = 4;
int *ptrEnaPinIzq = &enPinIzq;

///////////////////////////////
// CONSTANTES
const int stepPinIzq = 3;
const int dirPinIzq = 2;
const int velocidadGeneral = 700;


/////////////////////////////////////7
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
    findLimitA = findLimit_Function(stepPinIzq, dirPinIzq, velocidadGeneral, HIGH , &ptrEnaPinIzq, limitSwitch1);
  }
  else if (findLimitB == true) { // Encontramos limite trasero
    findLimitB = findLimit_Function(stepPinIzq, dirPinIzq, velocidadGeneral, LOW,  &ptrEnaPinIzq, limitSwitch2); // setPin, pinDireccion, velocity, direccion
    contador++;
  }
  else if (findMediumLimit == true) {
    digitalWrite(enPinIzq, LOW);
    for (int i = 0; i < contador / 2; i++) {
      moveMotor(stepPinIzq, dirPinIzq, velocidadGeneral, HIGH);
    }
    findMediumLimit = false;
    sprintf(("Se avanzo %3d pasos de %3d/2"),contador/2,contador);
  }
  else {
    digitalWrite(enPinIzq, HIGH);
  }
}

boolean findLimit_Function(int localStepPin, int localDirectionPin, int velocity, int dir, int **k_enaPin, ezButton limitSwitchT) {
  int state = limitSwitchT.getState();
  if (state == HIGH) {
    digitalWrite(*k_enaPin, LOW);
    moveMotor(localStepPin, localDirectionPin, velocity, dir);
  }
  else {
    Serial.println("The limit switch1: TOUCHED");
    digitalWrite(*k_enaPin, HIGH);
    delay(1000);
    return false;
  }
  return true;
}
void moveMotor(int localStepPin, int localDirectionPin, int velocity, int dir) { // setPin, pinDireccion, velocity, direccion
  digitalWrite(localDirectionPin, dir);
  digitalWrite(localStepPin, HIGH);
  delayMicroseconds(velocity);
  digitalWrite(localStepPin, LOW);
  delayMicroseconds(velocity);
  return;
}
