import sys
input = sys.stdin.readline
x, y, d, t = map(int, input().split())

l = (x**2 + y**2) ** 0.5

if (t >= l) | (d <= t):
    ans = l
elif (l // d == 0) & (d - l >= t):
    ans = 2.0 * t
elif (l // d == 0) & (d - l < t):
    ans = t + d - l
elif (l // d == 1) & (l - d >= t):
    ans = 2.0 * t
elif (l // d == 1) & (l - d < t):
    ans = t + l - d
elif l % d == 0:
    ans = (l // d) * t
else:
    ans = min((l // d) * t + (l - (l // d) * d), (l // d + 1) * t)

print(ans)
