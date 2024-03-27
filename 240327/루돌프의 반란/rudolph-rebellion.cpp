#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#include <algorithm>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct p
{
    int idx;
    int x;
    int y;
    int d;

    p() {}
    p(int idx, int x, int y, int d) : idx(idx), x(x), y(y), d(d) {}

    bool const operator<(p temp) const
    {
        if (d == temp.d)
        {
            if (x == temp.x)
            {
                return y > temp.y;
            }
            return x > temp.x;
        }
        return d < temp.d;
    }
};

struct rudolf
{
    int x;
    int y;

    rudolf() {}
    rudolf(int x, int y) : x(x), y(y) {}
};

struct santa
{
    int x;
    int y;
    int score;
    int panic;
    bool dead;

    santa() {}
    santa(int x, int y, int score, int panic, int dead) : x(x), y(y), panic(panic), score(score), dead(dead) {}
};

int myMap[55][55];
int rx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
int ry[8] = {1, 0, -1, 1, -1, 1, 0, -1};
int sx[4] = {-1, 0, 1, 0};
int sy[4] = {0, 1, 0, -1};
rudolf ru;
santa sa[35];

int N, M, P, C, D;

void print_all()
{
    for (int i = 1; i <= P; i++)
    {
        cout << sa[i].score << " ";
    }
    cout << "\n";
}

int find_dir(int x, int y, int nx, int ny)
{
    if (nx > x && ny > y)
    {
        return 0;
    }
    else if (nx > x && ny == y)
    {
        return 1;
    }
    else if (nx > x && ny < y)
    {
        return 2;
    }
    else if (nx == x && ny > y)
    {
        return 3;
    }
    else if (nx == x && ny < y)
    {
        return 4;
    }
    else if (nx < x && ny > y)
    {
        return 5;
    }
    else if (nx < x && ny == y)
    {
        return 6;
    }
    else
    {
        return 7;
    }
}

int find_opp_dir(int dir)
{
    if (dir == 0)
    {
        return 2;
    }
    else if (dir == 1)
    {
        return 3;
    }
    else if (dir == 2)
    {
        return 0;
    }
    else if (dir == 3)
    {
        return 1;
    }
}

// 루돌프가 충돌할 산타를 찾는다
int find_crush_santa()
{
    vector<p> v;
    for (int i = 1; i <= P; i++)
    {
        santa cur = sa[i];

        int d = (ru.x - cur.x) * (ru.x - cur.x) + (ru.y - cur.y) * (ru.y - cur.y);

        if (!cur.dead)
        {
            v.push_back(p(i, cur.x, cur.y, d));
        }
    }

    sort(v.begin(), v.end());

    return v[0].idx;
}

int find_crush_rudolf(int idx)
{

    int cur_d = (ru.x - sa[idx].x) * (ru.x - sa[idx].x) + (ru.y - sa[idx].y) * (ru.y - sa[idx].y);
    int shortest = cur_d;
    int dir = -1;

    for (int k = 0; k < 4; k++)
    {
        int next_x = sa[idx].x + sx[k];
        int next_y = sa[idx].y + sy[k];

        if (next_x >= 0 && next_x < N && next_y >= 0 && next_y < N && myMap[next_x][next_y] == 0)
        {
            int d = (ru.x - next_x) * (ru.x - next_x) + (ru.y - next_y) * (ru.y - next_y);

            if (d < cur_d && d < shortest)
            {
                shortest = d;
                dir = k;
            }
        }
    }

    return dir;
}

// 산타들의 상호작용
void interaction(int cur_sa, int nxt_sa, int dir, bool crushed)
{
    santa cur = sa[cur_sa];
    santa nxt = sa[nxt_sa];

    int next_x;
    int next_y;

    if (crushed)
    {
        next_x = nxt.x + rx[dir];
        next_y = nxt.y + ry[dir];
    }
    else
    {
        next_x = nxt.x + sx[dir];
        next_y = nxt.y + sy[dir];
    }

    if (myMap[next_x][next_y] == 0)
    {
        sa[nxt_sa].x = next_x;
        sa[nxt_sa].y = next_y;
        myMap[next_x][next_y] = nxt_sa;

        sa[cur_sa].x = nxt.x;
        sa[cur_sa].y = nxt.y;
        myMap[nxt.x][nxt.y] = cur_sa;
    }
    else
    {
        interaction(nxt_sa, myMap[next_x][next_y], dir, crushed);
        sa[cur_sa].x = nxt.x;
        sa[cur_sa].y = nxt.y;
        myMap[nxt.x][nxt.y] = cur_sa;
    }
}

