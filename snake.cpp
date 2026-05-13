#include<iostream>
#include<cstring>
#include<vector>
using namespace std;

///关于众多图的注解,以便后续查看
// map 原图
// base 去掉蛇的图
// last_picture 最后一步的快照
// pass 将障碍物bool化的图
// first_dir 以蛇头为中心，第一次走到每一格子所需求的第一个方向
// distence_map 以蛇头为中心的最短的距离

struct position{
    int x,y;
};

position head,food;

vector<position> body;

char map[20][21];

void map_filler (char map[20][21])
{
    for (int i = 0; i < 20; i++)
    {
        scanf("%s", map[i]);
        fflush(stdout);
    }



    cout << "test" << endl;
}

void find_head_food (char map[20][21])
{
    for (int i = 0; i < 20; i++)
    for (int j = 0; j < 20; j++)
    {
        if (map[i][j] == 'H') head = {i, j};
        if (map[i][j] == 'F') food = {i, j};
    }
    body.push_back(head);
}

void find_body (char map[20][21])
{
    position current,previous,possible;
    current.x = head.x;
    current.y = head.y;
    previous = {-1,-1};
    possible = {-1,-1};
    int choose_x[4] = {1,-1,0,0};
    int choose_y[4] = {0,0,1,-1};
    while(true)
    {
        bool found = false;
        for(int i = 0;i < 4;i++)
        {
            possible.x = current.x + choose_x[i];
            possible.y = current.y + choose_y[i];
            if(possible.x > 19||possible.x < 0||possible.y > 19||possible.y < 0) continue;
            if(map[possible.x][possible.y] == 'B' && (possible.x != previous.x||possible.y != previous.y))
            {
                body.push_back({possible.x, possible.y});
                previous = current;
                current = possible;
                found = true;
                break;
            }
        }
        if (!found) break;
    }
}

char base[20][21];
void make_base_map ()
{
    for (int i = 0; i < 20; i++) 
    {
        for (int j = 0; j < 20; j++) 
        {
            if (map[i][j] == '#' || map[i][j] == 'O')base[i][j] = map[i][j];
            else base[i][j] = '.';
        }
        base[i][20] = '\0';
    }
}

char last_picture[20][21];
void make_last_picture()
{
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
        {
            last_picture[i][j] = base[i][j];
            last_picture[i][20] = '\0';  // 确保每行是合法字符串
        }
        // 画食物
        if (food.x != -1)
            last_picture[food.x][food.y] = 'F';
        // 画蛇身
        last_picture[body[0].x][body[0].y] = 'H';
        
    for (size_t i = 1; i < body.size(); i++)
        last_picture[body[i].x][body[i].y] = 'B';
}

int score = 0;          // 得分
int move_count = 0;  //我嘞个屎山 // 移动次数计数器（用于每 N 次增长）
char cur_dir = 'W';     // 初始方向向上（题目默认）


//功能性函数（第二部）
//1.方向检测函数(防止直接180度自杀)这个ai里面会用的
bool is_opposite (char snake_dir,char turn_dir)
{
    if((snake_dir=='W' && turn_dir=='S') || (snake_dir=='S' && turn_dir=='W') ||
        (snake_dir=='A' && turn_dir=='D') || (snake_dir=='D' && turn_dir=='A')) return true; 
    else return false;
}
//2.行进一步以后的贪吃蛇的蛇头(关于蛇尾巴涉及到增长问题，需要分开算)
// 根据方向计算新蛇头位置，不修改 body
void get_new_head(char dir, position &nh) 
{
    nh = body[0];            // 从当前蛇头出发
    if (dir == 'W') nh.x--;
    else if (dir == 'S') nh.x++;
    else if (dir == 'A') nh.y--;
    else if (dir == 'D') nh.y++;
}
void change_snake(char turn_dir,position new_head)
{
    position nh;
    get_new_head(turn_dir, nh);
    body.insert(body.begin(), nh);
    //这里应该要更新一下蛇头，要不然蛇头后面用不了了就
    head.x = body[0].x;
    head.y = body[0].y;
}
//3贪吃蛇是否需要增长的判定
//（附加移动次数统计，因为每一步都需要判断是否需要增长，两者等价）
bool judge_extend(int move_count,position food,position head,int N)//下面那个泛用性太差了，换这个来judge
{
    if(food.x == head.x && food.y == head.y || move_count + 1 == N)return true;
    return false;
}

bool if_extend(int &move_count,position &food,position head,int N)
{
    bool extend = false;
    if(food.x == head.x && food.y == head.y) 
    {
        extend = true;
        score += 10;
        food = {-1,-1};
        if (move_count == N) move_count = 0; 
    }
    else if(move_count == N) 
    {
        extend = true;
        move_count = 0;
    }
    move_count += 1;
    return extend;
}

//4贪吃蛇尾部的更新
void extender(int N)
{
    bool extend = if_extend(move_count, food,head,N);
    if(extend) return;
    else body.pop_back();//如果需要更新尾部，就更新，删除最后一个就好了，不需要就不用
}

//5接收食物的信号
bool accepter()
{
    int x,y;
    cin>>x>>y;
    fflush(stdout);
    if(x == 20 && y == 20) return true;//这种情况是无所谓的继续就行了
    else if(x == 100 && y == 100)//一切都结束了，需要打印结束时候的地图和分数
    {
        for (int i = 0; i < 20; i++)
        cout << last_picture[i] << '\n';
        cout << score << '\n' << flush;    // 最后刷新
        return false;
    }
    else if(x >= 0 && x < 20 && y >= 0 && y < 20)//需要更新地图中的food
    {
        food.x = x;
        food.y = y;
    }
    return true;
}


