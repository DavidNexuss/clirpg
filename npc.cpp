#include "npc.h"
#include "materials.h"
#include "util.h"
#include <iostream>
using namespace std;
NPC::NPC(string name,bool _friendly,int hardness) : 
    Character(name,RANDOM(1) + 1.3, 100 * hardness * RANDOM(1) + 40 * RANDOM(1) + 10, hardness * RANDOM(1) + 2,hardness * RANDOM(1) + 1) 
    {        
        setFriendly(_friendly);
    }

    void NPC::meet(Character* other,int relationship_value)
    {
        if (relas.count(other) >= 0) return;
        relas[other] = relationship_value;
    }
    
    int NPC::getRela(Character* other) { return relas[other];}
    int NPC::proccessEvent(Character* other, int event)
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

    void NPC::chooseActive()
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

    void NPC::chooseArmor()
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

    void NPC::printNPCStatus()
    {
        cout << "friendly " << friendly << endl;
    }

    void NPC::queryInfo(Character* p)
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