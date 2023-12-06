#ifndef A_STAR_H
#define A_STAR_H
#include <queue> //使用优先级队列
#include "image.h"
#include "food.h"
#include "snake.h"
Direction a_star_search(int x, int y, FoodList fd, Snake sn);
#endif