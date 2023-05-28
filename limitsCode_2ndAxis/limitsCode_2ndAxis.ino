#include <ezButton.h>
// POINTERS
const int enPinIzq = 4;
int *ptrEnaPinIzq = &enPinIzq;

const int enPinDer = 8;
int *ptrEnaPinDer = &enPinDer;

const int enPinCenter= 35;
int *ptrEnaPinCenter = &enPinCenter;

///////////////////////////////
// CONSTANTES
const int stepPinIzq = 3;
const int dirPinIzq = 2;

const int stepPinDer = 7;
const int dirPinDer = 6;

const int stepPinCenter = 33;
const int dirPinCenter = 31;



const int velocidadGeneral = 70;


/////////////////////////////////////7
int contador = 0;
volatile boolean findLimitAIzq = true;
volatile boolean findLimitBIzq = true;

volatile boolean findLimitADer = true;
volatile boolean findLimitBDer = true;

volatile boolean findLimitCenterA = true;
volatile boolean findLimitCenterB = true;

volatile boolean findMediumLimit = true;


ezButton limitSwitch1Izq(12);  // create ezButton object that attach to pin 12;
ezButton limitSwitch2Izq(53);

ezButton limitSwitch1Der(10);  // create ezButton object that attach to pin 10;
ezButton limitSwitch2Der(9);

ezButton limitSwitch1Center(39);  // create ezButton object that attach to pin 10;
ezButton limitSwitch2Center(41);

void setup() {
  Serial.begin(9600);
  ///////// LIMIT SWITCH BOUNCE TIMES
  limitSwitch1Der.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2Der.setDebounceTime(50);

  limitSwitch1Izq.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2Izq.setDebounceTime(50);

  limitSwitch1Center.setDebounceTime(50); // set debounce time to 50 milliseconds
  limitSwitch2Center.setDebounceTime(50);

  ///// INTERRUPTIONS FOR LIMITS
  attachInterrupt(digitalPinToInterrupt(12), limitesAlcanzados, RISING);
  attachInterrupt(digitalPinToInterrupt(53), limitesAlcanzados, RISING);
  attachInterrupt(digitalPinToInterrupt(10), limitesAlcanzados, RISING);
  attachInterrupt(digitalPinToInterrupt(9), limitesAlcanzados, RISING);

  ////////// PIN MODES
  pinMode(stepPinIzq, OUTPUT);
  pinMode(dirPinIzq, OUTPUT);
  pinMode(enPinIzq, OUTPUT);

  pinMode(stepPinDer, OUTPUT);
  pinMode(dirPinDer, OUTPUT);
  pinMode(enPinDer, OUTPUT);

  pinMode(stepPinCenter, OUTPUT);
  pinMode(dirPinCenter, OUTPUT);
  pinMode(enPinCenter, OUTPUT);

  digitalWrite(enPinIzq, LOW);
  digitalWrite(enPinDer, LOW);
  digitalWrite(enPinCenter, LOW);
}

void loop() {
  limitSwitch1Izq.loop(); // MUST call the loop() function first
  limitSwitch2Izq.loop();

  limitSwitch1Der.loop(); // MUST call the loop() function first
  limitSwitch2Der.loop();

  limitSwitch1Center.loop(); // MUST call the loop() function first
  limitSwitch2Center.loop();
  
  if (findLimitCenterA == true) { // Encontramos limite lateral izquierdo
    findLimitCenterA = findLimit_Function(stepPinCenter, dirPinCenter, velocidadGeneral, HIGH , &ptrEnaPinCenter, limitSwitch1Center);
  }
  else if (findLimitCenterB == true) { // Encontramos limite lateral derecho
    findLimitCenterB = findLimit_Function(stepPinCenter, dirPinCenter, velocidadGeneral, LOW , &ptrEnaPinCenter, limitSwitch2Center);
  }
  else if (findLimitAIzq == true || findLimitADer == true) { // Encontramos limite frontal
    findLimitAIzq = findLimit_Function(stepPinIzq, dirPinIzq, velocidadGeneral, LOW , &ptrEnaPinIzq, limitSwitch1Izq);
    findLimitADer = findLimit_Function(stepPinDer, dirPinDer, velocidadGeneral, HIGH , &ptrEnaPinDer, limitSwitch1Der);
  }
  else if (findLimitBIzq == true || findLimitBDer == true) { // Encontramos limite trasero
    findLimitBIzq = findLimit_Function(stepPinIzq, dirPinIzq, velocidadGeneral, HIGH,  &ptrEnaPinIzq, limitSwitch2Izq); // setPin, pinDireccion, velocity, direccion
    findLimitBDer = findLimit_Function(stepPinDer, dirPinDer, velocidadGeneral, LOW,  &ptrEnaPinDer, limitSwitch2Der);
    contador = contador + 1;
    //Serial.println(contador);
  }
  else if (findMediumLimit == true) {
    contador = abs(contador);
    digitalWrite(enPinIzq, LOW);
    digitalWrite(enPinDer, LOW);
    Serial.print(contador);
    for (int i = 0; i <(contador/2) + 3000; i++) {
      moveMotor(stepPinIzq, dirPinIzq, velocidadGeneral, LOW);
      moveMotor(stepPinDer, dirPinDer, velocidadGeneral, HIGH);
    }
    findMediumLimit = false;
    
  }
  else {
    digitalWrite(enPinIzq, HIGH);
    digitalWrite(enPinDer, HIGH);

  }

}

boolean findLimit_Function(int localStepPin, int localDirectionPin, int velocity, int dir, int **k_enaPin, ezButton limitSwitchT) {
  int state = limitSwitchT.getState();
  if (state == HIGH) {
    digitalWrite(*k_enaPin, LOW);
    moveMotor(localStepPin, localDirectionPin, velocity, dir);
  }
  else {
    //Serial.println("The limit switch: TOUCHED");
    digitalWrite(*k_enaPin, HIGH);
    //delay(1000);
    return false;
  }
  return true;
}
void moveMotor(int localStepPin, int localDirectionPin, int velocity, int dir) { // setPin, pinDireccion, velocity, direccion
  digitalWrite(localDirectionPin, dir);
  digitalWrite(localStepPin, LOW);
  digitalWrite(localStepPin, HIGH);
  delayMicroseconds(velocity);
  return;
}

void limitesAlcanzados()
{
  if (findLimitAIzq == true || findLimitADer == true || findLimitBIzq == true || findLimitBDer == true || findMediumLimit == true) {
    return;
  }
  else { // STOP MOTORS
    digitalWrite(enPinIzq, HIGH);
    digitalWrite(enPinDer, HIGH);

  }
}
