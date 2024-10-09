#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#define FASTIO() ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;

struct Node
{
	int id;
	int max_depth;
	int p;
	int c;
	vector<Node*> v;

	Node() {};
	Node(int id, int p,  int c, int max_depth) :id(id), p(p), c(c), max_depth(max_depth) {};
};

unordered_map<int, Node*> tree;

bool checkMaxDepth(Node* cur, int depth)
{
	if (cur->max_depth <= depth) return 0;
	if (cur->p == -1) return 1;
	return checkMaxDepth(tree[cur->p], depth + 1);
}

void addNode()
{
	int a, b, c, d;
	cin >> a >> b >> c >> d;

	Node* n = new Node(a, b, c, d);

	if (tree.size() == 0 || b==-1) tree[a] = n;
	else if(checkMaxDepth(tree[b], 1))
	{
		tree[a] = n;
		tree[b]->v.push_back(n);

		//cout << "tree 추가 성공\n";
	}
	//else cout << "최대 깊이 초과 - 실패" << "\n";
	
}

void changeColor()
{
	int id, c;
	cin >> id >> c;

	if (tree.find(id) == tree.end()) return;

	queue<Node*> q;
	tree[id]->c = c;
	q.push(tree[id]);

	while (!q.empty())
	{
		Node* cur = q.front();
		q.pop();

		for (int i = 0; i < cur->v.size(); i++)
		{
			Node* nxt = cur->v[i];
			nxt->c = c;
			if (nxt->v.size() > 0) q.push(nxt);
		}
	}
}

void getColor()
{
	int id;
	cin >> id;
	if(tree.find(id) != tree.end())cout << tree[id]->c << "\n";
}

void countColor(Node* node, unordered_set<int>& colors)
{
	colors.insert(node->c);

	for (Node* child : node->v) {
		if (colors.size() == 5) return;
		countColor(child, colors);
	}
}

int getScore()
{
	int sum = 0;

	for (auto it = tree.begin(); it != tree.end(); it++)
	{
		unordered_set<int> colors;
		countColor(tree[it->first], colors);
		sum += colors.size() * colors.size();
	}
	return sum;
}



void printTree()
{
	for (auto it = tree.begin(); it!=tree.end(); it++)
	{
		Node* cur = it->second;
		cout << cur->id << " " << cur->c << "\n";

		for (int i = 0; i < cur->v.size(); i++) cout << cur->v[i]->id << " ";
		cout << "\n";
	}
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

		if (cmd == 100) addNode();
		else if (cmd == 200) changeColor();
		else if (cmd == 300) getColor();
		else if (cmd == 400) cout<<getScore()<<"\n";

		//printTree();
	}
}