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

	bool const operator<(sushi temp) const {
		return pos > temp.pos;
	}
};

struct guest
{
	long pos;
	long t;
	long n;

	guest(){}
	guest(long pos, long t, long n):pos(pos),t(t),n(n){}
};

map<string, vector<sushi>> noGuestSushi;
map<string, queue<sushi>> GuestSushi;
map<string, guest> guestlist;
set<string> cur_guests;

long L;
long leftover = 0;
int Q;

void makeSushi()
{
	long t, pos;
	string name;

	cin >> t >> pos >> name;
	
	if (cur_guests.find(name) == cur_guests.end())
	{
		noGuestSushi[name].push_back(sushi(pos, t));
	}
	else
	{
		GuestSushi[name].push(sushi(pos, t));
	}
}

void welcomeGuset()
{
	long t, pos, n;
	string name;

	cin >> t >> pos >> name >> n;

	cur_guests.insert(name);
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
				GuestSushi[name].push(sushi((cur.pos + (t - cur.t)) % L, t));
			}
		}

		if (n == 0)
		{
			cur_guests.erase(name);
			leftover += GuestSushi[name].size();
			GuestSushi.erase(name);
			guestlist.erase(name);
		}
		else
		{
			guestlist[name].n = n;
		}
	}
}

void takePicture()
{
	long t;
	cin >> t;

	for (auto it = GuestSushi.begin(); it != GuestSushi.end(); it++)
	{
		if (cur_guests.find(it->first) != cur_guests.end())
		{
			queue<sushi> q= GuestSushi[it->first];
			queue<sushi> temp;
			GuestSushi[it->first] = temp;

			guest g = guestlist[it->first];

			while (!q.empty())
			{
				sushi cur = q.front();
				q.pop();

				if (g.n == 0)
				{
					break;
				}
				else
				{
					long cur_dist = g.pos - cur.pos;
					if (cur_dist < 0) cur_dist = cur_dist + L;

					if (cur_dist <= t - cur.t) {
						g.n--;
					}
					else temp.push(cur);
				}
			}

			if (g.n == 0)
			{
				leftover += temp.size()+ GuestSushi[it->first].size();
				cur_guests.erase(it->first);
				guestlist.erase(it->first);
			}
			else
			{
				GuestSushi[it->first] = temp;
				guestlist[it->first].n = g.n;
			}
		}
	}

	long sum = 0;
	for (auto it = noGuestSushi.begin(); it != noGuestSushi.end(); it++)
	{
		sum += it->second.size();
	}

	for (auto it = GuestSushi.begin(); it != GuestSushi.end(); it++)
	{
		sum += it->second.size();
	}

	cout << cur_guests.size() << " " << sum + leftover << "\n";

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