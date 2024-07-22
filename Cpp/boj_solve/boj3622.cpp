#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;

int N;
void solve() {
	int A, a, B, b, P;
	std::cin >> A >> a >> B >> b >> P;
	if (A < B) std::swap(A, B), std::swap(a, b);
	bool f = 0;
	if (A + B <= P) f = 1;
	if (A <= P && B <= a) f = 1;
	//std::cout << (f ? "Yes\n" : "No\n");
	return;
}
int main() { solve(); return 0; }//boj3622