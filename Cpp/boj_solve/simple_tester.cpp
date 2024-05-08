#include <iostream>
#include <Windows.h>
typedef long long ll;

void tester() {
	static int cnt = 0;
	std::cout << cnt++ << "\n";
}

void solve() {
	for (;;) {
		tester();
		Sleep(1000);
	}
}

int main() { solve(); return 0; }