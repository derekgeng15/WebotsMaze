#include "Maze.h"
#include "MazeBFS.h"
#include "MazeWebots.h"

using namespace std;
#define MAZE_LENGTH 60
#define MAZE_WIDTH 60
#define TILE_SIZE 12
enum STATE{
  READING, FOLLOWING, PIT, VICTIM
}state;
enum FOLLOWING_STATE{
  OBS, TURNING, TURNADJ, FORWARD, FORADJ
}fstate;

int mdir[] = {-MAZE_WIDTH, 1, MAZE_WIDTH, -1};
int main(int argc, char **argv) {
  init();
  double target = 0;
  double tang = 0;
  double init = 0;
  //MAZE STUFF
  Maze maze;
  gen_bMaze(&maze, MAZE_LENGTH, MAZE_WIDTH);
  stack<int> path;
  Direction currDir = Direction::UP;
  int startTile = maze.length/2 * maze.width + maze.length/2;
  int currTile = startTile;
  maze.tiles[currTile].seen = 1;
  state = STATE::READING;
  fstate = FOLLOWING_STATE::TURNING;
  
  bool gunningForHome = false;
  int tsteps = 0;
  while (robot->step(timeStep) != -1) {
      cout<<"------------------------------------"<<endl;
      if(gunningForHome){
        cout<<"GUNNING"<<endl;
        if(currTile == startTile)
          break;
      }
        
      readSensors();  
      printData();
      cout<<"U: "<<(int)maze.tiles[currTile].U;
      cout<<", R: "<<(int)maze.tiles[currTile].R;
      cout<<", D: "<<(int)maze.tiles[currTile].D;
      cout<<", L: "<<(int)maze.tiles[currTile].L;
      cout<<", V: "<<(int)maze.tiles[currTile].victim<<endl;
      if(si.color[RGBA::G] < 15 && state != PIT){
        maze.tiles[path.top()].U = 0;
        maze.tiles[path.top()].D = 0;
        maze.tiles[path.top()].L = 0;
        maze.tiles[path.top()].R = 0;
        if(path.top() - maze.width >= 0)
          maze.tiles[path.top() - maze.width].D = 0;
        if(path.top() + maze.width < maze.length * maze.width)
          maze.tiles[path.top() + maze.width].U = 0;
        if(path.top()%maze.width - 1 >= 0)
          maze.tiles[path.top() - 1].R = 0;
        if(path.top()%maze.width + 1 < maze.width)
          maze.tiles[path.top() + 1].L = 0;
        path.pop();
        target = -TILE_SIZE/2 + 3;
        init = si.lEncCt;
        state = PIT;
        fstate = TURNING;
      }
      switch(state){
       case STATE::READING:
       {
         cout<<"READING"<<endl;
         for(int i = 0; i < NUM_OF_DIRECTIONS; i++){
               if(si.cDist[i * 2] < TILE_SIZE && si.cDist[i * 2 + 1] < TILE_SIZE)
                 block_tile(&maze, currTile, (Direction)((i + currDir > 3)?(i + currDir - 4):(i + currDir)));
                 
         }
         if(!newTile_BFS(&maze, currTile, &path, currDir)){
           gunningForHome = true;
           fTile_BFS(&maze, currTile, &path, currDir, startTile);
         }
         state = FOLLOWING;
         break;
       }
       case STATE::FOLLOWING:
       {
         cout<<"FOLLOWING"<<endl;
         if(path.empty()){
           state = READING;
           break;
         }
         switch(fstate){
           
           case FOLLOWING_STATE::TURNING:
           {
             cout<<"TURNING"<<endl;
             if((si.lHeat >= 30 || si.rHeat >= 30) && !maze.tiles[currTile].victim){
                  cout<<"HEAT VICTIM"<<endl;
                    maze.tiles[currTile].victim = 1;
                
                setSpeed(0, 0);
                int step = 0;
                while(robot->step(timeStep) != -1 && step < 7 / 0.032)
                  step++;
                writeEmitter('T');
              }
             if(turn(dir[get_direction(&maze, currTile - path.top())])){
               setSpeed(0, 0);
                
               init = si.lEncCt;
               currDir = get_direction(&maze, currTile - path.top());
               target = (((si.dist[0] > si.dist[1])?si.dist[0]:si.dist[1])<100)?((si.dist[0] > si.dist[1])?si.dist[0]:si.dist[1]):TILE_SIZE;
               target = TILE_SIZE;
               fstate = FORWARD;
               // if(si.dist[0] < 100)
                 // target = fmod(si.dist[0], TILE_SIZE) + 15;
               // if(si.dist[1] < 100)
                 // target = fmod(si.dist[1], TILE_SIZE) + 15;
               // double re = (si.dist[2] > si.dist[3])?si.dist[2]:si.dist[3];
               // double le = (si.dist[6] > si.dist[7])?si.dist[6]:si.dist[7];
               // if(re < 100 && le < 100){
                   // double ew = fmod(le, TILE_SIZE) - fmod(re, TILE_SIZE);
                   // tang = atan(ew/target) * 180 / M_PI + dir[currDir];
                   // target = sqrt(target * target + ew * ew);
                   // fstate = TURNADJ;
                 // }
               // else 
                 // fstate = FORWARD;
             }
             break;
           }
           case FOLLOWING_STATE::TURNADJ:
           {
             cout<<"TURNADJ"<<endl;
             if(turn(tang)){
               setSpeed(0, 0);
               fstate = FORWARD;
             }
             break;
           }
           case FOLLOWING_STATE::FORWARD:
           {
             cout<<"FORWARD"<<endl;
             if(forward(target, init)){
               setSpeed(0, 0);
               fstate = FORADJ;
             }
             break;
           }
           case FOLLOWING_STATE::FORADJ:
           {
             cout<<"FORADJ"<<endl;
             if(turn(dir[currDir])){
               setSpeed(0, 0);
               currTile = path.top();
               maze.tiles[currTile].seen = 1;
               path.pop();
               fstate = TURNING;
             }
             break;
           }
         }
       break;
       }
       case STATE::PIT:
       {
         cout<<"PIT"<<endl;
         if(forward(target, init)){
               setSpeed(0, 0);
               maze.tiles[currTile].seen = 1;
               fstate = TURNING;
               state = READING;
         }
         break;
       }
       case STATE::VICTIM:
       {
         
       }
      }
  };
  writeEmitter('E');
  cout<<"Done!"<<endl;
  delete robot;
  return 0;
}
