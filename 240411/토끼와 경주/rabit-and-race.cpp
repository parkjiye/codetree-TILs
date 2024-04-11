#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct rabbit {
	int idx;
	long d;
	long cnt;
	int x;
	int y;
	long score;
	int j_t;

	rabbit(int idx, long d, long cnt, int x, int y, long score, int j_t):idx(idx),d(d),cnt(cnt),x(x),y(y),score(score), j_t(j_t){}
	rabbit(){}
};

bool run_rabbit(rabbit a, rabbit b) {
	if (a.cnt == b.cnt) {
		if (a.x + a.y == b.x + b.y) {
			if (a.x == b.y) {
				if (a.y == b.y) {
					return a.idx < b.idx;
				}
				return a.y < b.y;
			}
			return a.x < b.x;
		}
		return a.x + a.y < b.x + b.y;
	}
	return a.cnt < b.cnt;
}

bool top_rabbit(rabbit a, rabbit b) {
	if (a.x + a.y == b.x + b.y) {
		if (a.x == b.y) {
			if (a.y == b.y) {
				return a.idx > b.idx;
			}
			return a.y > b.y;
		}
		return a.x > b.x;
	}
	return a.x + a.y > b.x + b.y;
}

bool top_rabbit_final(rabbit a, rabbit b) {
	return a.score > b.score;
}

struct p {
	int x;
	int y;

	p(){}
	p(int x, int y):x(x),y(y){}

	bool const operator<(p tmp)const {
		if (x + y == tmp.x + tmp.y) {
			if (x == tmp.x) {
				return y > tmp.y;
			}
			return x > tmp.x;
		}
		return x + y > tmp.x + tmp.y;
	}
};

int N, M, P, Q;
vector<rabbit> r;

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };
map<int, long> jump_dist;

p jump() {
	vector<p> v;
	for (int k = 0; k < 4; k++) {
		rabbit cur = r[0];
		int d = jump_dist[cur.idx];
		long next_x, next_y;

		//상
		if (k == 0) {
			if (cur.x - d >= 0) {
				next_x = cur.x - d;
			}
			else {
				long jump_cnt = d % ((N - 1) * 2);

				if (jump_cnt == 0) {
					next_x = cur.x;
				}
				else if (jump_cnt > cur.x) {
					jump_cnt -= cur.x;
					cur.x = 0;

					if (jump_cnt >= N) {
						jump_cnt -= (N - 1);
						next_x = (N - 1) - jump_cnt;
					}
					else {
						next_x = jump_cnt;
					}
				}
				else {
					next_x = cur.x - jump_cnt;
				}
			}

			v.push_back(p(next_x, cur.y));
		}
		//하
		else if (k == 1) {
			if (cur.x + d < N) {
				next_x = cur.x + d;
			}
			else {
				long jump_cnt = d % ((N - 1) * 2);

				if (jump_cnt == 0) {
					next_x = cur.x;
				}
				else if (jump_cnt > N - cur.x - 1) {
					jump_cnt -= (N - cur.x - 1);
					cur.x = N - 1;

					if (jump_cnt >= N) {
						jump_cnt -= (N - 1);
						next_x = (N - 1 - jump_cnt);
					}
					else {
						next_x = cur.x-jump_cnt;
					}
				}
				else {
					next_x = cur.x + jump_cnt;
				}
			}
			v.push_back(p(next_x, cur.y));
		}
		//좌
		else if (k == 2) {
			if (cur.y - d >= 0) {
				next_y = cur.y - d;
			}
			else {
				long jump_cnt = d % ((M - 1) * 2);

				if (jump_cnt == 0) {
					next_y = cur.y;
				}
				else if (jump_cnt > cur.y) {
					jump_cnt -= cur.y;
					cur.y = 0;

					if (jump_cnt >= M) {
						jump_cnt -= (M- 1);
						next_y = (M - 1) - jump_cnt;
					}
					else {
						next_y = jump_cnt;
					}
				}
				else {
					next_y = cur.y - jump_cnt;
				}
			}

			v.push_back(p(cur.x, next_y));
		}
		//우
		else {
			if (cur.y + d < M) {
				next_y = cur.y + d;
			}
			else {
				long jump_cnt = d % ((M - 1) * 2);

				if (jump_cnt == 0) {
					next_y = cur.y;
				}
				else if (jump_cnt > M - cur.y - 1) {
					jump_cnt -= (M - cur.y - 1);
					cur.y = M - 1;

					if (jump_cnt >= M) {
						jump_cnt -= (M - 1);
						next_y = (M - 1 - jump_cnt);
					}
					else {
						next_y = cur.y-jump_cnt;
					}
				}
				else {
					next_y = cur.y + jump_cnt;
				}
			}
			v.push_back(p(cur.x, next_y));
		}

		
	}

	// for (int i = 0; i < 4; i++) {
	// 	cout << v[i].x << " " << v[i].y << "\n";
	// }

	 sort(v.begin(), v.end());

	 //cout <<"next_pos is "<< v[0].x << " " << v[0].y << "\n";
	 return v[0];
}

void print_all() {
	for (int i = 0; i < P; i++) {
		cout << r[i].idx << " " << r[i].x << " " << r[i].y << " " << r[i].score << "\n";
	}
}

int main() {
	FASTIO();
	cin >> Q;

	for (int q = 1; q <= Q; q++) {
		int cmd;
		cin >> cmd;

		if (cmd == 100) {
			cin >> N >> M >> P;

			for (int i = 1; i <= P; i++) {
				int x, y;
				cin >> x >> y;

				r.push_back(rabbit(x, y, 0, 0, 0, 0, 0));
				jump_dist.insert({ x, y });
			}
		}
		else if (cmd == 200) {
			int K, S;
			cin >> K >> S;

			long total = 0;

			for (int k = 0; k < K; k++) {
				//가장 우선순위가 높은 토끼 뽑기
				sort(r.begin(), r.end(), run_rabbit);
				//cout << r[0].idx << "\n";

				//멀리 보내주기
				p nxt = jump();
				r[0].x = nxt.x;
				r[0].y = nxt.y;
				r[0].score -= (nxt.x + nxt.y+2);
				total += (nxt.x + nxt.y+2);
				r[0].cnt += 1;
				r[0].j_t = q;
			}

			for (int p = 0; p < P; p++) {
				r[p].score += total;
			}
			sort(r.begin(), r.end(), top_rabbit);

			int idx = 0;
			while (1) {
				if (r[0].j_t == q) {
					r[0].score += S;
					break;
				}
				else {
					idx += 1;
				}
			}

		}
		else if (cmd == 300) {
			int pid;
			long L;
			cin >> pid >> L;
			
			jump_dist[pid] *= L;
		}
		else {
			sort(r.begin(), r.end(), top_rabbit_final);
			cout << r[0].score << "\n";
		}

		//print_all();
	}
}