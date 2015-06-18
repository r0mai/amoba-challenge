#include "SampleAmobaClient.hpp"

#include <cstdlib>

Position SampleAmobaClient::getMove(
    const Map& map, int yourColor, int playerCount)
{
    while (true) {
        int x = rand() % map.width() + map.minX();
        int y = rand() % map.height() + map.minY();

        if (map.get(x, y) == 0) {
            return Position{ x, y };
        }
    }
}

std::string SampleAmobaClient::getName() const
{
    return "Sample";
}

