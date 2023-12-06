#ifndef SNAKE_H
#define SNAKE_H

#include "image.h"
#include <list>
#include <cstdlib>
class Node
{
public:
    int x;
    int y;
    int pos;
    Node(int _x = block_num_x / 2, int _y = block_num_y / 3, int _pos = 0) : x(_x), y(_y), pos(_pos){};
    friend class Snake;
};

class Snake
{
public:
    // 属性
    list<Node *> body;
    int direc;
    // 方法
    void init();
    int size() { return body.size(); }
    void add();
    void draw();
    void move();
    int judge(int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM]);
    Snake()
    {
        body.push_back(new Node());
        direc = DOWN;
        add();
    }
    // ~Snake()
    // {
    //     for (auto it = body.begin(); body.size(); it = body.begin())
    //     {
    //         delete it;
    //     }
    // }
    int len() { return body.size(); }
    int search(int _x, int _y);
};
#endif