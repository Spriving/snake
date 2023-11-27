#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <graphics.h>
#include <time.h>
using namespace std;
/*宏*/
#define BLOCK_SIZE 20
#define MAX_BLOCK_NUM 20
/*全局变量*/
enum Direction
{
    RIGHT,
    DOWN,
    LEFT,
    UP,
    STAY = -1, // 方向保持不变
};
enum Object
{
    SPACE,  // 0->空格
    FOOD,   // 1->食物
    BARRIER // 2->障碍物或蛇身
};
extern int block_num_x;                       // x方向格子数,default 15
extern int block_num_y;                       // y方向格子数,default 12
extern int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM]; // 每一个格子中的内容:空格/食物/障碍物
extern int block_x[MAX_BLOCK_NUM + 1];        // x方向每一个格子的x坐标
extern int block_y[MAX_BLOCK_NUM + 1];        // y方向每一个格子的y坐标
/*全局函数*/
void delay_ms(int ms);
void init_block();                            // 初始化格子的位置
bool out_of_bound(int xx, int yy);            // 判断是否出界
void restrict_xy(int &xx, int &yy);           // 将x,y坐标限制在格子范围内
void fill_color(int x, int y, color_t color); // 在格子中填充指定颜色
#endif