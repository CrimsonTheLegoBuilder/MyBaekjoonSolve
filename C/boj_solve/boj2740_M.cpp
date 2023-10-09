#include <iostream>
typedef long long ll;

struct M {
	int** m;
	int r, c;
	M(int r, int c) : r(r), c(c) {
		m = new int* [r];
		for (int i = 0; i < r; i++) {
			m[i] = new int[c];
			for (int j = 0; j < c; j++) {
				m[i][j] = 0;
			}
		}
	}
	~M() {
		//std::cout << "destruct\n";
		for (int i = 0; i < r; i++) {
			delete[] m[i];
		}
		delete[] m;
	}
	M operator * (const M& R) const {
		M p(r, R.c);
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < R.c; j++) {
				for (int k = 0; k < c; k++) {
					p.m[i][j] += m[i][k] * R.m[k][j];
				}
			}
		}
		return p;
	}
};


int main() {
	int n, m, k;
	std::cin >> n >> m;
	M A(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			std::cin >> A.m[i][j];
		}
	}
	std::cin >> m >> k;
	M B(m, k);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < k; j++) {
			std::cin >> B.m[i][j];
		}
	}
	M C = A * B;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			std::cout << C.m[i][j] << " ";
		}
		std::cout << "\n";
	}
	return 0;
}
