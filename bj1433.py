import sys

N, target = map(int, sys.stdin.readline().strip().split())
high_sl = 0.0
high_sv = 0.0
low_sl = 0.0
low_sv = 0.0
ans = 0.0
for _ in range(N):
    c, sv = map(int, sys.stdin.readline().strip().split())
    sl = sv * c / 100.0
    if c == target:
        ans += sv
    elif c > target:
        high_sl += sl
        high_sv += sv
    else:
        low_sl += sl
        low_sv += sv
print(high_sv, high_sl, low_sv, low_sl)

if low_sv < 0.000001:
    print(ans)
    exit(0)
x = (100 * high_sl - target * high_sv) / (target * low_sv - 100 * low_sl)
if x > 1.0:
    ans += high_sv / x + low_sv
else:
    ans += high_sv + x * low_sv
print(ans)
