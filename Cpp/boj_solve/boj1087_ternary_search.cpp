#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
typedef long double ld;
const int LEN = 50;
int N;
ld LIMIT = 2000;
ld MAX = 1e17;
ld MIN = -1e17;

struct Pos {
	ld x, y;
	Pos operator * (const ld& n) const { return { n * x, n * y }; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
} pos_l[LEN], pos_r[LEN];
struct Rat {
	Pos P, V;
	Pos operator * (const ld& n) const { return P + (V * n); }
} RATS[LEN];

void move(ld& time_l, ld& time_r) {
	for (int i = 0; i < N; i++) 
		pos_l[i] = RATS[i] * time_l, pos_r[i] = RATS[i] * time_r;
}
ld find_max(Pos A[]) {
	ld Mx = MIN, My = MIN, mx = MAX, my = MAX;
	for (int i = 0; i < N; i++) {
		Mx = std::max(Mx, A[i].x);
		mx = std::min(mx, A[i].x);
		My = std::max(My, A[i].y);
		my = std::min(my, A[i].y);
	}
	return std::max(std::abs(Mx - mx), std::abs(My - my));
}
ld ternary_search() {
	ld s = 0, e = LIMIT, l, r, Ll, Lr;
	int cnt = 100;
	while (cnt--) {
		l = (s + s + e) / 3;
		r = (s + e + e) / 3;
		move(l, r);
		Ll = find_max(pos_l);
		Lr = find_max(pos_r);
		if (Ll < Lr) e = r;
		else s = l;
	}
	move(s, e);
	ld L = find_max(pos_l);
	return L;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	for (int i = 0; i < N; i++)
		std::cin >> RATS[i].P.x >> RATS[i].P.y >> RATS[i].V.x >> RATS[i].V.y;
	std::cout << ternary_search() << "\n";
	return;
}
int main() { solve(); return 0; }//boj1087