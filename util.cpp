#include "util.h"
using namespace std;

#define ALPHABET_SIZE ('z' - 'a' + 1)
#define NAME_SIZE 10

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