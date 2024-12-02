#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 3005;

int N, i, j; ll T;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x * p.y < y * p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
} P[LEN], V[LEN];
bool cmp(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
	std::sort(P, P + N, cmp);
	for (i = 2; i < N; i++) {
		for (j = 0; j < i; j++) V[j] = P[i] - P[j];
		Pos v(0, 0); std::sort(V, V + i);
		for (j = 0; j < i; j++) T += V[j] / v, v += V[j];
	}
	std::cout << T / 2 << "." << (T & 1) * 5 << "\n";
	return;
}
int main() { solve(); return 0; }//boj8166
