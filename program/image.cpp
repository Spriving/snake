#include "image.h"
int block_x[block_num_x + 1];
int block_y[block_num_y + 1];

void init_block()
{
    for (int i = 0; i <= block_num_x; i++)
    {
        block_x[i] = 100 + i * block_size;
    }
    for (int j = 0; j <= block_num_y; j++)
    {
        block_y[j] = 80 + j * block_size;
    }
}
void fill_color(int x, int y, color_t color)
{
    setfillcolor(color);
    ege_fillrect(block_x[x], block_y[y], block_size, block_size);
}