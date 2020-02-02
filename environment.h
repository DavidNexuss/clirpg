#pragma once
#include "character.h"
#include "npc.h"

#include <vector>
class Environment
{
    std::string name;
    std::vector<Item> dropped_items;
    int hardness;

    public:

    std::vector<NPC> npcs;
    
    Environment() : Environment(-1) {}
    Environment(int _hardness);

    std::string getName();

    inline void addNPC(NPC c);
    inline void addItem(Item item);

    int countAlive();
    void step();

    void printEnvironment();

    int getHardness();

    void fightPeopleMenu(Character* p,NPC *a);
    void meetPeopleMenu(Character* p);
    bool queryOptions(Character* p);
};

bool attackNPC(Character* a,NPC* b);
void talkNPC(Character* a,NPC* b);

bool attack(Character* a,Character* b);
void talk(Character* a,Character* b);
