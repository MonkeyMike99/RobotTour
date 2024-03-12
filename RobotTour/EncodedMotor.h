#pragma once
#include "Arduino.h"

class EncodedMotor{
  public:
    EncodedMotor(int ENCA,int ENCB,int PWM, int in1, int in2);
    EncodedMotor();
    void begin(float ki, float kd, float kp);

    void setMotorPos(int target,int minVal);
    int getPosition();
    void readEncoder();
  private:
    void setMotor(int dir, int pwmVal,int minVal);
    
    float _ki;
    float _kd;
    float _kp;
    
    int _ENCA;
    int _ENCB;
    int _PWM;
    int _in1;
    int _in2;

    int _pos;
    int _target;

    long prevT;
    float eprev;
    float eintegral;
};
