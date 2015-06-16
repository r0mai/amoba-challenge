#include "Map.hpp"

#include <iostream>
#include <string>

int hasNInARow(const Map& map, int n) {
    int w = map.size();
    int h = map[0].size();

    for (int x = 0; x <= w - n; ++x) {
        for (int y = 0; y < h; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x+i][y] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y <= h - n; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x][y+i] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    for (int x = 0; x <= w - n; ++x) {
        for (int y = 0; y <= h - n; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x+i][y+i] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    for (int x = n-1; x < w; ++x) {
        for (int y = 0; y <= h - n; ++y) {
            int origin = map[x][y];
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map[x-i][y+i] != origin) {
                    break;
                }
            }
            if (i == n) {
                return origin;
            }
        }
    }

    return 0;
}

bool hasEmpty(const Map& map) {
    int w = map.size();
    int h = map[0].size();

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (map[x][y] == 0) {
                return true;
            }
        }
    }
    return false;
}

void printMap(const Map& map) {
    int w = map.size();
    int h = map[0].size();

    std::cout << std::string(w, '-') << std::endl;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            std::cout << playerToChar(map[x][y]);
        }
        std::cout << '\n';
    }
    std::cout << std::string(w, '-');
    std::cout << std::endl;
}

char playerToChar(int num)
{
    switch (num) {
    case 0: return '.';
    case 1: return 'X';
    case 2: return 'O';
    default: return num + '0';
    }
}

