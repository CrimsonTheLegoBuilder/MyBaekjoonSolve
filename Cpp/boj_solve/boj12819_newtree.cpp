#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
typedef long double ld;
inline int sign(const ll& x) { return x < 0 ? -1 : x > 0; }

//#define DEBUG
#define REMOVE

int T, N, M, K, A;
struct Pos {
	int x, y;
	int i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} n, a, aa, b, c, pvt; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
bool cmpr(const Pos& p, const Pos& q) {
	int tq = ccw(pvt, p, q);
	if (!tq) return (p - pvt).Euc() < (q - pvt).Euc();
	return tq > 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> A >> n;
	Polygon P(N), R, L;
	aa = Pos(1e9, 1e9, -1);
	b = Pos(1e9, 1e9, -1);
	c = Pos(1e9, 1e9, -1);
	T = 1;
	for (Pos& p : P) {
		std::cin >> p, p.i = T++; 
		if (p.i == A) a = p;
	}
#ifdef DEBUG
	std::cout << "P = [\n";
	for (Pos& p : P) std::cout << " (" << p.x << ", " << p.y << "),\n";
	std::cout << "]\n";
#endif
	for (Pos& p : P) {
		if (p.i == A) continue;
		int tq = ccw(a, n, p);
		if (!tq) {
			if (on_seg_strong(a, n, p)) { std::cout << "0 0\n"; return; }
			if (on_seg_strong(a, p, n)) {
				if (aa.i == -1 || on_seg_strong(n, aa, p)) aa = p;
			}
		}
		else if (tq > 0) {
			L.push_back(p);
			if (c.i == -1 || cross(a, c, p) < 0 || on_seg_strong(a, c, p)) c = p;
		}
		else if (tq < 0) {
			R.push_back(p);
			if (b.i == -1 || cross(a, b, p) > 0 || on_seg_strong(a, b, p)) b = p;
		}
	}
	if (b.i == -1 || c.i == -1) { std::cout << "0 0\n"; return; }
	if (aa.i == -1) {
		int tq = ccw(b, c, n);
		if (tq <= 0) std::cout << "0 0\n";
		else std::cout << b.i << " " << c.i << "\n";
		return;
	}
#ifdef REMOVE
	Polygon tmp;
	int sz;

	sz = R.size();
	pvt = a; std::sort(R.begin(), R.end(), cmpr);
	tmp.push_back(R[0]);
	for (int i = 1; i < sz; i++) if (ccw(a, R[i - 1], R[i])) tmp.push_back(R[i]);
	R = tmp;
	pvt = aa; std::sort(R.begin(), R.end(), cmpr);

	tmp.clear();

	sz = L.size();
	pvt = a; std::sort(L.begin(), L.end(), cmpr);
	tmp.push_back(L[0]);
	for (int i = 1; i < sz; i++) if (ccw(a, L[i - 1], L[i])) tmp.push_back(L[i]);
	L = tmp;
#else
	pvt = aa; std::sort(R.begin(), R.end(), cmpr);
	pvt = a; std::sort(L.begin(), L.end(), cmpr);
#endif

	int rsz = R.size(), lsz = L.size();
	int r = 0, l = 0;
	while (r < rsz && l < lsz) {
		if (ccw(R[r], L[l], n) <= 0) {
			Pos t = R[r];
			r++;
			while (r < rsz && ccw(a, t, R[r]) <= 0) r++;
		}
		else {
			if (ccw(R[r], L[l], aa) < 0) {
				std::cout << R[r].i << " " << L[l].i << "\n";
#ifdef DEBUG
				std::cout << "P[A]:: " << P[A] << "\n";
				std::cout << "R[r]:: " << R[r] << "\n";
				std::cout << "L[l]:: " << L[l] << "\n";
#endif
				return;
			}
			Pos t = L[l];
			l++;
			while (l < lsz && ccw(aa, t, L[l]) <= 0) l++;
		}

	}
	std::cout << "0 0\n";
	return;
}
int main() { solve(); return 0; }//boj12819 New Tree
//https://github.com/trojsten/vpcpc/blob/master/tester/day-mix1/newtree/newtree.cpp

/*

일단 점들을 갈라준다. 일적선 상에 있으면 그 점의 위치 관계도 판단해준다.
양쪽 중 한 쪽에 나무가 아예 없으면 삼각형을 만들 수 없다.
일직선 상에 나무가 없다면 가장 가까이 있는 나무 2개로 바로 삼각형을 만들 수 있어야 한다. 안 되면 fuck이다.
옛날 나무와 새 나무 사이에 나무가 있으면 걍 fuck이다.
양쪽 점군을 번갈아가면서 삼각형이 만들어지는지를 본다.
양쪽을 서로 다른 점을 기준으로 잡아 각도순 정렬로 가까운 각도 - 거리 순 정렬한다.
가장 가까운 점 2개부터 조건을 만족하는지 확인하고, 안 되면 정렬 기준이 아닌 점과의 외적 값을 계산해준다.
오른쪽 점군은 aa에 대해 정렬했으므로 new가 왼쪽에 있다면 왼쪽 점군을 보고,
new가 오른쪽에 있다면 a와의 외적 값이 안쪽으로 가는 점이 될때까지 포인터를 옮겨준다.
왼쪽 점군을 볼 때는 오른쪽 점군에서 이미 new가 안에 있다는 건 확인하고 왔으므로 aa가 밖에 있는지를 확인한다.
밖에 있다면 찾은거고, 안에 있다면 똑같이 aa 기준으로 외적 값이 안쪽으로 갈 때까지 포인터를 이동해준다.
가운데에 직선을 하나 놓고 돌리는 형태로 풀어낸다고 생각하면 될 듯.
제한 시간 때문에 정렬을 3번씩 하는 것도 좀 무섭긴 하다.

*/