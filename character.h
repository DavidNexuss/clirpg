#pragma once
#include "item.h"

#define INV_SIZE 24
#define ARMOR_SLOTS 4
class Character
{
    int active = 0;
   
    int level = 1;
    int xp = 0;

    int know_level = 0;

    int health;
    int strength;
    int defense;

    public:
    
    ground_t* ground;
    std::string name;

    int armor[ARMOR_SLOTS];
    Item inventory[INV_SIZE];
    bool friendly = true;

    Character(std::string _name ,double _multiplier,int _bstrenght,int _bhealth,int _bdefense);

    int getHealth();
    int getStrength();
    int getArmor();
    bool alive();
    void kill();
    bool takeDamage(Character* other);
    
    int getLevel();
    int getXP();
    void addXP(int _xp);
    bool isArmor(int x);
    void setArmor(int i,int a);
    int* getArmorSlotes(); 
    
    void setGround(ground_t* _ground);

    void setFriendly(bool _friendly);
    
    bool addItem(Item t);
    void setActive(int i);
    Item& getActive();
    void consumeActive();
    void dropItems();
    void dropItem(int i);

    void printCharacter(bool inventory);
    void printInventory();
    
    void addKnowLevel(int a);
    int getKnowLevel();

    void queryInventoryMenu();
};