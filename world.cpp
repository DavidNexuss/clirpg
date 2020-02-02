#include "world.h"
#include "util.h"
#include <iostream>
using namespace std;

#define WORLD_SIZE 10
void World::generateEnvironment(int i, int j)
{
    Environment e((int)((i + 1) * (j + 1) * RANDOM(1)));
    world_matrix[i][j] = e;
}

World::World() : world_matrix(WORLD_SIZE,vector<Environment>(WORLD_SIZE)),x(0),y(0){
    for (int i = 0; i < WORLD_SIZE; i++)
        for (int j = 0; j < WORLD_SIZE; j++)
            generateEnvironment(i,j);

    active_environment = &world_matrix[x][y];
}

Environment* World::getActiveEnvironment(){ return active_environment;}
void World::setActiveEnvironment(int i, int j)
{
    active_environment = &world_matrix[i][j];
}
void World::stepCurrentEnvironment()
{
    active_environment->step();
}
void World::printActiveEnvironment()
{
    cout << "environment: " << endl;
    active_environment->printEnvironment(); 
}

void World::printWorld()
{
    cout << "Printing world" << endl;
    for (size_t i = 0; i < WORLD_SIZE; i++)
    for (size_t j = 0; j < WORLD_SIZE; j++)
        world_matrix[i][j].printEnvironment();
}

void World::printBasicEnvInfo(int i,int j)
{
    Environment& env = world_matrix[i][j];
    cout << "Land of " << env.getName() << " (" << env.getHardness() << ")";
}

inline int MOD(int a,int b)
{
    if (a < 0) return (b + a) % b;
    else return a % b;
}

inline void i(int& a)
{
    a++;
    a = a % WORLD_SIZE;
}
inline void d(int& a)
{
    a--;
    a = MOD(a,WORLD_SIZE);
}

void World::queryWolrdOptions()
{
    cout << "You are in the overwolrd" << endl;
    cout << "You are currently in the land of " << active_environment->getName() << "{" << x << "," << y << "}" << endl;
    cout << "Where do you want to go?" << endl;
    
    cout << "1) North"; printBasicEnvInfo(x % WORLD_SIZE,MOD((y + 1),WORLD_SIZE)); cout << endl;
    cout << "2) South"; printBasicEnvInfo(x % WORLD_SIZE,MOD((y - 1),WORLD_SIZE)); cout << endl;
    cout << "3) East"; printBasicEnvInfo(MOD((x + 1), WORLD_SIZE), y % WORLD_SIZE); cout << endl;
    cout << "4) West"; printBasicEnvInfo(MOD((x - 1) , WORLD_SIZE), y % WORLD_SIZE); cout << endl;
    cout << "5) Nowhere" << endl;

    int response;
    cin >> response;
    if (response < 0 || response > 5) queryWolrdOptions();

    if (response == 1) i(y); else
    if (response == 2) d(y); else 
    if (response == 3) i(x); else 
    if (response == 4) d(x);

    active_environment = &world_matrix[x][y];
}