#include "world.h"
#include "player.h"
#include "item.h"
#include "npc.h"
#include <ctime>
#include <iostream>

using namespace std;
void gameLoop(Player *p,World& w)
{
    do
    {
        if (w.getActiveEnvironment()->queryOptions(p))
            w.queryWolrdOptions();
        w.getActiveEnvironment()->step();
    } while (p->alive());
    
    cout << "You have been killed! " << endl;
    cout << "Final result: " << endl;
    p->printCharacter(true);

}
int main()
{
    unsigned long seed = time(NULL);

    srand(seed);
    cout << "Using seed: " << seed;
    loadItems();
    World w;
    Player p("david",2,3);
    gameLoop(&p,w);
}
