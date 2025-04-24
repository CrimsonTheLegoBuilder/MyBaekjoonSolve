#include <iostream>
#include <stack>
#include <algorithm>
#include <string.h>
typedef long long ll;
const int LEN = 100'002;
ll box[LEN]{ 0 };
std::stack<ll> S;


int main() {
	int n;
	while (1) {
		std::cin >> n;
		if (!n) return 0;
		ll area = 0;

		memset(box, 0, sizeof box);

		for (int i = 1; i <= n; i++) {
			std::cin >> box[i];
		}

		while (!S.empty()) S.pop();
		S.push(0);

		for (int i = 1; i <= n+1; i++) {
			while (!S.empty() && box[S.top()] > box[i]) {
				ll tmp = S.top(); S.pop();
				area = std::max(area, box[tmp] * (i - S.top() - 1));
			}
			S.push(i);
		}

		std::cout << area << "\n";
	}
}

