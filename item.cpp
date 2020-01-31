#include "util.h"
#include "item.h"
#include "materials.h"
#include <iostream>
#include <algorithm>
using namespace std;

#define HARDNESS 8

string itemLiterals[] = {"","Weapon","Food","Armor","Miscellania"};
string qualityLiterals[] = {"F","E","D","C","B","A","A+","S","SS","Z","Z+","GOD"};
const int qualityLiteralsSize = sizeof(qualityLiterals) / sizeof(qualityLiterals[0]);
vector<Item> items;
void Item::printItem()
{
      cout << "[" << itemLiterals[type] << "] " << name << " (" << val << ") TIER: " << qualityLiterals[quality] << "   ";

}

Item getRandomItem()
{
    int i = rand() % items.size();
    int v = (rand() % HARDNESS / 2) - HARDNESS / 2;
    Item it = items[i];
    it.val += v;

    double d = rand() % qualityLiteralsSize;
    it.val = it.val * (1 + d / double(qualityLiteralsSize));
    it.quality = (int)d;

    return it;
}
Item getHardnessItem(int max) 
{
    for (size_t i = 0; i < items.size(); i++)
    {
        if (items[i].val > max)
            return items[i - 1];
    }
    return items[items.size() - 1];
}


bool sortItems(const Item &a, const Item &b)
{
    return a.val == b.val ? a.quality < b.quality : a.val < b.val;
}

void loadItems()
{
    for (int i = 0; i < ITEM_LIST_SIZE ; i++)
    {
        Item it;
        it.type = (i % 4) + 1;
        int mat_id = rand() % MATERIALS_SIZE;
        
        double quality = rand() % qualityLiteralsSize;
        it.val = ((rand() % HARDNESS ) * 3 + rand() % HARDNESS) * mat_id * quality / qualityLiteralsSize;
        it.quality = (int) quality;
        
        switch (it.type) {
            case ARMOR:
                it.name = materials[mat_id] + " " + armor_types[rand() % ARMOR_SIZE];
            break;
            case WEAPON:
                it.name = materials[mat_id] + " " + weapon_types[rand() % WEAPONS_SIZE];
            break;
            case FOOD:
                it.name = getRandomName();
            break;
            case MISC:
                it.name = materials[mat_id] + " " + getRandomName();
            break;
            default :
                it.name = getRandomName();
            break;
        }


        items.push_back(it);
    }

    sort(items.begin(), items.end(),sortItems);
}


void printItemList()
{
    for(int i = 0; i < items.size(); i++)
    {
        items[i].printItem(); cout << endl;
    }
}