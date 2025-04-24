#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>
typedef long long ll;
const int LEN = 3e5;
const ll MAX = 1e17;
int N, M;
ll cur_area = 0, area = 0;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
} Pizza[LEN];
std::vector<Pos> C, UH, LH;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret =  (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
std::vector<Pos> half_monotone_chain(std::vector<Pos>& C, bool r = 0) {
	std::vector<Pos> H;
	if (!r) std::sort(C.begin(), C.end());
	else if (r) std::reverse(C.begin(), C.end());
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
	return H;
}
Pos ternary_search(std::vector<Pos>& H, const Pos& p1, const Pos& p2) {
	int s = 0, e = H.size() - 1, l, r;
	ll X = MAX;
	Pos EX = H[0];
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ll cl = cross(p1, p2, H[l]), cr = cross(p1, p2, H[r]);
		if (cl > cr) s = l;
		else e = r;
	}
	for (int i = s; i <= e; i++) {
		if (cross(p1, p2, H[i]) < X) {
			X = cross(p1, p2, H[i]);
			EX = H[i];
		}
	}
	return EX;
}
bool cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return ccw(d1, d2, d3) * ccw(d1, d2, d4) < 0;
}
bool cross(const Pos& cur, const Pos& nxt) {
	Pos p1 = cur, p2 = nxt;
	if (p2 < p1) std::swap(p1, p2);
	Pos pl = ternary_search(LH, p1, p2);
	Pos pu = ternary_search(UH, p2, p1);
	return cross(cur, nxt, pu, pl);
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> Pizza[i].x >> Pizza[i].y;
	std::cin >> M;
	C.resize(M);
	for (int i = 0; i < M; i++) std::cin >> C[i].x >> C[i].y;
	LH = half_monotone_chain(C);
	UH = half_monotone_chain(C, 1);
	for (int i = 0, j = 1; i < N; i++) {
		while (i != j && ccw(Pizza[i], Pizza[j], LH[0]) > 0 && !cross(Pizza[i], Pizza[j])) {
			area = std::max(area, cur_area);
			j = (j + 1) % N;
			Pos a = Pizza[i], b = Pizza[j], c = Pizza[(j + N - 1) % N];
			cur_area += std::abs(cross(a, b, c));
		}
		Pos a = Pizza[i], b = Pizza[(i + 1) % N], c = Pizza[j];
		cur_area -= std::abs(cross(a, b, c));
	}
	std::cout << area << "\n";
	return;
}
int main() { solve(); return 0; }//boj10756


	//freopen("sir/sir.in.3f", "r", stdin);
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& pos : C) H.push_back(pos);
//	}
//	else {
//		for (int i = 0; i < C.size(); i++) {
//			while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0)
//				H.pop_back();
//			H.push_back(C[i]);
//		}
//		H.pop_back();
//		int s = H.size() + 1;
//		for (int i = C.size() - 1; i >= 0; i--) {
//			while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0)
//				H.pop_back();
//			H.push_back(C[i]);
//		}
//		H.pop_back();
//	}
//	return H;
//}