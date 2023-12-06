#include "a_star.h"

int abs(int x)
{
    return x >= 0 ? x : -x;
}

inline int m_distance(int x1, int y1, int x2, int y2) // 曼哈顿距离//TODO未考虑虚墙情况
{                                                     // 1->蛇头，2->食物
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    if (!wall[0] && wall[1]) // 如果左右两边没有墙且上下有墙
    {
        if (x1 + block_num_x - x2 < dx)
        {
            dx = x1 + block_num_x - x2;
        }
        if (x2 + block_num_x - x1 < dx)
        {
            dx = x2 + block_num_x - x1;
        }
    }
    else if (!wall[1] && wall[0]) // 如果上下没有墙且左右有墙
    {
        if (y1 + block_num_y - y2 < dy)
        {
            dy = y1 + block_num_y - y2;
        }
        if (y2 + block_num_y - y1 < dy)
        {
            dy = y2 + block_num_y - y1;
        }
    }
    else if (!wall[0] && !wall[1])
    {
        if (x1 + block_num_x - x2 < dx)
        {
            dx = x1 + block_num_x - x2;
        }
        if (x2 + block_num_x - x1 < dx)
        {
            dx = x2 + block_num_x - x1;
        }
        if (y1 + block_num_y - y2 < dy)
        {
            dy = y1 + block_num_y - y2;
        }
        if (y2 + block_num_y - y1 < dy)
        {
            dy = y2 + block_num_y - y1;
        }
    }
    return dx + dy;
}

int dg(int x, int y, int st, Snake sn)
{
    if (sn.search(x, y)) // 如果有蛇,且步数大于尾长.视为空地
    {
        if (sn.len() - sn.search(x, y) < st)
        {
            return 1;
        }
    }
    if (map[x][y] == BARRIER)
    {
        return 10000;
    }
    else
    {
        return 1;
    }
}

int h(int x, int y, FoodList fd) // 取曼哈顿距离最小的食物作为目标
{
    int min_distance = 1000;
    for (auto it = fd.fl.begin(); it != fd.fl.end(); it++)
    {
        if (m_distance(x, y, (*it)->x, (*it)->y) < min_distance)
        {
            min_distance = m_distance(x, y, (*it)->x, (*it)->y);
        }
    }
    return min_distance;
}

int f(int gg, int hh)
{
    return -gg - 3 * hh;
}
class Point
{
public:
    int x;
    int y;
    int g;
    int f; // 搜索优先级
    int step;
    Point *parent;
    Point(int xx = 0, int yy = 0, int gg = 0, int ff = 0, Point *p = NULL, int st = 0) : x(xx), y(yy), g(gg), f(ff), step(st), parent(p){};
    Point(const Point &pp)
    {
        x = pp.x;
        y = pp.y;
        f = pp.f;
        g = pp.g;
        parent = pp.parent;
    }
    bool operator<(const Point &p) const;
    bool operator>(const Point &p) const;
};
bool Point::operator<(const Point &p) const
{
    return f < p.f;
}
bool Point::operator>(const Point &p) const
{
    return f > p.f;
}
// TODO
Direction a_star_search(int x, int y, FoodList fd, Snake sn)
{
    priority_queue<Point> pq;                         // 创建边缘集
    bool searched[MAX_BLOCK_NUM][MAX_BLOCK_NUM] = {}; // 创建探索集
    Point points[MAX_BLOCK_NUM][MAX_BLOCK_NUM] = {};
    int fs[MAX_BLOCK_NUM][MAX_BLOCK_NUM] = {};
    bool flag = false; // 是否找到最佳路径
    bool is_constant = true;
    Point *cur = NULL;
    points[x][y] = Point(x, y, 0, f(0, h(x, y, fd))); // 蛇头处为起点
    pq.push(points[x][y]);
    // pq.push(start); // 将蛇头位置加入优先级队列
    while (flag == false)
    {
        if (pq.empty())
        {
            return STAY; // 未搜索到目标
        }
        cur = &points[pq.top().x][pq.top().y];
        pq.pop();
        ////////////////////
        // cout << "x=" << cur->x << " y=" << cur->y << " g=" << cur->g << " h=" << h(cur->x, cur->y, fd) << " f=" << cur->f << endl;
        if (h(cur->x, cur->y, fd) == 0)
        {
            // cout << "des_x=" << cur->x << " des_y=" << cur->y << endl;
            flag = true; // 找到苹果后将flag置为true
            if (cur->f < -1000)
            {
                is_constant = false; // 不连通
            }
            break;
        }
        // if (map[cur->x][cur->y] == BARRIER)
        // {
        //     continue;
        // }
        for (int i = -1; i <= 1; i += 2) // 若没找到，将周围的四个点加入边缘集
        {
            for (int j = 0; j <= 1; j++)
            {
                int xx = cur->x + i * j;       // 0,-1,0,1
                int yy = cur->y + i * (j - 1); // 1,0,-1,0
                restrict_xy(xx, yy);           // 将出界的格子限制到另一端
                // if (out_of_bound(xx, yy))
                // {
                //     continue;
                // }
                if (!searched[xx][yy]) // 如果没有搜寻过
                {
                    int gg = cur->g + dg(xx, yy, points[xx][yy].step, sn);
                    int hh = h(xx, yy, fd);
                    int ff = f(gg, hh);
                    points[xx][yy] = Point(xx, yy, gg, ff, &points[cur->x][cur->y], points[xx][yy].step + 1);
                    pq.push(points[xx][yy]);
                    fs[xx][yy] = ff;
                    searched[xx][yy] = true;
                }
                else if (f(cur->g + dg(xx, yy, points[xx][yy].step, sn), h(xx, yy, fd)) > fs[xx][yy]) // 搜寻过且f更大
                {
                    int gg = cur->g + dg(xx, yy, points[xx][yy].step, sn);
                    int hh = h(xx, yy, fd);
                    int ff = f(gg, hh);
                    points[xx][yy] = Point(xx, yy, gg, ff, &points[cur->x][cur->y], points[xx][yy].step + 1);
                    pq.push(points[xx][yy]);
                    fs[xx][yy] = ff;
                }
            }
        }
    }
    ///////////
    cout << "path:";
    // 还原最佳路径
    while (cur->parent->x != x || cur->parent->y != y)
    {
        cur = &points[cur->parent->x][cur->parent->y];
        ////////////
        cout << "[" << cur->x << "," << cur->y << "] ";
    }
    ////////////
    cout << "current:x=" << x << " y=" << y << endl;
    cout << "parent:x=" << cur->x << " y=" << cur->y << endl;
    if (cur->x - x == 1 || x - cur->x > block_num_x - 2)
    {
        return RIGHT;
    }
    if (cur->x - x == -1 || cur->x - x > block_num_x - 2)
    {
        return LEFT;
    }
    if (cur->y - y == 1 || y - cur->y > block_num_y - 2)
    {
        return DOWN;
    }
    if (cur->y - y == -1 || cur->y - y > block_num_y - 2)
    {
        return UP;
    }
    return STAY;
}