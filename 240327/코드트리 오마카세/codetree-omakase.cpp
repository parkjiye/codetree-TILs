#include <iostream>
#include <queue>
#include <map>
#include <vector>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct people
{
    int pos;
    int eaten;
    int expected;

    people() {}
    people(int pos, int eaten, int expected) : pos(pos), eaten(eaten), expected(expected) {}
};

struct waiting_sushi
{
    int incoming_t;
    int pos;

    waiting_sushi() {}
    waiting_sushi(int incoming_t, int pos) : incoming_t(incoming_t), pos(pos) {}
};

struct eaten_sushi
{
    int eaten_time;
    string name;

    eaten_sushi() {}
    eaten_sushi(int eaten_time, string name) : eaten_time(eaten_time), name(name) {}

    bool const operator<(eaten_sushi temp) const
    {
        return eaten_time > temp.eaten_time;
    }
};

int L, Q;
int cnt = 0;

map<string, vector<waiting_sushi>> m;
map<string, people> p;
priority_queue<eaten_sushi> pq;

int main()
{
    FASTIO();
    cin >> L >> Q;

    int cur_time = 1;

    for (int q = 0; q < Q; q++)
    {
        int cmd, t;
        cin >> cmd >> t;

        while (!pq.empty())
        {
            if (pq.top().eaten_time > t)
            {
                break;
            }

            eaten_sushi cur = pq.top();
            pq.pop();
            cnt -= 1;

            p[cur.name].eaten += 1;

            if (p[cur.name].expected == p[cur.name].eaten)
            {
                p.erase(cur.name);
            }
        }

        // 초밥 올려놓기
        if (cmd == 100)
        {
            int x;
            string n;
            cin >> x >> n;

            if (p.find(n) != p.end())
            {
                int et;

                if (p[n].pos > x)
                {
                    et = t + (p[n].pos - x);
                    pq.push(eaten_sushi(et, n));
                    cnt += 1;
                }
                else if (p[n].pos == x)
                {
                    p[n].eaten += 1;

                    if (p[n].expected == p[n].eaten)
                    {
                        p.erase(n);
                    }
                }
                else
                {
                    et = t + (L - x) + p[n].pos;
                    pq.push(eaten_sushi(et, n));
                    cnt += 1;
                }
            }
            else if (m.find(n) != m.end())
            {
                m[n].push_back(waiting_sushi(t, x));
                cnt += 1;
            }
            else
            {
                vector<waiting_sushi> v;
                v.push_back(waiting_sushi(t, x));
                m.insert({n, v});
                cnt += 1;
            }
        }
        // 손님 입장
        else if (cmd == 200)
        {
            int x, c;
            string n;

            cin >> x >> n >> c;

            p.insert({n, people(x, 0, c)});

            if (m.find(n) != m.end())
            {
                vector<waiting_sushi> v = m[n];

                for (int i = 0; i < v.size(); i++)
                {
                    int et;
                    v[i].pos = (v[i].pos + (t - v[i].incoming_t)) % L;

                    if (v[i].pos < x)
                    {
                        et = t + (x - v[i].pos);
                        pq.push(eaten_sushi(et, n));
                    }
                    else if (v[i].pos == x)
                    {
                        p[n].eaten += 1;

                        if (p[n].expected == p[n].eaten)
                        {
                            p.erase(n);
                        }
                        cnt -= 1;
                    }
                    else
                    {
                        et = t + (L - v[i].pos) + x;
                        pq.push(eaten_sushi(et, n));
                    }
                }

                m.erase(n);
            }
        }
        // 출력
        else
        {
            cout << p.size() << " " << cnt << "\n";
        }

    }
}