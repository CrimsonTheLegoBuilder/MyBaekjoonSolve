#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>

#include <cstdio>

typedef long long ll;
typedef long double ld;
const int LEN = 1e5 + 1;

int N, M, Q;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	ld mag() { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
bool on_seg(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
int inner_check_bi_search(const Polygon& H, const Pos& p) {
	int sz = H.size();
	if (sz < 3 || cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg(H[0], H[1], p) || on_seg(H[0], H[sz - 1], p)) return 0;
	int s = 1, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg(H[s], H[e], p)) return 0;
	else return -1;
}
ll area(const Polygon& H) {
	ll a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a;
}
void norm(Polygon& H) { ll a = area(H); if (a < 0) std::reverse(H.begin(), H.end()); }
bool query(int q) {
	std::cin >> N >> M;
	if (!N && !M) return 0;
	Polygon H(N), P(M);
	//for (Pos& p : H) std::cin >> p;
	for (Pos& p : H) {
		std::string input;
		std::getline(std::cin, input);
		char unicode_minus = '\u2212';
		std::replace(input.begin(), input.end(), '?', '-');
		//std::cout << "fuck:: " << input << '\n';
		std::istringstream input_stream(input);
		input_stream >> p;
	}
	norm(H);
	//for (Pos& p : P) std::cin >> p;
	for (Pos& p : P) {
		std::string input;
		std::getline(std::cin, input);
		char unicode_minus = '\u2212';
		std::replace(input.begin(), input.end(), '?', '-');
		//std::cout << "fuck:: " << input << '\n';
		std::istringstream input_stream(input);
		input_stream >> p;
	}
	//std::cout << "FUCK::\n";
	//std::cout << "N, M " << N << " " << M << " H, P " << H.size() << " " << P.size() << "\n";
	int ret = 0;
	for (const Pos& p : P) ret += (inner_check_bi_search(H, p) >= 0);
	std::cout << "Data set " << q << ": " << ret << "\n";
	return 1;
}
void solve() {
	std::cout.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);

	int q = 1;
	while (query(q++));
	return;
}
int main() { solve(); return 0; }//boj22790 Inside Information

/*

4 6
0 0
2 2
5 5
5 -1
5 3
7 3
5 -1
5 -2
2 2
4 1
3 1
0 0
5 0
5 5
0 5
0 0

5 -1

*/