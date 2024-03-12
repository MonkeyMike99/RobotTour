#pragma once
#include "Arduino.h"
#include "EncodedMotor.h"
#include "TwoWheeledCar.h"
#include "GridPosition.h"
#include "Vector.h"
class PathCar{
  public:
    PathCar(TwoWheeledCar robot);
    void begin(Vector<GridPosition> path,int time);
    void readREncoder();
    void readLEncoder();
    int getDirection();
    void updateCar();
    void createInstructions(Vector<GridPosition> path);
    int getLPosition();
    int getRPosition();
    void setInstructions(Vector<int> instructions);
  private:
    TwoWheeledCar _robot;
    int _interval;
    int _prevTime;
    Vector<int> _instructions;
    void move();
    int findMove(GridPosition cur,GridPosition aft,int dir);
};
