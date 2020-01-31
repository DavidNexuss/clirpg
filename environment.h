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

    inline std::string getName();

    inline void addNPC(NPC c);
    inline void addItem(Item item);

    void step();

    void printEnvironment();

    void meetPeopleMenu(Character* p);
    void queryOptions(Character* p);
};

void attack(Character* a,NPC* other);
void talk(Character* a,NPC* other);