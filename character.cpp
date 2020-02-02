#include "util.h"
#include "character.h"
#include <iostream>
#include <cmath>
using namespace std;

Character::Character(string _name,double multiplier,int bstrength,int bhealth,int bdefense) : name(_name) {
    
    health = (int)(RANDOM(100) * multiplier + bhealth);
    strength = (int)(RANDOM(5) * multiplier + bstrength);
    defense = (int)(RANDOM(4) * multiplier + bdefense);

    for (size_t i = 0; i < INV_SIZE; i++)
    {
        inventory[i] = nullItem;
    }

    for (size_t j = 0;  j < ARMOR_SLOTS;  j++)
    {
        armor[j] = 0;
    }    
}


void Character::setGround(ground_t* _ground) { ground = _ground; }

int* Character::getArmorSlotes() { return armor; } 

int Character::getLevel() { return level;}
int Character::getXP() { return xp;}

void Character::addXP(int _xp) {xp += _xp; level = (int)sqrt(xp);}

void Character::setFriendly(bool _friendly)
{
    friendly = _friendly;
}
void Character::setActive(int i)
{
    active = i;
}

void Character::setArmor(int i,int a)
{
    armor[a] = i;
}
bool Character::isArmor(int x) 
{
    for (size_t i = 0; i < ARMOR_SLOTS; i++)
    {
        if (x == armor[i]) return true;
    }
    return false;
}
void Character::consumeActive()
{
    if (inventory[active].type == FOOD)
        health += inventory[active].val;
    inventory[active] = nullItem;
}
bool Character::takeDamage(Character* other)
{
    if (!alive()) return false;
    health -= other->getStrength() / getArmor();
    return health <= 0;
}


int Character::getHealth() {return health;}
int Character::getStrength() { 
    if (inventory[active].type != WEAPON) return strength;
    else {
        return strength + inventory[active].val;
    }
}
int Character::getArmor() {
    double armorVal = defense;
    for (int i = 0; i < 4; i++)
        armorVal += inventory[armor[i]].type == ARMOR ? inventory[armor[i]].val / 10.0 : 1;
    return armorVal;
}
bool Character::alive() { return health > 0; }

void Character::kill()
{
    health = 0;
    dropItems();
}
void Character::dropItems()
{
    for (size_t i = 0; i < INV_SIZE; i++)
    {
        dropItem(i);
    }
}

void Character::dropItem(int i)
{
    if (inventory[i].type == 0) return;
    ground->push_back(inventory[i]);
    inventory[i] = nullItem;
}
bool Character::addItem(Item t)
{
    for (int i = 0; i < INV_SIZE; i++)
        if (inventory[i].type == 0){
            inventory[i] = t;
            return false;
        }
    return true;
}


Item& Character::getActive() { return inventory[active]; }

void Character::printCharacter(bool inventory)
{
    cout << "Character: " << name << endl;
    cout << health << " HP " << strength << " SP " << defense << " DP " << level << " LVL " << xp << " XP " << endl;
    cout << "-------------------------------------" << endl;
    cout << getStrength() << " SSP " << getArmor() << " DDP" << endl;
    if(inventory) printInventory();  
}
void Character::printInventory()
{
    for (size_t i = 0; i < INV_SIZE; i++)
    {
        if (inventory[i].type == 0) continue;
        cout << "[" << i << "]";
        cout << "-> ";
        inventory[i].printItem();

        if (i == active) cout << "[ACTIVE] ";
        if (isArmor(i)) cout << "[ARMOR] ";
        cout << endl;
    }
    
}

void Character::addKnowLevel(int a)
{
    know_level += a;
}

int Character::getKnowLevel() 
{ 
    return know_level;
}

void Character::queryInventoryMenu()
{
    
    printCharacter(true);
    cout << "What do you want to do?" << endl;
    cout << "1) Continue" << endl;
    cout << "2) Set active" << endl;
    cout << "3) Set armor" << endl;
    cout << "4) Consume active" << endl;

    int response;
    cin >> response;
    switch (response)
    {
    case 1:
        return;
    case 2:
        cout << "Insert active index:" << endl;
        cin >> response;
        if (response < 0 || response > INV_SIZE || inventory[response].type == 0) return queryInventoryMenu();
        setActive(response);
        cout << "New active: " << endl;
        getActive().printItem(); cout << endl;
        break;
    case 3:
        cout << "Armor slots: " << endl;
        for (size_t i = 0; i < 4; i++)
        {
            cout << i << ")";
            if (inventory[armor[i]].type != 0) inventory[armor[i]].printItem();
            cout << endl;   
        }
        cout << "Insert inventory index" << endl;
        cin >> response;
        if (response < 0 || response > INV_SIZE || inventory[response].type == 0 || isArmor(response)) return queryInventoryMenu();
        cout << "Insert armor slot" << endl;
        int response2;
        cin >> response2;
        if (response2 < 0 || response2 > ARMOR_SLOTS - 1) return queryInventoryMenu();
        setArmor(response,response2);
        break;
    case 4:
        if (getActive().type != FOOD){
             cout << "Active must be food!" << endl;
             return queryInventoryMenu();
        }
        cout << "Eating food..." << endl;
        consumeActive();
        break;
    default:
        cout << "Incorrect answer." << endl;
        return queryInventoryMenu();
    }
}
