#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
typedef long long ll;
typedef long double ld;
const int LEN = 200'001;
const ld PI = 3.1415926535;

ld intersect(const ll& x1, const ll& r1, const ll& x2, const ll& r2) {
	if (x2 - r2 < x1 - r1) return x2 - r2;
	return (r2 * r2 - x2 * x2 - r1 * r1 + x1 * x1) / (ld)(-2 * x2 + 2 * x1);
}

int N, X[LEN], L[LEN], st[LEN];
ld S[LEN], l[LEN], r[LEN];

ld f(ld c, ld x, ld r) {
	ld y = sqrt(r * r - (x - c) * (x - c));
	//ld fan = r * r * acos(y / r) / 2;
	ld fan = r * r * atan2l(std::abs(x - c), y) / 2;
	ld tri = (c - x) * y / 2;
	return fan + tri;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	(std::cout << std::fixed).precision(9);
	//freopen("../../../input_data/D/1-09.in", "r", stdin);
	//freopen("../../../input_data/D/ret.txt", "w", stdout);
	std::cin >> N;
	for (int i = 0, sp = 0, j; i < N; ++i) {
		std::cin >> X[i] >> L[i];
		l[i] = X[i] - L[i]; r[i] = X[i];
		while (sp) {
			j = st[sp - 1];
			if (X[j] <= X[i] - L[i]) break;
			if ((ll)L[i] * L[i] - ((ll)X[i] - X[j]) * ((ll)X[i] - X[j]) <= (ll)L[j] * L[j]) {
				S[i] += S[j];
				l[i] = X[j];
				break;
			}
			if ((l[i] = intersect(X[j], L[j], X[i], L[i])) > l[j]) {
				S[i] += (S[j] -= f(X[j], l[i], L[j]) - f(X[j], r[j], L[j]));
				r[j] = l[i];
				break;
			}
			--sp;
		}
		st[sp++] = i;
		S[i] += f(X[i], l[i], L[i]);
		std::cout << S[i] << '\n';
	}
}//jay0202
