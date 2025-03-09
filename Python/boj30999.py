N, M = map(int, input().split())
C = 0
for _ in range(N):
    S = input()
    o = S.count("O")
    x = S.count("X")
    C += o > x
print(C)
