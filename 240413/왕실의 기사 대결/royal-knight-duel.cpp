#include <iostream>
#include <string.h>
#include <unordered_set>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct p {
	int x;
	int y;

	p(){}
	p(int x, int y):x(x),y(y){}
};

struct soldier {
	p pos;
	p area;
	int power;
	int damage = 0;
	bool dead = false;

	soldier(){}
	soldier(p pos, p area, int power):pos(pos),area(area),power(power){}
};

int L, N, Q;
int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };
int myMap[45][45];
bool trap[45][45];
soldier s[33];

void print_all() {
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cout << myMap[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	for (int i = 1; i <= N; i++) {
		cout << s[i].damage << " ";
	}
	cout << "\n";
}

unordered_set<int> findArroundSoldier(int n, int d) {
	soldier cur = s[n];
	unordered_set<int> arround_s;

	for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
		for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {

			int next_x = i + dx[d];
			int next_y = j + dy[d];

			if (next_x < 0 || next_x >= L || next_y < 0 || next_y >= L || myMap[next_x][next_y] == -1) {
				continue;
			}
			if (myMap[next_x][next_y] > 0 && myMap[next_x][next_y]!=n) {
				arround_s.insert(myMap[next_x][next_y]);
			}
		}
	}

	return arround_s;
}

bool canPushAll(int n, int d) {
	soldier cur = s[n];
	bool canpush = true;

	for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
		for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {

			int next_x = i + dx[d];
			int next_y = j + dy[d];

			if (next_x < 0 || next_x >= L || next_y < 0 || next_y >= L || myMap[next_x][next_y] == -1) {
				return false;
			}
		}
	}

	unordered_set<int> arround_s = findArroundSoldier(n, d);

	for (auto it = arround_s.begin(); it != arround_s.end(); it++) {
		canpush = canpush && canPushAll(*it, d);
	}

	return canpush;
}

void move(int n, int d) {
	soldier cur = s[n];

	for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
		for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {

			myMap[i][j] = 0;
		}
	}

	s[n].pos = p(cur.pos.x + dx[d], cur.pos.y + dy[d]);
	cur = s[n];

	for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
		for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {
			myMap[i][j] = n;
		}
	}

}

void erase_soldier(int n) {
	soldier cur = s[n];

	for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
		for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {
			myMap[i][j] = 0;
		}
	}
}

void onHit(int n) {
	soldier cur = s[n];

	for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
		for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {

			if (trap[i][j]) {
				s[n].damage += 1;
			}
		}
	}

	if (s[n].damage >= s[n].power) {
		s[n].dead = true;
		erase_soldier (n);
	}
}

void push(int n, int d, int dph) {
	soldier cur = s[n];
	unordered_set<int> arround_s = findArroundSoldier(n, d);

	for (auto it = arround_s.begin(); it != arround_s.end(); it++) {
		push(*it, d, dph+1);
	}
	move(n, d);

	if (dph != 0) {
		onHit(n);
	}
}

int main() {
	FASTIO();
	cin >> L >> N >> Q;

	memset(trap, false, sizeof(trap));
	memset(myMap, 0, sizeof(myMap));

	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			int x;
			cin >> x;

			if (x == 1) trap[i][j] = true;
			if (x == 2) myMap[i][j] = -1;
		}
	}

	for (int n = 1; n <= N; n++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;

		s[n] = soldier(p(r - 1, c - 1), p(h - 1, w - 1), k);
	}

	for (int n = 1; n <= N; n++) {
		soldier cur = s[n];

		for (int i = cur.pos.x; i <= cur.pos.x + cur.area.x; i++) {
			for (int j = cur.pos.y; j <= cur.pos.y + cur.area.y; j++) {
				myMap[i][j] = n;
			}
		}
	}

	for (int q = 1; q <= Q; q++) {
		int n, d;
		cin >> n >> d;
		if (s[n].dead) continue;

		if (canPushAll(n, d)) {
			push(n, d, 0);
		}

		//print_all();
	}

	int answer = 0;

	for (int n = 1; n <= N; n++) {
		if (!s[n].dead) answer += s[n].damage;
	}

	cout << answer << "\n";

}