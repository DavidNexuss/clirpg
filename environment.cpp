#include "environment.h"
#include "util.h"
#include "materials.h"
#include <iostream>
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
        l = (int)RANDOM(5) / (l + 1);
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
                        npcs[j].takeDamage(&npcs[i]);
                        npcs[j].proccessEvent(&npcs[i],EVENT_ATTACK);
                    }
            }
    }

    void Environment::printEnvironment()
    {
        cout << "Name: " << name << " Hardness: " << hardness << endl;
        cout << "NPCS: " << npcs.size() << endl;
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

    void Environment::meetPeopleMenu(Character* p)
    {

        for (size_t i = 0; i < npcs.size(); i++)
        {
            cout << "[" << i << "]";
            npcs[i].queryInfo(p);
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
            attack(p,&npcs[response2]);
            break;
        case 2:
            talk(p,&npcs[response2]);
            break;
        case 3:
        default:
            break;
        
        }
    }
    void Environment::queryOptions(Character* p)
    {
        
        p->printCharacter(false);
        cout << "This is the land of the " << name << endl;
        cout << "Seams to be a " << toughness[MIN(hardness,TOUGH_SIZE - 1)] << " place" << endl;
        cout << "You can see " << npcs.size() << " people" << endl;
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
        case 4:
            p->queryInventoryMenu();
            break;
        
        default:
            break;
        }
    }


void attack(Character* a,NPC* other)
{
    cout << a->name << "->" << "attacks " << other->name << endl;
    other->meet(a,0);
    other->proccessEvent(a,EVENT_ATTACK);
    if (other->takeDamage(a)) a->addXP(other->getXP());
    a->addXP(5);
}

void talk(Character* a,NPC* other)
{
    cout << a->name << "->" << "talks " << other->name << endl;
    other->meet(a,RANDOM(30)+5);
    other->proccessEvent(a,EVENT_TALK);
    a->addXP((int)(RANDOM(3)));
}