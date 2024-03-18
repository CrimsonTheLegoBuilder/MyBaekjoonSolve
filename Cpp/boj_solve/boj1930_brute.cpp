#include <iostream>
#include <algorithm>
#include <vector>
int K, a, b, c, d;

struct Face { int f, l, r; };
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> K;
	Face A = { 0, 0, 0 }, B = { 0, 0, 0 };
	while (K--) {
		std::cin >> a >> b >> c >> d;
		int x = std::min({ a, b, c, d });
		if (a == x) A.f = b, A.l = c, A.r = d;
		else if (b == x) A.f = a, A.l = d, A.r = c;
		else if (c == x) A.f = a, A.l = b, A.r = d;
		else if (d == x) A.f = a, A.l = c, A.r = b;
		std::cin >> a >> b >> c >> d;
		if (a == x) B.f = b, B.l = c, B.r = d;
		else if (b == x) B.f = a, B.l = d, B.r = c;
		else if (c == x) B.f = a, B.l = b, B.r = d;
		else if (d == x) B.f = a, B.l = c, B.r = b;
		if (A.f == B.f && A.l == B.l && A.r == B.r) std::cout << "1\n";
		else if (A.f == B.l && A.l == B.r && A.r == B.f) std::cout << "1\n";
		else if (A.f == B.r && A.l == B.f && A.r == B.l) std::cout << "1\n";
		//else std::cout << "0\n";
	}
	return;
}
int main() { solve(); return 0; }//boj1930
