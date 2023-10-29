#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long long ll;
int N, M, vx, vy;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator==(const Pos& p) const { return x == p.x && y == p.y; }
}s;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool X(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
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
std::vector<Pos> H, D;
ll GCD(ll x, ll y) {
	if (!y) return x;
	return GCD(y, x % y); 
}
ll A(std::vector<Pos>& H) {  // return area * 2;
	Pos p = { 0, 0 };
	int l = H.size();
	ll a = 0;
	for (int i = 0; i < l; i++) {
		a += cross(p, H[i], H[(i + 1) % l]);
	}
	return a;
}
bool I(std::vector<Pos>& H, const Pos& x) {
	int h = H.size() - 1;
	if (h < 2 || cross(H[0], H[1], x) < 0 || cross(H[0], H[h], x) > 0) return 0;
	if (X(H[0], H[1], x) || X(H[0], H[h], x) > 0) return 1;
	int s = 0, e = h, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll C = cross(H[0], H[m], x);
		if (!C) return (dot(H[0], x, H[m]) >= 0);
		else if (C > 0) s = m;
		else e = m;
	}
	//if (X(H[s], H[e], x)) return 1;
	return cross(H[s], H[e], x) >= 0;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	D.resize(N);
	for (int i = 0; i < N; i++) {
		std::cin >> D[i].x >> D[i].y;
	}
	H = monotone_chain(D);
	ll area = A(H);
	if (!area) {
		if (H.size() == 1) {
			bool f = 1;
			for (int i = 0; i < M; i++) {
				std::cin >> s.x >> s.y;
				if (s == H[0]) f = 0;
			}
			std::cout << f << "\n";
			return 0;
		}
		else if (H.size() == 2) {
			vx = std::abs(H[0].x - H[1].x);
			vy = std::abs(H[0].y - H[1].y);
			int cnt = GCD(vx, vy) + 1;
			//int cnt = std::gcd(vx, vy) + 1;
			for (int i = 0; i < M; i++) {
				std::cin >> s.x >> s.y;
				if (X(H[0], H[1], s)) cnt--;
			}
			std::cout << cnt << "\n";
			return 0;
		}
	}
	int l = H.size(), b = 0;
	for (int i = 0; i < l; i++) {
		vx = std::abs(H[i].x - H[(i + 1) % l].x);
		vy = std::abs(H[i].y - H[(i + 1) % l].y);
		b += GCD(vx, vy);
	}
	//Pick`s Theorem : A = i + b/2 - 1
	// 2i = 2A - b + 2
	ll t = area - b + 2;
	t >>= 1;
	//std::cout << t << " " << b << " " << area << " DEBUG\n";

	t += b;
	for (int i = 0; i < M; i++) {
		std::cin >> s.x >> s.y;
		if (I(H, s)) t--;
	}
	std::cout << t << "\n";
	return 0;
}


/*
4 9
1 -1
1 1
-1 -1
-1 1
0 0
1 -1
1 1
-1 -1
-1 1
1 0
0 1
-1 0
0 -1

3 6
0 0
2 0
0 4
1 2
1 1
1 0
0 1
0 2
0 3
*/