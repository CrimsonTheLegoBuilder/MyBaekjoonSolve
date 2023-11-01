#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long long ll;
int N;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	//bool operator==(const Pos& p) const { return x == p.x && y == p.y; }
};
std::vector<Pos> C, H;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
//bool X(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (!cross(d1, d3, d2) && (dot(d1, d3, d2) >= 0));
//}
//std::vector<Pos> monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
void monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
	//std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		//return H;
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
	//return H;
	return;
}
bool I(std::vector<Pos>& H, const Pos& x) {
	int h = H.size() - 1;
	if (h < 2 || cross(H[0], H[1], x) <= 0 || cross(H[0], H[h], x) >= 0) return 0;
	int s = 0, e = h, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll C = cross(H[0], H[m], x);
		if (!C) return (dot(H[0], x, H[m]) > 0);
		else if (C > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], x) > 0;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	C.resize(N);
	for (int i = 0; i < N; i++) {
		std::cin >> C[i].x >> C[i].y;
	}
	monotone_chain(C, H);
	std::cout << (I(H, { 0, 0 }) ? "No" : "Yes") << "\n";
	return 0;
}
