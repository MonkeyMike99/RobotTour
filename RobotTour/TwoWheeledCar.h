#pragma once
#include "Arduino.h"
#include "EncodedMotor.h"

class TwoWheeledCar{
  public:
    TwoWheeledCar();
    TwoWheeledCar(EncodedMotor leftMotor, EncodedMotor rightMotor);
    void begin(long stepsForForward,long stepsForTurn);
    void startingMove();
    void endingMove();
    void blockMove();
    void smallMove();
    void rightTurn();
    void leftTurn();
    void reverseRightTurn();
    void readLEncoder();
    void readREncoder();
    void updateMotors();
    long getRPosition();
    long getLPosition();
    EncodedMotor _leftMotor;
    EncodedMotor _rightMotor;
    int getDirection();
    long getLTarget();
    
  private:
    long _stepsForForward;
    long _stepsForTurn;
    long _rTarget;
    long _lTarget;
    int _direction;
};
