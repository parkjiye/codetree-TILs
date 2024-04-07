#include <iostream>
#include <queue>
#include <string.h>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int n, m, k;
int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };

struct p {
	int x;
	int y;

	p(){}
	p(int x, int y):x(x),y(y){}
};

struct player {
	int idx;
	int power;
	int gun;
	int d;
	p pos;

	player() {};
	player(int idx, int power, int gun, int d, p pos) :idx(idx), power(power), gun(gun), d(d), pos(pos){}
};

struct land {
	priority_queue<int> pq;
	int player;

	land(){}
	land(int player, priority_queue<int> pq):player(player),pq(pq){}
};

land myMap[25][25];
player pl[35];
int points[35];

void print_all() {
	cout << "플레이어 위치"<<"\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << myMap[i][j].player << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	cout << "총 수" << "\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << myMap[i][j].pq.size() << " ";
		}
		cout << "\n";
	}
	cout << "가지고 있는 총" << "\n";
	for (int i = 1; i <= m; i++) {
		cout << pl[i].gun << " ";
	}
	cout << "\n";
	cout << "포인트" << "\n";
	for (int i = 1; i <= m; i++) {
		cout << points[i] << " ";
	}
	cout << "\n";
}

p move(int idx) {
	player cur = pl[idx];
	p cur_pos = pl[idx].pos;

	int next_x = cur_pos.x + dx[cur.d];
	int next_y = cur_pos.y + dy[cur.d];

	int opp = cur.d > 1 ? cur.d - 2 : cur.d + 2;

	if (next_x < 0 || next_x >= n || next_y < 0 || next_y >= n) {
		next_x = cur_pos.x + dx[opp];
		next_y = cur_pos.y + dy[opp];

		pl[idx].d = opp;
	}

	return p(next_x, next_y);
}

void pick_gun(int idx, p cur) {
	//cout << myMap[cur.x][cur.y].pq.size() << "\n";
	myMap[cur.x][cur.y].pq.push(pl[idx].gun);
	pl[idx].gun = myMap[cur.x][cur.y].pq.top();
	myMap[cur.x][cur.y].pq.pop();
	//cout << myMap[cur.x][cur.y].pq.size() << "\n";
}

pair<p, int> fight(int cur, int nxt) {
	int cur_sum = pl[cur].power + pl[cur].gun;
	int nxt_sum = pl[nxt].power + pl[nxt].gun;

	int point = abs(cur_sum - nxt_sum);

	if (cur_sum == nxt_sum) {
		if (pl[cur].power > pl[nxt].power) {
			return { p(cur, nxt),  point};
		}
		else {
			return { p(nxt, cur), point };
		}
	}
	else if (cur_sum > nxt_sum) {
		return { p(cur, nxt), point };
	}
	else {
		return { p(nxt, cur), point };
	}
}

p loser_move(int idx, p cur_pos) {
	int d = pl[idx].d;

	for (int k = 0; k < 4; k++)
	{
		int next_x = cur_pos.x + dx[(d + k) % 4];
		int next_y = cur_pos.y + dy[(d + k) % 4];

		if (next_x >= 0 && next_x < n && next_y >= 0 && next_y < n && myMap[next_x][next_y].player == 0) {
			pl[idx].d = (d + k) % 4;
			return p(next_x, next_y);
		}
	}
}

int main()
{
	FASTIO();
	cin >> n >> m >> k;

	memset(points, 0, sizeof(points));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int x;
			cin >> x;

			if (x != 0) {
				myMap[i][j].pq.push(x);
			}
		}
	}

	for (int i = 0; i < m; i++) {
		int x, y, d, s;
		cin >> x >> y >> d >> s;

		myMap[x - 1][y - 1].player = i + 1;
		pl[i + 1] = player(i + 1, s, 0, d, p(x-1, y-1));
	}

	//print_all();

	for (int t = 0; t < k; t++) {
		for (int i = 1; i <= m; i++) {
			p cur = pl[i].pos;

			//본인의 방향으로 한 칸 이동
			p nxt = move(i);
			//cout << nxt.x << " " << nxt.y << "\n";

			//플레이어가 없는 경우
			if (myMap[nxt.x][nxt.y].player==0) {
				//총이 있는 경우
				if (myMap[nxt.x][nxt.y].pq.size() > 0) {
					//내게 총이 없다면 고대로 득템
					if (pl[i].gun == 0) {
						pl[i].gun = myMap[nxt.x][nxt.y].pq.top();
						myMap[nxt.x][nxt.y].pq.pop();
					}
					//총 줍줍
					else {
						pick_gun(i, nxt);
					}
				}
				pl[i].pos = nxt;
				myMap[cur.x][cur.y].player = 0;
				myMap[nxt.x][nxt.y].player = i;
			}
			else {
				myMap[cur.x][cur.y].player = 0;
				//싸운다
				pair<p, int> f = fight(i, myMap[nxt.x][nxt.y].player);
				int winner = f.first.x;
				int loser = f.first.y;

				points[winner] += f.second;

				//졌다면
				if (pl[loser].gun!=0) {
					myMap[nxt.x][nxt.y].pq.push(pl[loser].gun);
					pl[loser].gun = 0;
				}
				p dest = loser_move(loser, nxt);

				if (myMap[dest.x][dest.y].pq.size()) {
					pl[loser].gun = myMap[dest.x][dest.y].pq.top();
					myMap[dest.x][dest.y].pq.pop();
				}
				pl[loser].pos = dest;
				myMap[dest.x][dest.y].player = loser;

				//이겼다면
				pick_gun(winner, nxt);
				pl[winner].pos = nxt;
				myMap[nxt.x][nxt.y].player = winner;
			}
			//print_all();
		}
	}
    for (int i = 1; i <= m; i++) {
		cout << points[i] << " ";
	}
}