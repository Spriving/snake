#include "snake.h"

void Snake::init() // TODO完成Snake的Init()函数
{
    body.clear();
    body.push_back(new Node());
    direc = DOWN;
    add();
}
void Snake::add()
{
    int new_x, new_y;
    if (body.size() == 0) // 如果蛇身长度为0，随机生成
    {
        body.push_back(new Node());
    }
    else if (body.size() == 1) // 如果蛇身长度为1，在前进方向的反方向生成
    {
        int dx = 0, dy = 0;
        switch (direc)
        {
        case UP:
            dy = 1;
            break;
        case RIGHT:
            dx = -1;
            break;
        case DOWN:
            dy = -1;
            break;
        case LEFT:
            dx = 1;
            break;
        }
        new_x = body.back()->x + dx;
        int new_y = body.back()->y + dy;
    }
    else // 如果蛇身长度>1，根据最后两个Node的相对位置确定
    {
        new_x = 2 * body.back()->x;
        new_y = 2 * body.back()->y;
        auto it = body.end();
        it--;
        new_x -= (*it)->x;
        new_y -= (*it)->y;
    }
    restrict_xy(new_x, new_y);
    body.push_back(new Node(new_x, new_y, body.size()));
}

void Snake::draw()
{
    for (auto it = body.begin(); it != body.end(); it++)
    {
        fill_color((*it)->x, (*it)->y, GREEN);
    }
}

void Snake::move()
{
    char dx = 0, dy = 0;
    switch (direc)
    {
    case UP:
        dy = -1;
        break;
    case RIGHT:
        dx = 1;
        break;
    case DOWN:
        dy = 1;
        break;
    case LEFT:
        dx = -1;
        break;
    }
    int new_x = body.front()->x + dx;
    int new_y = body.front()->y + dy;
    restrict_xy(new_x, new_y);
    Node *temp = new Node(new_x, new_y, -1);
    body.insert(body.begin(), new Node(*temp));
    delete temp;
    for (auto it = body.begin(); it != body.end(); it++) // 更新位置信息
    {
        (*it)->pos++;
    }
    body.pop_back(); // 删去最后一个结点
}

int Snake::judge(int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM])
{
    /*判断蛇移动后当前的状态*/
    int s_x = body.front()->x;
    int s_y = body.front()->y;
    int res = SPACE;
    switch (map[s_x][s_y])
    {
    case BARRIER: // 如果是蛇或障碍物，返回记号，结束游戏
        res = BARRIER;
        break;
    case FOOD: // 如果是Food，返回记号及食物坐标
        res = FOOD;
        break;
    }
    return res;
}

int Snake::search(int _x, int _y)
{
    for (auto it = body.begin(); it != body.end(); it++)
    {
        if ((*it)->x == _x && (*it)->y == _y)
            return (*it)->pos;
    }
    return -1;
}