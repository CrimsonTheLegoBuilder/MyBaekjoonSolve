import sys

n = int(sys.stdin.readline().strip())

for _ in range(n):
    V, E = map(int, sys.stdin.readline().strip().split())
    print(2 - V + E)
