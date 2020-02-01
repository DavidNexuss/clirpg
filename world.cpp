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

World::World() : world_matrix(WORLD_SIZE,vector<Environment>(WORLD_SIZE)){
    for (int i = 0; i < WORLD_SIZE; i++)
        for (int j = 0; j < WORLD_SIZE; j++)
            generateEnvironment(i,j);

    active_environment = &world_matrix[0][0];
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
