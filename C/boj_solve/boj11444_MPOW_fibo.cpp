#include <iostream>
#include <algorithm>
typedef long long ll;
const ll MOD = 1'000'000'007;

struct MX {
	ll** mx;
	ll r, c;
	//default constructor
	MX(int r, int c) : r(r), c(c) {
		mx = new ll* [r];
		for (int i = 0; i < r; i++) {
			mx[i] = new ll[c];
			for (int j = 0; j < c; j++) {
				mx[i][j] = 0;
			}
		}
	}
	//copy constructor
	MX(const MX& R) : r(R.r), c(R.c) {
		mx = new ll* [r];
		for (int i = 0; i < r; i++) {
			mx[i] = new ll[c];
			for (int j = 0; j < c; j++) {
				mx[i][j] = R.mx[i][j];
			}
		}
	}
	//destructor
	~MX() {
		for (int i = 0; i < r; i++) {
			delete[] mx[i];
		}
		delete[] mx;
	}
	//overloading
	MX operator * (const MX& P) const {
		MX Q(r, P.c);
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < P.c; j++) {
				for (int k = 0; k < c; k++) {
					Q.mx[i][j] += mx[i][k] * P.mx[k][j];
					Q.mx[i][j] %= MOD;
				}
			}
		}
		return Q;
	}
	//copy assignment operator
	MX& operator = (const MX& R) {
		if (this == &R) return *this;
		for (int i = 0; i < r; i++) {
			delete[] mx[i];
		}
		delete[] mx;
		r = R.r; c = R.c;
		mx = new ll* [r];
		for (int i = 0; i < r; i++) {
			mx[i] = new ll[c];
			for (int j = 0; j < c; j++) {
				mx[i][j] = R.mx[i][j];
			}
		}
		return *this;
	}
};
MX mxpow(MX a, ll B) {
	if (B == 1) return a;
	ll b = B >> 1;
	MX A = mxpow(a, b);
	if (B % 2) return A * A * a;
	else return A * A;
}



int main() {
	MX U(2, 2);
	U.mx[0][0] = 1; U.mx[0][1] = 1;
	U.mx[1][0] = 1; U.mx[1][1] = 0;
	ll N; std::cin >> N;
	if (N <= 1) {
		std::cout << N << "\n";
		return 0;
	}
	MX T = mxpow(U, N - 1);
	std::cout << T.mx[0][0] % MOD << "\n";
	//while (1) {
	//	ll N; std::cin >> N;
	//	if (!N) return 0;
	//	MX T = mxpow(U, N);
	//	std::cout << T.mx[0][0] % MOD << "\n";
	//}
	return 0;
}