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
//		if (d == 1 || d == 2) w = max(w, v); //�ִ� �ʺ�
//		if (d == 3 || d == 4) h = max(h, v); //�ִ� ����
//	}
//	std::cout << w << " " << h << " DEBUG\n";
//	for (i = 1; i <= 6; i++) {
//		std::cout << i << " DEBUG\n" ;
//		if ((arr[i].v == h && arr[i + 1].v == w) || (arr[i].v == w && arr[i + 1].v == h)) {//�ִ� �ʺ�� ���̴� ������ �پ��ִٴ� ����.
//			std::cout << "DEBUG\n";
//			std::cout << w * h * k - arr[i + 3].v * arr[i + 4].v * k << "\n";
//			//std::cout << " \n";
//			area = w * h - arr[i + 3].v * arr[i + 4].v;
//			cout << w << " " << h << " " << arr[i + 3].v  << " " << arr[i + 4].v << " " << k << " " << "\n";
//			break;
//		}
//	}
//	//std::cout << area * k << "\n";  //�� ��� �ݽð� �������� �޴´ٴ� ������ ���Ͽ� �̷� ������ ���� ����.
//	//cout << area << k << "\n";  //�� ��� �ݽð� �������� �޴´ٴ� ������ ���Ͽ� �̷� ������ ���� ����.
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
		if (d == 1 || d == 2) w = max(w, v); //�ִ� �ʺ�
		if (d == 3 || d == 4) h = max(h, v); //�ִ� ����
	}
	for (i = 1; i <= 6; i++) {
		if ((arr[i].v == h && arr[i + 1].v == w) || (arr[i].v == w && arr[i + 1].v == h)) {//�ִ� �ʺ�� ���̴� ������ �پ��ִٴ� ����.
			cout << (w * h - arr[i + 3].v * arr[i + 4].v) * k;  //�� ��� �ݽð� �������� �޴´ٴ� ������ ���Ͽ� �̷� ������ ���� ����.
			break;
		}
	}
}