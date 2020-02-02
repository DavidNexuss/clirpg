#include "environment.h"
#include "util.h"
#include "materials.h"
#include <iostream>
#include <algorithm>
using namespace std;

Environment::Environment(int _hardness)
{   
    hardness = _hardness;
    if (hardness == -1) return;
    int l = (int)RANDOM(5);
    for (size_t i = 0; i < l; i++)
    {
        addNPC(getRandomNPC(RANDOM(2) > 1,hardness));
    }
    l = (int)RANDOM(5);
    for (size_t i = 0; i < l; i++)
    {
        addItem(getHardnessItem(hardness * 100));
    }
    name = places[MIN((int)RANDOM(hardness),PLACES_SIZE)] + " " + getRandomName();
}

string Environment::getName() { return name; }

void Environment::addNPC(NPC c)
{
    npcs.push_back(c);
    c.ground = &dropped_items;
}

void Environment::addItem(Item item)
{
    dropped_items.push_back(item);
}

void Environment::step()
{
    for (size_t i = 0; i < npcs.size(); i++)
    for (size_t j = 0; j < npcs.size(); j++)
        {
            if (i == j) continue;

                int c = (int)RANDOM(100);
                if (c > 85)
                {
                    npcs[i].meet(&npcs[j],(int)RANDOM(20));
                    npcs[j].meet(&npcs[i],(int)RANDOM(20));
                }
                npcs[i].proccessEvent(&npcs[j],EVENT_NONE);
                if (npcs[i].getRela(&npcs[j]) < -20 * (RANDOM(1) + 1))
                {
                    attackNPC(&npcs[i],&npcs[j]);
                }
        }

        
}
int Environment::countAlive()
{
    int j = 0;
    for (size_t i = 0; i < npcs.size(); i++)
    {
        if (npcs[i].alive())j++;
    }
    return j;
    
}
void Environment::printEnvironment()
{
    cout << "Name: " << name << " Hardness: " << hardness << endl;
    cout << "NPCS: " << countAlive() << endl;
    for (size_t i = 0; i < npcs.size(); i++)
    {
        npcs[i].printCharacter(true);
    }
    cout << "Dropped Items: " << dropped_items.size() << endl;
    for (size_t i = 0; i < dropped_items.size(); i++)
    {
        dropped_items[i].printItem(); cout << endl;
    }
}

void Environment::fightPeopleMenu(Character* p,NPC* a)
{
    if (!p->alive()) return;
    cout << "Fighting " << a->name << endl;
    p->printCharacter(false);
    a->queryInfo(p);
    cout << "Enemy HP: " << a->getHealth() << endl;
    cout << "1) Use "; p->getActive().printItem(); cout << endl;
    cout << "2) Change inventory " << endl;
    cout << "3) Flee " << endl;

    int response;
    cin >> response;
    if (response < 0 || response > 3) return fightPeopleMenu(p,a);

    if (response == 1)
    {
        Item& active = p->getActive();
        if (active.type == MISC || active.type == ARMOR || active.type == 0)
        {
            cout << "Invalid active" << endl;
            return fightPeopleMenu(p,a);
        }

        if (active.type == WEAPON)
        {
            if (attackNPC(p,a)) cout << a->name << " defeated! " << endl;
        }
        else if (active.type == FOOD) p->consumeActive();
    }
    else if (response == 2)
    {
        p->queryInventoryMenu();
    }
    
    if (response != 3 && a->alive())
    {
        a->step(p);
        return fightPeopleMenu(p,a);
    }
}
void Environment::meetPeopleMenu(Character* p)
{

    for (size_t i = 0; i < npcs.size(); i++)
    {
        if (npcs[i].alive())
        {
            cout << "[" << i << "]";
            npcs[i].queryInfo(p);
        }
    }
    cout << "What do you want to do?" << endl;
    cout << "1) Attack" << endl;
    cout << "2) Talk" << endl;
    cout << "3) Trade" << endl;
    cout << "4) Nothing" << endl;

    int response;
    cin >> response;
    while (response <= 0 || response > 4)
    {
        cout << "Invalid option" << endl;
        cin >> response;
    }

    if (response == 4) return;

    int response2;
    cout << "Choose person" << endl;
    cin >> response2;
    while (response2 < 0 || response2 >= npcs.size())
    {
        cout << "Invalid option" << endl;
        cin >> response2;
    }

    switch (response)
    {
    case 1:
        fightPeopleMenu(p,&npcs[response2]);
        break;
    case 2:
        talk(p,&npcs[response2]);
        break;
    case 3:
    default:
        break;
    
    }
}
bool Environment::queryOptions(Character* p)
{
    
    p->printCharacter(false);
    cout << "This is the land of the " << name << endl;
    cout << "Seams to be a " << toughness[MIN(hardness,TOUGH_SIZE - 1)] << " place" << endl;
    cout << "You can see " << countAlive() << " people" << endl;
    cout << "What do you want to do?" << endl;

    cout << "1) Meet people" << endl;
    cout << "2) Look for things" << endl;
    cout << "3) Exit place" << endl;
    cout << "4) Look inventory" << endl;

    int response;
    cin >> response;
    switch (response)
    {
    case 1:
        meetPeopleMenu(p);
    break;
    case 2:
    {
        int r = (int)RANDOM(100);
        if (r > 85 && dropped_items.size() > 0)
        {
            Item it = dropped_items[dropped_items.size() -1];
            if (p->addItem(it))
            {
                cout << "Inventory full" << endl;
            }else{ 
                dropped_items.pop_back();
                cout << "Found: ";
                it.printItem(); cout << endl;
            }
        }
        cout << "Nothing found" << endl;
    }
    break;
    case 3:
        return 1;
    case 4:
        p->queryInventoryMenu();
        break;
    
    default:
        break;
    }
    return 0;
}

int Environment::getHardness() 
{ 
    return hardness; 
}

bool attack(Character* a,Character* b)
{
    cout << a->name << "->" << " attacks " << b->name << endl;
    if (b->takeDamage(a))
    {
         a->addXP(b->getXP());
         return true;
    }
    a->addXP(5); 
    return false;
}
void talk(Character* a,Character* b)
{
    cout << a->name << "->" << " talks " << b->name << endl;
    a->addXP((int)(RANDOM(3)));
}

bool attackNPC(Character* a,NPC* other)
{
    if (attack(a,other)) return true;
    other->meet(a,0);
    other->proccessEvent(a,EVENT_ATTACK);
}

void talkNPC(Character* a,NPC* other)
{
    talk(a,other);
    other->meet(a,RANDOM(30)+5);
    other->proccessEvent(a,EVENT_TALK);
}
