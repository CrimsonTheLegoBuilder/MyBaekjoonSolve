#include<iostream>
#include<cstdio>
#include<vector>
#define N 200000
using namespace std;
int read()
{
	char c = 0;
	int sum = 0;
	while (c < '0' || c>'9') c = getchar();
	while ('0' <= c && c <= '9') sum = sum * 10 + c - '0', c = getchar();
	return sum;
}
struct reads
{
	long long sum;
	int sz, l, r;
	bool operator <= (const reads& t)const
	{
		return sum * t.sz <= t.sum * sz;
	}
};
reads dque[N + 1], dque2[N + 1];
reads operator + (reads a, reads b)
{
	return { a.sum + b.sum, a.sz + b.sz, a.l, b.r };
}
vector<reads>p[N + 1];
int n, lg[N + 1], a[N + 1], top, top2;
long long s[N + 1], s2[N + 1], rst, ans[N + 1];
long long query(reads d)
{
	int ps = 0;
	for (int i = lg[top]; i >= 0; --i)
		if (ps + (1 << i) <= top && d <= dque[ps + (1 << i)])
			ps += (1 << i);
	return (dque[top].r - dque[ps].r) * d.sum + s[dque[ps].r] * d.sz;
}
long long query2(reads d)
{
	int ps = 0;
	for (int i = lg[top2]; i >= 0; --i)
		if (ps + (1 << i) <= top2 && d <= dque2[ps + (1 << i)])
			ps += (1 << i);
	return (dque2[ps].l - dque2[top2].l) * d.sum + (s[n] - s[dque2[ps].l - 1]) * d.sz;
}
int main()
{
	reads d;
	for (int i = 2; i <= N; ++i) lg[i] = lg[i >> 1] + 1;
	n = read(), dque2[0].l = n + 1;
	for (int i = 1; i <= n; ++i) a[i] = read(), s[i] = s[i - 1] + a[i], s2[i] = s2[i - 1] + 1ll * i * a[i];
	for (int i = 1; i <= n; ++i) ans[i] = 1ll * a[i] * (n - 1) + s[i - 1] * (n - i) + (s[n] - s[i]) * (n + i) + s2[i - 1] - (s2[n] - s2[i]);
	for (int i = n; i >= 1; --i)
	{
		d = { a[i],1,i,i };
		while (top2 && dque2[top2] <= d) d = d + dque2[top2], p[i].push_back(dque2[top2]), top2--;
		dque2[++top2] = d;
	}
	for (int i = 1; i <= n; ++i)
	{
		if (i != 1)
		{
			d = { a[i - 1],1,i - 1,i - 1 };
			while (top && dque[top] <= d) d = dque[top] + d, rst -= query2(dque[top]), top--;
			dque[++top] = d, rst += query2(dque[top]);
		}
		rst -= query(dque2[top2]), --top2;
		for (int j = (int)(p[i].size()) - 1; j >= 0; --j) dque2[++top2] = p[i][j], rst += query(dque2[top2]);
		ans[i] -= rst;
	}
	for (int i = 1; i <= n; ++i) printf("%lld ", ans[i]);
	puts("");
	return 0;
}