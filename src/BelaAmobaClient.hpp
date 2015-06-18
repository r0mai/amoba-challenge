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
                    " open: " << (std::get<3>(tpl) == Openness::open) <<
                    " half: " << (std::get<3>(tpl) == Openness::half) <<
                    " weight: " << calculate(tpl) <<
                    std::endl;
            }
            if (std::get<1>(tpl) == 0) 
                return 0;
            // ide mágiázni valamit!!
            return (std::get<1>(tpl) * 4 +
                (std::get<0>(tpl) == we) +
                (std::get<3>(tpl) == Openness::open ? 4 : std::get<3>(tpl) == Openness::half ? 0 : -2)) /
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
                    auto tr0 = tplv[0];
                    auto tr1 = tplv[1];

                    if (std::get<2>(tplv[0]) <= 1)
                    {
                        std::get<3>(tr1) = static_cast<Openness>(1 + static_cast<int>(std::get<3>(tplv[1])));
                    }

                    if (std::get<2>(tplv[1]) <= 1)
                    {
                        std::get<3>(tr0) = static_cast<Openness>(1 + static_cast<int>(std::get<3>(tplv[0])));
                    }

                    return std::max(calculate(tr0, debug), calculate(tr1, debug));
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
            std::transform(weights.begin(), weights.end(), weights.begin(), [](const std::uint8_t& t){return t - t % 2; });
            return std::accumulate(weights.begin(), weights.end(), std::uint32_t(0), [](std::uint32_t res, std::uint8_t num){return (res << 8) + num; });
        }
    };

public:
    virtual Position getMove(const Map& map, int yourColor, int playerCount)
    {
        int w = map.width();
        int h = map.height();
        std::vector<std::vector<Info>> weights(w, std::vector<Info>(h, Info(yourColor, playerCount)));


        for (int y = 1; y < h-1; ++y) {
            if (map.get(map.minX(), map.minY() + y) == 0)
            {
                weights[0][y].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
                weights[0][y].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[0][y].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }
            if (map.get(map.maxX(), map.minY() + y) == 0)
            {
                weights[w - 1][y].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
                weights[w - 1][y].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[w - 1][y].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }
        }
        if (map.get(map.minX(), map.minY()) == 0)
            weights[0][0].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
        if (map.get(map.minX(), map.maxY()) == 0)
            weights[0][h - 1].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
        if (map.get(map.maxX(), map.minY()) == 0)
            weights[w - 1][0].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);
        if (map.get(map.maxX(), map.maxY()) == 0)
            weights[w - 1][h - 1].setNewWeight(-1, 0, 1, Info::Direction::down, Info::Openness::half);

        for (int x = 0; x < w; ++x) {
            if (map.get(map.minX() + x, map.minY()) == 0)
            {
                weights[x][0].setNewWeight(-1, 0, 1, Info::Direction::right, Info::Openness::half);
                weights[x][0].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[x][0].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }
            if (map.get(map.minX() + x, map.maxY()) == 0)
            {
                weights[x][h - 1].setNewWeight(-1, 0, 1, Info::Direction::right, Info::Openness::half);
                weights[x][h - 1].setNewWeight(-1, 0, 1, Info::Direction::leftdown, Info::Openness::half);
                weights[x][h - 1].setNewWeight(-1, 0, 1, Info::Direction::rightdown, Info::Openness::half);
            }

            for (int y = 0; y < h; ++y) {
                int origin = map.get(map.minX() + x, map.minY() + y);
                if (origin == 0) {
                    continue;
                }
                auto lambda = [&](int xvec, int yvec, Info::Direction dir)
                {
                    if (!map.valid(map.minX() + x - xvec, map.minY() + y - yvec) ||
                        map.get(map.minX() + x - xvec, map.minY() + y - yvec) != origin)
                    {
                        int countSame = 1;
                        for (; map.valid(map.minX() + x + countSame * xvec, map.minY() + y + countSame * yvec); ++countSame)
                        {
                            if (map.get(map.minX() + x + countSame * xvec, map.minY() + y + countSame * yvec) != origin)
                            {
                                break;
                            }
                        }
                        Info::Openness op = static_cast<Info::Openness>(
                            (!map.valid(map.minX() + x - xvec, map.minY() + y - yvec) ||
                            map.get(map.minX() + x - xvec, map.minY() + y - yvec) != 0) +
                            (!map.valid(map.minX() + x + countSame * xvec, map.minY() + y + countSame * yvec) ||
                            map.get(map.minX() + x + countSame * xvec, map.minY() + y + countSame * yvec) != 0));


                        for (int prevDist = 1; map.valid(map.minX() + x - prevDist * xvec, map.minY() + y - prevDist * yvec) &&
                            map.get(map.minX() + x - prevDist * xvec, map.minY() + y - prevDist * yvec) == 0; ++prevDist)
                        {
                            weights[x - prevDist * xvec][y - prevDist * yvec].setNewWeight(origin, countSame, prevDist, dir, op);
                        }

                        for (int nextDist = countSame; map.valid(map.minX() + x + nextDist * xvec, map.minY() + y + nextDist * yvec) &&
                            map.get(map.minX() + x + nextDist * xvec, map.minY() + y + nextDist * yvec) == 0; ++nextDist)
                        {
                            weights[x + nextDist * xvec][y + nextDist * yvec].setNewWeight(origin, countSame, nextDist - countSame + 1, dir, op);
                        }
                    }
                };
                lambda(1, 0, Info::Direction::down); // down
                lambda(0, 1, Info::Direction::right); // right
                lambda(1, 1, Info::Direction::rightdown); // rightdown
                lambda(1, -1, Info::Direction::leftdown); // leftdown
            }
        }

        std::uint32_t maxWeight = 0;
        std::vector<Position> maxWeights;
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                std::cout << static_cast<char>('.' + (weights[i][j].getWeight() >> 24) );
                if (maxWeight < weights[i][j].getWeight() && map.get(map.minX() + i, map.minY() + j) == 0)
                {
                    maxWeight = weights[i][j].getWeight();
                    maxWeights.clear();
                    maxWeights.push_back(Position{ i, j });
                }
                else if (maxWeight == weights[i][j].getWeight() && map.get(map.minX() + i, map.minY() + j) == 0)
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
        return Position{ map.minX() + std::get<0>(pos), map.minY() + std::get<1>(pos) };
    }

    virtual std::string getName() const
    {
        return "Bela";
    }
};