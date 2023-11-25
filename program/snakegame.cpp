#include "snake.h"
#include "food.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
/*宏定义*/
#define WINDOW_W 600
#define WINDOW_H 400
#define MAX_BARRIER_NUM 15
#define MIN_BARRIER_NUM 0
/*全局变量定义*/
int mode = 2; // 游戏模式:0->默认,1->按配置游戏,2->回放
bool newgame = true;
int speed = 2;                         // 游戏速度/帧率
int point = 0;                         // 游戏分数
int step_num = 0;                      // 游戏进行的步数
unsigned int seed = 1;                 // 随机种子
int map[MAX_BLOCK_NUM][MAX_BLOCK_NUM]; // 每一个格子中的内容:空格/食物/障碍物
Snake sn;                              // 蛇
FoodList fd;                           // 食物
bool wall[4] = {1, 0, 1, 0};           // 四面墙的虚实,右,下,左,上
int barrier_num;                       // 障碍物个数
int barrier[MAX_BARRIER_NUM][2];       // 障碍物位置
key_msg game_msg;                      // 键盘消息
vector<string> msg_record;             // 游戏记录
/*函数声明*/
void default_game();
void read_map(string map);
void read_config(string cfg) { return; }
void config_game(string mp, string cfg);
void play_record(string rec);
void init_barrier();
void gameover();
void update_map();
void draw_barrier();
int turn();
void update_record(int direc, string food_msg);
int main()
{
    /*预处理*/
    seed = time(0); // 设置随机种子
    srand(seed);
    initgraph(WINDOW_W, WINDOW_H); // 设置窗口大小
    setcaption("snake");           // 设置窗口标题
    init_block();                  // 初始化格子的位置
    switch (mode)
    {
    case 0: // 默认游戏模式
        default_game();
        break;
    case 1: // 指定config,map文件的游戏模式
        config_game("map/default.map", "");
        break;
    case 2:
        play_record("record/record.rec");
        break;
    }
    /*后处理*/
    getch();
    closegraph();
}
/*main()结束*/
/*函数定义*/
void default_game()
{
    /*单局游戏的准备*/
    /*局部变量*/
    int judge_res;
    init_barrier(); // 初始化生成障碍物
    point = 0;
    /*单局游戏循环*/
    while (1)
    {
        int new_direc = turn();
        cleardevice();
        string food_msg = fd.update(map);
        fd.draw();
        sn.move();
        sn.draw();
        draw_barrier();
        update_map();
        judge_res = sn.judge(map);
        if (judge_res == FOOD)
        {
            for (list<Food *>::iterator it = fd.fl.begin(); it != fd.fl.end(); it++)
            {
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
        update_record(new_direc, food_msg);
        if (judge_res == BARRIER)
        {
            gameover();
            break;
        }
        delay_fps(speed);
    }
    /*单局游戏结束*/
    ofstream fout("record/record.rec");
    for (int i = 0; i < msg_record.size(); i++)
    {
        fout << msg_record[i] << endl;
    }
    fout.close();
}

void read_map(string map)
{
    ifstream fin(map);
    if (fin)
    {
        fin >> block_num_x >> block_num_y >> wall[0] >> wall[1] >> wall[2] >> wall[3];
        fin >> barrier_num;
        for (int i = 0; i < barrier_num; i++)
        {
            fin >> barrier[i][0] >> barrier[i][1];
        }
    }
    else
    {
        cout << "can't open " << map << endl;
        getch();
    }
}
void config_game(string mp, string cfg)
{
    /*单局游戏的准备*/
    /*局部变量*/
    int judge_res;
    read_map(mp);
    read_config(cfg);
    point = 0;
    /*单局游戏循环*/
    while (1)
    {
        int new_direc = turn();
        cleardevice();
        string food_msg = fd.update(map);
        fd.draw();
        sn.move();
        sn.draw();
        draw_barrier();
        update_map();
        judge_res = sn.judge(map);
        if (judge_res == FOOD)
        {
            for (list<Food *>::iterator it = fd.fl.begin(); it != fd.fl.end(); it++)
            {
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
        update_record(new_direc, food_msg);
        if (judge_res == BARRIER)
        {
            gameover();
            break;
        }
        delay_fps(speed);
    }
    /*单局游戏结束*/
    ofstream fout("record/record.rec");
    for (int i = 0; i < msg_record.size(); i++)
    {
        fout << msg_record[i] << endl;
    }
    fout.close();
}

void play_record(string rec) // TODO:debug->食物不能正常读取
{
    ifstream fin(rec);
    if (!fin)
    {
        cout << "Failed to open " << rec << endl;
        return;
    }
    string str = "";
    getline(fin, str);
    if (str[0] = 'c') // 如果是mode==1按配置-地图模式保存
    {
        read_config(str);
        getline(fin, str);
        read_map(str);
    }
    else // 如果是mode==0保存随机地图信息
    {
    }
    /*单局游戏的准备*/
    point = 0;
    step_num = 0;
    string line = "";
    /*单局游戏循环*/
    while (1)
    {
        getline(fin, line);
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); // 清除空格
        int pos = 1;
        while (pos < line.size())
        {
            switch (line[pos++])
            {
            case 'W':
                sn.direc = UP;
                break;
            case 'A':
                sn.direc = LEFT;
                break;
            case 'S':
                sn.direc = DOWN;
                break;
            case 'D':
                sn.direc = RIGHT;
                break;
            case 'F':
                if (step_num != 0)
                {
                    for (list<Food *>::iterator it = fd.fl.begin(); it != fd.fl.end(); it++)
                    {
                        if ((*it)->x == sn.body.front()->x && (*it)->y == sn.body.front()->y)
                        {
                            point += (*it)->point + 1;
                            cout << "snake_head:" << sn.body.front()->x << ' ' << sn.body.front()->y << endl;
                            fd.fl.erase(it);
                            break;
                        }
                        else
                        {
                            cout << "Food not found\n";
                        }
                        sn.add();
                    }
                }
                int x = 0, y = 0, p = 0;
                while (line[pos] != ',')
                {
                    x = x * 10 + (line[pos++] - 48);
                    cout << x << endl;
                }
                pos++;
                while (line[pos] != ',')
                {
                    y = y * 10 + (line[pos++] - 48);
                }
                pos++;
                p = (line[pos++] - 48) - 1;
                cout << x << ' ' << y << ' ' << p << '|';
                fd.fl.push_back(new Food(x, y, p));
            }
        }
        if (line.size() == 0)
        {
            gameover();
            return;
        }
        cleardevice();
        sn.move();
        sn.draw();
        fd.draw();
        draw_barrier();
        cout << "Point:" << point << ' ';
        step_num++;
        delay_fps(speed);
    }
}
void init_barrier()
{                // 生成四面墙
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
    if (wall[3]) // 上
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
int turn() // 转弯
{          // 用一个循环队列生成一个缓冲区，解决一帧内连续转向使得第二次转向丢失的问题
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
                flag = true;
            }
            break;
        }
        if (flag)
        {
            tail = (tail + 1) % 100;
            cusion_size++;
        }
    }
    if (sn.direc != old_direc)
    {
        return sn.direc;
    }
    return -1;
}
void init_record(string map, string cfg)
{
    if (newgame)
    {
        msg_record.clear();
        if (mode == 0) // 默认模式，记录地图信息
        {
            string str = "";
            str = to_string(block_num_x) + " " + to_string(block_num_y);
            msg_record.push_back(str); // 第一行：xy方向格子数
            str = " ";
            for (int i = 0; i < 4; i++) // 第二行：四面墙的虚实
            {
                str += to_string(wall[i]);
                if (i < 3)
                {
                    str += " ";
                }
            }
            msg_record.push_back(str);
            str = to_string(barrier_num);
            msg_record.push_back(str); // 第三行：障碍物数量
            for (int i = 0; i < barrier_num; i++)
            {
                str = to_string(barrier[i][0]) + " " + to_string(barrier[i][1]);
                msg_record.push_back(str); // 第n行：第n-3个障碍物的位置,n>=4
            }
        }
        else if (mode == 1) // 配置游戏模式，记录地图文件和配置文件
        {
            msg_record.push_back(cfg);
            msg_record.push_back(map);
        }
    }
}
void update_record(int direc, string food_msg)
{
    static int gametime;
    if (newgame)
    {
        newgame = false;
        gametime = 0;
        direc = sn.direc;
    }
    string msg = "";
    msg += to_string(gametime);
    switch (direc)
    {
    case UP:
        msg += " W";
        break;
    case LEFT:
        msg += " A";
        break;
    case DOWN:
        msg += " S";
        break;
    case RIGHT:
        msg += " D";
        break;
    }
    msg += food_msg;
    msg_record.push_back(msg);
    gametime++;
}