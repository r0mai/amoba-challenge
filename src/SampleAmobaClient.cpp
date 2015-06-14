#include "SampleAmobaClient.hpp"

#include <cstdlib>

AmobaClient::Position SampleAmobaClient::getMove(
    const Map& map, int yourColor, int playerCount)
{
    while (true) {
        int x = rand() % map.size();
        int y = rand() % map[0].size();

        if (map[x][y] == 0) {
            return {x, y};
        }
    }
}