// 루돌프와 산타의 충돌
void crush(int sa_idx, int dir, bool crushed)
{
    santa cur = sa[sa_idx];

    int next_x;
    int next_y;

    if (crushed)
    {
        next_x = cur.x + rx[dir] * C;
        next_y = cur.y + ry[dir] * C;
    }
    else
    {
        next_x = cur.x + sx[dir] * D;
        next_y = cur.y + sy[dir] * D;
    }

    // 격자 밖인 경우
    if (next_x < 0 || next_x >= N || next_y < 0 || next_y >= N)
    {
        sa[sa_idx].dead = true;
        myMap[cur.x][cur.y] = 0;
        return;
    }
    // 아무도 없는 경우
    else if (myMap[next_x][next_y] == 0)
    {
        myMap[cur.x][cur.y] = 0;
        myMap[next_x][next_y] = sa_idx;
        sa[sa_idx].x = next_x;
        sa[sa_idx].y = next_y;
        return;
    }
    // 다른 산타가 있는 경우
    else if (myMap[next_x][next_y] != 0)
    {
        // 상호작용
        interaction(sa_idx, myMap[next_x][next_y], dir, crushed);
    }
}

int main()
{
    FASTIO();
    cin >> N >> M >> P >> C >> D;

    int x, y;
    cin >> x >> y;

    ru = rudolf(x - 1, y - 1);

    for (int i = 1; i <= P; i++)
    {
        int n;
        cin >> n >> x >> y;

        sa[n] = santa(x - 1, y - 1, 0, -1, false);
        myMap[x - 1][y - 1] = n;
    }

    for (int t = 1; t <= M; t++)
    {
        // 루돌프
        // 1. 가장 가까운 산타를 찾는다
        int num = find_crush_santa();
        santa crush_santa = sa[find_crush_santa()];
        int crush_dir = find_dir(ru.x, ru.y, crush_santa.x, crush_santa.y);
        //cout << "I'm going to crush " << num << "to " << crush_dir << "\n";

        int next_ru = ru.x + rx[crush_dir];
        int next_ry = ru.y + ry[crush_dir];

        // 2. 산타가 없는 경우
        if (myMap[next_ru][next_ry] == 0)
        {
            ru = rudolf(next_ru, next_ry);
        }
        // 3. 산타가 있는 경우
        else
        {
            ru = rudolf(next_ru, next_ry);
            // 충돌
            int cur_santa = myMap[next_ru][next_ry];
            // 1. 점수를 더해준다
            sa[cur_santa].score += C;
            // 2. 기절 상태
            sa[cur_santa].panic = t;

            myMap[sa[cur_santa].x][sa[cur_santa].y] = 0;

            sa[cur_santa].x = next_ru;
            sa[cur_santa].y = next_ry;

            crush(cur_santa, crush_dir, true);
        }

        // 산타
        for (int i = 1; i <= P; i++)
        {
            if (sa[i].panic == t || sa[i].panic == t - 1 || sa[i].dead)
            {
                continue;
            }

            int s_crush_dir = find_crush_rudolf(i);

            //cout << "Santa " << i << "will crush to " << s_crush_dir << "\n";

            if (s_crush_dir != -1)
            {
                int next_x = sa[i].x + sx[s_crush_dir];
                int next_y = sa[i].y + sy[s_crush_dir];

                // 루돌프와 충돌]
                if (next_x == ru.x && next_y == ru.y)
                {
                    sa[i].score += D;
                    sa[i].panic = t;
                    myMap[sa[i].x][sa[i].y] = 0;
                    sa[i].x = next_x;
                    sa[i].y = next_y;

                    crush(i, find_opp_dir(s_crush_dir), false);
                }
                else
                {
                    myMap[sa[i].x][sa[i].y] = 0;
                    sa[i].x = next_x;
                    sa[i].y = next_y;
                    myMap[sa[i].x][sa[i].y] = i;
                }
            }
        }

        int survivor = 0;

        for (int i = 1; i <= P; i++)
        {
            if (!sa[i].dead)
            {
                sa[i].score += 1;
                survivor += 1;
            }
        }

        if (survivor == 0)
        {
            print_all();
            return 0;
        }

        //print_all();
    }
    print_all();
}