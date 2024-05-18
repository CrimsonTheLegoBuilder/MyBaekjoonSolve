#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 1e5;
const ll MAX = 1e9;
ll A, B, C, D, W, H;
ll a, b, c, d, w, h;
ll ret = 0;
ll SUM[2][4] = {
	{ 2, 1, 0, 1 },
	{ 1, 2, 0, 1 }
};

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> A >> B >> C >> D;
	H = D - B;
	A += MAX;
	B += MAX;
	C += MAX;
	D += MAX;

	ll even, odd;
	if (B & 1) {
		odd = (H + 1) / 2;
		even = H - odd;
	}
	else {
		even = (H + 1) / 2;
		odd = H - even;
	}

	while (A < C && (C - A) % 4) {
		switch (A % 4) {
		case 0:
			ret += even * 2 + odd;
			break;
		case 1:
			ret += even + odd * 2;
			break;
		case 2:
			ret += odd;
			break;
		case 3:
			ret += even;
		}
		A++;
	}

	W = C - A;
	ret += H * W;
	std::cout << ret << "\n";

}
int main() { solve(); return 0; }