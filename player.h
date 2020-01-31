#pragma once
#include "character.h"
#include <string>
class Player : public Character
{
    int x,y;
    public:
    Player(std::string name,int _x,int _y);
};