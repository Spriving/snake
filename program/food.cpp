#include "food.h"

int food_num = 3;
float food_p1 = 0.4;
float food_p2 = 0.3;
float food_p3 = 0.3;

void Food::draw()
{
    fill_color(x, y, EGERGB(0xff - 50 * point, 0, 0));
    // fill_color(x, y, RED);
}

void FoodList::update(int map[block_num_x][block_num_y])
{
    while (fl.size() < len)
    {
        int xx, yy;
        do
        {
            xx = rand() % block_num_x;
            yy = rand() % block_num_y;
        } while (map[xx][yy] != SPACE);
        fl.push_back(new Food(xx, yy));
    }
    for (auto it = fl.begin(); it != fl.end(); it++)
    {
        (*it)->draw();
    }
}