
#include <Servo.h>

int pos =0; //
int positionInit = 0; // position initiale
int positionFinal = 180;

Servo servo1;
Servo servo2;

//****** capteurs photoresistances
const int sensor1=A0; //droite
const int sensor2=A1; //gauche

//****** position optimale
int optimalPointSensor1Value = 0;
int optimalPointSensor2Value = 0;
int optimalPosition = 0;


const int ERROR_MIN = 20; //erreur entre deux points tolerable

void setup() {
   servo1.attach(9); 
   servo2.attach(10); 
   Serial.begin(9600);
 
}

void loop() {
  int sensor1Value = analogRead(sensor1);
  int sensor2Value = analogRead(sensor2);

  // des que la lumière apparait, alors faire une recherche
  if( optimalPointSensor1Value <= 20 && optimalPointSensor2Value <= 20  &&  sensor1Value >= 40 && sensor2Value >= 40 ){
    
     recherchePositionOptimale(sensor1Value, sensor2Value); 
     
  }

  trackSolar(sensor1Value, sensor2Value);
   
  
    
}

/**
 * Recherche de la position optimale en utilisant le servo1 (servo du bas).
 * Le servo fait un tour complet donc 360° par pas de 10°. A chaque pas, les mesures des capteurs sont recuperees. A la toute premriere recuperation, ces valeurs sont stockées 
 * dans des variables. A chaque autre pas, les nouvelles mesures recuperees sont comparees aux valeures precedentes. 
 * Si c'est superieure, alors ces nouvelles valeurs sont stockées.
 * 
 */
void rechercheHorizontale(){
  int alpha = 0;
  for(alpha=0; alpha<360; alpha+=10){
    tournerServo1(10, position_initiale);

    int val1 = analogRead(sensor1);
    int val2 = analogRead(sensor2);
    
    //Si c'est la position est optimale
    if( optimalPosition(val1, val2) ){
       optimalPointSensor1Value = val1;
       optimalPointSensor2Value = val2;
    }

  }
  
}

void rechercheVerticale(){
  int alpha = 0;
  for(alpha=0; alpha<90; alpha+=10){
    tournerServo1(10, position_initiale);

    int val1 = analogRead(sensor1);
    int val2 = analogRead(sensor2);
    
    //Si c'est la position est optimale
    if( optimalPosition(val1, val2) ){
       optimalPointSensor1Value = val1;
       optimalPointSensor2Value = val2;
    }

  }
}

/**
 * Verifier si la position est optimale par rapport à la derniere position stockée
 */
boolean optimalPosition(int val1, int val2){
  if( val1 >  max(optimalSensor1Value,optimalSensor2Value) || val2 < max(optimalSensor1Value,optimalSensor2Value) ){
      return true;
  }
  else{
    return false;
  }
}

/**
 * Suivre le soleil.
 * Tourner vers le capteur qui reçoit plus de lumière (à une erreur près)
 * 
 */
void trackSolar(int val1, int val2){

  if(val1 - val2 >= 100){
     tournerServo2(); //tourner vers sensor1
     updatePosition(); //
     
  }else if(val2 - val1>= 100){
    tournerServo2(); //tourner vers sensor2
    updatePosition(); //
    
  } else if( val1 - optimalSensor1Value < -100 || val2 - optimalSensor2Value < -100){ // une difference anormale, faire une recherche verticale
    val1 = optimalSensor1Value;
    val2 = optimalSensor2Value;
    rechercheVerticale();

    if(val1 - optimalSensor1Value < -100 || val2 - optimalSensor2Value < -100){
       rechercheHorizontale();
    }
  }
 
 /* else if( max(val1, val2) - max(optimalSensor1Value, optimalSensor2Value) > 100 ){  // si la nouvelle position differe de trop par rapport a la derniere position choisie
      optimalSensor1Value = val1;
      optimalSensor2Value = val2;
  }
  
  if( optimalSensor1Value - optimalSensor2Value > 100 ){
      tournerServo2(); //tourner vers sensor1
  }*/
}

void updatePosition(int val1, int val2, int angle){
  optimalSensor1Value = val1;
  optimalSensor2Value = val2;
}

void tournerServo1(int angle, int initPos){
  for(pos = initPos; pos <= angle+initPos; pos += 10)    // goes from 180 degrees to 0 degrees
  {                               
        servo1.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void tournerServo2(int angle, int initPos){
  for(pos = initPos; pos <= angle+initPos; pos += 10)    // goes from 180 degrees to 0 degrees
  {                               
    servo2.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}



