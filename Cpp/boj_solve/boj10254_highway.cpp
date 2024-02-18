#include <iostream>
#include <vector>
#include <algorithm>
typedef long long ll;
int T, N;
ll MD;

struct Pos {
	ll x, y;
	bool operator<(const Pos& p) const {
		if (x == p.x) return (y < p.y);
		return x < p.x;
	}
}C1, C2;
std::vector<Pos> A, H;
struct Vec { ll vx, vy; };

ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
}
ll cross(const Vec& v1, const Vec& v2) {
	return v1.vx * v2.vy - v1.vy * v2.vx;
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
Vec V(std::vector<Pos>& H, int i) {
	int f = (i + 1) % H.size();
	i %= H.size();
	return { H[f].x - H[i].x, H[f].y - H[i].y };

}
ll rotating_calipers(std::vector<Pos>& H) {
	ll MD = 0;
	int i = 0, f2i = 1, l = H.size();
	for (i; i <= l; i++) {
		while ((f2i + 1) % l != i % l && cross(H[i % l], H[(i + 1) % l], H[f2i % l], H[(f2i + 1) % l]) > 0) {
			if (MD < cal_dist_sq(H[i % l], H[f2i % l])) {
				MD = cal_dist_sq(H[i % l], H[f2i % l]);
				C1.x = H[i % l].x, C1.y = H[i % l].y;
				C2.x = H[f2i % l].x, C2.y = H[f2i % l].y;
			}
			f2i++;
		}
		if (MD < cal_dist_sq(H[i % l], H[f2i % l])) {
		MD = cal_dist_sq(H[i % l], H[f2i % l]);
		C1.x = H[i % l].x, C1.y = H[i % l].y;
		C2.x = H[f2i % l].x, C2.y = H[f2i % l].y;
		}
	}
	return MD;
}
ll RC(std::vector<Pos>& H) {
	ll MD = 0;
	int f = 0, l = H.size();
	for (int i = 0; i < l; i++) {
		while (cross(V(H, i), V(H, f)) > 0) {
			if (MD < cal_dist_sq(H[i], H[f])) {
				MD = cal_dist_sq(H[i], H[f]);
				C1.x = H[i].x, C1.y = H[i].y;
				C2.x = H[f].x, C2.y = H[f].y;
			}
			f = (f + 1) % l;
			//if (MD < cal_dist_sq(H[i], H[f])) {
			//	MD = cal_dist_sq(H[i], H[f]);
			//	C1.x = H[i].x, C1.y = H[i].y;
			//	C2.x = H[f].x, C2.y = H[f].y;
			//}
		}
		if (MD < cal_dist_sq(H[i], H[f])) {
			MD = cal_dist_sq(H[i], H[f]);
			C1.x = H[i].x, C1.y = H[i].y;
			C2.x = H[f].x, C2.y = H[f].y;
		}
	}
	return MD;
}




int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//std::cout << std::fixed;
	//std::cout.precision(9);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		A.resize(N);
		for (int i = 0; i < N; i++) {
			std::cin >> A[i].x >> A[i].y;
		}
		H = monotone_chain(A);
		MD = RC(H);
		std::cout << C1.x << " " << C1.y << " " << C2.x << " " << C2.y << "\n";
	}
	return 0;
}