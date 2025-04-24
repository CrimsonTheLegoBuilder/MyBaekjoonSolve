#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
typedef long long ll;
typedef long double ld;
int N;
ld L, P, PI = 3.14159265358979;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
};
std::vector<Pos> A, H;

ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
ll cal_dist_sq(const Pos& d1, const Pos& d2) {
	return (d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y);
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && cross(H[H.size() - 2], H[H.size() - 1], H[H.size() - 1], C[i]) <= 0) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> L;
	P = 2 * L * PI;
	A.resize(N);
	for (int i = 0; i < N; i++) {
		std::cin >> A[i].x >> A[i].y;
	}
	H = monotone_chain(A);
	int l = H.size();
	for (int i = 0; i < l; i++) {
		P += sqrtl(cal_dist_sq(H[i], H[(i + 1) % l]));
	}
	std::cout << (int)(P + 0.5);
	return 0;
}