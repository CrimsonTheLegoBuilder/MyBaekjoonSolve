#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <map>
typedef long long ll;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::map<ll, int> MapTorque;
const int LEN = 2e5 + 5;

#define FUCK
//#define DEBUG

int N, M, T, I[LEN];
ll Q;
struct Pos {
	int x, y;
	int idx;
	Pos(int X = 0, int Y = 0, int IDX = 0) : x(X), y(Y), idx(IDX) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} X = Pos(1, 0), Y = Pos(0, 1), pos[LEN];
const Pos O = Pos(0, 0);
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool cmp_X(const Pos& p, const Pos& q) {
	ll tqp = cross(O, X, p), tqq = cross(O, X, q);
	if (tqp == tqq) {
		ll fp = dot(O, X, p), fq = dot(O, X, q);
		assert(fp != fq);
		return fp < fq;
	}
	return tqp > tqq;
}
int cx;
struct Info {
	ll t;
	int idx;
	bool operator<(const Info& o) const { return t > o.t; }
} infos[LEN];
ll fenwick[LEN];
void update(int i, ll d) {
	while (i <= cx) {
		fenwick[i] += d;
		i += i & -i;
	}
}
ll sum(int i) {
	ll result = 0;
	while (i > 0) {
		result += fenwick[i];
		i -= i & -i;
	}
	return result;
}
ll ans[LEN];
void query() {
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> pos[i], pos[i].idx = i;
	std::sort(pos, pos + N, cmp_X);

	for (int i = 0; i < N; ++i) infos[i] = { cross(O, Y, pos[i]), pos[i].idx };
	std::sort(infos, infos + N);
	for (int i = cx = I[infos[0].idx] = 1; i < N; ++i) {
		if (infos[i].t != infos[i - 1].t) ++cx;
		I[infos[i].idx] = cx;
	}

#ifdef DEBUG
	for (int i = 0; i < N; i++) {
		std::cout << "pos[" << pos[i].idx << "]:: x, y:: " << pos[i].x << " " << pos[i].y << "\n";
		std::cout << "pos[" << pos[i].idx << "]:: i, j:: " << pos[i].i << " " << pos[i].j << "\n";
	}
#endif

	memset(fenwick, 0, sizeof fenwick);
	memset(ans, 0, sizeof ans);
	Vint v;
	for (int i = 0; i < N; ++i) v.push_back(pos[i].idx);
	for (const int& i : v) {
		ans[i] = sum(I[i]);
		update(I[i], 1);
	}
	Q = 0;
	for (int i = 0; i < N; ++i) Q += ans[i];
	std::cout << Q << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/E_winds/tests/01.in", "r", stdin);
	//freopen("../../../input_data/E_winds/tests/01_ret.txt", "w", stdout);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); }//boj5419 North-Western Winds