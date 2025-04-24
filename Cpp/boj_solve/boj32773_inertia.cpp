#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
typedef double ld;

int N; ld I;
struct Pos {
	ld x, y;
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
} P[1001];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(1);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
	for (int i = 0; i < N; i++) {
		const Pos& p0 = P[i], & p1 = P[(i + 1) % N];
		I += (p0 / p1) * (p0 * p0 + p0 * p1 + p1 * p1);
	}
	std::cout << std::abs(I) / 12. << "\n";
	return;
}
int main() { solve(); return 0; }//boj32773


//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//inline ll sq(const int& x) { return (ll)x * x; }
//
//int N;
//struct Pos {
//	int x, y;
//	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
//	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
//	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//};
//typedef std::vector<Pos> Polygon;
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(1);
//	std::cin >> N;
//	Polygon P(N);
//	for (Pos& p : P) std::cin >> p;
//	ll I = 0;
//	for (int i = 0; i < N; i++) {
//		const Pos& p0 = P[i], & p1 = P[(i + 1) % N];
//		ll tq = p0 / p1;
//		I += tq * (sq(p0.x) + p0.x * p1.x + sq(p1.x));
//		I += tq * (sq(p0.y) + p0.y * p1.y + sq(p1.y));
//	}
//	std::cout << std::abs(I) / 12. << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj32773