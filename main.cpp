#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>
#include "materials.h"

#define WEAPON 1
#define FOOD 2
#define ARMOR 3
#define MISC 4

#define INV_SIZE 24
#define ARMOR_SLOTS 4
#define ALPHABET_SIZE ('z' - 'a' + 1)
#define HARDNESS 8
#define NAME_SIZE 10
#define ITEM_LIST_SIZE 1000

#define EVENT_NONE 0
#define EVENT_TALK 1
#define EVENT_ATTACK 2
#define EVENT_TRADE 3

#define RANDOM(x) ((rand() % 100) * x / 100.0)
#define WORLD_SIZE 10

#define MIN(a,b) ( a < b ? a : b )
#define MAX(a,b) ( a > b ? a : b )

using namespace std;

const string itemLiterals[] = {"","Weapon","Food","Armor","Miscellania"};
const string qualityLiterals[] = {"F","E","D","C","B","A","A+","S","SS","Z","Z+","GOD"};
const int qualityLiteralsSize = sizeof(qualityLiterals)/sizeof(qualityLiterals[0]);

string getRandomName()
{
    int l = (rand() % NAME_SIZE) + 3;
    char vowels[] = { 'a', 'e', 'i', 'o', 'u' };
    int vowelsSize = 5;
    
    char name[l + 1];
    for (int i = 0; i < l; i++)
    {
        char c = (rand() % ALPHABET_SIZE) + 'a';
        if (i % 2)
            while (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') c = (rand() % ALPHABET_SIZE) + 'a';
        else c = vowels[rand() % 5];
        name[i] = c;
    }
    name[l] = '\0';
    return string(name);
}
struct Item 
{

    static vector<Item> items;
    int type;
    string name;
    int val;
    int quality;
    
    void printItem()
    {
        cout << "[" << itemLiterals[type] << "] " << name << " (" << val << ") TIER: " << qualityLiterals[quality] << "   ";

    }

};
typedef vector<Item> ground_t;

Item nullItem = { 0, "", 0, 0}; 
vector<Item> items;

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

class Character
{
    int active = 0;
   
    int health;
    int strength;
    int defense;

    int level = 1;
    int xp = 0;

    int know_level = 0;

    public:
    
    ground_t* ground;
    string name;
    int armor[ARMOR_SLOTS];
    Item inventory[INV_SIZE];
    bool friendly = true;
    
    Character(string _name,double multiplier,int bstrength,int bhealth,int bdefense){
        
        name = _name;
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
    
    
    void setGround(ground_t* _ground) { ground = _ground; }

    int* getArmorSlotes() { return armor; } 

    int getLevel() { return level;}
    int getXP() { return xp;}
    
    void addXP(int _xp) {xp += _xp; level = (int)sqrt(xp);}

    void setFriendly(bool _friendly)
    {
        friendly = _friendly;
    }
    void setActive(int i)
    {
        active = i;
    }

    void setArmor(int i,int a)
    {
        armor[a] = i;
    }
    bool isArmor(int x) 
    {
        for (size_t i = 0; i < ARMOR_SLOTS; i++)
        {
            if (x == armor[i]) return true;
        }
        return false;
    }
    void consumeActive()
    {
        if (inventory[active].type == FOOD)
            health += inventory[active].val;
        inventory[active] = nullItem;
    }
    bool takeDamage(Character* other)
    {
        if (!alive()) return false;
        health -= other->getStrength() / getArmor();
        return health <= 0;
    }
    
    int getHealth() { return health; }
    int getStrength() { 
        if (inventory[active].type != WEAPON) return strength;
        else {
            return strength + inventory[active].val;
        }
    }
    int getArmor() {
        double armorVal = defense;
        for (int i = 0; i < 4; i++)
            armorVal += inventory[armor[i]].type == ARMOR ? inventory[armor[i]].val / 10.0 : 1;
        return armorVal;
    }
    bool alive() {return health > 0;}

    void kill()
    {
        health = 0;
        dropItems();
    }
    void dropItems()
    {
        for (size_t i = 0; i < INV_SIZE; i++)
        {
            dropItem(i);
        }
    }

    void dropItem(int i)
    {
        if (inventory[i].type == 0) return;
        ground->push_back(inventory[i]);
        inventory[i] = nullItem;
    }
    bool addItem(Item t)
    {
        for (int i = 0; i < INV_SIZE; i++)
            if (inventory[i].type == 0){
                inventory[i] = t;
                return false;
            }
        return true;
    }


    Item& getActive() { return inventory[active]; }

    void printCharacter(bool inventory)
    {
        cout << "Character: " << name << endl;
        cout << health << " HP " << strength << " SP " << defense << " DP " << level << " LVL " << xp << " XP " << endl;
        cout << "-------------------------------------" << endl;
        cout << getStrength() << " SSP " << getArmor() << " DDP" << endl;
        if(inventory) printInventory();  
    }
    void printInventory()
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

    void addKnowLevel(int a)
    {
        know_level += a;
    }

    inline int getKnowLevel() { return know_level;}

    void queryInventoryMenu()
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
};

class NPC : public Character {

    unordered_map<Character*,int> relas;

    public:
    NPC(string name,bool _friendly,int hardness) : 
    Character(name,RANDOM(1) + 1.3, 100 * hardness * RANDOM(1) + 40 * RANDOM(1) + 10, hardness * RANDOM(1) + 2,hardness * RANDOM(1) + 1) 
    {        
        setFriendly(_friendly);
    }

    void meet(Character* other,int relationship_value)
    {
        if (relas.count(other) >= 0) return;
        relas[other] = relationship_value;
    }
    
    int getRela(Character* other){ return relas[other];}
    int proccessEvent(Character* other, int event)
    {
        if (!alive()) return 0;
        if (relas.count(other) <= 0) return 0;

        int relationship_value = relas[other];
        
        switch (event) {
            case EVENT_NONE:
                if (friendly && other->friendly && relationship_value < 10 * RANDOM(1) + 1) relationship_value++;
            break;
            case EVENT_TALK:
                if (friendly)
                {
                    if (other->friendly)relationship_value+=5;
                    else relationship_value += 2;
                } else {
                    if (other->friendly)relationship_value -= 2;
                    else relationship_value -= 4;
                }
            break;
            case EVENT_ATTACK:
                relationship_value -= 30;
            break;
            case EVENT_TRADE:
                if (friendly)relationship_value += 6;
                else return 1;
            default:
                relationship_value += 1;
                break;
        }

        int diff = relationship_value - relas[other];
        cout << name << "->" << " relationship with " << other->name 
        << (diff > 0 ? " incremented" : " decremented") << " by" << diff << endl;
        relas[other] = relationship_value;
        return 0;
    }

    void chooseActive()
    {
        if (getHealth() < (RANDOM(30) + 10))
        {
            int food = 0;
            for (size_t i = 0; i < INV_SIZE; i++)
            {
                if (inventory[i].type == FOOD && inventory[i].val > inventory[food].val)
                    food = i;   
            }
            setActive(food);
        }
        else
        {
            int weapon = 0;
            for (size_t i = 0; i < INV_SIZE; i++)
            {
                if (inventory[i].type == WEAPON && inventory[i].val > inventory[weapon].val)
                    weapon = i;
            }
            setActive(weapon);
        }

        cout << name << "->" << " is handing "; getActive().printItem(); cout << endl;
    }

    void chooseArmor()
    {
        vector<int> tmp_armor(4,0);
        int j = 0;
        for (size_t i = 0; i < INV_SIZE; i++)
        {
            if (inventory[i].type == ARMOR && inventory[i].val > tmp_armor[j])
            {
                tmp_armor[j] = i;
                j++;
            }
            if (j == ARMOR_SLOTS) break; 
        }
        
        for (size_t i = 0; i < ARMOR_SLOTS; i++)
        {
            setArmor(tmp_armor[i],i);
        }
        
    }

    void printNPCStatus()
    {
        cout << "friendly " << friendly << endl;
    }

    void queryInfo(Character* p)
    {
        
        int ratio = getStrength() / p->getStrength();
        cout << "Known information of " + name << endl;
        int know = getKnowLevel();
        if (know < 2){

            cout << "Seams to be a " << toughness[MIN(ratio,TOUGH_SIZE - 1)] << " guy" << endl; 
            cout << "I don't know! " << endl;
            if (know == 1) cout << "Seams to be " << (friendly ? "friendly" : "unfriendly") << " though." << endl;
        }

        if (know == 2)
        {
            printCharacter(false);
            getActive().printItem(); cout << endl;
        }

        if (know >= 3)
        {
            printCharacter(true);
        }
    }
};

NPC getRandomNPC(bool friendly,int hardness)
{
    NPC npc(getRandomName(),friendly,hardness);

    int l = hardness * (RANDOM(6) * RANDOM(5) + 3);
    l = MIN(l,INV_SIZE);

    for (int i = 0; i < l; i++)
    {
        npc.inventory[i] = getHardnessItem(hardness + RANDOM(200));
    }

    npc.chooseActive();
    npc.chooseArmor();

    return npc;
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

class Environment 
{

    vector<Item> dropped_items;
    string name;
    int hardness;

    public:
    vector<NPC> npcs;
    Environment() : Environment(-1) {}
    Environment(int _hardness)
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

    string getName() { return name; }

    void addNPC(NPC c)
    {
        npcs.push_back(c);
        c.ground = &dropped_items;
    }

    void addItem(Item item)
    {
        dropped_items.push_back(item);
    }

    void step()
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

    void printEnvironment()
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

    void meetPeopleMenu(Character* p)
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
    void queryOptions(Character* p)
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
};

class World 
{ 
    vector<vector<Environment>> world_matrix;
    Environment* active_environment;
    
    void generateEnvironment(int i, int j)
    {
        Environment e((int)((i + 1) * (j + 1) * RANDOM(1)));
        world_matrix[i][j] = e;
    }
    public:
    

    World() : world_matrix(WORLD_SIZE,vector<Environment>(WORLD_SIZE)){
        for (int i = 0; i < WORLD_SIZE; i++)
            for (int j = 0; j < WORLD_SIZE; j++)
                generateEnvironment(i,j);

        active_environment = &world_matrix[0][0];
    }

    inline Environment* getActiveEnvironment(){ return active_environment;}
    void setActiveEnvironment(int i, int j)
    {
        active_environment = &world_matrix[i][j];
    }
    void stepCurrentEnvironment()
    {
        active_environment->step();
    }
    void printActiveEnvironment()
    {
        cout << "environment: " << endl;
        active_environment->printEnvironment(); 
    }

    void printWorld()
    {
        cout << "Printing world" << endl;
        for (size_t i = 0; i < WORLD_SIZE; i++)
        for (size_t j = 0; j < WORLD_SIZE; j++)
            world_matrix[i][j].printEnvironment();
    }
};
class Player : public Character {
    
    int x = 0,y = 0;
    World* world;
    public:
        Player(string name,int _x,int _y,World* _world) : Character(name,0.5,30,2,3)
        {
            world = _world;
            addItem(getHardnessItem(100));
            int l = (int)(RANDOM(3) + 1);
            for (size_t i = 0; i < l; i++)
            {
                addItem(getHardnessItem(100 + RANDOM(100)));
            }
        };
};


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


void gameLoop(Player *p,World& w)
{
    do
    {
        w.getActiveEnvironment()->queryOptions(p);
    } while (p->alive());
    
}
int main()
{
    srand((unsigned)time(NULL));
    loadItems();
    printItemList();
    World w;
    Player p("david",2,3,&w);
    gameLoop(&p,w);
}
