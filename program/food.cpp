#include "food.h"

int food_num = 3;
float food_p1 = 0.4;
float food_p2 = 0.3;
float food_p3 = 0.3;

void Food::draw()
{
    fill_color(x, y, EGERGB(0xff - 50 * point, 0, 0));
}

string FoodList::update(int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM])
{
    string msg = "";
    while (fl.size() < len)
    {
        int xx, yy, pp;
        do
        {
            xx = rand() % block_num_x;
            yy = rand() % block_num_y;
            float r = rand() / float(32768);
            if (r < food_p1)
                pp = 0;
            else if (r < food_p1 + food_p2)
                pp = 1;
            else
                pp = 2;
        } while (map[xx][yy] != SPACE);
        fl.push_back(new Food(xx, yy, pp));
        msg += " F " + to_string(xx) + " " + to_string(yy) + " " + to_string(pp + 1);
    }
    for (auto it = fl.begin(); it != fl.end(); it++)
    {
        (*it)->draw();
    }
    return msg;
}