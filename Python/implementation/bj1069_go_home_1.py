import sys

x, y, d, t = map(int, sys.stdin.readline().split())

l = (x**2 + y**2) ** 0.5
jump = l // d
mod = l % d

if d <= t:
    ans = l
else:
    if jump == 0:
        ans = min(2.0 * t, t + d - mod, mod)
    else:
        ans = min((jump + 1) * t, jump * t + mod)

print(ans)
