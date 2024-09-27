#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
typedef long long ll;
typedef long double ld;
typedef std::vector<ld> V;
const ld PI = acos(-1);
ld A;

int main() {
	std::cout << std::fixed;
	std::cout.precision(15);
	
	V v = {
        27.91,
        28.09,
        28.08,
        27.91,
        28.28,
        28.26,
        28.09,
        28.09,
        28.03,
        27.91,
    };
    int sz = v.size();
	A = 0;
	for (const ld& s : v) A += 60. / s;
	std::cout << A / sz << "\n";

    // CSV 파일 생성
    std::ofstream file("speed_data.csv");
    file << std::fixed;
    file.precision(15);

    if (file.is_open()) {
        // 벡터 데이터를 출력 (쉼표로 구분된 값)
        for (size_t i = 0; i < v.size(); ++i) {
            file << v[i] << "," << 60. / v[i];
            if (i != v.size() - 1) {
                file << "\n";
            }
        }
        file.close();
        std::cout << "vector_data.csv 파일이 생성되었습니다.\n";
    }
    else {
        std::cerr << "파일을 열 수 없습니다.\n";
    }

	return 0;
}

/*

analogWrite 255
4.138510329228883

200
{
20.98,
  20.68,
  21.01,
  20.51,
 21.05,
  20.99,
  20.49,
 21.11,
  20.70,
  21.08,
  };

  220
  {
  18.03,
    18.43,
    18.71,
    18.46,
    18.16,
    18.56,
    18.46,
  18.13,
  18.59,
  18.42,
  };

  240
  {
    16.20,
    16.01,
    16.25,
    16.15,
    16.18,
    16.27,
    16.20,
    16.31,
    16.37,
    16.17,
  };

  180
  {
  23.70,
  23.95,
  23.57,
  23.78,
  23.85,
  24.00,
  23.70,
  23.90,
  24.16,
  24.23,
  };

  160
  {
  27.91,
  28.09,
  28.08,
  27.91,
  28.28,
  28.26,
  28.09,
  28.09,
  28.03,
  27.91,
  };
*/
