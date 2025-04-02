#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-13;

int L, N, X, xl, xr;
struct Pos { ld x, y; };
std::vector<Pos> P;
std::vector<ld> D;
std::map<ld, int> M, R;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> L >> N >> X;
	xl = -X, xr = L + (L - X);
	P.resize(N); for (Pos& p : P) std::cin >> p.x >> p.y;
	for (Pos& p : P) {
		ld dx = p.x - X;
		ld dy = p.y;
		if (std::abs(dx) > TOL) {
			ld d = dy / dx;
			D.push_back(d);
			if (M.find(d) == M.end()) M[d] = 1;
			else M[d]++;
		}
		for (const ld& x : { xl, xr }) {
			dx = p.x - x;
			dy = p.y;
			if (std::abs(dx) > TOL) {
				ld d = dy / dx;
				if (R.find(d) == R.end()) R[d] = 1;
				else R[d]++;
			}
		}
	}
	std::sort(D.begin(), D.end());
	D.erase(unique(D.begin(), D.end()), D.end());
	int ret = 0;
	for (const ld& d : D) {
		int c = M[d];
		if (R.find(-d) != R.end()) c += R[-d];
		ld dx = d > 0 ? (L - X + L) : (X + L);
		ld dy = std::abs(dx * d);
		dx = d > 0 ? -X : L - X;
		ld r = dy / dx;
		if (M.find(r) != M.end()) c += M[r];
		ret = std::max(ret, c);
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj7273