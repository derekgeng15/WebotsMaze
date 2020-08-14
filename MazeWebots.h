#pragma once
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Camera.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/LightSensor.hpp>
#include <webots/Gyro.hpp>
#include <webots/GPS.hpp>
#include <cstring>
#include <cmath>

#include "VisualVictim.h"

#define WHEEL_RADIUS 2.0
#define WHEEL_BASE 5.8
#define encPerCm  1 / WHEEL_RADIUS
#define encPerDeg M_PI * WHEEL_BASE / 360 * encPerCm
#define NUM_DIST_SENSORS 8
#define NUM_IR_SENSORS 8
#define OBTHRESH 200

using namespace webots;

extern Robot *robot;
extern Motor *lMotor;
extern Motor *rMotor;
extern PositionSensor *lEnc;
extern PositionSensor *rEnc;
extern Gyro *gyro;
extern GPS *gps;
extern DistanceSensor * ds[8];
extern Camera * colorSensor;
extern Camera * lCam;
extern Camera * rCam;
extern LightSensor *lHeat;
extern LightSensor *rHeat;

extern int timeStep;

const double dir[] = {0, -90, 180, 90};
const char pdir[] = {'N', 'E', 'S', 'W'};
struct SensorInfo{
  CameraData lCamData;
  CameraData rCamData;
  double angle[3] = {0};
  double lEncCt = 0;
  double rEncCt = 0;
  double lHeat = 0;
  double rHeat = 0;
  double dist[8] = {0};
  double cDist[8] = {0};
  double coord[3] = {0};
  int color[4] = {0};
}extern si;

enum RGBA{
  R, G, B, A
};

enum GPSAXIS{
  X, Y, Z
};

enum ANGLE{
  YAW, PITCH,  ROLL
};
void init();
void readSensors();
void printData();
void setSpeed(double left, double right);
bool forward(double dist, double init);
bool turn(double angle);
