#include "Arduino.h"
#include "EncodedMotor.h"
#include <util/atomic.h>


EncodedMotor::EncodedMotor(int ENCA,int ENCB,int PWM, int in1, int in2){
    _ki=0;
    _kd=0;
    _kp=0;

    pinMode(ENCA,INPUT);
    pinMode(ENCB,INPUT);
    pinMode(PWM,OUTPUT);
    pinMode(in1,OUTPUT);
    pinMode(in2,OUTPUT);

    _ENCA=ENCA;
    _ENCB=ENCB;
    _PWM=PWM;
    _in1=in1;
    _in2=in2;

    _pos=0;
    _target=0;

    prevT = 0;
    eprev = 0;
    eintegral = 0;
}

EncodedMotor:: EncodedMotor(){
}

void EncodedMotor::begin(float kp, float kd, float ki){
  _ki=ki;
  _kd=kd;
  _kp=kp;
}

int EncodedMotor::getPosition(){
  return _pos;
}

void EncodedMotor::readEncoder(){
  int b = digitalRead(_ENCB);
  if(b > 0){
    _pos++;
  }
  else{
    _pos--;
  }
}

void EncodedMotor:: setMotor(int dir, int pwmVal,int minVal){
  if(pwmVal<minVal){
    pwmVal = minVal;
  }
  analogWrite(_PWM,pwmVal);
  if(dir == 1){
    digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
  }
  else{
    digitalWrite(_in1,LOW);
    digitalWrite(_in2,LOW);
  }  
}
void EncodedMotor:: setMotorPos(int target,int minVal){
  _target = target;
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 9.0e5 );
  prevT = currT;


  // error
  int e = _pos - _target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = _kp*e + _kd*dedt + _ki*eintegral;

  // motor power
  float pwr = fabs(u);
  if( pwr > 255 ){
    pwr = 255;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  setMotor(dir,pwr,minVal);


  // store previous error
  eprev = e;
}
