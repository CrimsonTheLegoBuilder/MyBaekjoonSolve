#include <iostream>
#include <cmath>
#include <algorithm>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;

struct Pii {
	ll x, y;
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
}P1, P2, P3, P4, c;
struct Pdd { ld x, y; }x;
struct Line {  // vy * x + vx * y - c == 0;
	ld vy, vx, c;
	ld operator / (const Line& l) const { return vy * l.vx - vx * l.vy; }
};

Line L(const Pii& d1, const Pii& d2) {
	ld vy = d2.y - d1.y, vx = d1.x - d2.x;
	ld c = vy * d1.x + vx * d1.y;
	return { vy, vx, c };
}
Pdd IP(const Line& L1, const Line& L2) {
	ld det = L1 / L2;
	return { (L1.c * L2.vx - L1.vx * L2.c) / det, (L1.vy * L2.c - L1.c * L2.vy) / det };
}
int C(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) {
	ll ccw = (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int C(const Pii& d1, const Pii& d2, const Pii& d3) {
	ll ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
}
int D(const Pii& d1, const Pii& d2, const Pii& d3) {
	ll dot = (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
	return dot > 0 ? 1 : dot < 0 ? -1 : 0;
}
bool X(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) {
	bool F1 = C(d1, d2, d3) * C(d2, d1, d4) > 0 && C(d3, d4, d1) * C(d4, d3, d2) > 0;
	bool F2 = (!C(d1, d2, d3) && D(d1, d3, d2) >= 0) ||
		(!C(d1, d2, d4) && D(d1, d4, d2) >= 0) ||
		(!C(d3, d4, d1) && D(d3, d1, d4) >= 0) ||
		(!C(d3, d4, d2) && D(d3, d2, d4) >= 0);
	return F1 || F2;
}
bool P(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) {
	bool F0 = !C(d1, d2, d3, d4);
	bool F1 = (d1 == d3 && d2 == d4) || (d1 == d4 && d2 == d3);
	bool F2 = F0 && (D(d1, d3, d2) > 0 || D(d1, d4, d2) > 0 || D(d3, d1, d4) > 0 || D(d3, d2, d4) > 0);
	if (F0 && !F1 && !F2) {
		if (d1 == d3 || d1 == d4) c = d1;
		if (d2 == d3 || d2 == d4) c = d2;
		std::cout << c.x << " " << c.y << "\n";
	}
	return F0;
}
void J(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) {
	if (!X(d1, d2, d3, d4)) {
		std::cout << "0\n";
		return;
	}
	std::cout << "1\n";
	if (!P(d1, d2, d3, d4)) {
		x = IP(L(d1, d2), L(d3, d4));
		std::cout << x.x << " " << x.y << "\n";
	}
	return;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(16);
	std::cin >> P1.x >> P1.y >> P2.x >> P2.y;
	std::cin >> P3.x >> P3.y >> P4.x >> P4.y;
	J(P1, P2, P3, P4);
	return 0;
}

//ld cross(const Line& L1, const Line& L2) {
//	return L1.vy * L2.vx - L1.vx * L2.vy;
//}
// || F1 || F2;
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) {
//	return (d2.x - d1.x) * (d4.y - d3.y) - (d2.y - d1.y) * (d4.x - d3.x);
//}
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}



/*
1000000 1000000 -1000000 -1000000
999999 1000000 -999999 -1000000

-1000000 1000000 1000000 -1000000
999999 1000000 500 -1000000

0 0 1 1
0 1 1 1
*/