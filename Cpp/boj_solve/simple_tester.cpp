#include <iostream>
typedef long long ll;

void tester() {
	ll a = 131 | 85;//전혀 규칙성이 없이 무작위한 값이 튀어나옴.
	std::cout << a << '\n';
}
int main() { tester(); return 0; }