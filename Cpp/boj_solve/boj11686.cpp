#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long long ll;
const int LEN = 5e5;
int N, M, cnt{ 0 };

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator==(const Pos& p) const { return x == p.x && y == p.y; }
} S[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
}
bool inner_check_bi_search(std::vector<Pos>& H, const Pos& x) {
	int sz = H.size() - 1;
	if (sz < 2 || cross(H[0], H[1], x) < 0 || cross(H[0], H[sz], x) > 0) return 0;
	if (on_seg(H[0], H[1], x) || on_seg(H[0], H[sz], x)) return 1;
	int s = 0, e = sz, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll C = cross(H[0], H[m], x);
		if (!C) return (dot(H[0], x, H[m]) >= 0);
		else if (C > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], x) > 0 || on_seg(H[s], H[e], x);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
std::vector<Pos> H, C;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y;
	H = monotone_chain(C);
	std::cin >> M;
	for (int i = 0; i < M; i++) std::cin >> S[i].x >> S[i].y;
	for (int i = 0; i < M; i++) cnt += inner_check_bi_search(H, S[i]);
	std::cout << cnt << "\n";
	return;
}
int main() { solve(); return 0; }