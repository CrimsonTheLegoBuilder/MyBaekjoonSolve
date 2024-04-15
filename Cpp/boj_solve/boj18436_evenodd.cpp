#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
const int INF = 2e9;
int N, Q, arr[LEN];

struct Node {
	int even, odd;
	Node(int E = 0, int O = 0) : even(E), odd(O) {}
	Node operator + (const Node& n) const {
		return Node(even + n.even, odd + n.odd);
	}
} tree[LEN << 2];
Node node(const int& x) { int f = x & 1; return Node(!f, f); }
Node init(int l = 1, int r = N, int n = 1) {
	if (l == r) return tree[n] = node(arr[l]);
	int m = l + r >> 1;
	Node L = init(l, m, n << 1);
	Node R = init(m + 1, r, n << 1 | 1);
	return tree[n] = L + R;
}
Node update(int idx, int x, int l = 1, int r = N, int n = 1) {
	if (r < idx || idx < l) return tree[n];
	if (l == r) { arr[n] = x; return tree[n] = node(x); }
	else if (l != r) {
		int m = l + r >> 1;
		Node L = update(idx, x, l, m, n << 1);
		Node R = update(idx, x, m + 1, r, n << 1 | 1);
		return tree[n] = L + R;
	}
}
Node search(int s, int e, int l = 1, int r = N, int n = 1) {
	if (r < s || e < l) return Node(0, 0);
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
			update(i, v);
		}
		else if (q == 2) {
			std::cin >> i >> j;
			std::cout << search(i, j).even << "\n";
		}
		else if (q == 3) {
			std::cin >> i >> j;
			std::cout << search(i, j).odd << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj18436
