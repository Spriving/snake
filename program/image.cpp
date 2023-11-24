#include "image.h"
int block_num_x = 15;
int block_num_y = 12;
int block_x[MAX_BLOCK_NUM + 1];
int block_y[MAX_BLOCK_NUM + 1];

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
void fill_color(int x, int y, color_t color)
{
    setfillcolor(color);
    ege_fillrect(block_x[x], block_y[y], BLOCK_SIZE, BLOCK_SIZE);
}