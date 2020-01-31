#include "player.h"
#include "util.h"
using namespace std;
Player::Player(string name,int _x,int _y) : Character(name,0.5,30,2,3)
{
    addItem(getHardnessItem(100));
    int l = (int)(RANDOM(3) + 1);
    for (size_t i = 0; i < l; i++)
    {
        addItem(getHardnessItem(100 + RANDOM(100)));
    }
}