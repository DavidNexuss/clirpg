#pragma once
#include <string>

#define RANDOM(x) ((rand() % 100) * x / 100.0)
#define MIN(a,b) ( a < b ? a : b )
#define MAX(a,b) ( a > b ? a : b )
std::string getRandomName();