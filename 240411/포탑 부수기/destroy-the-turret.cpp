#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>
#include <queue>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct p {
	int x;
	int y;
	vector<int> v;

	p(){}
	p(int x, int y, vector<int> v) :x(x), y(y), v(v) {}
};

struct turret {
	int idx;
	int power;
	int x;
	int y;
	int attack_t;
	int attacked_t;

	turret(){}
	turret(int power, int x, int y, int attack_t, int attcked_t):power(power),x(x),y(y),attack_t(attack_t), attacked_t(attacked_t){}
	
	bool const operator<(turret tmp) const {
		if (power == tmp.power) {
			if (attack_t == tmp.attack_t) {
				if (x + y == tmp.x + tmp.y) {
					return y < tmp.y;
				}
				return x + y < tmp.x + tmp.y;
			}
			return attack_t < tmp.attack_t;
		}
		return power > tmp.power;
	}
};

int rx[4] = { 0, 1, 0, -1 };
int ry[4] = { 1, 0, -1, 0 };

int tx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int ty[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int myMap[15][15];
bool visited[15][15];

map<int, turret> m;
vector<int> pth;
int N, M, K;

void print_all() {
	for (auto it = m.begin(); it != m.end(); it++) {
		cout << it->first << " " << it->second.power << "\n";
	}
	cout << "\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << myMap[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

bool bfs(int start_x, int start_y, int dest_x, int dest_y) {

	queue<p> q;
	vector<int> v;
	v.push_back(myMap[start_x][start_y]);
	q.push(p(start_x, start_y, v));
	visited[start_x][start_y] = true;

	while (!q.empty()) {
		p cur = q.front();
		q.pop();

		if (cur.x == dest_x && cur.y == dest_y) {
			pth = cur.v;
			return true;
		}
		
		for (int k = 0; k < 4; k++) {
			int next_x = (cur.x + rx[k] + N) % N;
			int next_y = (cur.y + ry[k] + M) % M;

			if (myMap[next_x][next_y] != 0 && !visited[next_x][next_y]) {
				visited[next_x][next_y] = true;
				vector<int> tmp = cur.v;
				tmp.push_back(myMap[next_x][next_y]);
				q.push(p(next_x, next_y, tmp));
			}
		}
	}

	return false;
}

void turret_attack(int a, int b, int k) {

	turret attacker = m[a];
	turret attacked = m[b];
	int pw = attacker.power;
	m[a].attack_t = k;
	m[b].power -= pw;
	m[b].attacked_t = k;


	for (int t = 0; t < 8; t++) {
		int next_x = (attacked.x + tx[t] + N) % N;
		int next_y = (attacked.y + ty[t] + M) % M;

		if (myMap[next_x][next_y] != 0 && myMap[next_x][next_y] != attacker.idx) {
			int cur = myMap[next_x][next_y];
			m[cur].power -= (pw / 2);
			m[cur].attacked_t = k;
		}
	}
}

void razer(int a, int b, int k) {
	turret attacker = m[a];
	turret attacked = m[b];
	memset(visited, 0, sizeof(visited));

	if (bfs(attacker.x, attacker.y, attacked.x, attacked.y)) {

		m[a].attack_t = k;
		int pw = m[a].power;

		for (int i = 1; i < pth.size(); i++) {
			int cur = pth[i];

			if (i != pth.size() - 1) {
				m[cur].attacked_t = k;
				m[cur].power -= (pw / 2);
			}
			else {
				m[cur].attacked_t = k;
				m[cur].power -= pw;
			}
			//cout << pth[i] << " " <<m[cur].power<<"\n";
		}
		//cout << "\n";
	}
	else {
		turret_attack(a, b, k);
	}
}
int main() {
	FASTIO();
	cin >> N >> M >> K;

	int cnt_turret = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int x;
			cin >> x;
			
			if (x != 0) {
				m.insert({ ++cnt_turret, turret(x, i, j, 0, 0) });
				m[cnt_turret].idx = cnt_turret;
				myMap[i][j] = cnt_turret;
			}
			else{
				myMap[i][j] = 0;
			}
		}
	}

	for (int k = 1; k <= K; k++) {
		
		vector<turret> v;
		//공격자 선정
		for (auto it = m.begin(); it != m.end(); it++) {
			v.push_back(it->second);
		}
		sort(v.begin(), v.end());

		//cout << v[0].idx << " " << v[0].power << " " << v[v.size() - 1].idx << " " << v[v.size() - 1].power << "\n";
		
		turret attacker = v[v.size() - 1];
		turret attacked = v[0];
		m[attacker.idx].power += (N + M);

		//공격자의 공격
		//레이저 공격
		razer(attacker.idx, attacked.idx, k);

		//포탑 부서짐
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (myMap[i][j] != 0 && m[myMap[i][j]].power <= 0) {
					m.erase(myMap[i][j]);
					myMap[i][j] = 0;
				}
			}
		}

		//포탑 정비
		for (auto it = m.begin(); it != m.end(); it++) {
			if (it->second.attacked_t!=k && it->second.attack_t!=k) {
				it->second.power += 1;
			}
		}

		//print_all();

		if (m.size() == 1) {
			break;
		}
	}

	vector<turret> v;
	for (auto it = m.begin(); it != m.end(); it++) {
		v.push_back(it->second);
	}
	sort(v.begin(), v.end());

	cout << v[0].power << "\n";

}