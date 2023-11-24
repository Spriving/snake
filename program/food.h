#ifndef FOOD_H
#define FOOD_H

#include "image.h"
#include <list>
#include <string>

extern int food_num;
extern float food_p1;
extern float food_p2;
extern float food_p3;

class Food
{
public:
    int point; // 0-2,表示1-3分
    int x;
    int y;
    Food(int xx = rand() % block_num_x, int yy = rand() % block_num_y, int pp = rand() % 3) : x(xx), y(yy), point(pp){};
    void draw();
    friend class Foodlist;
};
class FoodList //: public Food
{
public:
    list<Food *> fl;
    int len;
    string update(int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM]);
    FoodList(int _len = 3) : len(_len) {}
    ~FoodList()
    {
        for (auto it = fl.begin(); it != fl.end(); it++)
        {
            delete *it;
        }
    };
};

#endif