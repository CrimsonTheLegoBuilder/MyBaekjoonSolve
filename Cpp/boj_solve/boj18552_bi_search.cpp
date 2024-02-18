#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-5;
const ld MAX = 2e8;
const int LEN = 50;
int K, N, M, Z;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (ld n) const { return { x * n, y * n }; }
	Pos operator / (ld n) const { return { x / n, y / n }; }
	Pos operator ~ () const { return { -y, x }; }
	ld mag() { return hypot(x, y); }
} p, FR, FL, RR, RL, direction;
std::vector<Pos> tmp, LH, UH, potholes[LEN];
struct Exs {
	Pos EXU, EXL;
} exs[LEN];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool intersect(const Pos& d1, const Pos& d2, const Pos& s1, const Pos& s2) {
	bool f1 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	bool f2 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	return f1 && f2;
}
bool intersect(int i, const Pos& s1, const Pos& s2) {
	return intersect(exs[i].EXL, exs[i].EXU, s1, s2);
}
bool inner_bi_search(std::vector<Pos>& H, const Pos& x) {
	int sz = H.size();
	if (sz < 2 || cross(H[0], H[1], x) < TOL || cross(H[0], H[sz - 1], x) > -TOL) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		ll C = cross(H[0], H[m], x);
		if (C > 0) s = m;
		else e = m;
	}
	return cross(H[s], H[e], x) > 0;
}
int intersect(ld D) {
	Pos FRd = FR + (direction * D), FLd = FL + (direction * D);
	Pos RRd = RR + (direction * D), RLd = RL + (direction * D);
	bool f1, f2;
	int cnt{ 0 };
	for (int i = 0; i < N; i++) {
		f1 = intersect(i, FR, FRd)
			|| intersect(i, FL, FLd)
			|| intersect(i, RR, RRd)
			|| intersect(i, RL, RLd);
		f2 = inner_bi_search(potholes[i], FRd)
			|| inner_bi_search(potholes[i], FLd)
			|| inner_bi_search(potholes[i], RRd)
			|| inner_bi_search(potholes[i], RLd);
		cnt += f1 || f2;
	}
	return cnt;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
std::vector<Pos> lower_hull(std::vector<Pos>& C) {  //lower monotone chain
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
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
ld bi_search(int K) {
	ld s = 0, e = MAX, m = 0;
	int cnt = 100;
	while (cnt--) {
		m = (s + e) * .5;
		if (K < intersect(m)) e = m;
		else s = m;
	}
	return m;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(8);
	std::cin >> K;
	std::cin >> FR.x >> FR.y >> FL.x >> FL.y >> RL.x >> RL.y >> RR.x >> RR.y;
	std::cin >> N;
	direction = ~(FR - FL);
	ld MAG = direction.mag();
	direction = direction / MAG;
	Pos FRd = FR + (direction * 10);
	for (int i = 0; i < N; i++) {
		std::cin >> M;
		for (int j = 0; j < M; j++) {
			std::cin >> p.x >> p.y >> Z;
			if (!Z) potholes[i].push_back(p);
		}
		tmp = potholes[i];
		potholes[i] = monotone_chain(potholes[i]);
		LH = lower_hull(tmp);
		UH = upper_hull(tmp);
		Pos a = FR, b = FRd;
		if (b < a) std::swap(a, b);
		exs[i].EXL = ternary_search(LH, a, b);
		exs[i].EXU = ternary_search(UH, b, a);
	}
	ld ans = bi_search(K);
	if (ans > MAX - TOL) std::cout << "oo\n";
	else std::cout << ans << "\n";
	return;
}
int main() { solve(); return 0; }//boj18552

/*
1
2 0
0 0
0 -2
2 -2
2
4
1 2 0
1 4 0
3 2 0
3 4 0
4
-1 6 0
-1 8 0
1 6 0
1 8 0

1
2 0
0 0
0 -2
2 -2
1
4
1 2 0
1 4 0
3 2 0
3 4 0

1
2 0
0 0
0 -2
2 -2
2
4
2 2 0
3 2 0
2 4 0
3 2 0
4
0 5 0
0 7 0
-1 5 0
-1 7 0

1
-9999 0
-9999 1
-10000 1
-10000 0
2
4
10000 -1 0
10000 2 0
9999 -1 0
9999 2 0
4
9998 -1 0
9998 2 0
9997 -1 0
9997 2 0

*/

//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//typedef long long ll;
//typedef long double ld;
//const ld TOL = 1e-8;
//const ld MAX = 2e8;
//const int LEN = 50;
//int K, N, M, Z;
//
//bool z(const ld& x) { return std::abs(x) < TOL; }
//struct Pos {
//	ld x, y;
//	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (ld n) const { return { x * n, y * n }; }
//	Pos operator / (ld n) const { return { x / n, y / n }; }
//	Pos operator ~ () const { return { -y, x }; }
//	ld mag() { return hypot(x, y); }
//} p, FR, FL, RR, RL, direction;
//std::vector<Pos> potholes[LEN];
//struct Hulls {
//	std::vector<Pos> U, L;
//	Pos EXU, EXL;
//} hulls[LEN];
//ld cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ld dot(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//	return z(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//bool intersect(const Pos& d1, const Pos& d2, const Pos& s1, const Pos& s2) {
//	bool f1 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
//	bool f2 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
//	return f1 && f2;
//}
//bool intersect(int i, const Pos& s1, const Pos& s2) {
//	//int sz = potholes[i].size();
//	//Pos U = potholes[i][0], D = potholes[i][0];
//	//ld cu = -MAX, cd = MAX;
//	//for (int j = 0; j < sz; j++) {
//	//	if (cross(s1, s2, potholes[i][j]) > cu) cu = cross(s1, s2, potholes[i][j]), U = potholes[i][j];
//	//	if (cross(s1, s2, potholes[i][j]) < cd) cd = cross(s1, s2, potholes[i][j]), D = potholes[i][j];
//	//}
//	//return intersect(U, D, s1, s2);
//	return intersect(hulls[i].EXL, hulls[i].EXU, s1, s2);
//}
//bool inner_check(int i, const Pos& s2) {
//	int sz = potholes[i].size();
//	for (int j = 0; j < sz; j++) {
//		Pos cur = potholes[i][j], nxt = potholes[i][(j + 1) % sz];
//		if (cross(cur, nxt, s2) < TOL) return 0;
//	}
//	return 1;
//}
//bool inner_bi_search(std::vector<Pos>& H, const Pos& x) {
//	int sz = H.size();
//	if (sz < 2 || cross(H[0], H[1], x) < TOL || cross(H[0], H[sz - 1], x) > -TOL) return 0;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		ll C = cross(H[0], H[m], x);
//		if (C > 0) s = m;
//		else e = m;
//	}
//	return cross(H[s], H[e], x) > 0;
//}
//int intersect(ld D) {
//	Pos FRd = FR + (direction * D), FLd = FL + (direction * D);
//	Pos RRd = RR + (direction * D), RLd = RL + (direction * D);
//	bool f1, f2, f3, f4, f5;
//	int cnt{ 0 };
//	for (int i = 0; i < N; i++) {
//		f1 = intersect(i, FR, FRd);
//		f2 = intersect(i, FL, FLd);
//		f3 = intersect(i, RR, RRd);
//		f4 = intersect(i, RL, RLd);
//		//f5 = inner_check(i, FRd) || inner_check(i, FLd) || inner_check(i, RRd) || inner_check(i, RLd);
//		f5 = (inner_bi_search(potholes[i], FRd)
//			|| inner_bi_search(potholes[i], FLd)
//			|| inner_bi_search(potholes[i], RRd)
//			|| inner_bi_search(potholes[i], RLd));
//		cnt += (f1 || f2 || f3 || f4 || f5);
//	}
//	return cnt;
//}
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& p : C) H.push_back(p);
//		return H;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	int s = H.size() + 1;
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	return H;
//}
//std::vector<Pos> lower_hull(std::vector<Pos>& C) {  //lower monotone chain
//	std::vector<Pos> H;
//	//std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& pos : C) H.push_back(pos);
//		return H;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	return H;
//}
//std::vector<Pos> upper_hull(std::vector<Pos>& C) {  //upper monotone_chain
//	std::vector<Pos> H;
//	//std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (int i = C.size() - 1; i >= 0; i--) H.push_back(C[i]);
//		return H;
//	}
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
//			H.pop_back();
//		}
//		H.push_back(C[i]);
//	}
//	return H;
//}
//Pos ternary_search(std::vector<Pos>& H, const Pos& p1, const Pos& p2) {
//	int s = 0, e = H.size() - 1, l, r;
//	ld X = MAX;
//	Pos EX = H[0];
//	while (e - s > 2) {
//		l = (s + s + e) / 3;
//		r = (s + e + e) / 3;
//		ld cl = cross(p1, p2, H[l]), cr = cross(p1, p2, H[r]);
//		if (cl > cr) s = l;
//		else e = r;
//	}
//	for (int i = s; i <= e; i++) {
//		if (cross(p1, p2, H[i]) < X) {
//			X = cross(p1, p2, H[i]);
//			EX = H[i];
//		}
//	}
//	return EX;
//}
//ld bi_search(int K) {
//	ld s = 0, e = MAX, m = 0;
//	int cnt = 100;
//	while (cnt--) {
//		m = (s + e) * .5;
//		//std::cout << intersect(m) << "\n";
//		if (K < intersect(m)) e = m;
//		else s = m;
//	}
//	return m;
//}
//
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(8);
//	std::cin >> K;
//	std::cin >> FR.x >> FR.y >> FL.x >> FL.y >> RL.x >> RL.y >> RR.x >> RR.y;
//	std::cin >> N;
//	direction = ~(FR - FL);
//	Pos FRd = FR + direction, FLd = FL + direction;
//	Pos RRd = RR + direction, RLd = RL + direction;
//	for (int i = 0; i < N; i++) {
//		std::cin >> M;
//		for (int j = 0; j < M; j++) {
//			std::cin >> p.x >> p.y >> Z;
//			if (!Z) potholes[i].push_back(p);
//		}
//		potholes[i] = monotone_chain(potholes[i]);
//		hulls[i].L = lower_hull(potholes[i]);
//		hulls[i].EXL = ternary_search(hulls[i].L, FR, FRd);
//		hulls[i].U = upper_hull(potholes[i]);
//		hulls[i].EXU = ternary_search(hulls[i].U, FRd, FR);
//	}
//	ld MAG = direction.mag();
//	direction = direction / MAG;
//	//std::cout << direction.x << " " << direction.y << "\n";
//	ld ans = bi_search(K);
//	if (ans > MAX - 0.0001) std::cout << "oo\n";
//	else std::cout << ans << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj18552