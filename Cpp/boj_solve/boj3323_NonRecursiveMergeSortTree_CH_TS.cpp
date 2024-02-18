#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
const int LEN = 1 << 17;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { 
		if (y * p.x > x * p.y) return (x + x + y * y < p.dist());
		return y * p.x > x * p.y; 
	}
	bool operator<=(const Pos& p) const { return y * p.x >= x * p.y; }
	ll dist() const { return x + x + y * y; }
}pos[LEN];
std::vector<Pos> seg[LEN << 1];

ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
void lower_monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	C = H;
	return;
}
void init(int K) {
	for (int i = 1, k; i <= K; i++) {
		k = i;
		for (k |= LEN; k; k >>= 1) seg[k].push_back(pos[i]);
	}
	for (int i = 1; i < LEN << 1; i++) {
		lower_monotone_chain(seg[i]);
	}
}

Pos p1, p2;
bool ternary_search(std::vector<Pos>& H, const Pos& p1, const Pos& p2) {
	if (H.empty()) return 0;
	int s = 0, e = H.size() - 1;
	while (e - s >= 3) {
		int l = (s * 2 + e) / 3;
		int r = (s + e * 2) / 3;
		ll L = cross(p1, p2, p2, H[l]);
		ll R = cross(p1, p2, p2, H[r]);
		if (L <= 0 || R <= 0) return 1;
		if (L < R) e = r;
		else s = l;
	}
	for (int i = s; i <= e; i++) {
		if (cross(p1, p2, p2, H[i]) <= 0) return 1;
	}
	return 0;
}

//bool search(int n, const Pos& p1, const Pos& p2) {
//}

bool query(int l, int r, const Pos& p1, const Pos& p2) {
	l |= LEN, r |= LEN;
	while (l < r) {
		if (l & 1 && ternary_search(seg[l++], p1, p2)) return 1;
		if (r & 1 && ternary_search(seg[--r], p1, p2)) return 1;
		l >>= 1; r >>= 1;
	}
	return 0;
}
int lbound(const Pos H[], int N, const Pos& p) {
	int s = 0, e = N - 1;
	while (s < e) {
		int m = s + e >> 1;
		if (H[m] < p) s = m + 1;
		else e = m;
	}
	return e;
}
int ubound(const Pos H[], int N, const Pos& p) {
	int s = 0, e = N - 1;
	while (s < e) {
		int m = s + e >> 1;
		if (H[m] <= p) s = m + 1;
		else e = m;
	}
	return e;
}

int K, M;
int main() {
	//std::cin.tie(0)->sync_with_stdio(0);
	//std::cout.tie(0);
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	freopen("4-tri.in", "r", stdin);
	freopen("4-tri.out", "w", stdout);
	std::cin >> K >> M;
	for (int i = 1; i <= K; i++) {
		std::cin >> pos[i].x >> pos[i].y;
	}
	std::sort(pos + 1, pos + K + 1);
	init(K);
	while (M--) {
		std::cin >> p1.x >> p1.y >> p2.x >> p2.y;
		if (p2 < p1) std::swap(p1, p2);
		int l = lbound(pos, K, p1);
		int r = ubound(pos, K, p2) - 1;
		std::cout << (query(l, r, p1, p2) ? "Y\n" : "N\n");
	}
	return 0;
}
