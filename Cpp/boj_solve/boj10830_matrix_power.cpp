#include <iostream>
typedef long long ll;
const ll MOD = 1000;

struct M {
	int** m;
	int r, c;
	M(int r, int c) : r(r), c(c) {  //default constructor
		m = new int* [r];
		for (int i = 0; i < r; i++) {
			m[i] = new int[c];
			for (int j = 0; j < c; j++) {
				m[i][j] = 0;
			}
		}
	}
	M(const M& R) :r(R.r), c(R.c) {  //copy constructor
		m = new int* [r];
		for (int i = 0; i < r; i++) {
			m[i] = new int[c];
			for (int j = 0; j < c; j++) {
				m[i][j] = R.m[i][j];
			}
		}
	}
	~M() {  //destructor
		//std::cout << "destruct\n";
		for (int i = 0; i < r; i++) {
			delete[] m[i];
		}
		delete[] m;
	}
	M operator * (const M& R) const {  //overloading
		M p(r, R.c);
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < R.c; j++) {
				for (int k = 0; k < c; k++) {
					p.m[i][j] += m[i][k] * R.m[k][j];
					p.m[i][j] %= MOD;
				}
			}
		}
		return p;
	}
	M& operator = (const M& R) {  //copy assignment operator
		if (this == &R) return *this;
		for (int i = 0; i < r; i++) {
			delete[] m[i];
		}
		delete[] m;
		r = R.r; c = R.c;
		m = new int* [r];
		for (int i = 0; i < r; i++) {
			m[i] = new int[c];
			for (int j = 0; j < c; j++) {
				m[i][j] = R.m[i][j];
			}
		}
		return *this;
	}
};
M pow_m(M A, ll B) {  //divide & conquer
	if (B <= 1) return A;
	ll b = B >> 1;
	M A_ = pow_m(A, b);
	if (B % 2) return A_ * A_ * A;
	else return A_ * A_;
}


int main() {
	int n;
	ll B;
	std::cin >> n >> B;
	M A(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cin >> A.m[i][j];
		}
	}
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n; j++) {
	//		std::cout << A.m[i][j] << " ";
	//	}
	//	std::cout << "\n";
	//}
	M C(n, n);
	for (int i = 0; i < n; i++) C.m[i][i] = 1;

	//while (B--) {
	//	C = C * A;
	//}
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n; j++) {
	//		std::cout << C.m[i][j] << " ";
	//	}
	//	std::cout << "\n";
	//}
	C = pow_m(A, B);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << C.m[i][j]%1000 << " ";
		}
		std::cout << "\n";
	}
	return 0;
}



/*
5 100000000000
1 0 0 0 1
1 0 0 0 1
1 0 0 0 1
1 0 0 0 1
1 0 0 0 1

*/