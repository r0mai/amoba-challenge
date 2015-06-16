#pragma once

#include <vector>

using Map = std::vector<std::vector<int>>;

int hasNInARow(const Map& map, int n);
bool hasEmpty(const Map& map);
void printMap(const Map& map);
char playerToChar(int num);
