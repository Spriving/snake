# 程序规划

## 程序要求

### 游戏界面
- 开始界面
  - 用户操作：
    - 游戏难度1-10
    - 随机种子
    - 事物数量
    - 不同分值食物生成概率

- 游戏界面
  - 移动
  - 转向
  - 吃果果
  - 输出时间/分数等信息

- 游戏结算
  - 游戏记录：果子，方向改变

The code you provided is an implementation of the A* search algorithm. It seems like you're trying to find the shortest path to a goal (like food in a game of Snake). Here are a few points in the code that may cause a deadlock or infinite loop and suggestions for fixing them:

1. **Priority Queue Misuse**: When you take a `Point` out of the priority queue, you're storing it by reference (`Point *cur = &temp;`). However, `temp` is a local variable that will go out of scope, and the reference to it will become invalid once the loop iterates. You should instead store the points themselves, not a reference or pointer to a local temporary.

2. **Backtracking Logic**: The backtracking logic to reconstruct the path seems to be flawed. You're checking `cur->parent->x != x || cur->parent->y != y`, which will fail if the start point is the parent of the current point (because it will stop as soon as it reaches the start point's parent).

3. **Updating Searched Nodes**: The comment `// TODO:如何更新已经探索的节点？` suggests that you're unsure how to handle nodes that have already been visited. In A*, if you find a shorter path to a node that has already been visited, you should update its cost and its parent and then re-add it to the priority queue.

Here is a revised version of your loop with explanations:



您提供的代码是A*搜索算法的实现。这看起来就像是你在努力寻找通往目标的最短路径(就像Snake游戏中的食物)。以下是代码中可能导致死锁或无限循环的几个点，以及修复它们的建议:



1. **优先队列误用**:当你从优先队列中取出一个' Point '时，你通过引用(' Point *cur = &temp; ')来存储它。但是，' temp '是一个局部变量，它将超出作用域，并且一旦循环迭代，对它的引用将失效。相反，应该存储点本身，而不是本地临时变量的引用或指针。



2. **回溯逻辑**:重建路径的回溯逻辑似乎有缺陷。您正在检查' cur->parent->x != x || cur->parent->y != y '，如果起点是当前点的父点，则会失败(因为它一旦到达起点的父点就会停止)。



3.**: **更新搜索节点的评论/ / TODO:如何更新已经探索的节点吗?表示你不确定如何处理已经访问过的节点。在A*中，如果你找到一个更短的路径到一个已经被访问过的节点，你应该更新它的开销和它的父节点，然后重新将它添加到优先级队列中。



以下是修改后的循环，并附有解释:
```c++
while (!pq.empty())
{
    Point cur = pq.top();
    pq.pop();
    if (searched[cur.x][cur.y])
    {
        continue; // If the point has already been searched, skip it.
    }
    searched[cur.x][cur.y] = true; // Mark the point as searched.

    if (h(cur.x, cur.y, fd) == 0)
    {
        flag = true; // Found the goal.
        // Backtrack to find the direction to move.
        while (cur.parent != nullptr && (cur.parent->x != x || cur.parent->y != y))
        {
            cur = *(cur.parent);
        }
        break;
    }
    
    // Add neighbors to the queue.
    for (int i = -1; i <= 1; i += 2)
    {
        for (int j = -1; j <= 1; j += 2)
        {
            int xx = cur.x + i;
            int yy = cur.y + j;
            if (out_of_bound(xx, yy) || searched[xx][yy])
            {
                continue;
            }
            pq.push(Point(xx, yy, cur.g + dg(xx, yy, map), h(xx, yy, fd), &cur));
        }
    }
}

// Check direction to move.
if (flag)
{
    if (cur.x - x == 1)
    {
        return RIGHT;
    }
    if (cur.x - x == -1)
    {
        return LEFT;
    }
    if (cur.y - y == 1)
    {
        return DOWN;
    }
    if (cur.y - y == -1)
    {
        return UP;
    }
}
return STAY; // If flag is false, no path was found.
```

This code assumes you have a correct heuristic function `h` and distance function `dg`, and that your `Point` structure has a valid parent pointer to backtrack. Remember, when you modify `Point` objects, you need to ensure that their parent pointers remain valid and point to the actual objects in the priority queue, not to local copies or temporaries.