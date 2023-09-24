#include <iostream>

int main() {
	//freopen("boj26489.txt", "r", stdin);
	char s[20];
	int c=0;
	while (1) {
		//if (scanf("%s", s)) c += 1;
		std::cin.getline(s, 20);
		if (sizeof(s)) c += 1;
		else break;
	}
	std::cout << c;
	return 0;
}