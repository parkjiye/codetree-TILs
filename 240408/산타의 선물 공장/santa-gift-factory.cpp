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
	bool broken;
	int front_box;
	int back_box;
	int cnts;

	belt(){}
	belt(bool broken, int front_box, int back_box, int cnts):broken(broken), front_box(front_box), back_box(back_box), cnts(cnts) {}
};

belt b[15];
int Q, N, M;

void drop_box() {
	long w_max;
	cin >> w_max;
	long sum = 0;
	for (int i = 1; i <= M; i++) {

		if (!b[i].broken && b[i].cnts!=0) {
			long w = m[b[i].front_box].w;

			if (w <= w_max) {
				sum += w;

				int idx = b[i].front_box;

				//상자가 1개 남은 경우
				if (b[i].cnts == 1) {
					b[i].front_box = -1;
					b[i].back_box = -1;
					b[i].cnts = 0;
					m.erase(idx);
				}
				else {
					b[i].front_box = m[idx].b;
					m[m[idx].b].f = -1;
					m.erase(idx);
					b[i].cnts -= 1;
				}
			}
			else {

				if (b[i].cnts == 1) {
					continue;
				}

				int f_idx = b[i].front_box;
				int b_idx = b[i].back_box;

				//맨앞 수정
				m[m[f_idx].b].f = -1;
				b[i].front_box = m[f_idx].b;

				//맨뒤 수정
				m[b_idx].b = f_idx;

				m[f_idx].f = b_idx;
				m[f_idx].b = -1;
				b[i].back_box = f_idx;
				
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

		if (b[cur.b_idx].cnts == 1) {
			b[cur.b_idx] = belt(b[cur.b_idx].broken, -1, -1, 1);
		}
		else if (cur.f == -1) {
			m[cur.b].f = -1;
			b[cur.b_idx].front_box = cur.b;
		}
		else if (cur.b == -1) {
			m[cur.f].b = -1;
			b[cur.b_idx].back_box = cur.f;
		}
		else {
			m[cur.b].f = cur.f;
			m[cur.f].b = cur.b;
		}
		
		m.erase(r_id);
		b[cur.b_idx].cnts -= 1;
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
			int org_front = b[cur.b_idx].front_box;
			m[b[cur.b_idx].front_box].f = b[cur.b_idx].back_box;
			m[b[cur.b_idx].back_box].b = org_front;

			m[m[f_id].f].b = -1;
			b[cur.b_idx].back_box = m[f_id].f;
			m[f_id].f = -1;
			b[cur.b_idx].front_box = f_id;		
		}
	
	}
	else {
		cout << -1 << "\n";
	}
}

void print_all() {
	for (auto it = m.begin(); it != m.end(); it++) {
		cout << it->first << " " << it->second.f << " " << it->second.b << " "<<it->second.b_idx<<"\n";
	}
	for (int i = 1; i <= M; i++) {
		cout << b[i].front_box << " " << b[i].back_box << "\n";
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

	if (b[b_num].cnts == 0) {
		return;
	}

	for (int i = 1; i < M; i++) {
		int ch_idx = b_num + i;
		if (ch_idx > M) {
			ch_idx %= M;
		}
		if (!b[ch_idx].broken) {
			
			m[b[ch_idx].back_box].b = b[b_num].front_box;
			m[b[b_num].front_box].f = b[ch_idx].back_box;

			b[ch_idx].back_box = b[b_num].back_box;

			int idx = b[b_num].front_box;
			while (b[b_num].cnts!=0) {
				m[idx].b_idx = ch_idx;
				idx = m[idx].b;
				b[b_num].cnts -= 1;
				b[ch_idx].cnts += 1;
			}

			b[b_num].back_box = -1;
			b[b_num].front_box = -1;

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
				b[i] = belt(false, 0,0, N/M);
			}
			int cnt = 0;
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N / M; j++) {
					int x;
					cin >> x;

					int fr, ba;
					if (j == 0) {
						fr = -1;
						ba = v[cnt + 1];
						b[i + 1].front_box= v[cnt];
					}
					else if (j == N / M - 1) {
						ba = -1;
						fr = v[cnt - 1];
						b[i + 1].back_box = v[cnt];
					}
					else {
						fr = v[cnt - 1];
						ba = v[cnt + 1];
					}
					m.insert({ v[cnt], box(x, fr, ba, i + 1) });
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