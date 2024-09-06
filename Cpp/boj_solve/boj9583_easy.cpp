#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e5 + 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

#define LOWER 1
#define UPPER -1

int N;
ld X1, X2, Y1, Y2;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon U, L;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
Polygon monotone_chain(Polygon& C, int f = LOWER) {
	Polygon H;
	std::sort(C.begin(), C.end());
	if (f == UPPER) std::reverse(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		assert(0);
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	int sz = H.size();
	if (H[sz - 2].x == H[sz - 1].x) H.pop_back();
	if (f == UPPER) std::reverse(H.begin(), H.end());
	return H;
}
int idx_bi_search(const Polygon& H, const ld& x) {
	int s = 1, e = H.size() - 1;
	if (H[1].x > x) return 0;
	if (H[2].x > x) return 1;
	if (!sign(H.back().x - x)) return e;
	while (s < e) {
		int m = s + e + 1 >> 1;//refer to tourist
		if (sign(H[m].x - x) == 0) return m;
		if (sign(H[m].x - x) < 0) s = m;
		else e = m - 1;
	}
	return s;
}
ld height_search(const ld& x, const ld& w) {
	auto cal_y = [&](const Polygon& V, const int i, const ld& x_) -> ld {
		if (zero(V[i].x - x_)) return V[i].y;
		int nxt = std::min(i + 1, (int)V.size() - 1);
		ll den = V[nxt].x - V[i].x;
		ll num = V[nxt].y - V[i].y;
		return (ld)V[i].y + (x_ - V[i].x) * (ld)num / den;;
		};
	int l1, l2, u1, u2;
	ld x1 = x, x2 = x + w;
	l1 = idx_bi_search(L, x1);
	l2 = idx_bi_search(L, x2);
	u1 = idx_bi_search(U, x1);
	u2 = idx_bi_search(U, x2);
	ld y1 = cal_y(L, l1, x1);
	ld y2 = cal_y(U, u1, x1);
	ld y3 = cal_y(L, l2, x2);
	ld y4 = cal_y(U, u2, x2);
 	ld yu = std::min(y2, y4), yl = std::max(y1, y3);
	X1 = x, X2 = x + w, Y1 = yl, Y2 = yu;
	return std::max(yu - yl, (ld)0);
}
ld area_ternary_search(const ld& w) {
	ld s = L[0].x, e = L.back().x - w;
	ld x1, x2;
	//while (sign(e - s) > 0) {
	int cnt = 50; while (cnt--) {
		x1 = (s + s + e) / 3;
		x2 = (s + e + e) / 3;
		ld h1 = height_search(x1, w);
		ld h2 = height_search(x2, w);
		ld a1 = w * h1, a2 = w * h2;
		if (sign(a2 - a1) > 0) s = x1;
		else e = x2;
	}
	height_search((s + e) * .5, w);
	return (X2 - X1) * (Y2 - Y1);
}
void area_ternary_search() {
	ld s = 0, e = L.back().x - L[0].x;
	ld w1, w2;
	//while (sign(e - s) > 0) {
	int cnt = 50; while (cnt--) {
		w1 = (s + s + e) / 3;
		w2 = (s + e + e) / 3;
		ld a1 = area_ternary_search(w1);
		ld a2 = area_ternary_search(w2);
		if (sign(a2 - a1) > 0) s = w1;
		else e = w2;
	}
	area_ternary_search((s + e) * .5);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	//const std::string TASKNAME = "easy";
	//freopen((TASKNAME + ".in").c_str(), "r", stdin);
	//freopen((TASKNAME + ".out").c_str(), "w", stdout);
	std::cin >> N;
	Polygon H(N);
	for (int i = 0; i < N; i++) std::cin >> H[i];
	L = monotone_chain(H, LOWER);
	U = monotone_chain(H, UPPER);
	area_ternary_search();
	std::cout << X1 << " " << Y1 << " " << X2 << " " << Y2 << "\n";
	return;
}
int main() { solve(); return 0; }//boj9583

/*

4
-1 0
1 0
2 -10
-2 -10

-1.000000000 -10.000000000
1.000000000 -0.000000000

6
233127 -972446
-357996 -933723
-929678 -368373
-947555 319593
-563980 825789
837792 545990

-707706.1603165409 -587886.2601078567
386264.71217350144 636116.556677073

10
-91594 46960
-76949 84667
-66236 100000
43979 -24439
77627 -66150
100000 -98953
56673 -99689
-27595 -100000
-69277 -31494
-100000 24687
-27749.993634482 -99745.261889453
41428.379741335 -21559.204741932
-27749.99363549359986791 -99745.26188779030053411
41428.38019192011415726 -21559.20525066774280276

*/


/*
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <set>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <memory.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <sstream>

using namespace std;

#define x1 _x1
#define y1 _y1
#define left _left
#define right _right

const string TASKNAME = "easy";

const int N = 1000010;

int x[N], y[N];
int right, n;
double xa, ya, xb, yb, xc, yc, ans, opt_w, opt_h;
bool bad;

void get_ys(double xx, double& y1, double& y2) {
    if (x[0] >= xx || x[right] <= xx) {
        bad = true;
        return;
    }
    {
        int ll = 0, rr = right;
        while (ll < rr) {
            int mid = (ll + rr + 1) >> 1;
            if (x[mid] < xx) ll = mid;
            else rr = mid - 1;
        }
        long long a = y[ll + 1] - y[ll];
        long long b = x[ll] - x[ll + 1];
        long long c = -a * x[ll] - b * y[ll];
        if (b == 0) y2 = (y[ll] > y[ll + 1] ? y[ll] : y[ll + 1]);
        else y2 = 1.0 * (-a * xx - c) / b;
    }
    {
        int ll = right, rr = n - 1;
        while (ll < rr) {
            int mid = (ll + rr + 1) >> 1;
            if (x[mid] > xx) ll = mid;
            else rr = mid - 1;
        }
        long long a = y[ll + 1] - y[ll];
        long long b = x[ll] - x[ll + 1];
        long long c = -a * x[ll] - b * y[ll];
        if (b == 0) y1 = (y[ll] < y[ll + 1] ? y[ll] : y[ll + 1]);
        else y1 = 1.0 * (-a * xx - c) / b;
    }
}

double mh;
int it2;

double get_h(double w, double xx) {
    double y1, y2, y3, y4;
    bad = false;
    get_ys(xx, y1, y2);
    get_ys(xx + w, y3, y4);
    if (bad) return 0.0;
    if (y3 > y1) y1 = y3;
    if (y4 < y2) y2 = y4;
    if (y1 < y2) {
        if (y2 - y1 > mh && it2 > 150) {
            mh = y2 - y1;
            yc = y1;
        }
        return y2 - y1;
    }
    return 0;
}

double get_w_h(double w) {
    double lx = 1e9;
    for (int i = 0; i < n; i++)
        if (x[i] < lx) lx = x[i];
    double rx = 1e9;
    mh = 0.0;
    for (it2 = 0; it2 < 200; it2++) {
        double x1 = (2 * lx + rx) / 3;
        double x2 = (lx + 2 * rx) / 3;
        double h1 = get_h(w, x1);
        double h2 = get_h(w, x2);
        if (h2 == 0.0) rx = x2; else
            if (h2 < h1) rx = x2;
            else lx = x1;
    }
    xc = 0.5 * (lx + rx);
    if (mh * w > ans) {
        ans = mh * w;
        opt_w = w;
        opt_h = mh;
        xa = xc;
        ya = yc;
    }
    return mh * w;
}

int new_x[N], new_y[N];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d %d", x + i, y + i);
    int left = 0;
    for (int i = 1; i < n; i++)
        if (x[i] < x[left]) left = i;
    for (int i = 0; i < n; i++) {
        new_x[i] = x[left];
        new_y[i] = y[left];
        left = (left + 1) % n;
    }
    for (int i = 0; i < n; i++) {
        x[i] = new_x[i];
        y[i] = new_y[i];
    }
    x[n] = x[0];
    y[n] = y[0];
    right = 0;
    for (int i = 1; i < n; i++)
        if (x[i] > x[right]) right = i;
    double lw = 0, hw = 2e9;
    ans = 0.0;
    for (int it = 0; it < 200; it++) {
        double w1 = (2 * lw + hw) / 3;
        double w2 = (lw + 2 * hw) / 3;
        double q1 = get_w_h(w1);
        double q2 = get_w_h(w2);
        if (q2 == 0.0 || q2 < q1) hw = w2;
        else lw = w1;
    }
    xb = xa + opt_w;
    yb = ya + opt_h;
    printf("%.17lf %.17lf %.17lf %.17lf\n", xa, ya, xb, yb);
    return 0;
}
*/