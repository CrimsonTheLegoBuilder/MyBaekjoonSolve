#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ll INF = 1e17;
int N;

struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
};
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	int sz = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > sz && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	//freopen("boj9276_fo.in", "r", stdin);
	//freopen("boj9276_ans_txt", "w", stdout);
	while (std::cin >> N) {
		C.resize(N);
		for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i] *= 2;
		H = monotone_chain(C);
		N = H.size();
		if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; continue; }

		int R{ 0 }, U{ 0 }, L{ 0 };
		for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
		for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
		for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }

		ld MIN{ INF };
		for (int i = 0; i < N; i++) {
			while (dot(H[i], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
			while (cross(H[i], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
			while (dot(H[i], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
			ld h = cross(H[i], H[(i + 1) % N], H[U % N]) / (H[i] - H[(i + 1) % N]).mag();
			ld w = dot(H[i], H[(i + 1) % N], H[L % N], H[R % N]) / (H[i] - H[(i + 1) % N]).mag();
			MIN = std::min(MIN, h + w);
		}
		std::cout << MIN << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj9276

/*

3
0 0
2 0
1 2

3
0 0
2 0
-2 2

4.4721359 0.893327

*/


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//typedef long long ll;
//typedef long double ld;
//const ll INF = 1e17;
//const int LEN = 1e4;
//int N;
//
//struct Pos {
//	ll x, y;
//	Pos(ll X, ll Y) : x(X), y(Y) {}
//	Pos() : x(0), y(0) {}
//	//bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	//Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	//Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	//Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x }; }
//	//ll operator ! () const { return x * y; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	ll Euc() const { return x * x + y * y; }
//	ld mag() const { return hypot(x, y); }
//};
//std::vector<Pos> C, H;
//const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, pivot = { -1, -1 };
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
////ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
////int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
////	ll ret = cross(d1, d2, d3);
////	return !ret ? 0 : ret > 0 ? 1 : -1;
////}
////bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) {
////	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
////}
//std::vector<Pos> monotone_chain(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	std::sort(C.begin(), C.end());
//	if (C.size() <= 2) {
//		for (const Pos& p : C) H.push_back(p);
//		return H;
//	}
//	for (int i = 0; i < C.size(); i++) {
//		while (H.size() > 1 && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	int sz = H.size() + 1;
//	for (int i = C.size() - 1; i >= 0; i--) {
//		while (H.size() > sz && cross(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	H.pop_back();
//	return H;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(6);
//	//freopen("boj9276_fo.in", "r", stdin);
//	//freopen("boj9276_ans_txt", "w", stdout);
//	while (std::cin >> N) {
//		C.resize(N);
//		for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i] *= 2;
//		H = monotone_chain(C);
//		N = H.size();
//		if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; return; }
//		//for (int i = 0; i < N; i++) std::cout << H[i].x << " " << H[i].y << "\n";
//
//		int R{ 0 }, U{ 0 }, L{ 0 };
//		for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
//		//std::cout << "DEBUG R: " << R << " \n";
//		for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
//		//std::cout << "DEBUG U: " << U << " \n";
//		for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }
//		//std::cout << "DEBUG L: " << L << " \n";
//		R %= N, U %= N, L %= N;
//
//		ld MIN{ INF };
//		for (int i = 0; i <= N; i++) {
//			while (dot(H[i % N], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
//			while (cross(H[i % N], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
//			while (dot(H[i % N], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
//			//R %= N, U %= N, L %= N;
//			ld h = cross(H[i % N], H[(i + 1) % N], H[U % N]) / (H[i % N] - H[(i + 1) % N]).mag();
//			ld w = dot(H[i % N], H[(i + 1) % N], H[L % N], H[R % N]) / (H[i % N] - H[(i + 1) % N]).mag();
//			ld box = h + w;
//			MIN = std::min(MIN, box);
//			//std::cout << "DEBUG i: " << i << " U: " << U << " R: " << R << " L: " << L << " box: " << box << " area: " << MIN << "\n";
//		}
//		std::cout << MIN << "\n";
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj9276