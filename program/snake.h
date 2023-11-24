#ifndef SNAKE_H
#define SNAKE_H

#include "image.h"
#include <list>
#include <cstdlib>
enum Direction
{
    RIGHT,
    DOWN,
    LEFT,
    UP,
};
class Node
{
public:
    int x;
    int y;
    int pos;
    Node()
    {
        x = rand() % 8 + 4;
        y = rand() % 8 + 2;
        pos = 0;
    }
    Node(int _x, int _y, int _pos) : x(_x), y(_y), pos(_pos){};
    friend class Snake;
};

class Snake
{
public:
    // 属性
    list<Node *> body;
    int direc;
    int len;
    // 方法
    int size() { return body.size(); }
    void add();
    void draw();
    void move();
    int judge(int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM]);
    Snake()
    {
        direc = rand() % 4;
        add();
    }
    ~Snake()
    {
        for (auto it = body.begin(); it != body.end(); it++)
        {
            delete *it;
        }
    }
};
#endif