#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
int Q, K, P;
bool V[(int)1e7 + 1];

struct Seq {
	int k, p;
	bool operator < (const Seq& s) const { return p < s.p; }
};
std::priority_queue<Seq> Hh, Hl;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	freopen("b_double_queue.in", "r", stdin);
	freopen("b_double_queue.out", "w", stdout);
	memset(V, 0, sizeof V);
	while (1) {
		std::cin >> Q;
		if (!Q) return;
		if (Q == 1) {
			std::cin >> K >> P;
			Hh.push({ K, P });
			Hl.push({ K, -P });
			V[P] = 1;
		}
		if (Q == 2) {
			while (!Hh.empty() && !V[Hh.top().p]) {
				//V[Hh.top().p] = 1;
				Hh.pop();
			}
			if (Hh.empty()) {
				Hl = {};
				std::cout << "0\n";
				continue;
			}
			std::cout << Hh.top().k << "\n";
			V[Hh.top().p] = 0;
			Hh.pop();
		}
		if (Q == 3) {
			while (!Hl.empty() && !
				V[-Hl.top().p]) {
				//V[-Hl.top().p] = 1;
				Hl.pop();
			}
			if (Hl.empty()) {
				Hh = {};
				std::cout << "0\n";
				continue;
			}
			std::cout << Hl.top().k << "\n";
			V[-Hl.top().p] = 0;
			Hl.pop();
		}
	}
}

int main() {
	solve();
}