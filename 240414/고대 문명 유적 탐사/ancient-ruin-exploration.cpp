#include <iostream>
#include <string.h>
#include <queue>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int K, M;
int myMap[6][6];
int temp[6][6];
queue<int> waiting_list;
bool visited[5][5];

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

struct p
{
    int x;
    int y;
    p() {}
    p(int x, int y) : x(x), y(y) {}
};

struct turn
{
    p pos;
    int degree;
    int score;

    turn() {}
    turn(p pos, int degree, int score) : pos(pos), degree(degree), score(score) {}

    bool const operator<(turn temp) const
    {
        if (score == temp.score)
        {
            if (degree == temp.degree)
            {
                if (pos.y == temp.pos.y)
                {
                    return pos.x > temp.pos.x;
                }
                return pos.y > temp.pos.y;
            }
            return degree > temp.degree;
        }
        return score < temp.score;
    }
};

void print_all()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << myMap[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void turn_90(p pos)
{
    memcpy(temp, myMap, sizeof(myMap));
    int map_3[4][4];
    int tmp_3[4][4];
    p start = p(pos.x - 1, pos.y - 1);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            map_3[i][j] = myMap[start.x + i][start.y + j];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tmp_3[i][j] = map_3[3 - (j + 1)][i];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp[start.x + i][start.y + j] = tmp_3[i][j];
        }
    }
}

void turn_180(p pos)
{
    memcpy(temp, myMap, sizeof(myMap));
    int map_3[4][4];
    int tmp_3[4][4];
    p start = p(pos.x - 1, pos.y - 1);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            map_3[i][j] = myMap[start.x + i][start.y + j];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tmp_3[i][j] = map_3[3 - (i + 1)][3 - (j + 1)];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp[start.x + i][start.y + j] = tmp_3[i][j];
        }
    }
}

void turn_270(p pos)
{
    memcpy(temp, myMap, sizeof(myMap));
    int map_3[4][4];
    int tmp_3[4][4];
    p start = p(pos.x - 1, pos.y - 1);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            map_3[i][j] = myMap[start.x + i][start.y + j];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tmp_3[i][j] = map_3[3 - (j + 1)][i];
        }
    }

    memcpy(map_3, tmp_3, sizeof(tmp_3));

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            tmp_3[i][j] = map_3[3 - (i + 1)][3 - (j + 1)];
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp[start.x + i][start.y + j] = tmp_3[i][j];
        }
    }
}

int bfs(p start)
{
    queue<p> q;
    q.push(start);
    visited[start.x][start.y] = true;
    int idx = temp[start.x][start.y];

    int score = 1;

    while (!q.empty())
    {
        p cur = q.front();
        q.pop();

        for (int k = 0; k < 4; k++)
        {
            int next_x = cur.x + dx[k];
            int next_y = cur.y + dy[k];

            if (next_x >= 0 && next_x < 5 && next_y >= 0 && next_y < 5 && !visited[next_x][next_y] && idx == temp[next_x][next_y])
            {
                q.push(p(next_x, next_y));
                score += 1;
                visited[next_x][next_y] = true;
            }
        }
    }

    return score;
}

int cal_score()
{
    memset(visited, 0, sizeof(visited));
    int score = 0;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (!visited[i][j])
            {
                int s = bfs(p(i, j));

                if (s >= 3)
                {
                    score += s;
                }
            }
        }
    }

    return score;
}

bool canTurn()
{
    priority_queue<turn> pq;
    int score;

    for (int i = 1; i <= 3; i++)
    {
        for (int j = 1; j <= 3; j++)
        {
            turn_90(p(i, j));
            score = cal_score();
            if (score > 0)
            {
                pq.push(turn(p(i, j), 90, score));
            }
            turn_180(p(i, j));
            score = cal_score();
            if (score > 0)
            {
                pq.push(turn(p(i, j), 180, score));
            }
            turn_270(p(i, j));
            score = cal_score();
            if (score > 0)
            {
                pq.push(turn(p(i, j), 270, score));
            }
        }
    }

    if (pq.empty())
    {
        return false;
    }

    //cout << pq.top().pos.x << " " << pq.top().pos.y << " " << pq.top().degree << " " << pq.top().score << "\n";

    if (pq.top().degree == 90)
        turn_90(p(pq.top().pos.x, pq.top().pos.y));
    else if (pq.top().degree == 180)
        turn_180(p(pq.top().pos.x, pq.top().pos.y));
    else
        turn_270(p(pq.top().pos.x, pq.top().pos.y));

    memcpy(myMap, temp, sizeof(temp));

    return true;
}

void erase_bfs(p start)
{
    bool visit[6][6];
    memset(visit, 0, sizeof(visit));

    queue<p> q;
    q.push(start);
    visit[start.x][start.y] = true;
    int idx = myMap[start.x][start.y];
    myMap[start.x][start.y] = -1;

    while (!q.empty())
    {
        p cur = q.front();
        q.pop();

        for (int k = 0; k < 4; k++)
        {
            int next_x = cur.x + dx[k];
            int next_y = cur.y + dy[k];

            if (next_x >= 0 && next_x < 5 && next_y >= 0 && next_y < 5 && !visit[next_x][next_y] && idx == myMap[next_x][next_y])
            {
                q.push(p(next_x, next_y));
                myMap[next_x][next_y] = -1;
                visit[next_x][next_y] = true;
            }
        }
    }
}

int erase()
{
    memset(visited, 0, sizeof(visited));
    int score = 0;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (!visited[i][j])
            {
                int s = bfs(p(i, j));

                if (s >= 3)
                {
                    score += s;
                    erase_bfs(p(i, j));
                }
            }
        }
    }

    return score;
}

void fill()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 4; j >= 0; j--)
        {
            if (myMap[j][i] == -1)
            {
                myMap[j][i] = waiting_list.front();
                waiting_list.pop();
            }
        }
    }
}

int main()
{
    // 여기에 코드를 작성해주세요.
    FASTIO();
    cin >> K >> M;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int x;
            cin >> x;

            myMap[i][j] = x;
        }
    }

    for (int i = 0; i < M; i++)
    {
        int x;
        cin >> x;

        waiting_list.push(x);
    }

    for (int k = 1; k <= K; k++)
    {
        int answer = 0;

        if (!canTurn())
            break;

        // 유물 획득
        while (1)
        {
            int s = erase();

            if (s > 0)
            {
                answer += s;
                fill();
                memcpy(temp, myMap, sizeof(myMap));
            }
            else
            {
                break;
            }
            //print_all();
        }
        cout << answer << " ";
    }
    return 0;
}