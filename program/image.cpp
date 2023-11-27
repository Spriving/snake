#include "image.h"
int block_num_x = 15;
int block_num_y = 12;
int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM];
int block_x[MAX_BLOCK_NUM + 1];
int block_y[MAX_BLOCK_NUM + 1];
void delay_ms(int ms)
{
    clock_t start = clock();
    while (clock() - start < ms)
        ;
}
void init_block()
{
    for (int i = 0; i <= MAX_BLOCK_NUM; i++)
    {
        block_x[i] = 100 + i * BLOCK_SIZE;
    }
    for (int j = 0; j <= MAX_BLOCK_NUM; j++)
    {
        block_y[j] = 80 + j * BLOCK_SIZE;
    }
}
bool out_of_bound(int xx, int yy)
{
    return xx < 0 || xx >= block_num_x || yy < 0 || yy >= block_num_y;
}
void restrict_xy(int &xx, int &yy)
{

    while (xx < 0)
    {
        xx += block_num_x;
    }
    while (xx >= block_num_x)
    {
        xx -= block_num_x;
    }
    while (yy < 0)
    {
        yy += block_num_y;
    }
    while (yy >= block_num_y)
    {
        yy -= block_num_y;
    }
}

void fill_color(int x, int y, color_t color)
{
    setfillcolor(color);
    ege_fillrect(block_x[x], block_y[y], BLOCK_SIZE, BLOCK_SIZE);
}