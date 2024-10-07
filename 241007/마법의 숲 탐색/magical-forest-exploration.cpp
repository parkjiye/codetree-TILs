#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct gollem
{
	int id;
	int x;
	int y;
	int d; //0북1동2남3서

	gollem(){}
	gollem(int id, int x, int y, int d):id(id), x(x),y(y),d(d){}
};

struct p
{
	int x;
	int y;
	int g;

	p(){}
	p(int x, int y, int g) :x(x), y(y), g(g){}
};

int R, C, K;
int myMap[77][77];
bool isExit[77][77];
vector<gollem> v;

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };

int wx[5] = {-1, 0, 1, 1, 2};
int wy[5] = { -1, -2, -1, -2, -1 };

int ex[5] = { -1, 0, 1, 1, 2 };
int ey[5] = { 1, 2, 1, 2, 1 };

bool goDown(int id)
{
	id -= 1;
	gollem g = v[id];
	g.x += 1;
	for (int d = 1; d < 4; d++)
	{
		int next_x = g.x + dx[d];
		int next_y = g.y + dy[d];

		if (next_x < 0 || next_x >= R+3 || next_y < 0 || next_y >= C || myMap[next_x][next_y] != 0)
		{
			return false;
		}
	}
	
	myMap[g.x][g.y - 1] = g.id;
	myMap[g.x][g.y + 1] = g.id;
	myMap[g.x + 1][g.y] = g.id;
	g.x -= 1;
	myMap[g.x][g.y - 1] = 0;
	myMap[g.x][g.y + 1] = 0;
	myMap[g.x - 1][g.y] = 0;

	v[id].x = g.x + 1;

	//cout << "Go Down " << v[id].x << " " << v[id].y << "\n";

	return true;
}

bool goWest(int id)
{
	id -= 1;
	gollem g = v[id];
	
	for (int d = 0; d < 5; d++)
	{
		int next_x = g.x + wx[d];
		int next_y = g.y + wy[d];

		if (next_x < 0 || next_x >= R + 3 || next_y < 0 || next_y >= C || myMap[next_x][next_y] != 0)
		{
			return false;
		}
	}

	myMap[g.x + 1][g.y - 1] = g.id;
	myMap[g.x + 1][g.y - 2] = g.id;
	myMap[g.x + 2][g.y - 1] = g.id;

	myMap[g.x - 1][g.y] = 0;
	myMap[g.x][g.y] = 0;
	myMap[g.x][g.y + 1] = 0;

	v[id].x = g.x + 1;
	v[id].y = g.y - 1;
	v[id].d = (g.d -1+4)%4;

	//cout << "Go West " << v[id].x << " " << v[id].y << "\n";

	return true;
}

bool goEast(int id)
{
	id--;
	gollem g = v[id];

	for (int d = 0; d < 5; d++)
	{
		int next_x = g.x + ex[d];
		int next_y = g.y + ey[d];

		if (next_x < 0 || next_x >= R + 3 || next_y < 0 || next_y >= C || myMap[next_x][next_y] != 0)
		{
			return false;
		}
	}

	myMap[g.x + 1][g.y + 1] = g.id;
	myMap[g.x + 1][g.y + 2] = g.id;
	myMap[g.x + 2][g.y + 1] = g.id;

	myMap[g.x - 1][g.y] = 0;
	myMap[g.x][g.y] = 0;
	myMap[g.x][g.y - 1] = 0;

	v[id].x = g.x + 1;
	v[id].y = g.y + 1;
	v[id].d = (g.d+1) % 4;

	//cout << "Go East" << v[id].x << " " << v[id].y << "\n";

	return true;
}

bool checkOverMap()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < C; j++)
		{
			if (myMap[i][j] != 0)
			{
				return true;
			}
		}
	}
	return false;
}

int moveSouth(int id)
{
	id--;
	gollem cur = v[id];

	isExit[cur.x + dx[cur.d]][cur.y + dy[cur.d]] = true;

	/*for (int i = 0; i < R + 3; i++)
	{
		for (int j = 0; j < C; j++)
		{
			cout << isExit[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";*/

	queue<p> q;
	q.push(p(cur.x, cur.y, myMap[cur.x][cur.y]));
	bool visited[77][77];
	memset(visited, 0, sizeof(visited));
	visited[cur.x][cur.y] = true;

	int score = 0;

	while (!q.empty())
	{
		p curr = q.front();
		q.pop();

		if (curr.x > score)
		{
			score = curr.x;
		}

		for (int k = 0; k < 4; k++)
		{
			int next_x = curr.x + dx[k];
			int next_y = curr.y + dy[k];

			if (next_x >= 0 && next_x < R + 3 && next_y >= 0 && next_y < C && myMap[next_x][next_y] != 0 && !visited[next_x][next_y])
			{
				if (myMap[next_x][next_y] == curr.g)
				{
					q.push(p(next_x, next_y, curr.g));
					visited[next_x][next_y] = true;
				}
				else if (myMap[next_x][next_y] != curr.g && isExit[curr.x][curr.y])
				{
					q.push(p(next_x, next_y, myMap[next_x][next_y]));
					visited[next_x][next_y] = true;
				}
			}
		}
	}

	return score - 2;
}

void printmyMap()
{
	for (int i = 0; i < R+3; i++)
	{
		for (int j = 0; j < C; j++)
		{
			cout << myMap[i][j] << " ";
		}
		cout<<"\n";
	}
	cout << "\n";
}

int main()
{
	FASTIO();
	cin >> R >> C >> K;
	memset(myMap, 0, sizeof(myMap));

	long sum = 0;

	for (int k = 1; k <= K; k++)
	{
		//1. 골렘 정보 세팅
		int c, d;
		cin >> c >> d;
		c--;
		d;
		v.push_back(gollem(k, 1, c, d));

		myMap[0][c] = k;
		myMap[1][c - 1] = k;
		myMap[1][c + 1] = k;
		myMap[1][c] = k;
		myMap[2][c] = k;

		//2. 골렘 이동 체크
		while (1)
		{
			//printmyMap();

			if (goDown(k)) continue;
			else
			{
				if (goWest(k)) continue;
				else
				{
					if (goEast(k)) continue;
					else
					{
						break;
					}
				}
			}
			
		}

		//3. 골렘 위치 체크
		if (checkOverMap())
		{
			memset(myMap, 0, sizeof(myMap));
			memset(isExit, 0, sizeof(isExit));
			continue;
		}

		//4. 정령 최종 위치
		
		sum+=moveSouth(k);
		//cout << "sum : " << sum << "\n";
		
	}

    cout<<sum<<"\n";

}