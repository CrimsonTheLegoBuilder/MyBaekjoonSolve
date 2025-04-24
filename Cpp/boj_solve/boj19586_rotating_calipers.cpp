#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;

int N;
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	ld mag() const { return hypot(x, y); }
} H[50000];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> H[i].x >> H[i].y;
	return;
}
void rotating_calipers() {
	init();
	std::swap(H[0], *std::min_element(H, H + N));
	std::sort(H + 1, H + N, [&](const Pos& p, const Pos& q) {
		ll ret = cross(H[0], p, q);
		return ret == 0 ? (H[0] - p).Euc() < (H[0] - q).Euc() : ret > 0;
		});
	int S = -1;
	for (int i = 0; i < N; i++) {
		while (S >= 1 && cross(H[S - 1], H[S], H[i]) <= 0) S--;
		H[++S] = H[i];
	}
	N = S + 1;
	if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; return; }
	int R{ 0 }, U{ 0 }, L{ 0 };
	for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
	for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
	for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }
	ld MIN = INF;
	for (int i = 0, j; i < N; i++) {
		j = (i + 1) % N;
		while (dot(H[i], H[j], H[R % N], H[(R + 1) % N]) > 0) R++;
		while (cross(H[i], H[j], H[U % N], H[(U + 1) % N]) > 0) U++;
		while (dot(H[i], H[j], H[L % N], H[(L + 1) % N]) < 0) L++;
		ld h = cross(H[i], H[j], H[U % N]) / (H[i] - H[j]).mag();
		ld w = dot(H[i], H[j], H[L % N], H[R % N]) / (H[i] - H[j]).mag();
		MIN = std::min(MIN, h * w);
	}
	std::cout << MIN << "\n";
	return;
}
int main() { rotating_calipers(); return 0; }//boj19586

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ll INF = 1e17;
//int N;
//
//struct Pos {
//	ll x, y;
//	Pos(ll X, ll Y) : x(X), y(Y) {}
//	Pos() : x(0), y(0) {}
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	ld mag() const { return hypot(x, y); }
//};
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
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
//	std::cout.precision(8);
//	std::cin >> N;
//	C.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i] *= 2;
//	H = monotone_chain(C);
//	N = H.size();
//	if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; return; }
//
//	int R{ 0 }, U{ 0 }, L{ 0 };
//	for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
//	for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
//	for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }
//
//	ld MIN{ INF };
//	for (int i = 0; i < N; i++) {
//		while (dot(H[i], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
//		while (cross(H[i], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
//		while (dot(H[i], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
//		ld h = cross(H[i], H[(i + 1) % N], H[U % N]) / (H[i] - H[(i + 1) % N]).mag();
//		ld w = dot(H[i], H[(i + 1) % N], H[L % N], H[R % N]) / (H[i] - H[(i + 1) % N]).mag();
//		MIN = std::min(MIN, h + w);
//	}
//	std::cout << MIN << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj19586

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//typedef long long ll;
//typedef long double ld;
//const ll INF = 1e17;
//const int LEN = 5e4;
//int N;
//
//struct Pos {
//	ll x, y;
//	Pos(ll X, ll Y) : x(X), y(Y) {}
//	Pos() : x(0), y(0) {}
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	ld mag() const { return hypot(x, y); }
//};
//std::vector<Pos> C, H;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
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
//	std::cout.precision(8);
//	std::cin >> N;
//	C.resize(N);
//	for (int i = 0; i < N; i++) std::cin >> C[i].x >> C[i].y, C[i] *= 2;
//	H = monotone_chain(C);
//	N = H.size();
//	if (N == 2) { std::cout << (H[0] - H[1]).mag() << "\n"; return; }
//
//	int R{ 0 }, U{ 0 }, L{ 0 };
//	for (int j = 0; j < N; j++) if (dot(H[0], H[1], H[j], H[(j + 1) % N]) <= 0) { R = j; break; }
//	for (int k = R; k < N; k++) if (cross(H[0], H[1], H[k], H[(k + 1) % N]) <= 0) { U = k; break; }
//	for (int l = U; l <= N; l++) if (dot(H[0], H[1], H[l % N], H[(l + 1) % N]) >= 0) { L = l; break; }
//	R %= N, U %= N, L %= N;
//
//	ld MIN{ INF };
//	for (int i = 0; i <= N; i++) {
//		while (dot(H[i % N], H[(i + 1) % N], H[R % N], H[(R + 1) % N]) > 0) R++;
//		while (cross(H[i % N], H[(i + 1) % N], H[U % N], H[(U + 1) % N]) > 0) U++;
//		while (dot(H[i % N], H[(i + 1) % N], H[L % N], H[(L + 1) % N]) < 0) L++;
//		//R %= N, U %= N, L %= N;
//		ld h = cross(H[i % N], H[(i + 1) % N], H[U % N]) / (H[i % N] - H[(i + 1) % N]).mag();
//		ld w = dot(H[i % N], H[(i + 1) % N], H[L % N], H[R % N]) / (H[i % N] - H[(i + 1) % N]).mag();
//		MIN = std::min(MIN, h + w);
//	}
//	std::cout << MIN << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj19586