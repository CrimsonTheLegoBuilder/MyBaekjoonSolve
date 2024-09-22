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

int N, I[LEN], K[LEN];
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
} X, Y, pos[LEN];
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
	return tqp < tqq;
}
int cx;
void compress() {
	//compress x
	
	//struct Info {
	//	ll t;
	//	int idx;
	//	bool operator<(const Info& o) const { return t > o.t; }
	//};
	//static Info infos[LEN];
	//for (int i = 0; i < N; ++i) infos[i] = { cross(O, Y, pos[i]), pos[i].idx };
	//std::sort(infos, infos + N);
	//for (int i = 1, j = I[infos[0].idx] = 1; i < N; ++i) {
	//	if (infos[i].t != infos[i - 1].t) ++j;
	//	I[infos[i].idx] = j;
	//	cx = j;
	//}

	Vll CX;
	MapTorque MX;
	for (int i = 0; i < N; i++) CX.push_back(cross(O, Y, pos[i]));
	std::sort(CX.rbegin(), CX.rend());
	CX.erase(unique(CX.begin(), CX.end()), CX.end());
	int sz = CX.size();
	for (int i = 0; i < sz; i++) MX[CX[i]] = i + 1;
	for (int i = 0; i < N; i++) I[pos[i].idx] = MX[cross(O, Y, pos[i])];
	cx = CX.size();
	return;
}
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
bool col = 0;
int ans[LEN];
void f(int s, int e, const Vint& v) {
	if (s == e) return;
	int m = s + e >> 1;

	Vint on;
	Vint off;
	for (const int& i : v) {
		if (ans[i] <= m) {
			on.push_back(i);
			update(I[i], 1);
		}
		else {
			int recieved = sum(I[i]);
			if (col) recieved -= sum(I[i] - 1);
			if (recieved >= K[i]) {
				ans[i] = m;
				on.push_back(i);
				update(I[i], 1);
			}
			else {
				off.push_back(i);
				K[i] -= recieved;
			}
		}
	}
	for (const int& i : on) update(I[i], -1);
	f(s, m, on);
	f(m + 1, e, off);
}

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> X >> Y;
	for (int i = 0; i < N; i++) std::cin >> pos[i], pos[i].idx = i;
	for (int i = 0; i < N; i++) std::cin >> K[i];
	if (X / Y == 0) col = 1;
	if (X / Y < 0) std::swap(X, Y);
	std::sort(pos, pos + N, cmp_X);
	compress();

#ifdef DEBUG
	for (int i = 0; i < N; i++) {
		std::cout << "pos[" << pos[i].idx << "]:: x, y:: "<< pos[i].x << " " << pos[i].y << "\n";
		std::cout << "pos[" << pos[i].idx << "]:: i, j:: "<< pos[i].i << " " << pos[i].j << "\n";
	}
#endif

	Vint v;
	for (int i = 0; i < N; ++i) { ans[i] = i + 1; v.push_back(pos[i].idx); }
	f(1, N, v);
	for (int i = 0; i < N; ++i) std::cout << ans[i] << ' ';
	return;
}
int main() { solve(); }