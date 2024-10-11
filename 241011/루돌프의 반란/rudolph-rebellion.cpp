#include <iostream>
#include <queue>
#include <string.h>
#include <math.h>
#include <climits>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int N, M, P, C, D;

struct p
{
	int x;
	int y;
	int d;
	int id;

	p(){}
	p(int x, int y):x(x),y(y){}
	p(int x, int y, int d) :x(x), y(y),d(d) {}
	p(int x, int y, int d, int id):x(x),y(y),d(d),id(id){}

	bool const operator<(p temp) const {
		if (d == temp.d)
		{
			if (x == temp.x)
			{
				return y < temp.y;
			}
			return x < temp.x;
		}
		return d > temp.d;
	}
};

struct santa
{
	p pos;
	int sleep = -2;
	bool dead = false;
	int score = 0;

	santa(){}
	santa(p pos):pos(pos){}
};

santa santas[33];
int myMap[55][55];
p rudolf;

int rx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int ry[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int sx[4] = { -1, 0, 1, 0 };
int sy[4] = { 0, 1, 0, -1 };

bool allDead()
{
	for (int i = 1; i <= P; i++)
	{
		if (!santas[i].dead) return false;
	}
	return true;
}

void printScore()
{
	for (int i = 1; i <= P; i++)
	{
		cout << santas[i].score << " ";
	}
	cout << "\n";
}

void printMap()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << myMap[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int pickSanta()
{
	priority_queue<p> pq;

	for (int i = 1; i <= P; i++)
	{
		if (santas[i].dead) continue;

		santa cur = santas[i];

		int x_d = abs(cur.pos.x - rudolf.x);
		int y_d = abs(cur.pos.y - rudolf.y);

		pq.push(p(cur.pos.x, cur.pos.y, x_d * x_d + y_d * y_d, i));
	}

	return pq.top().id;
}

int checkDir(p p1, p p2)
{
	vector<pair<int, int>> v;

	for (int k = 0; k < 8; k++)
	{
		p nxt = p(p1.x + rx[k], p1.y + ry[k]);

		if (nxt.x >= 0 && nxt.x < N && nxt.y >= 0 && nxt.y < N)
		{
			int x_d = abs(p2.x - nxt.x);
			int y_d = abs(p2.y - nxt.y);
			v.push_back({ x_d * x_d + y_d * y_d, k });
		}
	}

	int distance = INT_MAX;
	int dir;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i].first < distance)
		{
			distance = v[i].first;
			dir = v[i].second;
		}
	}

	return dir;
}

void interaction(int d, int s, int m)
{
	p nxt = p(santas[m].pos.x + rx[d], santas[m].pos.y + ry[d]);

	if (nxt.x >= 0 && nxt.x < N && nxt.y >= 0 && nxt.y < N)
	{
		if (myMap[nxt.x][nxt.y] == 0)
		{
			santas[m].pos = nxt;
			myMap[nxt.x][nxt.y] = m;
			return;
		}
		else
		{
			interaction(d, m, myMap[nxt.x][nxt.y]);
			santas[m].pos = nxt;
			myMap[nxt.x][nxt.y] = m;
		}
	}
	else
	{
		myMap[santas[m].pos.x][santas[m].pos.y] = 0;
		santas[m].dead = true;
		return;
	}
}

void crash(int d, int s, int m, int score)
{
	santas[s].sleep = m;
	santas[s].score += score;

	p nxt = p(santas[s].pos.x + rx[d]*score, santas[s].pos.y + ry[d]*score);

	if (nxt.x >= 0 && nxt.x < N && nxt.y >= 0 && nxt.y < N)
	{
		if (myMap[nxt.x][nxt.y] == 0)
		{
			myMap[santas[s].pos.x][santas[s].pos.y] = 0;
			santas[s].pos = nxt;
			myMap[santas[s].pos.x][santas[s].pos.y] = s;
		}
		else // 상호작용
		{
			interaction(d, s, myMap[nxt.x][nxt.y]);
			santas[s].pos = nxt;
			myMap[nxt.x][nxt.y] = s;
		}
	}
	else
	{
		//myMap[santas[s].pos.x][santas[s].pos.y] = 0;
		santas[s].dead = true;
	}

}

void moveRudolf(int santa, int m)
{
	int selectedDir = checkDir(rudolf, santas[santa].pos);
	//cout <<selectedDir << "\n";

	p nxt = p(rudolf.x + rx[selectedDir], rudolf.y + ry[selectedDir]);
	//cout << nxt.x << " " << nxt.y << "\n";

	if (myMap[nxt.x][nxt.y] == 0)
	{
		myMap[rudolf.x][rudolf.y] = 0;
		rudolf = nxt;
		myMap[rudolf.x][rudolf.y] = -1;
	}
	else
	{
		crash(selectedDir, myMap[nxt.x][nxt.y], m, C);
		myMap[rudolf.x][rudolf.y] = 0;
		rudolf = nxt;
		myMap[rudolf.x][rudolf.y] = -1;
	}
}

void moveSanta(int s, int m)
{
	int dir = -1;
	
	p cur = santas[s].pos;
	int distance = abs(cur.x - rudolf.x) * abs(cur.x - rudolf.x) + abs(cur.y - rudolf.y) * abs(cur.y - rudolf.y);

	for (int k = 0; k < 4; k++)
	{
		p nxt = p(cur.x + sx[k], cur.y + sy[k]);

		if (nxt.x >= 0 && nxt.x < N && nxt.y >= 0 && nxt.y < N && myMap[nxt.x][nxt.y] <= 0 )
		{
			int x_d = abs(nxt.x - rudolf.x);
			int y_d = abs(nxt.y - rudolf.y);
			int tmp_d = x_d * x_d + y_d * y_d;

			if (tmp_d < distance)
			{
				dir = k;
				distance = tmp_d;
			}
		}
	}

	if (dir == -1) return;

	p nxt = p(cur.x + sx[dir], cur.y + sy[dir]);

	if (myMap[nxt.x][nxt.y] == -1)
	{
		if (dir == 0) dir = 6;
		else if (dir == 1) dir = 3;
		else if (dir == 2) dir = 1;
		else dir = 4;

		myMap[santas[s].pos.x][santas[s].pos.y] = 0;
		santas[s].pos = rudolf;
		crash(dir, s, m, D);
	}
	else
	{
		myMap[santas[s].pos.x][santas[s].pos.y] = 0;
		santas[s].pos = nxt;
		myMap[santas[s].pos.x][santas[s].pos.y] = s;
	}
	
}

int main()
{
	FASTIO();
	cin >> N >> M >> P >> C >> D;

	memset(myMap, 0, sizeof(myMap));
	int x, y;
	cin >> x >> y;

	rudolf = p(x - 1, y - 1);
	myMap[x - 1][y - 1] = -1;

	for (int i = 0; i < P; i++)
	{
		int id, x, y;
		cin >> id >> x >> y;

		santas[id] = santa(p(x - 1, y - 1));
		myMap[x - 1][y - 1] = id;
	}

	for (int m = 0; m < M; m++)
	{
		if (allDead()) break;

		//루돌프
		int selectedSanta = pickSanta();
		//cout << selectedSanta << "\n";
		moveRudolf(selectedSanta, m);

		//printMap();

		//산타
		for (int i = 1; i <= P; i++)
		{
			santa cur = santas[i];
			if (!cur.dead && cur.sleep + 2 <= m)
			{
				moveSanta(i, m);
			}
		}
		
		//printMap();

		for (int i = 1; i <= P; i++)
		{
			if (!santas[i].dead) santas[i].score += 1;
		}

		//printScore();
		
	}

	printScore();
}