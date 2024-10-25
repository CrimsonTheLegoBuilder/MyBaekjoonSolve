#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
typedef long long ll;
typedef double ld;
const int LEN = 1e5 + 1;

bool is_valid_number(const std::string& input) {
	size_t start = 0;
	// 첫 글자가 '-'이면 음수 가능하므로 넘어감
	if (input[start] == '-') {
		start++;
	}
	// 나머지 글자가 모두 숫자인지 확인
	for (size_t i = start; i < input.size(); ++i) {
		if (!std::isdigit(input[i])) {
			return false; // 숫자가 아닌 글자가 있으면 false 반환
		}
	}
	return true;
}
int make_int(const std::string& input) {
	size_t start = 0;
	int s = 1;
	int ret = 0;
	if (input[0] == '-' || (input[0] != '-' && !std::isdigit(input[0]))) start++, s = -1;
	for (size_t i = start; i < input.size(); ++i) {
		if (!std::isdigit(input[i])) continue;
		ret = ret * 10 + (input[i] - '0');
	}
	return ret * s;
}
int N, M, Q;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
};
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
	int ret = 0;
	std::string x, y;
	for (Pos& p : H) { std::cin >> x >> y; p = Pos(make_int(x), make_int(y)); }
	for (Pos& p : P) { std::cin >> x >> y; p = Pos(make_int(x), make_int(y)); }
	norm(H);
	for (const Pos& p : P) ret += (inner_check_bi_search(H, p) >= 0);
	std::cout << "Data set " << q << ": " << ret << "\n";
	return 1;
}
void solve() {
	std::cout.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	Q = 1; while (query(Q++));
	return;
}
int main() { solve(); return 0; }//boj22790 Inside Information
