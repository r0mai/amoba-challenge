#pragma once

#include <numeric>
#include <array>
#include <iostream>
#include <random>
#include "AmobaClient.hpp"

class BelaAmobaClient : public AmobaClient
{
    struct Info
    {
        Info(int we) :we(we){}
        int we;
        enum class Openness {open = 0, half, closed };
        enum class Direction {right = 0, down, rightdown, leftdown};

        // who, size, distance, openness
        std::array<std::vector<std::tuple<int, int, int, Openness>>, 4> weighting;


        int calculate(const std::tuple < int, int, int, Openness >& tpl) const
        {
            // meret * meret
            // mienk +3
            // openness +2
            // tavolsag /2
            // 
            return (std::get<1>(tpl) * std::get<1>(tpl) +
                (std::get<0>(tpl) == we) +
                (std::get<3>(tpl) == Openness::open)) / 
                std::get<2>(tpl);
        }

        int calculate(const std::vector < std::tuple < int, int, int, Openness > >& tplv) const
        {
            switch (tplv.size())
            {
            case 0: return 0;
            case 1: return calculate(tplv.back());
            case 2:
                if (std::get<0>(tplv[0]) != std::get<0>(tplv[1]))
                {
                    return std::max(calculate(tplv[0]), calculate(tplv[1]));
                }
                else
                {
                    return calculate(tplv[0]) + calculate(tplv[1]);
                }
            default: return 0;
            }
        }

        void setNewWeight(int who, int size, int distance,  Direction d, Openness openness)
        {
            if (distance > 2) return;

            weighting[static_cast<int>(d)].push_back(std::make_tuple(who, size, distance, openness));
        }

        int getWeight() const
        {
            return calculate(*std::max_element(weighting.begin(), weighting.end(),
                [this](const std::vector<std::tuple<int, int, int, Openness>>& tp0, const std::vector<std::tuple<int, int, int, Openness>>& tpl)
                {
                    return calculate(tp0) < calculate(tpl); 
                }));
        }
    };

public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount)
    {
        int w = map.size();
        int h = map[0].size();
        std::vector<std::vector<Info>> weights(w, std::vector<Info>(h, Info(yourColor)));


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

                    Info::Openness op = static_cast<Info::Openness>((x == 0 || map[x - 1][y] != 0) + (sameEnd >= w - 1 || map[sameEnd][y] != 0));
                    for (int xPrevs = x - 1; xPrevs >= 0 && map[xPrevs][y] == 0; --xPrevs)
                    {
                        weights[xPrevs][y].setNewWeight(origin, countSame, std::abs(xPrevs - x), Info::Direction::down, op);
                    }

                    for (int xNexts = sameEnd; xNexts < w && map[xNexts][y] == 0; ++xNexts)
                    {
                        weights[xNexts][y].setNewWeight(origin, countSame, xNexts - sameEnd + 1, Info::Direction::down, op);
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

                    Info::Openness op = static_cast<Info::Openness>((y == 0 || map[x][y - 1] != 0) + (sameEnd >= h - 1 || map[x][sameEnd] != 0));
                    for (int yPrevs = y - 1; yPrevs >= 0 && map[x][yPrevs] == 0; --yPrevs)
                    {
                        weights[x][yPrevs].setNewWeight(origin, countSame, std::abs(yPrevs - y), Info::Direction::right, op);
                    }

                    for (int yNexts = sameEnd; yNexts < h && map[x][yNexts] == 0; ++yNexts)
                    {
                        weights[x][yNexts].setNewWeight(origin, countSame, yNexts - sameEnd + 1, Info::Direction::right, op);
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

                    Info::Openness op = static_cast<Info::Openness>((x == 0 || y == 0 || map[x - 1][y - 1] != 0) + 
                        (sameXEnd >= w - 1 || sameYEnd >= h - 1 || map[sameXEnd + 1][sameYEnd + 1] != 0));
                    for (int xPrevs = x - 1, yPrevs = y - 1; xPrevs >= 0 && yPrevs >= 0 && map[xPrevs][yPrevs] == 0; --xPrevs,--yPrevs)
                    {
                        weights[xPrevs][yPrevs].setNewWeight(origin, countSame, std::abs(yPrevs - y), Info::Direction::rightdown, op);
                    }

                    for (int xNexts = sameXEnd, yNexts = sameYEnd; xNexts < w && yNexts < h && map[xNexts][yNexts] == 0; ++xNexts, ++yNexts)
                    {
                        weights[xNexts][yNexts].setNewWeight(origin, countSame, yNexts - sameYEnd + 1, Info::Direction::rightdown, op);
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

                    Info::Openness op = static_cast<Info::Openness>((x == 0 || sameYEnd >= h - 1 || map[x - 1][sameYEnd + 1] != 0) + 
                        (sameXEnd >= w - 1 || y == 0 || map[sameXEnd + 1][y - 1] != 0));
                    for (int xPrevs = x - 1, yPrevs = y + 1; xPrevs >= 0 && yPrevs < h && map[xPrevs][yPrevs] == 0; --xPrevs, ++yPrevs)
                    {
                        weights[xPrevs][yPrevs].setNewWeight(origin, countSame, yPrevs - y, Info::Direction::leftdown, op);
                    }

                    for (int xNexts = sameXEnd, yNexts = sameYEnd; xNexts < w && yNexts >= 0 && map[xNexts][yNexts] == 0; ++xNexts, --yNexts)
                    {
                        weights[xNexts][yNexts].setNewWeight(origin, countSame, xNexts - sameXEnd + 1, Info::Direction::leftdown, op);
                    }
                }
            }
        }
        int maxWeight = weights[0][0].getWeight();
        std::vector<Position> maxWeights;
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                std::cout << static_cast<char>('.' + weights[i][j].getWeight());
                if (maxWeight < weights[i][j].getWeight())
                {
                    maxWeight = weights[i][j].getWeight();
                    maxWeights.clear();
                    maxWeights.push_back(Position{ i, j });
                }
                else if (maxWeight == weights[i][j].getWeight())
                {
                    maxWeights.push_back(Position{ i, j });
                }
            }
            std::cout << std::endl;
        }

        static std::random_device rd;
        static std::mt19937 g(rd());

        return maxWeights[std::uniform_int_distribution<int>(0, maxWeights.size() - 1)(g)];
    }

    virtual std::string getName() const
    {
        return "Bela";
    }
};