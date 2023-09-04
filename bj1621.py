import sys

N = int(sys.stdin.readline().strip())
K, C = map(int, sys.stdin.readline().strip().split())
banana = tuple(map(int, sys.stdin.readline().strip().split()))
prefix = [0] * N
s = sum(banana[:K-1])
for i in range(K-1, N):
    s += banana[i]
    prefix[i] = s
    s -= banana[i-K+1]
# print(prefix)
dp = [0] * N
