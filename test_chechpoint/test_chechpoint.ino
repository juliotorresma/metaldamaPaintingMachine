#include <ezButton.h>
// POINTERS
const int enPinIzq = 4;
int *ptrEnaPinIzq = &enPinIzq;

const int enPinDer = 8;
int *ptrEnaPinDer = &enPinDer;

///////////////////////////////
// CONSTANTES
const int stepPinIzq = 3;
const int dirPinIzq = 2;

const int stepPinDer = 7;
const int dirPinDer = 6;

const int velocidadGeneral = 700;


/////////////////////////////////////7
volatile int contador = 0;
volatile boolean findLimitAIzq = true;
volatile boolean findLimitBIzq = true;

volatile boolean findLimitADer = true;
volatile boolean findLimitBDer = true;

volatile boolean findMediumLimit = true;


ezButton limitSwitch1Izq(12);  // create ezButton object that attach to pin 12;
ezButton limitSwitch2Izq(11); 

ezButton limitSwitch1Der(10);  // create ezButton object that attach to pin 10;
ezButton limitSwitch2Der(9);


void setup() {
  Serial.begin(9600);
  ///////// LIMIT SWITCH BOUNCE TIMES
  limitSwitch1Der.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2Der.setDebounceTime(50);

  limitSwitch1Izq.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2Izq.setDebounceTime(50);

  ////////// PIN MODES
  pinMode(stepPinIzq, OUTPUT);
  pinMode(dirPinIzq, OUTPUT);
  pinMode(enPinIzq, OUTPUT);

  pinMode(stepPinDer, OUTPUT);
  pinMode(dirPinDer, OUTPUT);
  pinMode(enPinDer, OUTPUT);
  
  digitalWrite(enPinIzq, LOW);
  digitalWrite(enPinDer, LOW);
}

void loop() {
  limitSwitch1Izq.loop(); // MUST call the loop() function first
  limitSwitch2Izq.loop();
  
  if (findLimitAIzq== true) { // Encontramos limite frontal
    findLimitAIzq = findLimit_Function(stepPinIzq, dirPinIzq, velocidadGeneral, HIGH , &ptrEnaPinIzq, limitSwitch1Izq);
  }
  else if (findLimitBIzq == true) { // Encontramos limite trasero
    findLimitBIzq = findLimit_Function(stepPinIzq, dirPinIzq, velocidadGeneral, LOW,  &ptrEnaPinIzq, limitSwitch2Izq); // setPin, pinDireccion, velocity, direccion
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
