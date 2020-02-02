#pragma once
#include <vector>
#include "environment.h"

class World 
{ 
    std::vector<std::vector<Environment>> world_matrix;
    Environment* active_environment;
    void generateEnvironment(int i, int j);
    
    int x,y;
    void printBasicEnvInfo(int i,int j);
    
    public:
    World();

    Environment* getActiveEnvironment();

    void setActiveEnvironment(int i, int j);
    void stepCurrentEnvironment();
    void printActiveEnvironment();

    void printWorld();

    void queryWolrdOptions();
};