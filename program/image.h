#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <graphics.h>
#include <time.h>
using namespace std;
/*宏*/
#define block_size 20
#define block_num_x 15 // 首和尾是墙体
#define block_num_y 12
/*全局变量*/
enum Object
{
    SPACE,  // 0->空格
    FOOD,   // 1->食物
    BARRIER // 2->障碍物或蛇身
};
extern int block_x[block_num_x + 1]; // x方向每一个格子的x坐标
extern int block_y[block_num_y + 1]; // y方向每一个格子的y坐标
/*全局函数*/
void init_block();                            // 初始化格子的位置
void fill_color(int x, int y, color_t color); // 在格子中填充指定颜色
#endif