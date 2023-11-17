#include "snake.h"
#include "food.h"
/*宏定义*/
#define WINDOW_W 600
#define WINDOW_H 400
#define MAX_BARRIER_NUM 10
#define MIN_BARRIER_NUM 3
/*全局变量定义*/
int speed = 2;
int point = 0;
unsigned int seed = 1;
int map[block_num_x][block_num_y]; // 记录每一个格子中的内容
// bool has_turned = false;
// bool has_moved = true;
Snake sn;
FoodList fd;
int barrier_num;
int barrier[MAX_BARRIER_NUM][2];
key_msg game_msg;
/*函数声明*/
void init_barrier();
void gameover();
void update_map();
void draw_barrier();
void turn();
int main()
{
    /*局部变量*/
    int judge_res;
    /*预处理*/
    seed = time(0);
    srand(seed);
    initgraph(WINDOW_W, WINDOW_H); // 设置窗口大小
    setcaption("snake");           // 设置窗口标题
    init_block();
    init_barrier();
    point = 0;
    // // TEST:使食物的x坐标与蛇头的x坐标相同
    // fd.update(map);
    // cout << fd.fl.front()->x << endl;
    // fd.fl.front()->x = sn.body.front()->x;
    // auto it = fd.fl.begin();
    // cout << (*it)->x << endl;
    /*游戏循环*/
    while (1)
    {
        turn();
        cleardevice();
        fd.update(map);
        sn.move();
        sn.draw();
        draw_barrier();
        update_map();
        judge_res = sn.judge(map);
        if (judge_res == BARRIER)
        {
            gameover();
            break;
        }
        else if (judge_res == FOOD)
        {
            for (list<Food *>::iterator it = fd.fl.begin(); it != fd.fl.end(); it++)
            {
                cout << (*it)->x << endl;
                if ((*it)->x == sn.body.front()->x && (*it)->y == sn.body.front()->y)
                {
                    point += (*it)->point;
                    fd.fl.erase(it);
                    break;
                }
                sn.add();
            }
        }
        cout << "Point:" << point << ' ';
        delay_fps(speed);
    }

    getch();
    /*后处理*/
    closegraph();
}
/*函数定义*/
void init_barrier()
{
    barrier_num = rand() % (MAX_BARRIER_NUM - MIN_BARRIER_NUM) + MIN_BARRIER_NUM;
    for (int i = 0; i < barrier_num; i++)
    {
        int x, y;
        do
        {
            x = rand() % block_num_x;
            y = rand() % block_num_y;
        } while (map[x][y] != SPACE);

        barrier[i][0] = x;
        barrier[i][1] = y;
        map[barrier[i][0]][barrier[i][1]] = BARRIER;
    }
}
void gameover()
{
    cout << "Game Over!";
    return;
}
void update_map()
{
    for (int i = 0; i < block_num_x; i++)
    {
        for (int j = 0; j < block_num_y; j++)
        {
            map[i][j] = SPACE; // 全部更新为空地SPACE
        }
    }
    for (int i = 0; i < barrier_num; i++)
    {
        map[barrier[i][0]][barrier[i][1]] = BARRIER; // 障碍物赋值为BARRIER
    }
    auto it = sn.body.begin();
    for (it++; it != sn.body.end(); it++) // 蛇头不映射到map中
    {
        map[(*it)->x][(*it)->y] = BARRIER; // 蛇身赋值为BARRIER
    }
    for (auto it = fd.fl.begin(); it != fd.fl.end(); it++)
    {
        map[(*it)->x][(*it)->y] = FOOD; // 食物赋值为FOOD
    }
}
void draw_barrier()
{
    for (int i = 0; i < barrier_num; i++)
    {
        fill_color(barrier[i][0], barrier[i][1], WHITE);
    }
}
void turn() // 转弯
{
    static int msg_cusion[100];
    static int head = 0;
    static int tail = 0;
    static int cusion_size = 0;
    int old_direc = sn.direc;
    int key_num;
    while (cusion_size || kbmsg())
    {
        if (cusion_size)
        {
            key_num = msg_cusion[head++];
            cusion_size--;
        }
        else
        {
            game_msg = getkey();
            key_num = game_msg.key;
        }
        switch (key_num)
        {
        case 'w':
        case 'W':
            if (old_direc != UP && old_direc != DOWN)
            {
                sn.direc = UP;
            }
            else if (sn.direc != UP || sn.direc != DOWN)
            {
                msg_cusion[tail++] = key_num;
                cusion_size++;
            }
            break;
        case 's':
        case 'S':
            if (old_direc != UP && old_direc != DOWN)
            {
                sn.direc = DOWN;
            }
            else if (sn.direc != UP || sn.direc != DOWN)
            {
                msg_cusion[tail++] = key_num;
                cusion_size++;
            }
            break;
        case 'd':
        case 'D':
            if (old_direc != LEFT && old_direc != RIGHT)
            {
                sn.direc = RIGHT;
            }
            else if (sn.direc != LEFT || sn.direc != RIGHT)
            {
                msg_cusion[tail++] = key_num;
                cusion_size++;
            }
            break;
        case 'a':
        case 'A':
            if (old_direc != LEFT && old_direc != RIGHT)
            {
                sn.direc = LEFT;
                cusion_size++;
            }
            else if (sn.direc != LEFT || sn.direc != RIGHT)
            {
                msg_cusion[tail++] = key_num;
                cusion_size++;
            }
            break;
        }
    }
}