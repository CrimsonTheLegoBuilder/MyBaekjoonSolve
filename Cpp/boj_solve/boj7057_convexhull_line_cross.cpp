#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long double ld;
const ld TOL = 1e-6, MAX = 1e17;
int N, M;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
}U, L, R, D;
std::vector<Pos> H, LH, UH;

ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
std::vector<Pos> half_monotone_chain(std::vector<Pos>& C, bool r = 0) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (r) std::reverse(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	return H;
}
Pos ternary_search(std::vector<Pos>& H, const Pos& p1, const Pos& p2) {
	int s = 0, e = H.size() - 1, l, r;
	ld X = MAX;
	Pos EX = H[0];
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ld cl = cross(p1, p2, H[l]), cr = cross(p1, p2, H[r]);
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
bool X(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return cross(d1, d2, d3) * cross(d1, d2, d4) < 0;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	freopen("dwarfs.in10", "r", stdin);
	freopen("dwarfs.out", "w", stdout);
	std::cin >> N;
	H.resize(N);
	for (int i = 0; i < N; i++) { std::cin >> H[i].x >> H[i].y; }
	LH = half_monotone_chain(H), UH = half_monotone_chain(H, 1);
	while (std::cin >> L.x >> L.y >> R.x >> R.y) {
		bool F = 0;
		if (LH.empty()) { std::cout << "GOOD\n"; continue; }
		if (R < L) std::swap(R, L);
		U = ternary_search(UH, R, L);
		D = ternary_search(LH, L, R);
		if (X(L, R, U, D)) F = 1;
		std::cout << (F ? "BAD\n" : "GOOD\n");
	}
	return 0;
}

//if (I(H, L) || I(H, R)) {
//	std::cout << "GOOD\n";
//	continue;
//}

/*
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long double ld;
const ld TOL = 1e-6, MAX = 1e17;
int N, M;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator<(const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
}U, L, R, D;
std::vector<Pos> T, LH, UH;

ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
std::vector<Pos> lower_hull(std::vector<Pos>& C) {  //lower monotone chain
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	return H;
}
std::vector<Pos> upper_hull(std::vector<Pos>& C) {  //upper monotone_chain
	std::vector<Pos> H;
	//std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (int i = C.size() - 1; i >= 0; i--) H.push_back(C[i]);
		return H;
	}
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	return H;
}
Pos ternary_search(std::vector<Pos>& H, const Pos& p1, const Pos& p2) {
	int s = 0, e = H.size() - 1, l, r;
	ld X = MAX;
	Pos EX = H[0];
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ld cl = cross(p1, p2, H[l]), cr = cross(p1, p2, H[r]);
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
bool X(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return cross(d1, d2, d3) * cross(d1, d2, d4) < 0;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("dwarfs.in10", "r", stdin);
	//freopen("dwarfs.out", "w", stdout);
	std::cin >> N;
	T.resize(N);
	for (int i = 0; i < N; i++) { std::cin >> T[i].x >> T[i].y; }
	std::sort(T.begin(), T.end());
	LH = lower_hull(T), UH = upper_hull(T);
	while (std::cin >> L.x >> L.y >> R.x >> R.y) {
		bool F = 0;
		if (LH.empty()) { std::cout << "GOOD\n"; continue; }
		if (R < L) std::swap(R, L);
		U = ternary_search(UH, R, L);
		D = ternary_search(LH, L, R);
		if (X(L, R, U, D)) F = 1;
		std::cout << (F ? "BAD\n" : "GOOD\n");
	}
	return 0;
}



*/