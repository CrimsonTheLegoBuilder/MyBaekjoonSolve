#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>
typedef long long ll;
const int LEN = 205;

int N, M, U, V, w, h;
char B[LEN][LEN];
char T[LEN][LEN];
std::string O;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M >> U >> V;
	for (int u = 0; u < U; u++) {
		for (int v = 0; v < V; v++) {
			std::cin >> T[u][v];
		}
	}
	h = N / U + (N % U != 0);
	w = M / V + (M % V != 0);
	auto valid = [&](const int& n, const int& m) -> bool {
		return 0 <= n && n < N && 0 <= m && m < M;
		};
	std::cin >> O;
	//std::cout << O << "\n";
	if (O == "clamp-to-edge") {
		//std::cout << "c\n";
		for (int i = 0; i < U; i++) {
			for (int j = 0; j < M; j++) {
				if (j < V) B[i][j] = T[i][j];
				else B[i][j] = T[i][V - 1];
			}
		}
		for (int j = 0; j < V; j++) {
			for (int i = U; i < N; i++) {
				B[i][j] = T[U - 1][j];
			}
		}
		for (int i = U; i < N; i++) {
			for (int j = V; j < M; j++) {
				B[i][j] = T[U - 1][V - 1];
			}
		}
	}
	else if (O == "repeat") {
		//std::cout << "r\n";
		for (int u = 0; u < U; u++) {
			for (int v = 0; v < V; v++) {
				B[u][v] = T[u][v];
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						int n = U * i + u;
						int m = V * j + v;
						if (valid(n, m)) B[n][m] = T[u][v];
					}
				}
			}
		}
	}
	else {
		//std::cout << "m\n";
		for (int u = 0; u < U; u++) {
			for (int v = 0; v < V; v++) {
				B[u][v] = T[u][v];
				for (int i = 0; i < h; i++) {
					for (int j = 0; j < w; j++) {
						int n = U * i + ((i % 2) ? (U - u - 1) : u);
						int m = V * j + ((j % 2) ? (V - v - 1) : v);
						if (valid(n, m)) B[n][m] = T[u][v];
					}
				}
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			std::cout << B[i][j];
		}
		std::cout << "\n";
	}
	return 0;
}