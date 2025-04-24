#include <iostream>
#include <algorithm>
#include <set>
typedef long long ll;
const int LEN = 10001;

struct Node {
	int n, c;
	bool operator < (const Node& v) const { return n < v.n; }
	bool operator == (const Node& v) const { return n == v.n && c == v.c; }
};
std::set<Node> graph[LEN];
bool V[LEN]{};
ll MAX = 0;
int nxt = 0;

void DFS(int v, ll sum) {
	V[v] = 1;
	bool F = 1;
	for (const Node w : graph[v]) {
		if (!V[w.n]) {
			F = 0; DFS(w.n, sum + w.c);
		}
	}
	if (F) {
		if (MAX < sum) {
			MAX = sum; nxt = v;
		}
	}
	return;
}

int main() {
	int n, v, w, c;
	std::cin >> n;
	for (int i = 1; i < n; i++) {
		std::cin >> v >> w >> c;
		graph[v].insert({ w,c });
		graph[w].insert({ v,c });
	}
	DFS(1, 0);
	memset(V, 0, sizeof(V));
	MAX = 0;
	//std::cout << nxt << "\n";
	DFS(nxt, 0);
	std::cout << MAX << "\n";
	return 0;
}