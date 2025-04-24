#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
const int INF = 2e9;
int N, Q, arr[LEN] = { INF };

struct Node {
	int num;
	Node(int NUM = 0) : num(NUM) {}
	bool operator < (const Node& n) const {
		return num < n.num;
	}
	Node operator + (const Node& n) const {
		return Node(std::min(num, n.num));
	}
} tree[LEN << 2];
Node init(int l = 1, int r = N, int n = 1) {
	if (l == r) return tree[n] = Node(arr[l]);
	int m = l + r >> 1;
	Node L = init(l, m, n << 1);
	Node R = init(m + 1, r, n << 1 | 1);
	return tree[n] = L + R;
}
Node update(int idx, int v, int l = 1, int r = N, int n = 1) {
	if (r < idx || idx < l) return tree[n];
	if (l == r) return tree[n] = v;
	else if (l != r) {
		int m = l + r >> 1;
		Node L = update(idx, v, l, m, n << 1);
		Node R = update(idx, v, m + 1, r, n << 1 | 1);
		return tree[n] = L + R;
	}
}
Node search(int s, int e, int l = 1, int r = N, int n = 1) {
	if (r < s || e < l) return Node(INF);
	if (s <= l && r <= e) return tree[n];
	int m = l + r >> 1;
	Node L = search(s, e, l, m, n << 1);
	Node R = search(s, e, m + 1, r, n << 1 | 1);
	return L + R;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	init();
	int q, i, j, v;
	std::cin >> Q;
	while (Q--) {
		std::cin >> q;
		if (q == 1) {
			std::cin >> i >> v;
			//arr[i] = v;
			update(i, v);
		}
		else if (q == 2) {
			std::cin >> i >> j;
			std::cout << search(i, j).num << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj14438
