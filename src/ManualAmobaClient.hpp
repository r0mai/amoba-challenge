#pragma once

#include <string>
#include <iostream>
#include "AmobaClient.hpp"

class ManualAmobaClient : public AmobaClient
{
    std::string name;
public:
    ManualAmobaClient()
    {
        std::cout << "Please add your name (manual player): ";
        std::getline(std::cin, name);
    }
    virtual Position getMove(const Map& map, int yourColor, int playerCount)
    {
        int x, y;
        std::cout << "Next is manual, " << yourColor << ". player, aka " << name << " turn." << std::endl;
        do
        {
            std::cout << "Your move is: ";
            std::cin >> x >> y;
        } while (!map.valid(x, y) || map.get(x, y) != 0);
        return Position{ x, y };
    }
    virtual std::string getName() const
    {
        return name;
    };

};