#ifndef FOOD_H
#define FOOD_H

#include "image.h"
#include <list>

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
    Food(int xx = rand() % block_num_x, int yy = rand() % block_num_y) : x(xx), y(yy)
    {
        float r = rand() / float(32768);
        if (r < food_p1)
            point = 0;
        else if (r < food_p1 + food_p2)
            point = 1;
        else
            point = 2;
    };
    void draw();
    friend class Foodlist;
};
class FoodList //: public Food
{
public:
    list<Food *> fl;
    int len;
    void update(int map[block_num_x][block_num_y]); // TODO:只能在空地生成food
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