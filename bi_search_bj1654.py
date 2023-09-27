import sys

N, M = map(int, sys.stdin.readline().strip().split())
wires = sorted([int(sys.stdin.readline().strip()) for _ in range(N)])
MAX = wires[-1]

# line = [i for i in range(MAX+1)]
s, e = 1, MAX
while s <= e:
    m = s + e >> 1
    tmp = 0
    for i in range(N):
        tmp += wires[i] // m
    if tmp < M:
        e = m - 1
    else:
        s = m + 1

print(e)
