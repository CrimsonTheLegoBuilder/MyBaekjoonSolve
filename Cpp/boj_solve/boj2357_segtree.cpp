#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
const int INF = 2e9;
int N, Q, arr[LEN];

struct Node {
	int Min, Max;
	Node operator + (const Node& S) const {
		int L = std::min(Min, S.Min);
		int R = std::max(Max, S.Max);
		return { L, R };
	}
	friend std::ostream& operator << (std::ostream& os, const Node& S) {
		os << S.Min << " " << S.Max;
		return os;
	}
} tree[LEN << 2];
Node init(int l = 1, int r = N, int n = 1) {
	if (l == r) return tree[n] = { arr[l], arr[l] };
	int m = l + r >> 1;
	Node L = init(l, m, n << 1);
	Node R = init(m + 1, r, n << 1 | 1);
	//Node M;
	//M.Min = std::min(L.Min, R.Min);
	//M.Max = std::max(L.Max, R.Max);
	//return tree[n] = M;
	return tree[n] = L + R;
}
Node search(int s, int e, int l = 1, int r = N, int n = 1) {
	if (r < s || e < l) return { INF, -INF };
	if (s <= l && r <= e) return tree[n];
	int m = l + r >> 1;
	Node L = search(s, e, l, m, n << 1);
	Node R = search(s, e, m + 1, r, n << 1 | 1);
	//Node M;
	//M.Min = std::min(L.Min, R.Min);
	//M.Max = std::max(L.Max, R.Max);
	//return M;
	return L + R;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> Q;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	init();
	//for (int i = 1; i <= N; i++) {
	//	std::cout << tree[i].idx << " min.idx " << tree[i].num << " min.num\n";
	//}
	int s, e;
	while (Q--) {
		std::cin >> s >> e;
		if (e < s) std::swap(s, e);
		Node q = search(s, e);
		std::cout << q << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj2357