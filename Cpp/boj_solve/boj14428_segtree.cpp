#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 100'001;
const int INF = 2e9;
int N, Q, arr[LEN] = { INF };

struct Node {
	int num, idx;
	bool operator < (const Node& n) const {
		return num == n.num ? idx < n.idx : num < n.num;
	}
} tree[LEN << 2];
Node init(int l = 1, int r = N, int n = 1) {
	if (l == r) return tree[n] = { arr[l], l };
	int m = l + r >> 1;
	Node L = init(l, m, n << 1);
	Node R = init(m + 1, r, n << 1 | 1);
	return tree[n] = std::min(L, R);
}
Node update(int idx, int v, int l = 1, int r = N, int n = 1) {
	if (r < idx || idx < l) return tree[n];
	if (tree[n].idx == idx) tree[n].num = v;
	if (l == r) return tree[n];
	if (l != r) {
		int m = l + r >> 1;
		Node L = update(idx, v, l, m, n << 1);
		Node R = update(idx, v, m + 1, r, n << 1 | 1);
		return tree[n] = std::min(L, R);
	}
}
Node search(int s, int e, int l = 1, int r = N, int n = 1) {
	if (r < s || e < l) return { INF, INF };
	if (s <= l && r <= e) return tree[n];
	int m = l + r >> 1;
	Node L = search(s, e, l, m, n << 1);
	Node R = search(s, e, m + 1, r, n << 1 | 1);
	return std::min(L, R);
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) std::cin >> arr[i];
	init();
	//for (int i = 1; i <= N; i++) {
	//	std::cout << tree[i].idx << " min.idx " << tree[i].num << " min.num\n";
	//}
	int q, i, j, v;
	std::cin >> Q;
	while (Q--) {
		std::cin >> q;
		if (q == 1) {
			std::cin >> i >> v;
			arr[i] = v;
			update(i, v);
		}
		else if (q == 2) {
			std::cin >> i >> j;
			std::cout << search(i, j).idx << "\n";
			//std::cout << tree[1].idx << " -> ans, num : " << tree[1].num << "\n";
		}
	}
	return 0;
}


//#include <iostream>
//#include <algorithm>
//#include <cmath>
//typedef long long ll;
//const int LEN = 100'001;
//const int INF = 2e9;
//int N, Q, arr[LEN] = { INF };
//int t[LEN << 2];
//void init_(int l, int r, int i = 1) {
//	if (l == r) { t[i] = l; return; }
//	int m = l + r >> 1;
//	init_(l, m, i << 1);
//	init_(m + 1, r, i << 1 | 1);
//	t[i] = arr[t[i << 1]] <= arr[t[i << 1 | 1]] ?
//		t[i << 1] : t[i << 1 | 1];
//}
//void update_(int idx, int l = 1, int r = N, int i = 1) {
//	if (r < idx || idx < l) return;
//	if (l == r) return;
//	int m = l + r >> 1;
//	update_(idx, l, m, i << 1);
//	update_(idx, m + 1, r, i << 1 | 1);
//
//	t[i] = arr[t[i << 1]] <= arr[t[i << 1 | 1]] ?
//		t[i << 1] : t[i << 1 | 1];
//}
//int get(int l, int r, int s = 1, int e = N, int i = 1) {
//	if (e < l || r < s) return 0;
//	if (l <= s && e <= r) return t[i];
//	int m = s + e >> 1;
//	int L = get(l, r, s, m, i << 1);
//	int R = get(l, r, m + 1, e, i << 1 | 1);
//
//	return arr[L] <= arr[R] ? L : R;
//}
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	for (int i = 1; i <= N; i++) std::cin >> arr[i];
//	init_(1, N);
//	int q, i, v;
//	std::cin >> Q;
//	while (Q--) {
//		std::cin >> q;
//		if (q == 1) {
//			std::cin >> i >> v;
//			arr[i] = v;
//			update_(i);
//		}
//		else if (q == 2) {
//			std::cin >> i >> v;
//			std::cout << get(i, v) << "\n";
//		}
//	}
//	return 0;
//}


