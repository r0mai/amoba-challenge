#pragma once

#include <functional>
#include <cstdint>
#include <numeric>
#include <array>
#include <iostream>
#include <random>
#include "AmobaClient.hpp"

class BelaAmobaClient : public AmobaClient
{
    struct Info
    {
        Info(int we, int players) :we(we), players(players), need(players + 3){}
        int we;
        int players;
        int need;
        enum class Openness {open = 0, half, closed };
        enum class Direction {right = 0, down, rightdown, leftdown};

        // who, size, distance, openness
        std::array<std::vector<std::tuple<int, int, int, Openness>>, 4> weighting;

        int calculate(const std::tuple < int, int, int, Openness >& tpl, bool debug = false) const
        {
            // meret * meret
            // mienk +3
            // openness +2
            // tavolsag /2
            // 
            if (debug)
            {
                std::cerr << "we: " << (std::get<0>(tpl) == we) << 
                    " size: " << std::get<1>(tpl) <<
                    " distance: " << std::get<2>(tpl) << 
                    " openness: " << (std::get<3>(tpl) == Openness::open) << 
                    " weight: " << calculate(tpl) <<
                    std::endl;
            }
            if (std::get<1>(tpl) == 0) 
                return 0;
            // ide mágiázni valamit!!
            return (std::get<1>(tpl) * 5 +
                (std::get<0>(tpl) == we ? 1 : 0) +
                (std::get<3>(tpl) == Openness::open ? 0 : -2)) /
                (std::get<1>(tpl) > 2 ? std::get<2>(tpl) : 1);
        }

        int calculateS(const std::vector < std::tuple < int, int, int, Openness > >& tplv, bool debug = 0) const
        {
            if (debug)
            {
                std::cerr << "DEBUG " << tplv.size() << std::endl;
            }

            switch (tplv.size())
            {
            case 0: return 0;
            case 1: return calculate(tplv.back(), debug);
            case 2:
                if (std::get<0>(tplv[0]) != std::get<0>(tplv[1]))
                {
                    if (std::get<3>(tplv[0]) != Openness::open && std::get<3>(tplv[1]) != Openness::open &&
                        std::get<2>(tplv[0]) <= 1 && std::get<2>(tplv[1]) <= 1 &&
                        std::get<1>(tplv[0]) + 1 < need && std::get<1>(tplv[1]) + 1 < need)
                    {
                        return 0;
                    }
                    return std::max(calculate(tplv[0], debug), calculate(tplv[1], debug));
                }
                else
                {
                    if (std::get<3>(tplv[0]) != Openness::open && std::get<3>(tplv[1]) != Openness::open &&
                        std::get<2>(tplv[0]) <= 1 && std::get<2>(tplv[1]) <= 1 &&
                        std::get<1>(tplv[0]) + std::get<1>(tplv[1]) + 1 < need)
                    {
                        return 0;
                    }
                    auto cp = tplv[0];
                    std::get<1>(cp) += std::get<1>(tplv[1]);
                    std::get<2>(cp) += std::get<2>(tplv[1]) - 1;
                    std::get<3>(cp) = static_cast<Openness>(static_cast<int>(std::get<3>(cp)) + static_cast<int>(std::get<3>(tplv[1])));

                    return calculate(cp, debug);
                }
            default: return 0;
            }
        }

        void setNewWeight(int who, int size, int distance,  Direction d, Openness openness)
        {
            if (distance > 2) return;

            weighting[static_cast<int>(d)].push_back(std::make_tuple(who, size, distance, openness));
        }

        std::uint32_t getWeight(bool debug = 0) const
        {
            std::vector<std::uint8_t> weights(4);
            std::transform(weighting.begin(), weighting.end(), weights.begin(), std::bind(&Info::calculateS, this, std::placeholders::_1, debug));
            std::sort(weights.begin(), weights.end(), std::greater<std::uint8_t>());
            return std::accumulate(weights.begin(), weights.end(), std::uint32_t(0), [](std::uint32_t res, std::uint8_t num){return (res << 8) + num; });
        }
    };

public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount)
    {
        int w = map.size();
        int h = map[0].size();
        std::vector<std::vector<Info>> weights(w, std::vector<Info>(h, Info(yourColor, playerCount)));


        for (int y = 1; y < h-1; ++y) {
            if (map[0][y] == 0)
            {
                weights[0][y].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
                weights[0][y].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[0][y].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }
            if (map[w-1][y] == 0)
            {
                weights[w - 1][y].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
                weights[w - 1][y].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[w - 1][y].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }
        }
        if (map[0][0] == 0)
            weights[0][0].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
        if (map[0][h - 1] == 0)
            weights[0][h - 1].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
        if (map[w - 1][0] == 0)
            weights[w - 1][0].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
        if (map[w - 1][h - 1] == 0)
            weights[w - 1][h - 1].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);

        for (int x = 0; x < w; ++x) {
            if (map[x][0] == 0)
            {
                weights[x][0].setNewWeight(-1, 0, 1, Info::Direction::right, Info::Openness::half);
                weights[x][0].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[x][0].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }
            if (map[x][h-1] == 0)
            {
                weights[x][h - 1].setNewWeight(-1, 0, 1, Info::Direction::right, Info::Openness::half);
                weights[x][h - 1].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[x][h - 1].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }

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
        std::uint32_t maxWeight = weights[0][0].getWeight();
        std::vector<Position> maxWeights;
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                std::cout << static_cast<char>('.' + (weights[i][j].getWeight() >> 24) );
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

        auto pos = maxWeights[std::uniform_int_distribution<int>(0, maxWeights.size() - 1)(g)];
        std::cerr << "Position: " << std::get<0>(pos) << ", " << std::get<1>(pos) << std::endl;
        weights[std::get<0>(pos)][std::get<1>(pos)].getWeight(true);
        return pos;
    }

    virtual std::string getName() const
    {
        return "Bela";
    }
};