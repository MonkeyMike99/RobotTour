#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#include "Arduino.h"
#include "EncodedMotor.h"
#include "TwoWheeledCar.h"
#include "PathCar.h"
#include "GridPosition.h"
#include "Vector.h"

#define ENCAL 2 // YELLOW
#define ENCBL 7 // WHITE
#define PWML 4
#define IN2L 5
#define IN1L 6

#define ENCAR 3 // YELLOW
#define ENCBR 8 // WHITE
#define PWMR 9
#define IN2R 10
#define IN1R 11

#define FORWARD 0
#define RIGHT 1
#define LEFT 3
#define REVRIGHT 2
#define SMALLMOVE 4
#define STARTINGMOVE 5


EncodedMotor leftMotor(ENCAR,ENCBL,PWML,IN1L,IN2L);
EncodedMotor rightMotor(ENCAL,ENCBR,PWMR,IN1R,IN2R);
TwoWheeledCar robot(leftMotor,rightMotor);
Vector<int> instructions;

int interval=0;
long prevTime=0;
long curTime=0;

GridPosition b1(0,0);
GridPosition b2(1,0);
GridPosition b3(2,0);
GridPosition b4(3,0);
GridPosition b5(0,1);
GridPosition b6(1,1);
GridPosition b7(2,1);
GridPosition b8(3,1);
GridPosition b9(0,2);
GridPosition b10(1,2);
GridPosition b11(2,2);
GridPosition b12(3,2);
GridPosition b13(0,3);
GridPosition b14(1,3);
GridPosition b15(2,3);
GridPosition b16(3,3);
Vector<int> createInstructions(Vector<GridPosition> path,Vector<int>& instructions){
  GridPosition blockCur = path.at(0);
  GridPosition blockAft = path.at(1);
  int dir = 0;
  int instruction = 0;
  int list[100];
  instructions.setStorage(list);
  instructions.push_back(STARTINGMOVE);
  while(path.size()-1>0){
    instruction = findMove(blockCur,blockAft, dir);
    
    switch(instruction){
      case FORWARD:
        instructions.push_back(FORWARD);

        break;
      case RIGHT:
        instructions.push_back(RIGHT);
        instructions.push_back(SMALLMOVE);
        dir = (dir+1)%4;
        break;
      case LEFT:
        instructions.push_back(LEFT);
        instructions.push_back(SMALLMOVE);
        dir = (dir-1)%4;
        break;
      case REVRIGHT:
        instructions.push_back(RIGHT);
        instructions.push_back(REVRIGHT);
        instructions.push_back(FORWARD);
        dir = (dir+2)%4;
        break;
    }
//    Serial.println(instruction);
    Serial.println(instructions.at(instructions.size()-1));
    path.remove(0);
    blockCur = path.front();
    blockAft = path.at(1);
  }
//    Serial.println(instruction);
  Serial.println(instructions.at(instructions.size()-1));
  return instructions;
}



void setup() {
  Serial.begin(9600);
  
  GridPosition path1[]={GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0),GridPosition(0,0)};
  Vector<GridPosition> path;
  
  path.setStorage(path1);
  path.push_back(b2);
  path.push_back(b3);
  path.push_back(b7);
  path.push_back(b8);
  path.push_back(b12);
  path.push_back(b11);
  path.push_back(b15);
  path.push_back(b11);
  path.push_back(b10);
  path.push_back(b6);
  path.push_back(b5);
  path.push_back(b6);
  path.push_back(b10);
  path.push_back(b9);
  path.push_back(b13);
  path.push_back(b14);

  
  Serial.println(path.size());
  robot.begin(940,528);
  attachInterrupt(digitalPinToInterrupt(ENCAR),readLeft,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCAL),readRight,RISING);
  pinMode(53,OUTPUT);
  pinMode(52,OUTPUT);
  digitalWrite(53,LOW);
  digitalWrite(52,HIGH);
  
  createInstructions(path,instructions);
  interval = (60*1000/instructions.size());
  Serial.println("target pos");
  prevTime = 0;
  curTime = 0;
  
}

void loop() {
  updateCar();
  robot.updateMotors();
//  Serial.print(robot.getLTarget());
//  Serial.print(" ");
//  Serial.print(robot.getLPosition());
//  Serial.print(" ");
//  Serial.print(robot.getRPosition());
//  Serial.println();
  delay(1);
}
void readLeft(){
//  robotTour.readLEncoder();
  robot.readLEncoder();
}
void readRight(){
//  robotTour.readREncoder();
  robot.readREncoder();
}


void updateCar(){
  long curTime = millis()+(interval-1000);
  if((curTime- prevTime)>interval && !instructions.empty()){
    moveCar(instructions);
    prevTime = curTime;
  }
}

void moveCar(Vector<int> in){
  int instruction = in.at(0);
  for(int i=0;i<instructions.size();i++){
    Serial.println(instructions.at(i));
  }
  Serial.println(instruction);
  instructions.remove(0);
  switch(instruction){
    case FORWARD:
      robot.blockMove();
      break;
    case RIGHT:
      robot.rightTurn();
      break;
    case LEFT:
      robot.leftTurn();
      break;
    case REVRIGHT:
      robot.reverseRightTurn();
      break;
    case SMALLMOVE:
      robot.smallMove();
      break;
    case STARTINGMOVE:
      robot.startingMove();
      break;
  }
  
}



int  findMove(GridPosition cur, GridPosition after,int curDirection){
  int xDif = after.x-cur.x;
  int yDif = (after.y-cur.y)*2;
  int finalDif = xDif+yDif;
  int intendedDir = 0;
//  Serial.println(finalDif);
  switch(finalDif){
    case 2:
      intendedDir = 0;
      break;
    case 1:
      intendedDir = 1;
      break;
    case -2:
      intendedDir = 2;
      break;
    case -1:
      intendedDir = 3;
      break;
  }
//  Serial.println((intendedDir-curDirection)%4);
  return ((intendedDir-curDirection)+16)%4;
}
