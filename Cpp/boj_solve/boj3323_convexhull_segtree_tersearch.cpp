#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
const int LEN = 100'000;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return y * p.x > x * p.y; }
	bool operator<=(const Pos& p) const { return y * p.x >= x * p.y; }
	ll dist() const { return x * x + y * y; }
}pos[LEN];
std::vector<Pos> seg[LEN << 2];

ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
void lower_monotone_chain(std::vector<Pos>& H, const Pos C[], int s, int e) {
	if (e - s <= 2) {
		for (int i = s; i <= e; i++) {
			H.push_back(C[i]);
		}
		return;
	}
	for (int i = s; i <= e; i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	return;
}
void init(int s, int e, int i = 1) {
	if (s == e) {
		seg[i] = { pos[s] };
		return;
	}
	int m = s + e >> 1;
	init(s, m, i << 1); init(m + 1, e, i << 1 | 1);
	//std::vector<Pos> C;
	//for (const Pos& p : seg[i << 1]) C.push_back(p);
	//for (const Pos& p : seg[i << 1 | 1]) C.push_back(p);
	lower_monotone_chain(seg[i], pos, s, e);
}

Pos p1, p2;
bool ternary_search(std::vector<Pos>& H) {
	int s = 0, e = H.size() - 1;
	while (e - s >= 3) {
		int l = (s * 2 + e) / 3;
		int r = (s + e * 2) / 3;
		ll L = cross(p1, p2, p2, H[l]);
		ll R = cross(p1, p2, p2, H[r]);
		//if (L <= 0 || R <= 0) return 1;
		if (L < R) e = r;
		else s = l;
	}
	for (int i = s; i <= e; i++) {
		if (cross(p1, p2, p2, H[i]) <= 0) return 1;
	}
	return 0;
}

bool search(const Pos& l, const Pos& r, int s, int e, int i = 1) {
	if (r < pos[s] || pos[e] < l) return 0;
	if (l <= pos[s] && pos[e] <= r) return ternary_search(seg[i]);
	int m = s + e >> 1;
	return search(l, r, s, m, i << 1) || search(l, r, m + 1, e, i << 1 | 1);
}

int K, M;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("4-tri.in", "r", stdin);
	//freopen("4-tri.out", "w", stdout);
	std::cin >> K >> M;
	for (int i = 0; i < K; i++) {
		std::cin >> pos[i].x >> pos[i].y;
	}
	std::sort(pos, pos + K);
	init(0, K - 1);
	while (M--) {
		std::cin >> p1.x >> p1.y >> p2.x >> p2.y;
		if (p2 < p1) std::swap(p1, p2);
		std::cout << (search(p1, p2, 0, K - 1) ? "Y\n" : "N\n");
	}
	return 0;
}