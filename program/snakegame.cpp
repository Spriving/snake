#include "snake.h" //TODO:初始时蛇不能向随机方向移动
#include "food.h"
/*宏定义*/
#define WINDOW_W 600
#define WINDOW_H 400
#define MAX_BARRIER_NUM 10
#define MIN_BARRIER_NUM 3
/*全局变量定义*/
int speed = 2;                     // 游戏速度/帧率
int point = 0;                     // 游戏分数
unsigned int seed = 1;             // 随机种子
int map[block_num_x][block_num_y]; // 每一个格子中的内容:空格/食物/障碍物
Snake sn;                          // 蛇
FoodList fd;                       // 食物
bool wall[4] = {1, 0, 1, 0};       // 四面墙的虚实,右,下,左,上
int barrier_num;                   // 障碍物个数
int barrier[MAX_BARRIER_NUM][2];   // 障碍物位置
key_msg game_msg;                  // 键盘消息
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
    seed = time(0); // 设置随机种子
    srand(seed);
    initgraph(WINDOW_W, WINDOW_H); // 设置窗口大小
    setcaption("snake");           // 设置窗口标题
    init_block();                  // 初始化格子的位置
    /*单局游戏的准备*/
    init_barrier(); // 初始化生成障碍物
    point = 0;
    // // TEST:使食物的x坐标与蛇头的x坐标相同
    // fd.update(map);
    // cout << fd.fl.front()->x << endl;
    // fd.fl.front()->x = sn.body.front()->x;
    // auto it = fd.fl.begin();
    // cout << (*it)->x << endl;
    /*单局游戏循环*/
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
                    point += (*it)->point + 1;
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
    if (wall[0]) // 右
    {
        for (int i = 0; i < block_num_y; i++)
        {
            map[block_num_x - 1][i] = BARRIER;
        }
    }
    if (wall[1]) // 下
    {
        for (int i = 0; i < block_num_x; i++)
        {
            map[i][0] = BARRIER;
        }
    }
    if (wall[2]) // 左
    {
        for (int i = 0; i < block_num_y; i++)
        {
            map[0][i] = BARRIER;
        }
    }
    if (wall[2]) // 上
    {
        for (int i = 0; i < block_num_x; i++)
        {
            map[i][block_num_y - 1] = BARRIER;
        }
    }
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
    for (int i = wall[2]; i < block_num_x - wall[0]; i++)
    {
        for (int j = wall[3]; j < block_num_y - wall[1]; j++)
        {
            map[i][j] = SPACE; // 非墙体处全部更新为空地SPACE
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
    if (wall[0]) // 右
    {
        for (int i = 0; i < block_num_y; i++)
        {
            fill_color(block_num_x - 1, i, WHITE);
        }
    }
    if (wall[1]) // 下
    {
        for (int i = 0; i < block_num_x; i++)
        {
            fill_color(i, block_num_y - 1, WHITE);
        }
    }
    if (wall[2]) // 左
    {
        for (int i = 0; i < block_num_y; i++)
        {
            fill_color(0, i, WHITE);
        }
    }
    if (wall[2]) // 上
    {
        for (int i = 0; i < block_num_x; i++)
        {
            fill_color(i, 0, WHITE);
        }
    }
    for (int i = 0; i < barrier_num; i++)
    {
        fill_color(barrier[i][0], barrier[i][1], WHITE);
    }
}
void turn() // 转弯
{           // 用一个循环队列生成一个缓冲区，解决一帧内连续转向使得第二次转向丢失的问题
    static int msg_cusion[100];
    static int head = 0;        // 队首
    static int tail = 0;        // 队尾
    static int cusion_size = 0; // 队伍长度
    int old_direc = sn.direc;   // 原方向
    int key_num;                // 键盘消息
    int flag = false;           // 是否向cusion中加入消息
    while (!flag && cusion_size || kbmsg())
    {
        if (!flag && cusion_size)
        {
            key_num = msg_cusion[head];
            head = (head + 1) % 100;
            cusion_size--;
            cout << cusion_size;
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
            else if (sn.direc != UP && sn.direc != DOWN)
            {
                msg_cusion[tail] = key_num;
                tail = (tail + 1) % 100;
                cusion_size++;
                flag = true;
            }
            break;
        case 's':
        case 'S':
            if (old_direc != UP && old_direc != DOWN)
            {
                sn.direc = DOWN;
            }
            else if (sn.direc != UP && sn.direc != DOWN)
            {
                msg_cusion[tail] = key_num;
                tail = (tail + 1) % 100;
                cusion_size++;
                flag = true;
            }
            break;
        case 'd':
        case 'D':
            if (old_direc != LEFT && old_direc != RIGHT)
            {
                sn.direc = RIGHT;
            }
            else if (sn.direc != LEFT && sn.direc != RIGHT)
            {
                msg_cusion[tail] = key_num;
                tail = (tail + 1) % 100;
                cusion_size++;
                flag = true;
            }
            break;
        case 'a':
        case 'A':
            if (old_direc != LEFT && old_direc != RIGHT)
            {
                sn.direc = LEFT;
            }
            else if (sn.direc != LEFT && sn.direc != RIGHT)
            {
                msg_cusion[tail] = key_num;
                tail = (tail + 1) % 100;
                cusion_size++;
                flag = true;
            }
            break;
        }
    }
}