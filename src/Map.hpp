#pragma once

#include <algorithm>
#include <vector>
#include <unordered_map>


using Coordinate = int;
using Position = std::tuple<Coordinate, Coordinate>;

class Map
{
public:
    virtual Coordinate minX() const = 0;
    virtual Coordinate minY() const = 0;
    virtual Coordinate maxX() const = 0;
    virtual Coordinate maxY() const = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void set(Coordinate x, Coordinate y, int value) = 0;
    virtual int get(Coordinate x, Coordinate y) const = 0;
    virtual bool valid(Coordinate x, Coordinate y) const final {
        return minX() <= x && x <= maxX() && minY() <= y && y <= maxY();
    }
    virtual bool valid(Position p) const final {
        return valid(std::get<0>(p), std::get<1>(p));
    }
};

class BoundedMap : public Map, std::vector < std::vector<int> >
{
    using Vector = std::vector < int > ;
    using Matrix = std::vector < Vector >;
public:
    BoundedMap(int width, int height) : Matrix(width, Vector(height)){}

    Coordinate minX() const override { return 0; }
    Coordinate minY() const override { return 0; }
    Coordinate maxX() const override { return width() - 1; }
    Coordinate maxY() const override { return height() - 1; }

    int width() const override { return this->size(); }
    int height() const override { return (*this)[0].size(); }

    void set(Coordinate x, Coordinate y, int value) override {
        (*this)[x][y] = value;
    }

    int get(Coordinate x, Coordinate y) const override {
        return (*this)[x][y];
    }
};

template<class Key, class T, class Hash = std::hash<Key>, class EqTo = std::equal_to<Key> >
class OverindexMap : std::unordered_map<Key, T, Hash, EqTo>
{
    const T tmp;
public:
    template<class... Ts>
    OverindexMap(Ts&&... params) :tmp(std::forward<Ts>(params)...) {}

    using std::unordered_map<Key, T, Hash, EqTo>::operator[];
    
    const T& operator[](const Key& key) const {
        auto it = this->find(key);
        if (it == this->end())
            return tmp;
        
        return it->second;
    }
};

class InfiniteMap : public Map, OverindexMap < Coordinate, OverindexMap<Coordinate, int> > {
    int needToWin;
    Coordinate xMin, yMin, xMax, yMax;
public:
    InfiniteMap(int needToWin) : needToWin(needToWin), xMin(-needToWin), yMin(-needToWin), xMax(needToWin), yMax(needToWin){}

    Coordinate minX() const override { return xMin; }
    Coordinate minY() const override { return yMin; }
    Coordinate maxX() const override { return xMax; }
    Coordinate maxY() const override { return yMax; }

    int width() const override { return xMax - xMin + 1; }
    int height() const override { return yMax - yMin + 1; }

    void set(Coordinate x, Coordinate y, int value) override {
        (*this)[x][y] = value;
        xMin = std::min(xMin, x - needToWin);
        yMin = std::min(yMin, y - needToWin);
        xMax = std::max(xMax, x + needToWin);
        yMax = std::max(yMax, y + needToWin);
    }

    int get(Coordinate x, Coordinate y) const override {
        return (*this)[x][y];
    }
};

int hasNInARow(const Map& map, int n);
bool hasEmpty(const Map& map);
void printMap(const Map& map);

char playerToChar(int num);
