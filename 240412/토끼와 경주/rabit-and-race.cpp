#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct rabbit {
	int idx;
	int cnt_jump;
	long dst_jump;
	int x;
	int y;
	long score;

	rabbit(int idx,  int cnt_jump, long dst_jump, int x, int y, long score) :idx(idx), cnt_jump(cnt_jump),dst_jump(dst_jump), x(x), y(y), score(score) {}
	rabbit() {}

	bool operator<(const rabbit temp) const
	{
		if (cnt_jump == temp.cnt_jump)
		{
			if (x + y == temp.x + temp.y)
			{
				if (x == temp.x)
				{
					if (y == temp.y)
					{
						return idx > temp.idx;
					}
					return y > temp.y;
				}
				return x > temp.x;
			}
			return x + y > temp.x + temp.y;
		}
		return cnt_jump > temp.cnt_jump;
	}
};


struct cmp
{
    bool operator()(rabbit a, rabbit b)
    {
        if (a.x + a.y == b.x + b.y)
        {
            if (a.x == b.x)
            {
                if (a.y == b.y)
                {
                    return a.idx < b.idx;
                }
                return a.y < b.y;
            }
            return a.x < b.x;
        }
        return a.x + a.y < b.x + b.y;
    }
};

bool top_rabbit_final(rabbit a, rabbit b) {
	return a.score > b.score;
}

struct p {
	int x;
	int y;

	p() {}
	p(int x, int y) :x(x), y(y) {}

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
unordered_map<int, rabbit> r;

int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

p jump(rabbit mvp) {
	vector<p> v;

	for (int k = 0; k < 4; k++) {
		rabbit cur = mvp;
		long d = mvp.dst_jump;
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
						next_x = cur.x - jump_cnt;
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
						jump_cnt -= (M - 1);
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
						next_y = cur.y - jump_cnt;
					}
				}
				else {
					next_y = cur.y + jump_cnt;
				}
			}
			v.push_back(p(cur.x, next_y));
		}
	}

	//  for (int i = 0; i < 4; i++) {
	//  	cout << v[i].x << " " << v[i].y << "\n";
	//  }

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
				long x, y;
				cin >> x >> y;

				r.insert({ x, rabbit(x, 0, y, 0, 0, 0) });
			}
		}
		else if (cmd == 200) {
			int K, S;
			cin >> K >> S;

			priority_queue<rabbit> pq;
			priority_queue<rabbit, vector<rabbit>, cmp> tp;

			for (auto it = r.begin(); it != r.end(); it++)
			{
				pq.push(it->second);
			}

			long total = 0;

			for (int k = 0; k < K; k++) {
				//가장 우선순위가 높은 토끼 뽑기
				rabbit mvp = pq.top();
				pq.pop();

				//멀리 보내주기
				p nxt = jump(mvp);
				
				mvp.x = nxt.x;
				mvp.y = nxt.y;
				mvp.cnt_jump += 1;
				mvp.score -= (nxt.x + nxt.y + 2);
				total += (nxt.x + nxt.y + 2);

				r[mvp.idx]=mvp;

				pq.push(mvp);
				tp.push(mvp);
			}

			for (auto it = r.begin(); it != r.end(); it++) {
				r[it->first].score += total;
			}
			
			r[tp.top().idx].score += S;

		}
		else if (cmd == 300) {
			int pid;
			long L;
			cin >> pid >> L;

			r[pid].dst_jump *= L;
		}
		else {
			
			long long answer = 0;
			for (auto it = r.begin(); it != r.end(); it++)
			{
				if (answer < it->second.score)
				{
					answer = it->second.score;
				}
			}

			cout << answer << "\n";
		}

		//print_all();
	}
}