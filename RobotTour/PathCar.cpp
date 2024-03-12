#include "Arduino.h"
#include "EncodedMotor.h"
#include "TwoWheeledCar.h"
#include "PathCar.h"
#include "GridPosition.h"
#include "Vector.h"

#define FORWARD 0
#define RIGHT 1
#define LEFT 3
#define REVRIGHT 2

PathCar:: PathCar(TwoWheeledCar robot){
  _robot = robot;
  _interval = 0;
  _prevTime = 0;
  _instructions = {};
}

void PathCar:: begin(Vector<GridPosition> path, int dur){
  createInstructions(path);
  _interval = (_instructions.size()*1000/dur);
}

void PathCar:: readREncoder(){
  _robot.readREncoder();
}

void PathCar:: readLEncoder(){
  _robot.readLEncoder();
}
int PathCar:: getDirection(){
  return _robot.getDirection();
}

void PathCar:: updateCar(){
  int curTime = millis();
  if((curTime- _prevTime)>_interval && !_instructions.empty()){
    move();
    _prevTime = curTime;
  }
  _robot.blockMove();
}

void PathCar:: move(){
  int instruction = _instructions.front();
  _instructions.remove(0);
  switch(instruction){
    case FORWARD:
      _robot.blockMove();
      break;
    case RIGHT:
      _robot.rightTurn();
      break;
    case LEFT:
      _robot.leftTurn();
      break;
    case REVRIGHT:
      _robot.reverseRightTurn();
      break;
  }
}

void PathCar:: createInstructions(Vector<GridPosition> path){
  GridPosition blockCur = path.front();
  GridPosition blockAft = path.at(1);
  int dir = 0;
  int instruction = 0;
  path.remove(0);
  Vector<int> instructions;
  while(path.size()-1>0){
    instruction = findMove(blockCur,blockAft, dir);
  
    switch(instruction){
      case FORWARD:
        instructions.push_back(FORWARD);
        break;
      case RIGHT:
        instructions.push_back(RIGHT);
        dir = (dir+1)%4;
        break;
      case LEFT:
        instructions.push_back(LEFT);
        dir = (dir-1)%4;
        break;
      case REVRIGHT:
        instructions.push_back(RIGHT);
        instructions.push_back(REVRIGHT);
        dir = (dir+2)%4;
        break;
    }
    path.remove(0);
    blockCur = path.front();
    blockAft = path.at(1);
  }
  _instructions = instructions;
}

int PathCar:: findMove(GridPosition cur, GridPosition after,int curDirection){
  int xDif = after.x-cur.x;
  int yDif = (after.y-cur.y)*2;
  int finalDif = xDif-yDif;
  int intendedDir = 0;
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
  return (intendedDir-curDirection)%4;
}
int PathCar:: getLPosition(){
  return _robot.getLPosition();
}
int PathCar:: getRPosition(){
  return _robot.getRPosition();
}
void PathCar:: setInstructions(Vector<int> instructions){
  _instructions = instructions;
}
