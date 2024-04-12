#include<iostream>
#include<string.h>
#include <queue>
#include <vector>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int N, Q;
int parents[100005];
int authority[100005];
bool onoff[100005];
vector<int> child[100005];

int main()
{
	FASTIO();
	cin >> N >> Q;

	for (int q = 1; q <= Q; q++) {
		int cmd;
		cin >> cmd;

		if (cmd == 100) {
			for (int i = 1; i <= N; i++) {
				int x;
				cin >> x;

				parents[i] = x;
				child[x].push_back(i);
			}
			for (int i = 1; i <= N; i++) {
				int x;
				cin >> x;

				authority[i] = x;
			}

			memset(onoff, 1, sizeof(onoff));
		}
		else if (cmd == 200) {
			int c;
			cin >> c;

			onoff[c] = onoff[c] ? 0 : 1;
		}
		else if (cmd == 300) {
			int c, p;
			cin >> c >> p;
			authority[c] = p;
		}
		else if (cmd == 400) {
			int c1, c2;
			cin >> c1 >> c2;

			int c1p = parents[c1];
			int c2p = parents[c2];

			for (int i = 0; i < child[c1p].size(); i++) {
				if (child[c1p][i] == c1) {
					child[c1p][i] = c2;
				}
			}
			for (int i = 0; i < child[c2p].size(); i++) {
				if (child[c2p][i] == c2) {
					child[c2p][i] = c1;
				}
			}

			parents[c1] = c2p;
			parents[c2] = c1p;
		}
		else if (cmd == 500) {
			int c;
			cin >> c;
			queue<pair<int, int>> q;

			int cnt = 0;
			int dph = 1;
			
			for (int i = 0; i < child[c].size(); i++) {
				int ch = child[c][i];
				if (onoff[ch]) {
					if (authority[ch] >= dph) {
						cnt += 1;
						//cout << ch << " ";
					}
					q.push({ ch, dph + 1 });
					
				}
			}

			while (!q.empty()) {
				int cur = q.front().first;
				int cur_dph = q.front().second;
				q.pop();

				for (int i = 0; i < child[cur].size(); i++) {
					int ch = child[cur][i];
					if (onoff[ch]) {
						if (authority[ch] >= cur_dph) {
							cnt += 1;
							//cout << ch << " ";
						}
						q.push({ ch, cur_dph + 1 });
						
					}
				}
			}

			cout << cnt << "\n";
		}
	}

}