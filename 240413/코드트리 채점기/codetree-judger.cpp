#include<iostream>
#include <set>
#include <queue>
#include <map>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int N, Q;

struct waiting_p {
	int p;
	string domain;
	string url;
	int enter_time;

	waiting_p(){}
	waiting_p(int p, string domain, string url, int enter_time):p(p),domain(domain), url(url), enter_time(enter_time){}

	bool const operator<(waiting_p tmp) const {
		if (p == tmp.p) {
			return enter_time > tmp.enter_time;
		}
		return p > tmp.p;
	}
};

set<string> waiting_url;
priority_queue<int, vector<int>, greater<int>> tester;
priority_queue<waiting_p> waiting_list;
map<string, int> testing_list;
map<string, pair<int, int>> history_list;
string test_domain[50005];

void print_all() {
	if (waiting_list.empty()) {
		cout << 0 << "\n";
	}
	else {
		cout << waiting_list.size() << " " << waiting_list.top().domain + "/" + waiting_list.top().url << "\n";
	}

	for (auto it = testing_list.begin(); it != testing_list.end(); it++) {
		cout << it->first << " " << it->second << "\n";
	}
}


int main() {
	FASTIO();
	cin >> Q;

	for (int q = 1; q <= Q; q++) {

		int cmd;
		cin >> cmd;

		if (cmd == 100) {
			string u;
			cin >> N >> u;

			for (int i = 1; i <= N; i++) {
				tester.push(i);
				test_domain[i] = "";
			}
			
			if (waiting_url.find(u) == waiting_url.end()) {
				string domain, url;
				int idx = u.find("/");
				domain = u.substr(0, idx);
				url = u.substr(idx+1);

				waiting_list.push(waiting_p(1, domain, url, 0));
				waiting_url.insert(u);
			}
		}
		else if (cmd == 200) {
			int t, p;
			string u;
			cin >> t >> p >> u;

			if (waiting_url.find(u) == waiting_url.end()) {
				string domain, url;
				int idx = u.find("/");
				domain = u.substr(0, idx);
				url = u.substr(idx + 1);

				waiting_list.push(waiting_p(p, domain, url, t));
				waiting_url.insert(u);
			}
		}
		else if (cmd == 300) {
			int t;
			cin >> t;

			if (tester.empty()) continue;

			//채점가능하다면 채점, 불가능하면 vector에 넣어둠
			vector<waiting_p> v;
			waiting_p test_p;
			test_p.enter_time = -1;

			while (!waiting_list.empty()) {
				waiting_p cur = waiting_list.top();
				waiting_list.pop();

				//도메인이 현재 채점 진행 중인 도메인 중 하나인 경우
				if (testing_list.find(cur.domain) != testing_list.end()) {
					v.push_back(cur);
					continue;
				}
				//가장 최근에 진행된 시간에 대한 기준
				if (history_list.find(cur.domain) != history_list.end()) {
					pair<int, int> p = history_list[cur.domain];

					if (p.first + 3 * (p.second-p.first) > t) {
						v.push_back(cur);
						continue;
					}
				}
				test_p = cur;
				break;
			}

			for (int i = 0; i < v.size(); i++) {
				waiting_list.push(v[i]);
			}

			if (test_p.enter_time==-1) continue;

			//대기중인 url에서 제거
			waiting_url.erase(test_p.domain + "/" + test_p.url);
			
			//가장 앞의 채점기
			int tester_idx = tester.top();
			tester.pop();
			testing_list.insert({ test_p.domain, tester_idx });
			test_domain[tester_idx] = test_p.domain;

			//history에 넣어줌
			if (history_list.find(test_p.domain) != history_list.end()) {
				history_list[test_p.domain] = { t, -1 };
			}
			else {
				history_list.insert({ test_p.domain, {t, -1} });
			}
		}
		else if (cmd == 400) {
			int t, j;
			cin >> t >> j;

			if (test_domain[j] == "") continue;
			//채점기 다시 넣어주기
			string domain = test_domain[j];
			tester.push(j);
			testing_list.erase(domain);
			history_list[domain].second = t;
			test_domain[j] = "";
		}
		else if (cmd == 500) {
			int t;
			cin >> t;
			cout << waiting_list.size() << "\n";
		}

		//print_all();
	}
}