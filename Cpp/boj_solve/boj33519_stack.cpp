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
	Pos(int x_ = 0, int y_ = 0, int i_ = 0) : x(x_), y(y_), i(i_) {}
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
int main() { solve(); return 0; }//boj33519 Andrew the Diver