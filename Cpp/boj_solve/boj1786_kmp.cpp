#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
typedef std::vector<int> Vint;
int N;
Vint get_pi(const std::string& s) {
	int m = s.size(), j = 0;
	Vint pi(m, 0);
	for (int i = 1; i < m; i++) {
		while (j > 0 && s[i] != s[j]) j = pi[j - 1];
		if (s[i] == s[j]) pi[i] = ++j;
	}
	return pi;
}
Vint kmp(const std::string& T, const std::string& P) {
	Vint ans;
	Vint pi = get_pi(P);
	int t = T.size(), p = P.size(), j = 0;
	for (int i = 0; i < t; i++) {
		while (j > 0 && T[i] != P[j]) j = pi[j - 1];
		if (T[i] == P[j]) {
			if (j == p - 1) ans.push_back(i - p + 1), j = pi[j];
			else j++;
		}
	}
	return ans;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::string T, P;
	std::getline(std::cin, T);
	std::getline(std::cin, P);
	Vint matched = kmp(T, P);
	std::cout << matched.size() << "\n";
	for (const int& i : matched) std::cout << i + 1 << " ";
	return;
}
int main() { solve(); return 0; }//boj1786 refer to bowbowbow
