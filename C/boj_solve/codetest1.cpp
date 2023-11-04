#include <iostream>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <vector>
const int LEN = 2001;
int N, x, CNT[LEN];

struct I {
	int i;
	bool operator < (const I& x) const {
		int cnt = 0, cntx = 0, n = i, nx = x.i;
		while (n) {
			cnt += n % 2;
			cnt >>= 1;
		}
		while (nx) {
			cntx += nx % 2;
			cntx >>= 1;
		}
		return cnt > cntx;
	}
};
std::vector<I> X;
void mkSeq() {
	for (int i = 2; i < 4001; i++) {
		X.push_back({ i });
	}
	std::sort(X.begin(), X.end());
}

std::unordered_set<int> S;


int main() {
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> x;
		CNT[x]++;
		S.insert(x);
	}


}