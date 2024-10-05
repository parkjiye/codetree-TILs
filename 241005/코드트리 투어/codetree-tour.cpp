#include <iostream>
#include <vector>
#include <queue>
#include <string.h>
#include <algorithm>
#include <climits>
#include <set>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

int n, m;
int myMap[2002][2002] = { 0, };
vector<int> dist;
set<int> cancel;

struct t
{
	int v;
	int u;
	int w;

	t(){}
	t(int v, int u, int w):v(v),u(u),w(w){}

	bool const operator<(t tmp) const {
		if (v == tmp.v)
		{
			if (u == tmp.u)
			{
				return w < tmp.w;
			}
			return u < tmp.u;
		}
		return v < tmp.v;
	}
};

struct Good
{
	int idx;
	int revenue;
	int dest;
	int dist;

	Good(){}
	Good(int idx, int revenue, int dest):idx(idx),revenue(revenue), dest(dest){}
	Good(int idx, int revenue, int dest, int dist):idx(idx), revenue(revenue), dest(dest), dist(dist){}

	bool const operator<(Good temp) const {
		if (revenue - dist == temp.revenue - temp.dist)
		{
			return idx > temp.idx;
		}
		return revenue - dist < temp.revenue - temp.dist;
	};
};

void getDistance(int start)
{
	dist.assign(n, INT_MAX);
	dist[start] = 0;
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
	pq.push({ 0, start });

	while (!pq.empty())
	{
		int cur_dist = pq.top().first;
		int cur_node = pq.top().second;
		pq.pop();

		if (cur_dist > dist[cur_node]) continue;

		for (int i = 0; i < n; i++)
		{
			if (myMap[cur_node][i] != 0) {
				int new_dist = cur_dist + myMap[cur_node][i];
				if (new_dist < dist[i])
				{
					dist[i] = new_dist;
					pq.push({ new_dist, i });
				}
			}
		}
	}

	//for (int i = 0; i < n; i++) cout << dist[i] << " ";
}

void createLand()
{
	cin >> n >> m;
	vector<t> line;

	int v, u, w;
	for (int i = 0; i < m; i++)
	{
		cin >> v >> u >> w;

		if (v == u) continue;

		if (v > u) line.push_back(t(u, v, w));
		else line.push_back(t(v, u, w));
		
	}

	sort(line.begin(), line.end());
	

	t cur = line[0];
	myMap[cur.v][cur.u] = cur.w;
	myMap[cur.u][cur.v] = cur.w;

	for (int i = 1; i < line.size(); i++)
	{
		t nxt = line[i];
		if (cur.v == nxt.v && cur.u == nxt.u) continue;

		cur = nxt;
		myMap[cur.v][cur.u] = cur.w;
		myMap[cur.u][cur.v] = cur.w;
	}

	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << myMap[i][j] << " ";
		}
		cout << "\n";
	}*/
	getDistance(0);

}

priority_queue<Good, vector<Good>> goods;

void createGoods()
{
	int i, r, d;
	cin >> i >> r >> d;

	goods.push(Good(i, r, d, dist[d]));
}

void cancelGoods()
{
	int id;
	cin >> id;
	cancel.insert(id);
}

int sellGoods()
{
	while (!goods.empty())
	{
		Good cur = goods.top();
		//삭제된 상품?
		if (cancel.find(cur.idx) != cancel.end()) {
			goods.pop();
			continue;
		}
		else if (cur.revenue < cur.dist || cur.dist==INT_MAX)
		{
			return -1;
		}
		else
		{
			goods.pop();
			return cur.idx;
		}
	}
	return -1;
}

void changeStart()
{
	int s;
	cin >> s;

	getDistance(s);

	priority_queue<Good, vector<Good>> temp; 
	
	while (!goods.empty())
	{
		Good cur = goods.top();
		goods.pop();

		if (cancel.find(cur.idx) != cancel.end()) continue;
		
		temp.push(Good(cur.idx, cur.revenue, cur.dest, dist[cur.dest]));
		
		//if(!goods.empty())cout << goods.top().idx << " " << goods.top().revenue << " " << goods.top().dist << "\n";
	}

	goods = temp;
}

int main()
{
	FASTIO();
	int Q;
	cin >> Q;

	for (int q = 0; q < Q; q++)
	{
		int cmd;
		cin >> cmd;

		if (cmd == 100) createLand();
		else if (cmd == 200) createGoods();
		else if (cmd == 300) cancelGoods();
		else if (cmd == 400) cout << sellGoods() << "\n";
		else changeStart();

		//cout << goods.size() << "\n";
	}

}