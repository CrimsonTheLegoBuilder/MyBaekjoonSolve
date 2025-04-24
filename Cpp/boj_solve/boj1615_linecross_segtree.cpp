#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
struct Pos {
	int x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return y < p.y;
		return x < p.x;
	}
};
std::vector<Pos> lines;
std::vector<ll> segtree;
ll sum(int s, int e, int l, int r, int n) {
	if (e < l || r < s) return 0;
	if (s <= l && r <= e) return segtree[n];
	int m = l + r >> 1;
	ll L = sum(s, e, l, m, n << 1);
	ll R = sum(s, e, m + 1, r, n << 1 | 1);
	return L + R;
}
void update(int l, int r, int idx, int n, ll diff) {
	if (r < idx || idx < l) return;
	segtree[n] += diff;
	if (l != r) {
		int m = l + r >> 1;
		update(l, m, idx, n << 1, diff);
		update(m + 1, r, idx, n << 1 | 1, diff);
	}
}

int main() {
	int M, a, b;
	ll N, S = 0;
	std::cin >> N >> M;
	segtree.resize(N * 4);
	lines.resize(M);
	for (int m = 0; m < M; m++) {
		std::cin >> lines[m].x >> lines[m].y;
	}
	std::sort(lines.begin(), lines.end());
	for (int m = 0; m < M; m++) {
		b = lines[m].y;
		S += sum(b, N - 1, 0, N - 1, 1);
		//std::cout << b << " " << S << "\n";
		update(0, N - 1, b - 1, 1, 1);
	}
	std::cout << S << "\n";
	return 0;
}