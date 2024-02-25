//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <vector>
//#include <cmath>
//typedef long long ll;
//ll N, ans;
//
//int main() {
//	std::cin >> N;
//	ans = N % 8;
//	if (!ans) std::cout << "2\n";
//	else if (1 <= ans && ans <= 5) std::cout << ans << "\n";
//	//else std::cout << 10 - ans << "\n";
//	return 0;
//}

#include<iostream>
using namespace std;
int main()
{
    int n = 1, i, a, m;
    cin >> m;
    for (i = 1; i <= m; i++)
    {
        cin >> a;
        if (i == 1 || i == m)
        {
            n = n * a;
        }
    }
    cout << n;
    return 0;
}