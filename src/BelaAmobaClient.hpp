#pragma once

#include <random>
#include "AmobaClient.hpp"

class BelaAmobaClient : public AmobaClient
{

public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount)
    {
        int w = map.size();
        int h = map[0].size();

        Map weights(w, std::vector<int>(h, 0));


        for (int x = 0; x < w; ++x) {
            for (int y = 0; y < h; ++y) {

                int origin = map[x][y];
                if (origin == 0) {
                    continue;
                }
                
                // down
                if (x == 0 || map[x - 1][y] != origin)
                {
                    int countSame = 1;
                    int sameEnd = x + 1;
                    for (; sameEnd < w; ++sameEnd)
                    {
                        if (map[sameEnd][y] != origin)
                            break;

                        ++countSame;
                    }

                    for (int weight = countSame, xPrevs = x - 1; xPrevs >= 0 && map[xPrevs][y] == 0; --xPrevs)
                    {
                        weights[xPrevs][y] += weight;
                        weight /= 2;
                    }

                    for (int weight = countSame, xNexts = sameEnd + 1; xNexts < w && map[xNexts][y] == 0; ++xNexts)
                    {
                        weights[xNexts][y] += weight;
                        weight /= 2;
                    }
                }

                // right
                if (y == 0 || map[x][y - 1] != origin)
                {
                    int countSame = 1;
                    int sameEnd = y + 1;
                    for (; sameEnd < h; ++sameEnd)
                    {
                        if (map[x][sameEnd] != origin)
                            break;

                        ++countSame;
                    }

                    for (int weight = countSame, yPrevs = y - 1; yPrevs >= 0 && map[x][yPrevs] == 0; --yPrevs)
                    {
                        weights[x][yPrevs] += weight;
                        weight /= 2;
                    }

                    for (int weight = countSame, yNexts = sameEnd + 1; yNexts < h && map[x][yNexts] == 0; ++yNexts)
                    {
                        weights[x][yNexts] += weight;
                        weight /= 2;
                    }
                }

                // rightdown
                if (y == 0 || x == 0 || map[x - 1][y - 1] != origin)
                {
                    int countSame = 1;
                    int sameXEnd = x + 1;
                    int sameYEnd = y + 1;
                    for (; sameXEnd < w && sameYEnd < h; ++sameXEnd, ++sameYEnd)
                    {
                        if (map[sameXEnd][sameYEnd] != origin)
                            break;

                        ++countSame;
                    }

                    for (int weight = countSame, xPrevs = x - 1, yPrevs = y - 1; xPrevs >= 0 && yPrevs >= 0 && map[xPrevs][yPrevs] == 0; --xPrevs,--yPrevs)
                    {
                        weights[xPrevs][yPrevs] += weight;
                        weight /= 2;
                    }

                    for (int weight = countSame, xNexts = sameXEnd + 1, yNexts = sameYEnd + 1; xNexts < w && yNexts < h && map[xNexts][yNexts] == 0; ++xNexts, ++yNexts)
                    {
                        weights[xNexts][yNexts] += weight;
                        weight /= 2;
                    }
                }

                // leftdown
                if (y == h - 1 || x == 0 || map[x - 1][y + 1] != origin)
                {
                    int countSame = 1;
                    int sameXEnd = x + 1;
                    int sameYEnd = y - 1;
                    for (; sameXEnd < w && sameYEnd >= 0; ++sameXEnd, --sameYEnd)
                    {
                        if (map[sameXEnd][sameYEnd] != origin)
                            break;

                        ++countSame;
                    }

                    for (int weight = countSame, xPrevs = x - 1, yPrevs = y + 1; xPrevs >= 0 && yPrevs < h && map[xPrevs][yPrevs] == 0; --xPrevs, ++yPrevs)
                    {
                        weights[xPrevs][yPrevs] += weight;
                        weight /= 2;
                    }

                    for (int weight = countSame, xNexts = sameXEnd + 1, yNexts = sameYEnd - 1; xNexts < w && yNexts >= 0 && map[xNexts][yNexts] == 0; ++xNexts, --yNexts)
                    {
                        weights[xNexts][yNexts] += weight;
                        weight /= 2;
                    }
                }
            }
        }

        int maxWeight = weights[0][0];
        std::vector<Position> maxWeights;
        for (int i = 0; i < w; ++i)
        {
            for (int j = 0; j < h; ++j)
            {
                if (maxWeight < weights[i][j])
                {
                    maxWeight = weights[i][j];
                    maxWeights.clear();
                    maxWeights.push_back(Position{ i, j });
                }
                else if (maxWeight == weights[i][j])
                {
                    maxWeights.push_back(Position{ i, j });
                }


            }
        }

        static std::random_device rd;
        static std::mt19937 g(rd());

        return maxWeights[std::uniform_int_distribution<int>(0, maxWeights.size() - 1)(g)];
    }
};