#include <iostream>
#include <string>
const int LEN = 500'001;
std::string S, B;
int A[LEN];

bool C(const std::string& s, const std::string& b) {
	if (s.length() == b.length()) {
		int f = 0;
		for (int i = 0; i < s.length(); i++) {
			if (s[i] != b[i]) f++;
			if (f > 1) return 0;
		}
	}
	else if (s.length() < b.length()) {
		if (b.length() - s.length() > 1) return 0;
		for (int i = 0, j = 0; i < s.length(); i++, j++) {
			if (s[i] != b[j]) {
				if (i != j) return 0;
				i--;
			}
		}
	}
	else {
		if (s.length() - b.length() > 1) return 0;
		for (int i = 0, j = 0; i < b.length(); i++, j++) {
			if (b[i] != s[j]) {
				if (i != j) return 0;
				i--;
			}
		}
	}
	return 1;
}


int main() {
	int N, a= 0;
	std::cin >> N >> S;
	for (int i = 1; i <= N; i++) {
		std::cin >> B;
		if (C(S, B)) {
			A[a++] = i;
		}
	}
	std::cout << a << "\n";
	for (int i = 0; i < a; i++) {
		std::cout << A[i] << " ";
	}
	return 0;
}