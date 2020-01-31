#include "world.h"
#include "player.h"
#include "item.h"
#include "npc.h"
#include <ctime>

void gameLoop(Player *p,World& w)
{
    do
    {
        w.getActiveEnvironment()->queryOptions(p);
    } while (p->alive());
    
}
int main()
{
    srand((unsigned)time(NULL));
    loadItems();
    World w;
    Player p("david",2,3);
    gameLoop(&p,w);
}
