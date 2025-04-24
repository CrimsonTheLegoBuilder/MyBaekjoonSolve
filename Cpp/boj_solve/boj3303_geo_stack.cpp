#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 2e5 + 1;
int N;
bool V[LEN];

struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
std::vector<Pos> H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ll area(const std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
bool norm(std::vector<Pos>& H) {
	ll A = area(H);
	assert(A);
	if (A > 0) { std::reverse(H.begin(), H.end()); return 1; }
	return 0;
}
bool invisible(const Pos& p1, const Pos& p2, const Pos& t) {
	return ccw(O, p1, t) <= 0 && ccw(O, p2, t) >= 0 && ccw(p1, p2, t) >= 0;
}
std::vector<int> stack;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	H.resize(N);
	for (int i = 0; i < N; i++) std::cin >> H[i], H[i].i = i + 1;
	norm(H);//normalize cw

	int r = 0, l = 0;
	for (int i = 0; i < N; i++) {
		if (H[r] / H[i] < 0 || (!(H[r] / H[i]) && H[r].Euc() > H[i].Euc())) r = i;
		if (H[l] / H[i] > 0 || (!(H[l] / H[i]) && H[l].Euc() > H[i].Euc())) l = i;
	}
	stack.clear();
	stack.push_back(r);

	bool fvis = 1, bvis = 1, rvs = 0;
	for (int i = r; i < r + N; i++) {
		if (i % N == l) break;
		Pos& pre = H[(i - 1 + N) % N], cur = H[i % N], nxt = H[(i + 1) % N];
		if (fvis && bvis) {
			ll DIR = cur / nxt;//ccw(O, cur, nxt);
			int CCW = ccw(pre, cur, nxt);
			if (DIR < 0) {//move backward
				if (!rvs && CCW < 0) {
					rvs = 1;
					fvis = 0;
					continue;
				}
				rvs = 1;
				while (stack.size() && invisible(cur, nxt, H[stack.back()])) stack.pop_back();
			}
			else if (!DIR) {//move vertical
				if (cur.Euc() > nxt.Euc()) {
					if (stack.size() && stack.back() == i % N) stack.pop_back();
					if (stack.size() < 1 || H[stack.back()] / nxt > 0) stack.push_back((i + 1) % N);
				}
			}
			else if (DIR > 0) {//move forward
				if (rvs && CCW > 0) {
					stack.push_back(i % N);
					rvs = 0;
					bvis = 0;
					continue;
				}
				if (stack.size() && rvs && H[stack.back()] / cur > 0) stack.push_back(i % N);
				rvs = 0;
				if (stack.size() < 1 || H[stack.back()] / nxt > 0) stack.push_back((i + 1) % N);
			}
		}
		else if (!fvis) {
			if (H[stack.back()] / nxt > 0) {
				rvs = 0;
				fvis = 1;
				stack.push_back((i + 1) % N);
			}
		}
		else if (!bvis) {
			if (H[stack.back()] / nxt < 0) {
				stack.pop_back();
				rvs = 1;
				bvis = 1;
				while (stack.size() && invisible(cur, nxt, H[stack.back()])) stack.pop_back();
			}
		}
	}

	memset(V, 0, sizeof V);
	for (const int& i : stack) V[H[i].i] = 1;
	std::cout << stack.size() << "\n";
	for (int i = 1; i <= N; i++) if (V[i]) std::cout << i << " ";
	return;
}
int main() { solve(); return 0; }//boj3303 Printed Circuit Board

//#include <iostream>
//#include <algorithm>
//#include <vector>
//
//typedef long long ll;
//const int LEN = 200'001;
//
//struct Pos {
//	ll x, y;
//	int i;
//	ll s() const { return x * x + y * y; }
//	bool operator<(const Pos& r) const { return y * r.x < r.y * x; }
//	bool operator==(const Pos& r) const { return y * r.x == r.y * x; }
//	friend std::istream& operator>>(std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//} O = { 0, 0 }, p[LEN];
//ll cross(const Pos& p1, const Pos& p2, const Pos& p3) { return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x); }
//ll ccw(const Pos& p1, const Pos& p2, const Pos& p3) {
//	ll ret = cross(p1, p2, p3);
//	return ret < 0 ? -1 : !!ret;
//}
//
//bool invisible(const Pos& p1, const Pos& p2, const Pos& t) {
//	return ccw(O, p1, t) <= 0 && ccw(O, p2, t) >= 0 && ccw(p1, p2, t) >= 0;
//}
//
//int N, L, R;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cin >> N >> p[0];
//	p[0].i = 1;
//	ll area = 0;
//	for (int i = 1; i < N; ++i) {
//		std::cin >> p[i];
//		p[i].i = i + 1;
//		area += cross(O, p[i - 1], p[i]);
//	}
//	area += cross(O, p[N - 1], p[0]);
//	if (area > 0) std::reverse(p, p + N);
//	for (int i = 0; i < N; ++i) {
//		if (p[L] < p[i] || p[i] == p[L] && p[i].s() < p[L].s()) L = i;
//		if (p[i] < p[R] || p[i] == p[R] && p[i].s() < p[R].s()) R = i;
//	}
//	std::rotate(p, p + R, p + N);
//	L = (L - R + N) % N;
//
//	// std::cout << R << ' ' << L << '\n';
//	// std::cout << p[0].i << ' ' << p[L].i << '\n';
//
//	std::vector<int> stack; // monotonic, ccw order
//	stack.push_back(0);
//	if (L > 1) stack.push_back(1);
//
//	for (int i = 2, fv = -1, bv = -1, cw = 0; i <= L; ++i) {
//		// j -> i
//		ll dir = cross(O, p[i - 1], p[i]);
//		ll ccw = cross(p[i - 2], p[i - 1], p[i]);
//
//		if (!~fv && !~bv) {
//			if (dir < 0) { // cw
//				if (!cw && ccw < 0) {
//					cw = 1;
//					fv = i - 1;
//					continue;
//				}
//				cw = 1;
//				while (stack.size() && invisible(p[i - 1], p[i], p[stack.back()])) stack.pop_back();
//			}
//			else if (dir == 0) {
//				if (p[i].s() < p[i - 1].s()) {
//					if (stack.back() == i - 1) stack.pop_back();
//				}
//			}
//			else if (dir > 0) { // ccw
//				if (cw && ccw > 0) {
//					cw = 0;
//					bv = i - 1;
//					continue;
//				}
//				cw = 0;
//			}
//			if (stack.empty() || p[stack.back()] < p[i]) stack.push_back(i);
//		}
//		else if (~fv) {
//			if (p[fv] < p[i]) {
//				cw = 0;
//				fv = -1;
//				if (stack.empty() || p[stack.back()] < p[i]) stack.push_back(i);
//			}
//		}
//		else if (~bv) {
//			if (p[i] < p[bv]) {
//				cw = 1;
//				bv = -1;
//				while (stack.size() && invisible(p[i - 1], p[i], p[stack.back()])) stack.pop_back();
//				if (stack.empty() || p[stack.back()] < p[i]) stack.push_back(i);
//			}
//		}
//	}
//
//	std::vector<int> ret;
//	for (const int& i : stack) ret.push_back(p[i].i);
//	ret.erase(unique(ret.begin(), ret.end()), ret.end());
//	std::sort(ret.begin(), ret.end());
//	std::cout << ret.size() << '\n';
//	for (const int& i : ret) std::cout << i << ' ';
//}