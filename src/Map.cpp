#include "Map.hpp"

#include <iostream>
#include <string>

int hasNInARow(const Map& map, int n) {
    int w = map.width();
    int h = map.height();

    for (int x = 0; x <= w - n; ++x) {
        for (int y = 0; y < h; ++y) {
            int origin = map.get(map.minX() + x, map.minY() + y);
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map.get(map.minX() + x + i, map.minY() + y) != origin) {
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
            int origin = map.get(map.minX() + x, map.minY() + y);
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map.get(map.minX() + x, map.minY() + y + i) != origin) {
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
            int origin = map.get(map.minX() + x, map.minY() + y);
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map.get(map.minX() + x + i, map.minY() + y + i) != origin) {
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
            int origin = map.get(map.minX() + x, map.minY() + y);
            if (origin == 0) {
                continue;
            }
            int i;
            for (i = 1; i < n; ++i) {
                if (map.get(map.minX() + x - i, map.minY() + y + i) != origin) {
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
    int w = map.width();
    int h = map.height();

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            if (map.get(map.minX() + x, map.minY() + y) == 0) {
                return true;
            }
        }
    }
    return false;
}

void printMap(const Map& map) {
    int w = map.width();
    int h = map.height();

    std::cout << "/" << std::string(w + 2, '-') << "\\" << std::endl << "| ";
    for (int x = 0; x < w; ++x) {
        if (map.minX() + x == 0)
            std::cout << "|";
        else
            std::cout << std::abs((map.minX() + x) % 10);
    }
    std::cout << " |" << std::endl;
    for (int y = 0; y < h; ++y) {
        std::cout << "|";
        if (map.minY() + y == 0)
            std::cout << "-";
        else
            std::cout << std::abs((map.minY() + y) % 10);

        for (int x = 0; x < w; ++x) {
            std::cout << playerToChar(map.get(map.minX() + x, map.minY() + y));
        }

        if (map.minY() + y == 0)
            std::cout << "-";
        else
            std::cout << std::abs((map.minY() + y) % 10);

        std::cout << "|\n";
    }
    
    std::cout << "| ";

    for (int x = 0; x < w; ++x) {
        if (map.minX() + x == 0)
            std::cout << "|";
        else
            std::cout << std::abs((map.minX() + x) % 10);
    }
    std::cout << " |" << std::endl << "\\" << std::string(w + 2, '-') << "/";
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

