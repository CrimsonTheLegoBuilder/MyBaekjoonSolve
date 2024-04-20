#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
typedef long long ll;
int n, v[500001], i; 
ll c;
ll speed;
int main() {
	scanf("%d", &n);
	for (i = 1; i <= n; i++) scanf("%d", &v[i]);
	for (i = n; i > 0; i--) {
		if (v[i] > c) c++, speed += c;
		else speed += (ll)v[i], c = v[i];
	}
	printf("%d", speed);
	return 0;
}