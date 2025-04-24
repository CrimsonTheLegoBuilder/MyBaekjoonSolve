#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long long ll;
int T = 1, N, M, vx, vy;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator==(const Pos& p) const { return x == p.x && y == p.y; }
} u, l, r, d;
std::vector<Pos> D, P, HD, HP;
Pos pos(ll a, ll b) { return { a, b }; }

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool X(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
}
bool X(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	bool f1 = cross(d1, d2, d3) * cross(d2, d1, d4) > 0;
	bool f2 = cross(d3, d4, d1) * cross(d4, d3, d2) > 0;
	return f1 && f2;
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
bool I(std::vector<Pos>& H, const Pos& x) {
	int h = H.size() - 1;
	if (h < 1 || cross(H[0], H[1], x) < 0 || cross(H[0], H[h], x) > 0) return 0;
	if (X(H[0], H[1], x) || X(H[0], H[h], x)) return 1;
	int s = 0, e = h, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll ccw = cross(H[0], H[m], x);
		if (!ccw) return (dot(H[0], x, H[m]) >= 0);
		else if (ccw > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], x) > 0 || X(H[s], H[e], x);
}
bool brute() {
	for (const Pos& x : HP) if (I(HD, x)) return 1;
	for (const Pos& x : HD) if (I(HP, x)) return 1;
	for (int i = 0; i < HD.size(); i++) {
		for (int j = 0; j < HP.size(); j++) {
			if (X(HD[i], HD[(i + 1) % HD.size()], HP[j], HP[(j + 1) % HP.size()])) return 1;
		}
	}
	return 0;
}
void push_back(std::vector<Pos>& V) {
	ll x1, y1, x2, y2;
	std::cin >> x1 >> y1 >> x2 >> y2;
	V.push_back(pos(x1, y1));
	V.push_back(pos(x2, y1));
	V.push_back(pos(x1, y2));
	V.push_back(pos(x2, y2));
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	while (1) {
		std::cin >> N >> M;
		if (!N && !M) return 0;
		for (int i = 0; i < N; i++) push_back(D);
		for (int i = 0; i < M; i++) push_back(P);
		HD = monotone_chain(D); HP = monotone_chain(P);
		bool F = brute();
		//std::cout << "Case " << T++ << ": It is" << (F ? " not " : " ") << "possible to separate the two groups of vendors.\n\n";
		D.clear(); P.clear();
	}
}




//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	while (T) {
//		bool F = 0;
//		ll x1, y1, x2, y2;
//		std::cin >> N >> M;
//		if (!N && !M) return 0;
//		D.clear(); P.clear();
//		for (int i = 0; i < N; i++) {
//			//std::cin >> x1 >> y1 >> x2 >> y2;
//			//D.push_back(pos(x1, y1)); D.push_back(pos(x2, y1)); D.push_back(pos(x1, y2)); D.push_back(pos(x2, y2));
//			push_back(D);
//		}
//		for (int i = 0; i < M; i++) {
//			//std::cin >> x1 >> y1 >> x2 >> y2;
//			//P.push_back(pos(x1, y1)); P.push_back(pos(x2, y1)); P.push_back(pos(x1, y2)); P.push_back(pos(x2, y2));
//			push_back(P);
//		}
//		HD = monotone_chain(D);
//		HP = monotone_chain(P);
//		//for (const Pos& x : HP) if (I(HD, x)) F = 1;
//		//for (const Pos& x : HD) if (I(HP, x)) F = 1;
//		//for (int i = 0; i < HD.size(); i++) {
//		//	for (int j = 0; j < HP.size(); j++) {
//		//		if (X(HD[i], HD[(i + 1) % HD.size()], HP[j], HP[(j + 1) % HP.size()])) F = 1;
//		//	}
//		//}
//		F = trial();
//		std::cout << "Case " << T++ << ": It is" << (F ? " not " : " ") << "possible to separate the two groups of vendors.\n\n";
//	}
//}