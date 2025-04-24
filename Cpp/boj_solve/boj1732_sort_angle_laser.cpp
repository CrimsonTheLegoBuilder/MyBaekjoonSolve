#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
const int LEN = 1e5 + 1;

int N;
struct Pos {
	int x, y, z;
	inline Pos(int X = 0, int Y = 0, int Z = 0) : x(X), y(Y), z(Z) {}
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	bool operator < (const Pos& p) const {
		bool f1 = !x ? y > 0 : x > 0;
		bool f2 = !p.x ? p.y > 0 : p.x > 0;
		if (f1 != f2) return f1;
		ll tq = (ll)x * p.y - (ll)y * p.x;
		return tq ? tq > 0 : Euc() < p.Euc();
	}
	inline ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
inline bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
typedef std::vector<Pos> Polygon;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Polygon C(N);
	for (int i = 0; i < N; i++) std::cin >> C[i];
	std::sort(C.begin(), C.end());
	Polygon del;
	for (int i = 0, j; i < N; i = j) {
		j = i + 1;
		while (j < N && !(C[i] / C[j])) j++;
		int h = C[i].z;
		for (int k = i + 1; k < j; k++) {
			if (C[k].z > h) h = C[k].z;
			else del.push_back(C[k]);
		}
	}
	std::sort(del.begin(), del.end(), cmpx);
	for (const Pos& p : del) std::cout << p << "\n";
	return;
}
int main() { solve(); return 0; }