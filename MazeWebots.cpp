#include "MazeWebots.h"
using namespace std;

Robot *robot;
Motor *lMotor;
Motor *rMotor;
PositionSensor *lEnc;
PositionSensor *rEnc;
Gyro *gyro;
GPS *gps;
DistanceSensor * ds[8];
Camera * colorSensor;
Camera *lCam;
Camera *rCam;
LightSensor *lHeat;
LightSensor *rHeat;

Emitter *emitter;
SensorInfo si;

int timeStep;

void init(){
  robot = new Robot();
  timeStep = (int)robot->getBasicTimeStep();
  lMotor = robot->getMotor("left wheel motor");
  rMotor = robot->getMotor("right wheel motor");
  lEnc = lMotor->getPositionSensor();
  rEnc = rMotor->getPositionSensor();
  gyro = robot->getGyro("gyro");
  gps = robot->getGPS("gps");
  lHeat = robot->getLightSensor("left_heat_sensor");
  rHeat = robot->getLightSensor("right_heat_sensor");
  lCam = robot->getCamera("camera_left");
  rCam = robot->getCamera("camera_right");
  emitter = robot->getEmitter("emitter");
  colorSensor = robot->getCamera("colour_sensor");
  for(int i = 0; i < NUM_DIST_SENSORS; i++){
    ds[i] = robot->getDistanceSensor("ps" + std::to_string(i));
    ds[i]->enable(timeStep);
  }
  
  lEnc->enable(timeStep);
  rEnc->enable(timeStep);
  gyro->enable(timeStep);
  gps->enable(timeStep);
  colorSensor->enable(timeStep);
  lCam->enable(timeStep);
  rCam->enable(timeStep);
  lHeat->enable(timeStep);
  rHeat->enable(timeStep);
  
  lMotor->setPosition(INFINITY);
  rMotor->setPosition(INFINITY);
  lMotor->setVelocity(0);
  rMotor->setVelocity(0);
}

void readSensors(){
    si.lEncCt = lEnc->getValue();
    si.rEncCt = rEnc->getValue();
    for(int i = 0; i < NUM_DIST_SENSORS; i++)
      si.dist[i] = ds[i]->getValue() * 80 / 0.8;
    si.cDist[0] = sin(1.27) * si.dist[7];
    si.cDist[1] = sin(1.27) * si.dist[0];
    si.cDist[2] = si.dist[2];
    si.cDist[3] = si.dist[2];
    si.cDist[4] = sin(2 * M_PI - 5.21) * si.dist[3];
    si.cDist[5] = sin(4.21 - M_PI) * si.dist[4];
    si.cDist[6] = si.dist[5];
    si.cDist[7] = si.dist[5];
    for(int i = YAW; i != ROLL; i++){
      si.angle[i] += (timeStep / 1000.0) * (gyro->getValues())[i];
      si.angle[i] = fmod(si.angle[i], 2 * M_PI);
      if(si.angle[i] < 0)
        si.angle[i] += 2 * M_PI;
    }
    const unsigned char * img = colorSensor->getImage();
    for(int i = R; i != RGBA::A; i++)
      si.color[i] = img[i];
    for(int i = X; i <= GPSAXIS::Z; i++)
      si.coord[i] = gps->getValues()[i];
    si.lHeat = lHeat->getValue();
    si.rHeat = rHeat->getValue();
    
    si.lCamData.read(lCam->getHeight(), lCam->getWidth(), (void *)lCam->getImage());
    si.rCamData.read(rCam->getHeight(), rCam->getWidth(), (void *)rCam->getImage());
}
void printData(){
  std::cout<<"LEnc: "<<si.lEncCt<<", REnc: "<<si.rEncCt<<std::endl;
  std::cout<<"X: "<<si.coord[X]<<", Y: "<<si.coord[Y]<<", Z:"<<si.coord[Z]<<std::endl;
  for(int i = 0; i < NUM_DIST_SENSORS; i++)
    std::cout<<"ps"<<i<<": "<<si.dist[i]<<", ";
  std::cout<<std::endl;
  for(int i = 0; i < NUM_DIST_SENSORS; i++)
    std::cout<<"c"<<i<<": "<<si.cDist[i]<<", ";
  std::cout<<std::endl;
  std::cout<<"Yaw: "<<si.angle[0] * 180 / M_PI<<", Pitch: "<<si.angle[1] * 180 / M_PI<<", Roll: "<<si.angle[2] * 180 / M_PI<<std::endl;
  std::cout<<"R: "<<(int)(si.color[RGBA::R])<<", G: "<<(int)(si.color[RGBA::G])<<", B: "<<(int)(si.color[RGBA::B])<<", A: "<<(int)(si.color[RGBA::A])<<std::endl;
  std::cout<<"lHeat: "<<si.lHeat<<", rHeat: "<<si.rHeat<<std::endl;
  si.lCamData.display("lCam");
  si.rCamData.display("rCam");
}
void setSpeed(double left, double right){
  if(left > lMotor->getMaxVelocity())
    left = lMotor->getMaxVelocity();
  if(left < -lMotor->getMaxVelocity())
    left = -lMotor->getMaxVelocity();
  if(right > rMotor->getMaxVelocity())
    right = rMotor->getMaxVelocity();
  if(right < -rMotor->getMaxVelocity())
    right = -rMotor->getMaxVelocity();
  lMotor->setVelocity(left);
  rMotor->setVelocity(right);
}
bool forward(double dist, double init){
  static double kP = 5;
  static double kD = 0.1;
  double target = dist * encPerCm + init;
  double error = 0;
  static double prevError = 0;
  error = target - si.lEncCt;
  if(abs(error) < 0.1){
    setSpeed(0, 0);
    prevError = 0;
    return true;
  }
  setSpeed(error * kP + (error - prevError) * kD, error * kP + (error - prevError) * kD);
  prevError = error;
  return false;
}
bool turn(double angle){
  static double kP = 1;
  static double kD = 0.1;
  double error = angle - si.angle[YAW] * 180 / M_PI ;
  static double prevError = 0;
  if(error > 180)
    error = 360 - error;
  else if(error <= -180)
    error += 360;
  if(abs(error) < 0.05){
    setSpeed(0, 0);
    prevError = 0;
    return true;
  }
  setSpeed(-(error * kP + (error - prevError) * kD), error * kP + (error - prevError) * kD);
  prevError = error;
  return false;
}
void writeEmitter(char c){
  char message[9];
  int x = si.coord[X] * 100;
  int z = si.coord[Z] * 100;
  memcpy(&message[0], &x, 4);
  memcpy(&message[4], &z, 4);
  message[8] = c;
  emitter->send(message, 9);
}
