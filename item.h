#pragma once
#include <string>
#include <vector>

#define WEAPON 1
#define FOOD 2
#define ARMOR 3
#define MISC 4
#define ITEM_LIST_SIZE 1000

extern std::string itemLiterals[];
extern std::string qualityLiterals[];

struct Item 
{
    int type;
    std::string name;
    int val;
    int quality;
    
    void printItem();
};

typedef std::vector<Item> ground_t;

static Item nullItem = { 0, "", 0, 0}; 

extern std::vector<Item> items;

Item getRandomItem();
Item getHardnessItem(int max);

bool sortItems(const Item& a,const Item &b);
void loadItems();
void printItemList();