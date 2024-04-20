#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
const int LEN = 2e5 + 1;
int N, Q, T, x;
int num[LEN], idx[LEN];
bool V[LEN];
struct Pos { int x, y; };
std::vector<Pos> seq;
Pos P(int i, int j) {
	if (i > j) std::swap(i, j);
	return { i, j };
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> num[i], idx[num[i]] = i;
	int cnt = 0;
	for (int i = 1; i <= N; i++) {
        if (V[i]) continue;
        int cur = i;
        while (!V[cur]) {
            V[cur] = 1;
			if (V[num[cur - 1]]) break;
			if (cur == num[cur - 1]) break;
			//seq.push_back(P(cur, num[cur - 1]));

			int ii = idx[cur], jj = idx[num[cur - 1]];
			seq.push_back(P(ii + 1, jj + 1));
			//std::cout << "i, j " << ii << " " << jj << "\n";
			std::swap(idx[cur], idx[num[cur - 1]]);
			//std::cout << "cur " << cur << " " << num[cur - 1] << "\n";
			cur = num[cur - 1];
			std::swap(num[ii], num[jj]);
			cnt++;
        }
		//std::cout << V[1] << " " << V[2] << " " << V[3] << " " << V[4] << " \n";
    }
	std::cout << cnt << "\n";
	for (const Pos& p : seq) std::cout << p.x << " " << p.y << "\n";
	return;
}
int main() { solve(); return 0; }