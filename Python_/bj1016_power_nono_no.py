import sys

m, M = map(int, sys.stdin.readline().strip().split())

pnn = [0] + [1] * M

for i in range(2, int(M**0.5) + 1):
    if pnn[i] == 1:
        for j in range(i**2, M+1, i**2):
            pnn[j] = 0

print(sum(pnn[m:M+1]))
