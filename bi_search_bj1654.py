import sys

n, m = map(int, sys.stdin.readline().strip().split())
wires = [float(sys.stdin.readline().strip()) for _ in range(n)]
target = wires[0]
den = 10000000
start, end = 0, den
ratio = 0

while abs(start - end) > 0.00001:
    mid = (start + end) / 2
    ratio = mid / den
    line = target * ratio
    total = 0
    for i in range(n):
        total += int(wires[i] / line)
    # print(total)
    if total < m:
        end = mid
    else:
        start = mid

ans = target * ratio
print(int(ans))
