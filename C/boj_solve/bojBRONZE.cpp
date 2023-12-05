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

#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
typedef long long ll;
int N, x, y;

int main() {
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> x >> y;
		if (x < y) std::cout << "NO ";
		else std::cout << "MMM ";
		//std::cout << "BRAINS\n";
	}
	return 0;
}