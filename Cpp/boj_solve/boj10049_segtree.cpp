#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <set>
#include <queue>
typedef long long ll;
const ll INF = 1e18;
const int LEN = 1e5 + 1;
int W, H, N, a, b, c, d;

int P[LEN];
int find(int i) { return P[i] < 0 ? i : P[i] = find(P[i]); }
int join(int i, int j) {
	i = find(i), j = find(j);
	if (i == j) return 0;
	if (P[i] < P[j]) P[i] += P[j], P[j] = i;
	else P[j] += P[i], P[i] = j;
	return 1;
}
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
//Node init(int l = 1, int r = N, int n = 1) {
//	return tree[n];
//}
//Node search(int s, int e, int l = 1, int r = N, int n = 1) {
//	if (r < s || e < l) return { INF, -INF };
//	if (s <= l && r <= e) return tree[n];
//	int m = l + r >> 1;
//	Node L = search(s, e, l, m, n << 1);
//	Node R = search(s, e, m + 1, r, n << 1 | 1);
//	return L + R;
//}



std::priority_queue<int> X, Y;
int Xcoord[LEN], Ycoord[LEN];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> W >> H >> N;
	X.push(-H); Y.push(-W);
	for (int i = 0; i < N; i++) {
		std::cin >> a >> b >> c >> d;
		if (a == c) {
			Y.push(-b); Y.push(-d);
		}
		else if (b == d) {
			X.push(-a); X.push(-c);
		}
	}

	return;
}
int main() { solve(); return 0; }//boj10049 ï·ªêö¢ªêàÊ (Cutting) 