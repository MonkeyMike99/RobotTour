#include "Arduino.h"
#include "EncodedMotor.h"
#include "TwoWheeledCar.h"
#include <util/atomic.h>

TwoWheeledCar::TwoWheeledCar(EncodedMotor leftMotor,EncodedMotor rightMotor){
  _leftMotor = leftMotor;
  _rightMotor = rightMotor;
  _stepsForForward = 0;
  _stepsForTurn = 0;
  _rTarget = 0;
  _lTarget = 0;
  _direction = 0;
}
TwoWheeledCar:: TwoWheeledCar(){
  
}
void TwoWheeledCar::begin(long stepsForForward,long stepsForTurn){
  _stepsForForward = stepsForForward;
  _stepsForTurn = stepsForTurn;
  _leftMotor.begin(20,1.4,0.0);
  _rightMotor.begin(20,1.4,0.0);
}

void TwoWheeledCar:: smallMove(){
  _rTarget+= _stepsForForward-(_stepsForForward/100)*31;
  _lTarget+= _stepsForForward-(_stepsForForward/100)*31;
}
void TwoWheeledCar:: startingMove(){
  _rTarget+= (_stepsForForward-(_stepsForForward/100)*31)/2;
  _lTarget+= (_stepsForForward-(_stepsForForward/100)*31)/2;
}

void TwoWheeledCar:: blockMove(){
  _rTarget+= _stepsForForward;
  _lTarget+= _stepsForForward;
}

void TwoWheeledCar:: rightTurn(){
  _lTarget+= _stepsForTurn;
  _direction = (_direction+1)%4;
}

void TwoWheeledCar:: leftTurn(){
  _rTarget+= _stepsForTurn;
  _direction = (_direction-1)%4;
}
void TwoWheeledCar:: reverseRightTurn(){
  _rTarget-= _stepsForTurn;
  _direction = (_direction+1)%4;
}
void TwoWheeledCar:: readLEncoder(){
  _leftMotor.readEncoder();
}
void TwoWheeledCar:: readREncoder(){
  _rightMotor.readEncoder();
}
void TwoWheeledCar:: updateMotors(){
  _leftMotor.setMotorPos(_lTarget,35);
  _rightMotor.setMotorPos(-_rTarget,35);
}
long TwoWheeledCar:: getRPosition(){
  return -_rightMotor.getPosition();
}
long TwoWheeledCar:: getLPosition(){
  return  _leftMotor.getPosition();
}
int TwoWheeledCar:: getDirection(){
  return _direction;
}

long TwoWheeledCar:: getLTarget(){
  return _lTarget;
}
