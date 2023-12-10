#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
typedef long double ld;
const int LEN = 200'001;
const ld TOL = 1e-6, MAX = 1e17;
int N, M, Q, q, a = 1, cnt;
ld A, B, C;

bool z(ld x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator<(const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
} U, D, X, Y, P;
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
	ld MIN = MAX;
	Pos EX = H[0];
	while (e - s > 2) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		ld cl = cross(p1, p2, H[l]), cr = cross(p1, p2, H[r]);
		if (cl > cr) s = l;
		else e = r;
	}
	for (int i = s; i <= e; i++) {
		if (cross(p1, p2, H[i]) < MIN) {
			MIN = cross(p1, p2, H[i]);
			EX = H[i];
		}
	}
	return EX;
}
bool intersect(const Pos& x, const Pos& y, const Pos& u, const Pos& d) {
	bool f1 = cross(x, y, u) * cross(x, y, d) < 0;
	bool f2 = z(cross(x, y, u)) || z(cross(x, y, d));
	return f1 || f2;
}
Pos Y_s(ld A, ld B, ld C) { return { 0, C / B }; }
Pos X_s(ld A, ld B, ld C) { return { C / A, 0 }; }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	freopen("fencing_gold/3.in", "r", stdin);
	//freopen("dwarfs.out", "w", stdout);
	std::cin >> N >> Q;
	M = N;
	T.resize(N);
	for (int i = 0; i < N; i++) std::cin >> T[i].x >> T[i].y;
	std::sort(T.begin(), T.end());
	LH = lower_hull(T), UH = upper_hull(T);
	while (Q--) {
		std::cin >> q;
		if (q == 1) {
			std::cin >> P.x >> P.y;
			T.push_back(P);
			cnt++; N++;
			if (cnt == 300) {
				cnt = 0;
				M += 300;
				std::sort(T.begin(), T.end());
				LH = lower_hull(T), UH = upper_hull(T);
			}
		}
		else if (q == 2) {
			bool F = 0;
			std::cin >> A >> B >> C;
			X = X_s(A, B, C);
			Y = Y_s(A, B, C);
			if (Y < X) std::swap(Y, X);
			D = ternary_search(LH, X, Y);
			U = ternary_search(UH, Y, X);
			if (intersect(X, Y, U, D)) F = 1;
			std::cout << a << " " << (F ? "NO\n" : "YES\n");
			ld tmp = cross(X, Y, U);
			int side = z(tmp) ? 0 : tmp > 0 ? 1 : -1;
			for (int i = M; i < N; i++) {
				ld tmp = cross(X, Y, T[i]);
				int ccw = z(tmp) ? 0 : tmp > 0 ? 1 : -1;
				if (!ccw || ccw != side) F = 1;
			}
			std::cout << a++ << " " << (F ? "NO\n" : "YES\n");
		}
	}
	return;
}


int main() { solve(); return 0; }