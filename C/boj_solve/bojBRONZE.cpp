//#include <iostream>
//#include <algorithm>
//using namespace std;
//
//int main() {  //15963
//	int n, m;
//	cin >> n >> m;
//	cout << (n == m);
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//
//int main() {  //27327
//	int x;
//	cin >> x;
//	cout << x * 24;
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//
//int main() {  //27328
//	int A, B;
//	cin >> A >> B;
//	cout << ((A > B) - (A < B));
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//
//int main() {
//	int A, B;
//	cin >> A >> B;
//	cout << A * 10 + B + B;
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int A, B;
//	cin >> A >> B;
//	//cout << (((A << 1) >= B) ? "E\n" : "H\n");
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N;
//	cin >> N;
//	while (N--) {
//		std::string A;
//		cin >> A;
//		if (A == "Algorithm") std::cout << "204\n";
//		if (A == "DataAnalysis") std::cout << "207\n";
//		if (A == "ArtificialIntelligence") std::cout << "302\n";
//		if (A == "CyberSecurity") std::cout << "B101\n";
//		if (A == "Network") std::cout << "303\n";
//		if (A == "Startup") std::cout << "501\n";
//		//if (A == "TestStrategy") std::cout << "105\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N, A, B, X;
//	cin >> N;
//	while (N--) {
//		cin >> A >> B >> X;
//		//std::cout << A * (X - 1) + B << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N;
//	std::string A;
//	cin >> A;
//	if (A == "SONGDO") std::cout << "HIGHSCHOOL\n";
//	if (A == "CODE") std::cout << "MASTER\n";
//	if (A == "2023") std::cout << "0611\n";
//	//if (A == "ALGORITHM") std::cout << "CONTEST\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N;
//	std::string A = "WelcomeToSMUPC";
//	cin >> N;
//	std::cout << A[N % 15];
//	
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N, A, cnt = 0;
//	cin >> N;
//	while (N--) {
//		cin >> A;
//		cnt += (A & 1 ? -1 : 1);
//	}
//	//cout << (cnt > -1 ? "Happy" : "Sad");
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int H, I, A, R, C;
//	cin >> H >> I >> A >> R >> C;
//	//cout << H * I - A * R * C;
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	std::string A;
//	cin >> A;
//	if (A == "M") std::cout << "MatKor\n";
//	if (A == "W") std::cout << "WiCys\n";
//	if (A == "C") std::cout << "CyKor\n";
//	if (A == "A") std::cout << "AlKor\n";
//	//if (A == "$") std::cout << "$clear\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	ll t, e, f, Max, Mel;
//	std::cin >> t >> e >> f;
//	Max = 3 * t + 20 * e + 120 * f;
//	std::cin >> t >> e >> f;
//	Mel = 3 * t + 20 * e + 120 * f;
//	//cout << (Max > Mel ? "Max\n" : Max < Mel ? "Mel\n" : "Draw\n");
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	ll N, x, cnt = 0;
//	cin >> N;
//	while (N--) {
//		cin >> x;
//		cnt += x;
//	}
//	//cout << (!cnt ? "Stay\n" : cnt > 0 ? "Right\n" : "Left\n");
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//int N;
//void solve() {
//	std::cin >> N;
//	if (620 <= N) std::cout << "Red\n";
//	if (590 <= N) std::cout << "Orange\n";
//	if (570 <= N) std::cout << "Yellow\n";
//	if (495 <= N) std::cout << "Green\n";
//	if (450 <= N) std::cout << "Blue\n";
//	if (425 <= N) std::cout << "Indigo\n";
//	if (380 <= N) std::cout << "Violet\n";
//	return;
//}
//int main() { solve(); return 0; }

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	std::string name;
//	int age, weight;
//	while (1) {
//		std::cin >> name >> age >> weight;
//		if (name == "#") break;
//		if (age > 17 || weight >= 80) std::cout << name << " Senior\n";
//		//else std::cout << name << " Junior\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N;
//	std::cin >> N;
//	N++;
//	while (N--) {
//		for (int i = N; i > 0; i--) {
//			std::cout << '*';
//		}
//		//std::cout << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//using namespace std;
//typedef long long ll;
//
//int main() {
//	int N;
//	std::cin >> N;
//	N++;
//	while (N-- > 1) std::cout << N << "\n";
//	
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//typedef long long ll;
//
//int main() {
//	int A, B, C, D;
//	std::cin >> A >> B >> C >> D;
//	A += D / 3600;
//	D %= 3600;
//	B += D / 60;
//	D %= 60;
//	C += D;
//
//	if (C > 59) B += 1, C %= 60;
//	if (B > 59) A += 1, B %= 60;
//	if (A > 23) A %= 24;
//
//	//std::cout << A << " " << B << " " << C << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//typedef long long ll;
//int arr[3];
//
//int main() {
//	//int A, B, C;
//	//std::cin >> A >> B >> C;
//	std::cin >> arr[0] >> arr[1] >> arr[2];
//	std::sort(arr, arr + 3);
//	//std::cout << arr[0] << " " << arr[1] << " " << arr[2] << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//typedef long long ll;
////int arr[3];
//
//int main() {
//	int N, M, A, B, C, D, E;
//	std::cin >> N >> M >> A >> B >> C >> D >> E;
//	N *= M;
//	std::cout << A - N << " ";
//	std::cout << B - N << " ";
//	std::cout << C - N << " ";
//	std::cout << D - N << " ";
//	//std::cout << E - N << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//typedef long long ll;
//
//int main() {
//	int R1, R2, S;
//	std::cin >> R1 >> S;
//	S <<= 1;
//	//std::cout << S - R1;
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//typedef long long ll;
//
//int main() {
//	int x, y, a, b;
//	std::cin >> x >> y;
//	b = (x - y) >> 1;
//	if (x - b != y + b) std::cout << "-1\n";
//	else {
//		a = x - b;
//		//std::cout << a << " " << b << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
////#include <cstring>
//#include <string>
//typedef long long ll;
//int N;
//
//int main() {
//	std::string X;
//	std::cin >> N;
//	std::cin.ignore();
//	for (int i = 1; i <= N; i++) {
//		//getline(std::cin, X, '\n');
//		getline(std::cin, X);
//		std::cout << i << ". " << X << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <string>
//typedef long long ll;
//int N, x, y;
//
//int main() {
//	std::cin >> N;
//	for (int i = 1; i <= N; i++) {
//		std::cin >> x >> y;
//		if (x < y) std::cout << "NO ";
//		else std::cout << "MMM ";
//		//std::cout << "BRAINS\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <string>
//typedef long long ll;
//int N, Q, W, E;
//
//int main() {
//	std::cin >> N;
//	//std::cout << "Gnomes:\n";
//	while (N--) {
//		std::cin >> Q >> W >> E;
//		bool f1 = Q < W && W < E;
//		bool f2 = Q > W && W > E;
//		std::cout << (f1 || f2 ? "O" : "Uno");
//		std::cout << "rdered\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//typedef long long ll;
//typedef double ld;
//ld N;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(2);
//	while (1) {
//		std::cin >> N;
//		if (!N) return 0;
//		//std::cout << (1 + N + N * N + N * N * N + N * N * N * N) << "\n";
//	}
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//typedef long long ll;
//typedef long double ld;
//ld N;
//const ld G = 0.167;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(2);
//	while (1) {
//		std::cin >> N;
//		if (N < 0) return 0;
//		std::cout << "Objects weighing ";
//		std::cout << N;
//		std::cout << " on Earth will weigh ";
//		//std::cout << N * G << " on the moon.\n";
//	}
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//typedef long long ll;
//ll N;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	ll t = 0;
//	while (N--) {
//		std::cout << ++t << " ";
//		if (!(t % 6)) std::cout << "Go! ";
//	}
//	//if (t % 6) std::cout << "Go! ";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll N;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	while (N--) {
//		std::string S;
//		std::cin >> S;
//		std::cout << S[0];
//		for (int i = 1; i < S.size(); i++) {
//			if (S[i] != S[i - 1]) std::cout << S[i];
//		}
//		//std::cout << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll N;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	while (N--) {
//		std::string S;
//		std::cin >> S;
//		for (int i = 0; i < S.size(); i++) {
//			if (64 < S[i] && S[i] < 91) std::cout << (char)(S[i] + 32);
//			else std::cout << S[i];
//		}
//		//std::cout << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll L, A, B, C, D, X, Y;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> L >> A >> B >> C >> D;
//	X = A / C + (A % C != 0);
//	Y = B / D + (B % D != 0);
//	X = std::max(X, Y);
//	//std::cout << L - X << '\n';
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll A, B, C, D, E, X, Y;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> A >> B >> C >> D >> E;
//	X = std::min({ A, B, C });
//	Y = std::min({ D, E });
//	//std::cout << X + Y - 50 << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll A, B, C, D, T;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> A >> B >> C >> D;
//	T = A + B + C + D;
//	//std::cout << T / 60 << "\n" << T % 60 << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll H, M, S, T = 0;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	int N = 3;
//	while (N--) {
//		T = 0;
//		std::cin >> H >> M >> S;
//		T -= S;
//		T -= M * 60;
//		T -= H * 3600;
//		std::cin >> H >> M >> S;
//		T += S;
//		T += M * 60;
//		T += H * 3600;
//		H = T / 3600;
//		T %= 3600;
//		M = T / 60;
//		T %= 60;
//		S = T;
//		std::cout << H << " " << M << " " << S << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll s, T1{ 0 }, T2{ 0 };
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	int a{ 4 }, b{ 4 };
//	while (a--) {
//		std::cin >> s;
//		T1 += s;
//	}
//	while (b--) {
//		std::cin >> s;
//		T2 += s;
//	}
//	//std::cout << (T1 == T2 ? T1 : T1 > T2 ? T1 : T2) << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll g, b;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	while (1) {
//		std::cin >> b >> g;
//		if (!b && !g) return 0;
//		//std::cout << b + g << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll D, H, M, T{ 0 };
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> D >> H >> M;
//	D *= 1440; H *= 60;
//	ll D1 = 11ll * 1440;
//	ll H1 = 11ll * 60;
//	ll M1 = 11ll;
//	T -= D1 + H1 + M1;
//	T += D + H + M;
//	//std::cout << (T > -1 ? T : -1) << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll A, B, C;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> A >> B;
//	C = B - A;
//	//std::cout << B + C;
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//ll A, B, C, F;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> A >> B;
//	C = B - A;
//	if (C < 1) std::cout << "Congratulations, you are within the speed limit!\n";
//	else {
//		if (1 <= C && C <= 20) F = 100;
//		if (21 <= C && C <= 30) F = 270;
//		if (31 <= C) F = 500;
//		//std::cout << "You are speeding and your fine is $" << F << ".\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//char N;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	std::cout << "Naver ";
//	if (N == 'N' || N == 'n') std::cout << "D2\n";
//	//else std::cout << "Whale\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//int N, A, B, C, D, FU = 0, FD = 0;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> A >> B >> C >> D;
//	if (A < B && B < C && C < D) FU = 1;
//	if (A > B && B > C && C > D) FD = 1;
//	if (!FU && !FD) std::cout << "No ";
//	std::cout << "Fish ";
//	if (FU) std::cout << "Rising\n";
//	//if (FD) std::cout << "Diving\n";
//	else std::cout << "\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//typedef long double ld;
//ld w, h, BMI;
//const ld TOL = 1e-6;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> w >> h;
//	BMI = w / (h * h);
//	if (BMI > 25) std::cout << "Over";
//	else if (18.5 - TOL < BMI && BMI < 25 + TOL) std::cout << "Normal ";
//	else if (BMI < 18.5) std::cout << "Under";
//	//std::cout << "weight\n";
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//typedef long double ld;
//const ld TOL = 1e-6;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::string a;
//	while (std::cin >> a) {
//		if (a == "CU") { std::cout << "see you\n"; continue; }
//		if (a == ":-)") { std::cout << "I¡¯m happy\n"; continue; }
//		if (a == ":-(") { std::cout << "I¡¯m unhappy\n";continue; }
//		if (a == ";-)") { std::cout << "wink\n"; continue; }
//		if (a == ":-P") { std::cout << "stick out my tongue\n"; continue; }
//		if (a == "(~.~)") { std::cout << "sleepy\n"; continue; }
//		if (a == "TA") { std::cout << "totally awesome\n"; continue;} 
//		if (a == "CCC") { std::cout << "Canadian Computing Competition\n"; continue; }
//		if (a == "CUZ") { std::cout << "because\n"; continue; }
//		if (a == "TY") { std::cout << "thank-you\n"; continue; }
//		if (a == "YW") { std::cout << "you¡¯re welcome\n"; continue; }
//		if (a == "TTYL") { std::cout << "talk to you later\n"; continue; }
//		//else std::cout << a << "\n";
//	}
//	return 0;
//}

//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
//typedef long double ld;
//const ld TOL = 1e-6;
//int N;
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N;
//	int a = 1;
//	while (1) {
//		if (N < a * a) break;
//		a++;
//	}
//	std::cout << "The largest square has side length " << a - 1 << ".\n";
//	return 0;
//}

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
int N;

int main() {

}