#pragma once
#include "character.h"
#include <unordered_map>

#define EVENT_NONE 0
#define EVENT_TALK 1
#define EVENT_ATTACK 2
#define EVENT_TRADE 3

typedef int rela_t;

class NPC : public Character 
{
    std::unordered_map<Character*,rela_t> relas;
    public:
    
    NPC(std::string name,bool _friendly,int hardness); 

    void meet(Character* other,int relationship_value);
    
    int getRela(Character* other);
    int proccessEvent(Character* other, int event);

    void chooseActive();

    void chooseArmor();

    void printNPCStatus();

    void queryInfo(Character* p);
};

NPC getRandomNPC(bool friendly,int hardness);