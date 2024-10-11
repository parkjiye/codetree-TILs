#include<iostream>
#include <map>
#include <set>
#include <queue>
#include <vector>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct sushi
{
	long pos;
	long t;

	sushi(){}
	sushi(long pos, long t):pos(pos),t(t){}
};

struct eaten_sushi {
	long expected;
	string name;

	eaten_sushi(){}
	eaten_sushi(long expected, string name):expected(expected), name(name){}

	bool const operator<(eaten_sushi temp) const {
		return expected > temp.expected;
	}
};

struct guest
{
	long pos;
	long t;
	long n;
	bool out = false;

	guest(){}
	guest(long pos, long t, long n):pos(pos),t(t),n(n){}
};

map<string, vector<sushi>> noGuestSushi;
priority_queue<eaten_sushi> pq;
map<string, guest> guestlist;

long L;
long leftover = 0;
int Q;
int guests = 0;

void makeSushi()
{
	long t, pos;
	string name;

	cin >> t >> pos >> name;
	
	if (guestlist.find(name) == guestlist.end())
	{
		noGuestSushi[name].push_back(sushi(pos, t));
	}
	else
	{
		guest g = guestlist[name];
		long cur_d = g.pos - pos;
		if (cur_d < 0) cur_d += L;

		long expected = t + cur_d;

		pq.push(eaten_sushi(expected, name));
	}
}

void welcomeGuset()
{
	long t, pos, n;
	string name;

	cin >> t >> pos >> name >> n;

	guestlist.insert({ name, guest(pos, t, n) });

	if (noGuestSushi.find(name) != noGuestSushi.end())
	{
		vector<sushi> v = noGuestSushi[name];
		noGuestSushi[name].clear();

		for (int i = 0; i < v.size(); i++)
		{
			sushi cur = v[i];

			if ((cur.pos + (t - cur.t)) % L == pos && n>0) n--;
			else
			{
				long cur_d = pos - ((cur.pos + (t - cur.t)) % L);
				if (cur_d < 0) cur_d += L;
				long expected = t + cur_d;
				
				pq.push(eaten_sushi(expected, name));
			}
		}

		if (n == 0)
		{
			//cout << name << " 다먹고 떠남" << "\n";
			guestlist[name].out = true;
		}
		else
		{
			guestlist[name].n = n;
			guests += 1;
		}
	}

	while (!pq.empty())
	{
		if (pq.top().expected > t) break;

		eaten_sushi cur = pq.top();
		pq.pop();

		//cout << cur.name << " " << cur.expected << "\n";

		guest g = guestlist[cur.name];

		if (g.out) leftover += 1;
		else if (g.n == 0) leftover += 1;
		else if (!g.out && g.n > 0)
		{
			//cout << cur.expected << "에 스시 먹는 " << cur.name << "\n";
			guestlist[cur.name].n -= 1;

			if (guestlist[cur.name].n == 0)
			{
				guestlist[cur.name].out = true;
				guests -= 1;
			}
		}
	}
}

void takePicture()
{
	long t;
	cin >> t;

	while (!pq.empty())
	{
		if (pq.top().expected > t) break;

		eaten_sushi cur = pq.top();
		pq.pop();

		//cout << cur.name << " " << cur.expected << "\n";

		guest g = guestlist[cur.name];

		if (g.out) leftover += 1;
		else if (g.n == 0) leftover += 1;
		else if(!g.out && g.n>0)
		{
			//cout << cur.expected << "에 스시 먹는 " << cur.name << "\n";
			guestlist[cur.name].n -= 1;

			if (guestlist[cur.name].n == 0)
			{
				guestlist[cur.name].out = true;
				guests -= 1;
			}
		}
	}


	long sum = 0;
	for (auto it = noGuestSushi.begin(); it != noGuestSushi.end(); it++)
	{
		sum += it->second.size();
	}


	cout << guests << " " << sum + leftover + pq.size() << "\n";

}

int main()
{
	FASTIO();
	cin >> L >> Q;

	for (int q = 0; q < Q; q++)
	{
		int cmd;
		cin >> cmd;

		if (cmd == 100) makeSushi();
		else if (cmd == 200) welcomeGuset();
		else takePicture();

	}
}