//第三部分，研究蛇的ai情况
//1.研究是否这一步是安全的
bool is_safe (char dir ,int N)//不小心全整成全局了，那很方便了
{
     // 1. 禁止反向
    if (is_opposite(cur_dir, dir)) return false;
    // 2. 计算新蛇头(这里最好换一个蛇头代之，否则则会和后面的混合起来)
    position pre_new_head = head;
    get_new_head(dir,pre_new_head);
    if (base[pre_new_head.x][pre_new_head.y] == '#' || base[pre_new_head.x][pre_new_head.y] == 'O')
        return false;//判断撞墙
    bool will_grow = judge_extend(move_count,food,pre_new_head,N);//判断是否增长（还是extender的问题，已经修改）
    vector<position> predict_body = body;
    predict_body.insert(predict_body.begin(), pre_new_head);
    if (!will_grow) predict_body.pop_back();//预测新的蛇（entender写的有点集成性太高了，但是不好改我看）
    for (int i = 1; i < predict_body.size(); i++)
        if (predict_body[0].x == predict_body[i].x && predict_body[0].y == predict_body[i].y) return false;
    return true;
}

char bfs_direction ()
{
    //首先判断有没有食物
    if(food.x == -1) return '\0';
    //将base图中的墙和障碍物都标记成一样的东西
    bool pass[20][20] = {0};
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            if(base[i][j] == '#' || base[i][j] == 'O') pass[i][j] = 1;
    //将base中的蛇身设置为不可通行
    for (int i = 0; i < body.size() - 1; i++) //最后一个不读取
    {
        pass[body[i].x][body[i].y] = 1;
    }
    //bfs需要的模拟
    int dx[] = {-1, 1, 0, 0};   // W S A D
    int dy[] = {0, 0, -1, 1};
    char dchar[] = {'W', 'S', 'A', 'D'};
    int first_dir [20] [20];
    memset(first_dir, 0, sizeof(first_dir));
    //导航队列的逻辑
    //map_num 已经记录需要步数的已知的格子数 distense_map 用从蛇头到这个点的所需要的长度填充的图 num_mark 需要记录的已知的格子的总数
    int distence_map[20][20];
    memset(distence_map, -1, sizeof(distence_map));//这个ai说必须要的我也没啥办法咯
    vector<position> map_num;
    position start = body[0];
    map_num.push_back(start);
    distence_map[start.x][start.y] = 0;
    int num_mark = 0;
    //一直循环给图上所有位置标点（不全了就说明没戏了，得苟活）
    while (num_mark < map_num.size())
    {
        position cur = map_num[num_mark];
        num_mark++;
        if (cur.x == food.x && cur.y == food.y)//判断食物
        return dchar[ first_dir[cur.x][cur.y] ];
        for (int d = 0; d < 4; d++) //扩展邻居
        {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (nx < 0 || nx > 19 || ny < 0 || ny > 19) continue;// 超出地图边界
            if (pass[nx][ny]) continue;// 不能走（墙、障碍或身体）
            if (distence_map[nx][ny] == -1)// 这个格子没走过
            {
                distence_map[nx][ny] = distence_map[cur.x][cur.y] + 1; //添加这个格子
                if (cur.x == start.x && cur.y == start.y)//决定这个格子的方向
                first_dir[nx][ny] = d;
                else
                first_dir[nx][ny] = first_dir[cur.x][cur.y];
                map_num.push_back({nx, ny});//在步数地图中更新这个邻居
            }

        }
    }
    return '\0';
}

char ai_choose_direction(int N)
{
    //第一步，安全检测
    char dirs[] = {'W', 'S', 'A', 'D'};
    vector<char> safe_dirs;
    for (int i = 0;i < 4;i++) 
        if (is_safe(dirs[i], N))  safe_dirs.push_back(dirs[i]);
    if (safe_dirs.empty()) return cur_dir;//死到临头，再无话说
    //第二步，预测
    char bfs = bfs_direction();
    int safe_dirs_num = safe_dirs.size();
    for(int i = 0; i < safe_dirs_num; i++)
    {
        if(safe_dirs[i] == bfs)
        {
            return bfs;
        }
    }
    //第三步，横竖都是死
    return safe_dirs[0];
}


int main()
{
    // 1. 读取地图与初始化
    map_filler (map);//初始化填充地图
    int N ;
    cin>>N;
    fflush(stdout);
    find_head_food (map);//找食物和头
    find_body (map);//将身体找出
    make_base_map();//制作原装地图
    // 2. 交互循环
    while (true)
    {
        // 2.1 保存当前状态（用于碰撞后输出）
        make_last_picture();
        // 2.2 决定方向、输出方向
        char dir = ai_choose_direction(N);//这里需要编写贪吃蛇的ai   目前比较迷茫   没有想清楚
        // 2.3 输出移动前得分
        cout << dir << "\n" << score << "\n" << flush;
        // 2.4 计算移动、更新蛇、得分、食物
        change_snake(dir,head);                                              //这里存疑
        extender(N);
        // 2.5 读入评测程序返回的两个整数
        bool keep = accepter();
        // 2.6 根据返回值：结束则输出之前保存的地图和得分；否则更新食物坐标
        if(!keep) break;
    }
    return 0;
}