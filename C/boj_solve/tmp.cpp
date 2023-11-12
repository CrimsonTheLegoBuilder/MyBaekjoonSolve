//#include <iostream>
//#include <vector>
//
//using namespace std;
//
//struct DATA {
//	int d, v;
//};
//DATA arr[13];
//int main() {
//	int k, i, area, ans, w = 0, h = 0;
//	std::cin >> k;
//	for (i = 1; i <= 6; i++) {
//		int d, v;
//		std::cin >> d >> v;
//		std::cout << d << " " << v << " " << k << "\n";
//		arr[i] = arr[i + 6] = { d, v };
//		if (d == 1 || d == 2) w = max(w, v); //최대 너비
//		if (d == 3 || d == 4) h = max(h, v); //최대 높이
//	}
//	std::cout << w << " " << h << " DEBUG\n";
//	for (i = 1; i <= 6; i++) {
//		std::cout << i << " DEBUG\n" ;
//		if ((arr[i].v == h && arr[i + 1].v == w) || (arr[i].v == w && arr[i + 1].v == h)) {//최대 너비와 높이는 무조건 붙어있다는 성질.
//			std::cout << "DEBUG\n";
//			std::cout << w * h * k - arr[i + 3].v * arr[i + 4].v * k << "\n";
//			//std::cout << " \n";
//			area = w * h - arr[i + 3].v * arr[i + 4].v;
//			cout << w << " " << h << " " << arr[i + 3].v  << " " << arr[i + 4].v << " " << k << " " << "\n";
//			break;
//		}
//	}
//	//std::cout << area * k << "\n";  //그 경우 반시계 방향으로 받는다는 성질로 인하여 이런 식으로 압축 가능.
//	//cout << area << k << "\n";  //그 경우 반시계 방향으로 받는다는 성질로 인하여 이런 식으로 압축 가능.
//}


#include <iostream>
#include <vector>

using namespace std;

struct DATA {
	int d, v;
};
DATA arr[13];
int main() {
	int k, i;
	int w = 0, h = 0;
	cin >> k;
	for (i = 1; i <= 6; i++) {
		int d, v;
		cin >> d >> v;
		arr[i] = arr[i + 6] = { d, v };
		if (d == 1 || d == 2) w = max(w, v); //최대 너비
		if (d == 3 || d == 4) h = max(h, v); //최대 높이
	}
	for (i = 1; i <= 6; i++) {
		if ((arr[i].v == h && arr[i + 1].v == w) || (arr[i].v == w && arr[i + 1].v == h)) {//최대 너비와 높이는 무조건 붙어있다는 성질.
			cout << (w * h - arr[i + 3].v * arr[i + 4].v) * k;  //그 경우 반시계 방향으로 받는다는 성질로 인하여 이런 식으로 압축 가능.
			break;
		}
	}
}