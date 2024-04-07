#include <iostream>
#include <map>
#include <queue>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct box {
	long w;
	int f;
	int b;
	int b_idx;

	box(){}
	box(long w, int f, int b, int b_idx):w(w),f(f),b(b),b_idx(b_idx){}
};

map<int, box> m;

struct belt {
	deque<int> dq;
	bool broken;

	belt(){}
	belt(deque<int> dq, bool broken):dq(dq),broken(broken){}
};

belt b[15];
int Q, N, M;

void drop_box() {
	long w_max;
	cin >> w_max;
	long sum = 0;
	for (int i = 1; i <= M; i++) {

		if (!b[i].broken) {
			long w = m[b[i].dq.front()].w;

			if (w <= w_max) {
				sum += w;
				int idx = b[i].dq.front();
				b[i].dq.pop_front();
				m[b[i].dq.front()].f = -1;
				m.erase(idx);
			}
			else {
				int idx = b[i].dq.front();
				b[i].dq.pop_front();

				//맨앞 수정
				m[b[i].dq.front()].f = -1;

				//맨뒤 수정
				m[b[i].dq.back()].b = idx;

				m[idx].f = b[i].dq.back();
				m[idx].b = -1;
				b[i].dq.push_back(idx);
			}
		}
		
	}
	cout << sum << "\n";
}

void remove_box() {
	long r_id;
	cin >> r_id;

	if (m.find(r_id) != m.end()) {
		box cur = m[r_id];

		if (cur.f == -1) {
			m[cur.b].f = -1;
		}
		else if (cur.b == -1) {
			m[cur.f].b = -1;
		}
		else {
			m[cur.b].f = cur.f;
			m[cur.f].b = cur.b;
		}
		
		m.erase(r_id);

		for (auto it = b[cur.b_idx].dq.begin(); it != b[cur.b_idx].dq.end(); it++) {
			if (*it == r_id) {
				b[cur.b_idx].dq.erase(it);
				break;
			}
		}
		cout << r_id << "\n";
	}
	else {
		cout << -1 << "\n";
	}
}

void check_box() {
	long f_id;
	cin >> f_id;

	if (m.find(f_id) != m.end()) {
		box cur = m[f_id];
		cout << cur.b_idx << "\n";

		if (m[f_id].f == -1) {
			return;
		}
		else {
			m[m[f_id].f].b = -1;
			m[f_id].f = -1;
			m[b[cur.b_idx].dq.front()].f = b[cur.b_idx].dq.back();
			m[b[cur.b_idx].dq.back()].b = b[cur.b_idx].dq.front();
		}
		
		while (b[cur.b_idx].dq.front() != f_id) {
			int idx = b[cur.b_idx].dq.back();
			b[cur.b_idx].dq.pop_back();
			b[cur.b_idx].dq.push_front(idx);
		 }
	}
	else {
		cout << -1 << "\n";
	}
}

void print_all() {
	for (auto it = m.begin(); it != m.end(); it++) {
		cout << it->first << " " << it->second.f << " " << it->second.b << "\n";
	}
	for (int i = 1; i <= M; i++) {
		for (auto it = b[i].dq.begin(); it != b[i].dq.end(); it++) {
			cout << *it << " ";
		}
		cout << "\n";
	}
}

void broke_belt() {
	int b_num;
	cin >> b_num;

	if (b[b_num].broken) {
		cout << -1 << "\n";
		return;
	}
	else {
		cout << b_num << "\n";
	}

	b[b_num].broken = true;

	for (int i = 1; i < M; i++) {
		if (!b[b_num + i].broken) {
			int ch_idx = (b_num + i)%M;
			//cout << ch_idx << "\n";

			m[b[ch_idx].dq.back()].b = b[b_num].dq.front();
			m[b[b_num].dq.front()].f = b[ch_idx].dq.back();

			for (auto it = b[b_num].dq.begin(); it != b[b_num].dq.end(); it++) {
				//cout << *it << "\n";
				m[*it].b_idx = ch_idx;
				b[ch_idx].dq.push_back(*it);
			}
			//cout << "complete" << "\n";
			b[b_num].dq.clear();
			//cout << "complete" << "\n";
			return;
		}
	}
}

int main()
{
	FASTIO();
	cin >> Q;

	for (int q = 0; q < Q; q++) {
		int cmd;
		cin >> cmd;

		if (cmd == 100) {
			// 공장설립
			cin >> N >> M;
			vector<int> v;

			for (int i = 0; i < N; i++) {
				int x;
				cin >> x;
				v.push_back(x);
			}

			for (int i = 1; i <= M; i++) {
				b[i].broken = false;
			}
			int cnt = 0;
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N / M; j++) {
					int x;
					cin >> x;

					b[i + 1].dq.push_back(v[cnt]);

					int f, b;
					if (j == 0) {
						f = -1;
						b = v[cnt + 1];
					}
					else if (j == N / M - 1) {
						b = -1;
						f = v[cnt - 1];
					}
					else {
						f = v[cnt - 1];
						b = v[cnt + 1];
					}
					m.insert({ v[cnt], box(x, f, b, i + 1) });
					cnt += 1;
				}
			}

		}
		else if (cmd == 200) {
			// 물건 하차
			drop_box();
		}
		else if (cmd == 300) {
			// 물건 제거
			remove_box();
		}
		else if (cmd == 400) {
			//물건 확인
			check_box();
		}
		else {
			//벨트 고장
			broke_belt();
		}

		//print_all();
	}
}