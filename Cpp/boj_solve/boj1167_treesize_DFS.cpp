#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstring>
typedef long long ll;
const int LEN = 100'001;
ll MAX = 0;
int nxt = 0;

struct Node {
	int node, cost;
	bool operator < (const Node& v) const {
		return node < v.node;
	}
	bool operator == (const Node& v) const {
		return node == v.node && cost == v.cost;
	}
};

//std::vector<Node> graph[LEN];
std::set<Node> graph_s[LEN];
bool visited[LEN]{};

void DFS(int v, ll sum) {
	visited[v] = 1;
	bool flag = 1;
	for (const Node w : graph_s[v]) {
		if (!visited[w.node]) {
			DFS(w.node, sum + w.cost);
			flag = 0;
		}
	}
	if (flag) {
		if (MAX < sum) {
			MAX = sum; nxt = v;
			return;
		}
	}
	return;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	int n, v, w, c;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
		std::cin >> v;
		while (1) {
			std::cin >> w;
			if (!~w) break;
			std::cin >> c;
			//graph[v].push_back({ w, c });
			//graph[w].push_back({ v, c });
			graph_s[v].insert({ w, c });
			graph_s[w].insert({ v, c });
		}
	}
	DFS(1, 0);
	memset(visited, 0, sizeof(visited));
	MAX = 0;
	DFS(nxt, 0);
	std::cout << MAX << "\n";
	return 0;
}