#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int N, Q;
int parent[100005];
int authority[100005];
int val[100005];
int nx[100005][22];
bool noti[100005];

void print_all() {
	for (int n = 1; n <= N; n++) {
		for (int i = 0; i < 5; i++) {
			cout << nx[n][i] << " ";
		}
		cout << "\n";
	}

	for (int n = 1; n <= N; n++) {
		cout << val[n] << " ";
	}
	cout << "\n";
}

void init() {
	for (int n = 1; n <= N; n++) {
		int x;
		cin >> x;

		parent[n] = x;
	}
	for (int n = 1; n <= N; n++) {
		int x;
		cin >> x;
		//채팅의 권한이 20을 초과하는 경우 20으로 제한
		authority[n] = min(x, 20);
	}
	
	for (int n = 1; n <= N; n++) {
		int cur = n;
		int cur_a = authority[n];

		nx[cur][cur_a]++;

		while (parent[cur] && cur_a) {
			cur = parent[cur];
			cur_a--;
			if (cur_a) nx[cur][cur_a]++;
			val[cur]++;
		}
	}
}

void toggle_noti(int chat) {
	if (noti[chat]) {
		int cur = parent[chat];
		int num = 1;
		//상위 채팅으로 이동하여 noti값에 따라 nx와 val 값을 갱신
		while (cur) {
			for (int i = num; i <= 21; i++) {
				val[cur] += nx[chat][i];
				if (i > num) nx[cur][i - num] += nx[chat][i];
			}
			if (noti[cur])break;
			cur = parent[cur];
			num += 1;
		}
		noti[chat] = false;
	}
	else {
		int cur = parent[chat];
		int num = 1;
		//상위 채팅으로 이동하여 noti값에 따라 nx와 val 값을 갱신
		while (cur) {
			for (int i = num; i <= 21; i++) {
				val[cur] -= nx[chat][i];
				if (i > num) nx[cur][i - num] -= nx[chat][i];
			}
			if (noti[cur])break;
			cur = parent[cur];
			num += 1;
		}
		noti[chat] = true;
	}
}

void change_power(int chat, int power) {
	int bef_power = authority[chat];
	power = min(power, 20);
	authority[chat] = power;

	nx[chat][bef_power]--;
	if (!noti[chat]) {
		int cur = parent[chat];
		int num = 1;
		while (cur) {
			if (bef_power >= num) val[cur]--;
			if (bef_power > num) nx[cur][bef_power - num]--;
			if (noti[cur])break;
			cur = parent[cur];
			num += 1;
		}
	}

	nx[chat][power] += 1;
	if (!noti[chat]) {
		int cur = parent[chat];
		int num = 1;
		// 상위 채팅으로 이동하며 nx와 val 값을 갱신합니다.
		while (cur) {
			if (power >= num) val[cur]++;
			if (power > num) nx[cur][power - num]++;
			if (noti[cur]) break;
			cur = parent[cur];
			num++;
		}
	}
}

void change_parent(int chat1, int chat2) {
	bool bef_noti1 = noti[chat1];
	bool bef_noti2 = noti[chat2];

	if (!noti[chat1]) toggle_noti(chat1);
	if (!noti[chat2]) toggle_noti(chat2);

	swap(parent[chat1], parent[chat2]);

	if (!bef_noti1) toggle_noti(chat1);
	if (!bef_noti2) toggle_noti(chat2);
}

int main() {
	FASTIO();
	cin >> N >> Q;

	memset(noti, 0, sizeof(noti));

	for (int q = 1; q <= Q; q++) {
		int cmd;
		cin >> cmd;

		if (cmd == 100) {	
			init();
		}
		else if (cmd == 200) {
			int c;
			cin >> c;

			toggle_noti(c);
		}
		else if (cmd == 300) {
			int c, p;
			cin >> c >> p;

			change_power(c, p);
		}
		else if (cmd == 400) {
			int c1, c2;
			cin >> c1 >> c2;

			change_parent(c1, c2);
		}
		else if (cmd == 500) {
			int c;
			cin >> c;

			cout << val[c] << "\n";

		}

		//print_all();
	}
}