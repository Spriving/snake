#include "a_star.h"

int abs(int x)
{
    return x >= 0 ? x : -x;
}

inline int m_distance(int x1, int y1, int x2, int y2) // 曼哈顿距离//TODO未考虑虚墙情况
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int dg(int x, int y)
{
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
    Point *parent;
    Point(int xx, int yy, int gg = 0, int ff = 0, Point *p = NULL) : x(xx), y(yy), g(gg), f(ff), parent(p){};
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
Direction a_star_search(int x, int y, FoodList fd)
{
    priority_queue<Point> pq;                         // 创建边缘集
    bool searched[MAX_BLOCK_NUM][MAX_BLOCK_NUM] = {}; // 创建探索集
    int fs[MAX_BLOCK_NUM][MAX_BLOCK_NUM] = {};
    bool flag = false; // 是否找到最佳路径
    Point start = Point(x, y, 0, h(x, y, fd));
    Point cur = start;
    pq.push(start); // 将蛇头位置加入优先级队列
    while (flag == false)
    {
        if (pq.empty())
        {
            return STAY; // 未搜索到目标
        }
        cur = pq.top();
        pq.pop();
        ////////////////////
        cout << "x=" << cur.x << " y=" << cur.y << " g=" << cur.g << " h=" << h(cur.x, cur.y, fd) << " f=" << cur.f << endl;
        if (h(cur.x, cur.y, fd) == 0)
        {
            cout << "des_x=" << cur.x << " des_y=" << cur.y << endl;
            flag = true; // 找到苹果后将flag置为true
            break;
        }
        if (map[cur.x][cur.y] == BARRIER)
        {
            continue;
        }
        for (int i = -1; i <= 1; i += 2) // 若没找到，将周围的四个点加入边缘集
        {
            for (int j = 0; j <= 1; j++)
            {
                int xx = cur.x + i * j;       // 0,-1,0,1
                int yy = cur.y + i * (j - 1); // 1,0,-1,0
                // restrict_xy(xx, yy);
                if (out_of_bound(xx, yy))
                {
                    continue;
                }
                if (!searched[xx][yy])
                {
                    int gg = cur.g + dg(xx, yy);
                    int hh = h(xx, yy, fd);
                    int ff = f(gg, hh);
                    pq.push(Point(xx, yy, gg, ff, &cur));
                    fs[xx][yy] = ff;
                    searched[xx][yy] = true;
                }
                else if (f(cur.g + dg(xx, yy), h(xx, yy, fd)) > fs[xx][yy]) // TODO:如何更新已经探索的节点？
                {
                    int gg = cur.g + dg(xx, yy);
                    int hh = h(xx, yy, fd);
                    int ff = f(gg, hh);
                    pq.push(Point(xx, yy, gg, ff, &cur));
                    fs[xx][yy] = ff;
                }
            }
        }
        //////////////////////
        delay_ms(100);
    }
    ///////////
    cout << "path:";
    // 还原最佳路径
    while (cur.parent->x != x || cur.parent->y != y)
    {
        cur = *cur.parent;
        ////////////
        cout << "[" << cur.x << "," << cur.y << "] ";
    }
    ////////////
    cout << "current:x=" << x << " y=" << y << endl;
    cout << "parent:x=" << cur.x << " y=" << cur.y << endl;
    if (cur.x - x == 1)
    {
        return RIGHT;
    }
    if (cur.x - x == -1)
    {
        return LEFT;
    }
    if (cur.y - y == 1)
    {
        return DOWN;
    }
    if (cur.y - y == -1)
    {
        return UP;
    }
    return STAY;
}